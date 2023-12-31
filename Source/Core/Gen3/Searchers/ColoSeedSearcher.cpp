/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2023 by Admiral_Fish, bumba, and EzPzStreamz
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
#include <thread>

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

ColoSeedSearcher::ColoSeedSearcher(const ColoCriteria &criteria) : SeedSearcher(criteria)
{
}

void ColoSeedSearcher::startSearch(int threads)
{
    searching = true;

    auto *threadContainer = new std::thread[threads];

    u32 split = 0x10000 / threads;
    u32 start = 0;
    for (int i = 0; i < threads; i++, start += split)
    {
        if (i == threads - 1)
        {
            threadContainer[i] = std::thread([=] { search(start, 0x10000); });
        }
        else
        {
            threadContainer[i] = std::thread([=] { search(start, start + split); });
        }
    }

    for (int i = 0; i < threads; i++)
    {
        threadContainer[i].join();
    }

    delete[] threadContainer;

    std::sort(results.begin(), results.end());
    results.erase(std::unique(results.begin(), results.end()), results.end());
}

void ColoSeedSearcher::startSearch(const std::vector<u32> &seeds)
{
    searching = true;

    for (u32 seed : seeds)
    {
        if (!searching)
        {
            return;
        }

        XDRNG rng(seed);
        if (searchSeed(rng))
        {
            results.emplace_back(rng.getSeed());
        }

        progress++;
    }

    std::sort(results.begin(), results.end());
    results.erase(std::unique(results.begin(), results.end()), results.end());
}

void ColoSeedSearcher::generatePokemon(XDRNG &rng, u16 tsv, u8 nature, u8 gender, u8 genderRatio) const
{
    // Fake PID / IVs / Ability
    rng.advance(5);

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

        if ((high ^ low ^ tsv) >= 8)
        {
            break;
        }
    }
}

void ColoSeedSearcher::search(u32 start, u32 end)
{
    std::vector<u32> seeds;
    for (u32 low = start; low < end; low++, progress++)
    {
        for (u32 high = criteria.lead; high < 0x10000; high += 8)
        {
            if (!searching)
            {
                return;
            }

            XDRNG rng((high << 16) | low);
            if (searchSeedSkip(rng))
            {
                seeds.emplace_back(rng.getSeed());
            }
        }
    }

    std::lock_guard<std::mutex> lock(mutex);
    results.insert(results.end(), seeds.begin(), seeds.end());
}

bool ColoSeedSearcher::searchSeed(XDRNG &rng) const
{
    u8 enemyLead = rng.nextUShort(8);
    u8 playerLead;
    do
    {
        playerLead = rng.nextUShort(8);
    } while (enemyLead == playerLead);

    if (playerLead != criteria.lead)
    {
        return false;
    }

    u16 tsv = rng.nextUShort() ^ rng.nextUShort();
    for (u8 i = 0; i < 6; i++)
    {
        generatePokemon(rng, tsv, natures[enemyLead][i], genders[enemyLead][i], genderRatios[enemyLead][i]);
    }

    u8 playerName = rng.nextUShort(3);
    if (playerName != criteria.trainer)
    {
        return false;
    }

    tsv = rng.nextUShort() ^ rng.nextUShort();
    for (u8 i = 0; i < 6; i++)
    {
        generatePokemon(rng, tsv, natures[playerLead][i], genders[playerLead][i], genderRatios[playerLead][i]);
    }

    return true;
}

bool ColoSeedSearcher::searchSeedSkip(XDRNG &rng) const
{
    u8 enemyLead;
    XDRNGR reverse(rng);
    do
    {
        enemyLead = reverse.nextUShort(8);
    } while (enemyLead == criteria.lead);

    u16 tsv = rng.nextUShort() ^ rng.nextUShort();
    for (u8 i = 0; i < 6; i++)
    {
        generatePokemon(rng, tsv, natures[enemyLead][i], genders[enemyLead][i], genderRatios[enemyLead][i]);
    }

    u8 playerName = rng.nextUShort(3);
    if (playerName != criteria.trainer)
    {
        return false;
    }

    tsv = rng.nextUShort() ^ rng.nextUShort();
    for (u8 i = 0; i < 6; i++)
    {
        generatePokemon(rng, tsv, natures[criteria.lead][i], genders[criteria.lead][i], genderRatios[criteria.lead][i]);
    }

    return true;
}
