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

#include "GalesSeedSearcher.hpp"
#include <algorithm>
#include <cstring>
#include <future>

constexpr u16 hpStat[10][2] = { { 322, 340 }, { 310, 290 }, { 210, 620 }, { 320, 230 }, { 310, 310 },
                                { 290, 310 }, { 290, 270 }, { 290, 250 }, { 320, 270 }, { 270, 230 } };

GalesSeedSearcher::GalesSeedSearcher(const std::vector<u32> &criteria, u16 tsv) : SeedSearcher(criteria), tsv(tsv)
{
}

void GalesSeedSearcher::startSearch(int threads)
{
    searching = true;

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

void GalesSeedSearcher::startSearch(int threads, const std::vector<u32> &seeds)
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

void GalesSeedSearcher::search(u32 start, u32 end)
{
    for (u32 low = start; low < end; low++)
    {
        for (u32 high = criteria[0]; high < 0x10000; high += 5)
        {
            if (!searching)
            {
                return;
            }

            XDRNGR reverse((high << 16) | low);
            reverse.next();

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

void GalesSeedSearcher::search(const std::vector<u32>::const_iterator &start, const std::vector<u32>::const_iterator &end)
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

bool GalesSeedSearcher::searchSeed(XDRNG &rng)
{
    rng.next();

    u8 playerIndex = rng.nextUShort() % 5;
    if (playerIndex != criteria[0])
    {
        return false;
    }

    u8 enemyIndex = rng.nextUShort() % 5;
    if (enemyIndex != criteria[1])
    {
        return false;
    }
    rng.next();

    rng.advance(2); // SID/TID
    for (u8 i = 0; i < 2; i++)
    {
        rng.advance(2); // Temp PID

        u8 hpIV = rng.nextUShort() & 31;
        rng.advance(1); // Other IV Call

        rng.next(); // Ability

        generatePokemon(rng);

        u16 hp = (generateEVs(rng) >> 2) + hpIV + hpStat[enemyIndex + 5][i];
        if (hp != criteria[4 + i])
        {
            return false;
        }
    }
    rng.next();

    rng.advance(2); // SID/TID
    for (u8 i = 0; i < 2; i++)
    {
        rng.advance(2); // Temp PID

        u8 hpIV = rng.nextUShort() & 31;
        rng.advance(1); // Other IV Call

        rng.next(); // Ability

        generatePokemon(rng);

        u16 hp = (generateEVs(rng) >> 2) + hpIV + hpStat[playerIndex][i];
        if (hp != criteria[2 + i])
        {
            return false;
        }
    }

    return true;
}

void GalesSeedSearcher::generatePokemon(XDRNG &rng) const
{
    while (true)
    {
        u16 high = rng.nextUShort();
        u16 low = rng.nextUShort();

        u16 psv = (high ^ low) >> 3;
        if (psv != tsv)
        {
            break;
        }
    }
}

u8 GalesSeedSearcher::generateEVs(XDRNG &rng)
{
    u8 evs[6] = { 0, 0, 0, 0, 0, 0 };
    u16 sum = 0;

    for (u8 i = 0; i <= 100; i++)
    {
        for (u8 &ev : evs)
        {
            ev += rng.nextUShort() & 0xff;
            sum += ev;
        }

        if (sum == 510)
        {
            return evs[0];
        }

        if (490 < sum && sum < 530)
        {
            break;
        }

        if (510 < sum && i != 100)
        {
            std::memset(evs, 0, sizeof(evs));
            sum = 0;
        }
    }

    while (sum != 510)
    {
        for (u8 &ev : evs)
        {
            if (sum < 510)
            {
                if (ev < 255)
                {
                    ev++;
                    sum++;
                }
            }
            else if (sum > 510)
            {
                if (ev != 0)
                {
                    ev--;
                    sum--;
                }
            }
        }
    }

    return evs[0];
}
