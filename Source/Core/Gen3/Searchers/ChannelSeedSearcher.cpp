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

#include "ChannelSeedSearcher.hpp"
#include <algorithm>
#include <thread>

ChannelSeedSearcher::ChannelSeedSearcher(const std::vector<u8> &criteria) : criteria(criteria)
{
}

int ChannelSeedSearcher::getProgress() const
{
    return progress >> 1;
}

void ChannelSeedSearcher::startSearch(int threads)
{
    searching = true;

    auto *threadContainer = new std::thread[threads];

    u32 split = 0xBFFFFFFE / threads;
    u32 start = 0x40000001;
    for (int i = 0; i < threads; i++, start += split)
    {
        if (i == threads - 1)
        {
            threadContainer[i] = std::thread([=] { search(start, 0xffffffff); });
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

void ChannelSeedSearcher::search(u32 start, u32 end)
{
    std::vector<u32> seeds;
    for (u32 seed = start; seed < end; seed++, progress++)
    {
        if (!searching)
        {
            return;
        }

        XDRNG rng(seed);
        if (searchSeed(rng))
        {
            seeds.emplace_back(rng.getSeed());
        }
    }

    std::lock_guard<std::mutex> lock(mutex);
    results.insert(results.end(), seeds.begin(), seeds.end());
}

bool ChannelSeedSearcher::searchSeed(XDRNG &rng) const
{
    for (u8 compare : criteria)
    {
        u8 mask = 0;
        u8 shift = 0;
        u8 pattern = 0;

        while (mask != 7)
        {
            u8 num = rng.next() >> 30;
            if (shift == 0)
            {
                if ((compare > 20 && num != 0) || (compare < 20 && num == 0))
                {
                    return false;
                }

                if (num == 0)
                {
                    shift++;
                }
            }

            if ((mask & 1) == 0 && num == 1)
            {
                pattern += (num << shift++);
                mask |= 1;
            }
            else if ((mask & 2) == 0 && num == 2)
            {
                pattern += (num << shift++);
                mask |= 2;
            }
            else if ((mask & 4) == 0 && num == 3)
            {
                pattern += (num << shift++);
                mask |= 4;
            }
        }

        if (pattern != compare)
        {
            return false;
        }
    }

    return true;
}
