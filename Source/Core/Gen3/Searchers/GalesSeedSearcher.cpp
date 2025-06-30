/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2024 by Admiral_Fish, bumba, and EzPzStreamz
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
#include <thread>

constexpr u16 enemyHPStat[5][2] = { { 290, 310 }, { 290, 270 }, { 290, 250 }, { 320, 270 }, { 270, 230 } };

constexpr u16 playerHPStat[5][2] = { { 322, 340 }, { 310, 290 }, { 210, 620 }, { 320, 230 }, { 310, 310 } };

/**
 * @brief Generates EVs for a pokemon
 *
 * @param rng Starting PRNG state
 *
 * @return EV for the HP stat
 */
static u8 generateEVs(XDRNG &rng)
{
    u8 evs[6] = { 0, 0, 0, 0, 0, 0 };
    u16 sum = 0;

    for (u8 i = 0; i <= 100; i++)
    {
        for (u8 &ev : evs)
        {
            ev += rng.nextUShort(256);
            sum += ev;
        }

        if (sum == 510)
        {
            return evs[0];
        }
        else if (490 < sum && sum < 530)
        {
            break;
        }
        else if (510 < sum && i != 100)
        {
            std::memset(evs, 0, sizeof(evs));
            sum = 0;
        }
    }

    while (sum != 510)
    {
        for (u8 &ev : evs)
        {
            if (sum < 510 && ev < 255)
            {
                ev++;
                sum++;
            }
            else if (sum > 510 && ev != 0)
            {
                ev--;
                sum--;
            }
        }
    }

    return evs[0];
}

/**
 *  @brief Generates a pokemon
 *
 *  @param rng Starting PRNG state
 *
 *  @return Pokemon HP IV
 */
static u8 generatePokemon(XDRNG &rng, u16 tsv)
{
    // Temp PID
    rng.advance(2);

    u8 hp = rng.nextUShort(32);

    // Other IV Call / Ability
    rng.advance(2);

    u16 psv;
    do
    {
        psv = rng.nextUShort() ^ rng.nextUShort();
    } while ((psv ^ tsv) < 8);

    return hp;
}

GalesSeedSearcher::GalesSeedSearcher(const GalesCriteria &criteria) : criteria(criteria)
{
}

void GalesSeedSearcher::startSearch(int threads)
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

void GalesSeedSearcher::startSearch(const std::vector<u32> &seeds)
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

void GalesSeedSearcher::search(u32 start, u32 end)
{
    std::vector<u32> seeds;
    for (u32 low = start; low < end; low++, progress++)
    {
        for (u32 high = criteria.playerIndex; high < 0x10000; high += 5)
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

bool GalesSeedSearcher::searchSeed(XDRNG &rng) const
{
    rng.next();
    u8 playerIndex = rng.nextUShort(5);
    if (playerIndex != criteria.playerIndex)
    {
        return false;
    }

    u8 enemyIndex = rng.nextUShort(5);
    if (enemyIndex != criteria.enemyIndex)
    {
        return false;
    }
    rng.next();

    u16 tsv = rng.nextUShort() ^ rng.nextUShort();
    for (u8 i = 0; i < 2; i++)
    {
        u8 hpIV = generatePokemon(rng, tsv);
        u16 hp = (generateEVs(rng) >> 2) + hpIV + enemyHPStat[enemyIndex][i];
        if (hp != criteria.enemyHP[i])
        {
            return false;
        }
    }
    rng.next();

    tsv = rng.nextUShort() ^ rng.nextUShort();
    for (u8 i = 0; i < 2; i++)
    {
        u8 hpIV = generatePokemon(rng, tsv);
        u16 hp = (generateEVs(rng) >> 2) + hpIV + playerHPStat[playerIndex][i];
        if (hp != criteria.playerHP[i])
        {
            return false;
        }
    }

    return true;
}

bool GalesSeedSearcher::searchSeedSkip(XDRNG &rng) const
{
    u8 enemyIndex = rng.nextUShort(5);
    if (enemyIndex != criteria.enemyIndex)
    {
        return false;
    }
    rng.next();

    u16 tsv = rng.nextUShort() ^ rng.nextUShort();
    for (u8 i = 0; i < 2; i++)
    {
        u8 hpIV = generatePokemon(rng, tsv);
        u16 hp = (generateEVs(rng) >> 2) + hpIV + enemyHPStat[enemyIndex + 5][i];
        if (hp != criteria.enemyHP[i])
        {
            return false;
        }
    }
    rng.next();

    tsv = rng.nextUShort() ^ rng.nextUShort();
    for (u8 i = 0; i < 2; i++)
    {
        u8 hpIV = generatePokemon(rng, tsv);
        u16 hp = (generateEVs(rng) >> 2) + hpIV + playerHPStat[criteria.playerIndex][i];
        if (hp != criteria.playerHP[i])
        {
            return false;
        }
    }

    return true;
}
