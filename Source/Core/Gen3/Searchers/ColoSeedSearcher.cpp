/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2022 by Admiral_Fish, bumba, and EzPzStreamz
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "ColoSeedSearcher.hpp"
#include <algorithm>
#include <future>

constexpr u8 natures[8][6]
    = { { 0x16, 0x15, 0x0f, 0x13, 0x04, 0x04 }, { 0x0b, 0x08, 0x01, 0x10, 0x10, 0x0C }, { 0x02, 0x10, 0x0f, 0x12, 0x0f, 0x03 },
        { 0x10, 0x13, 0x03, 0x16, 0x03, 0x18 }, { 0x11, 0x10, 0x0f, 0x13, 0x05, 0x04 }, { 0x0f, 0x11, 0x01, 0x03, 0x13, 0x03 },
        { 0x01, 0x08, 0x03, 0x01, 0x03, 0x03 }, { 0x03, 0x0a, 0x0f, 0x03, 0x0f, 0x03 } };

constexpr u8 genders[8][6] = { { 0, 1, 1, 0, 0, 1 }, { 2, 1, 0, 0, 1, 0 }, { 0, 1, 0, 1, 0, 1 }, { 2, 1, 1, 1, 0, 0 },
                               { 0, 0, 0, 0, 0, 1 }, { 2, 1, 2, 0, 2, 1 }, { 2, 0, 0, 1, 1, 0 }, { 1, 0, 1, 0, 1, 0 } };

constexpr u8 genderRatios[8][6]
    = { { 0x1f, 0x7f, 0x7f, 0x7f, 0xbf, 0x7f }, { 0xff, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f }, { 0x1f, 0x3f, 0x7f, 0x7f, 0x7f, 0x7f },
        { 0xff, 0xbf, 0x7f, 0x7f, 0x1f, 0x7f }, { 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x7f }, { 0xff, 0x7f, 0xff, 0x7f, 0xff, 0x7f },
        { 0xff, 0x1f, 0x3f, 0x7f, 0x7f, 0x3f }, { 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f } };

ColoSeedSearcher::ColoSeedSearcher(const std::vector<u32> &criteria) : SeedSearcher(criteria)
{
}

void ColoSeedSearcher::startSearch(int threads)
{
    searching = true;
    threads = 1;

    std::vector<std::future<void>> threadContainer;

    u32 split = 0x10000 / threads;
    u32 start = 0;
    for (int i = 0; i < threads; i++)
    {
        if (i == threads - 1)
        {
            threadContainer.emplace_back(std::async(std::launch::async, [=] { search(start, 0x10000); }));
        }
        else
        {
            threadContainer.emplace_back(std::async(std::launch::async, [=] { search(start, start + split); }));
        }
        start += split;
    }

    for (int i = 0; i < threads; i++)
    {
        threadContainer[i].wait();
    }

    std::sort(results.begin(), results.end());
    results.erase(std::unique(results.begin(), results.end()), results.end());
}

void ColoSeedSearcher::startSearch(int threads, const std::vector<u32> &seeds)
{
    searching = true;

    if (seeds.size() < threads)
    {
        threads = seeds.size();
    }

    std::vector<std::future<void>> threadContainer;

    size_t split = seeds.size() / threads;
    size_t start = 0;
    for (int i = 0; i < threads; i++)
    {
        if (i == threads - 1)
        {
            threadContainer.emplace_back(std::async(std::launch::async, [=] { search(seeds.cbegin() + start, seeds.cend()); }));
        }
        else
        {
            threadContainer.emplace_back(std::async(std::launch::async, [=] { search(seeds.cbegin() + start, seeds.cbegin() + split); }));
        }
        start += split;
    }

    for (int i = 0; i < threads; i++)
    {
        threadContainer[i].wait();
    }

    std::sort(results.begin(), results.end());
    results.erase(std::unique(results.begin(), results.end()), results.end());
}

void ColoSeedSearcher::search(u32 start, u32 end)
{
    for (u32 low = start; low < end; low++)
    {
        for (u32 high = criteria[0]; high < 0x10000; high += 8)
        {
            if (!searching)
            {
                return;
            }

            // Mimic no duplicate enemy and trainer party
            XDRNGR reverse((high << 16) | low);
            while ((reverse.nextUShort() & 7) == criteria[0]) { }

            XDRNG rng(reverse.next());
            if (searchSeed(rng))
            {
                std::lock_guard<std::mutex> lock(mutex);
                results.emplace_back(rng.getSeed());
            }

            progress++;
        }
    }
}

void ColoSeedSearcher::search(const std::vector<u32>::const_iterator &start, const std::vector<u32>::const_iterator &end)
{
    for (auto it = start; it != end; it++)
    {
        if (!searching)
        {
            return;
        }

        XDRNG rng(*it);
        if (searchSeed(rng))
        {
            std::lock_guard<std::mutex> lock(mutex);
            results.emplace_back(rng.getSeed());
        }

        progress++;
    }
}

bool ColoSeedSearcher::searchSeed(XDRNG &rng)
{
    u8 enemyIndex = rng.nextUShort() & 7;
    u8 playerIndex;
    do
    {
        playerIndex = rng.nextUShort() & 7;
    } while (enemyIndex == playerIndex);

    if (playerIndex != criteria[0])
    {
        return false;
    }

    u16 tid = rng.nextUShort();
    u16 sid = rng.nextUShort();
    u16 tsv = (tid ^ sid) >> 3;
    for (u8 i = 0; i < 6; i++)
    {
        rng.advance(2); // Fake PID

        rng.advance(2); // IVs

        rng.next(); // Ability

        generatePokemon(rng, tsv, natures[enemyIndex][i], genders[enemyIndex][i], genderRatios[enemyIndex][i]);
    }

    u8 playerName = rng.nextUShort() % 3;
    if (playerName != criteria[1])
    {
        return false;
    }

    tid = rng.nextUShort();
    sid = rng.nextUShort();
    tsv = (tid ^ sid) >> 3;
    for (u8 i = 0; i < 6; i++)
    {
        rng.advance(2); // Fake PID

        rng.advance(2); // IVs

        rng.next(); // Ability

        generatePokemon(rng, tsv, natures[playerIndex][i], genders[playerIndex][i], genderRatios[playerIndex][i]);
    }

    return true;
}

void ColoSeedSearcher::generatePokemon(XDRNG &rng, u16 tsv, u8 nature, u8 gender, u8 genderRatio)
{
    while (true)
    {
        u16 high = rng.nextUShort();
        u16 low = rng.nextUShort();

        if (genderRatio != 0xff)
        {
            u8 pidGender = (low & 0xff) < genderRatio;
            if (pidGender != gender)
            {
                continue;
            }
        }

        u32 pid = (high << 16) | low;
        if (pid % 25 != nature)
        {
            continue;
        }

        u16 psv = (high ^ low) >> 3;
        if (psv != tsv)
        {
            break;
        }
    }
}
