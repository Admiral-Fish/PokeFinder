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

#include "IVCacheSearcher.hpp"
#include <Core/RNG/MT.hpp>
#include <Core/RNG/RNGList.hpp>
#include <algorithm>
#include <fstream>
#include <variant>

static u8 gen(MT &rng)
{
    return rng.next() >> 27;
}

template <typename Type>
static void write(std::ofstream &file, Type val)
{
    file.write(reinterpret_cast<char *>(&val), sizeof(val));
}

IVCacheSearcher::IVCacheSearcher(u32 initialAdvances, u32 maxAdvances) :
    SearcherBase<std::vector<u32>>(), initialAdvances(initialAdvances), maxAdvances(maxAdvances)
{
    entralink.resize(maxAdvances + 5);
    results.resize(maxAdvances + 3);
    roamer.resize(maxAdvances + 1);
}

void IVCacheSearcher::startSearch(int threads)
{
    activeThreads.store(threads);
    for (int i = 0; i < threads; i++)
    {
        threadContainer.emplace_back([this] {
            search(0x0, 0xffffffff);
            activeThreads.fetch_sub(1);
        });
    }
}

void IVCacheSearcher::writeResults(std::string_view file)
{
    std::ofstream stream(file.data(), std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);
    if (stream.is_open())
    {
        // Write magic identifier: CRC32 of "IVCache"
        write(stream, 0xd08cb7c0);

        // Write cache advances
        write(stream, initialAdvances);
        write(stream, maxAdvances);

        // Write seed sizes
        for (int i = 0; i < entralink.size(); i++)
        {
            std::ranges::sort(entralink[i]);
            write<u32>(stream, entralink[i].size());
        }

        for (int i = 0; i < results.size(); i++)
        {
            std::ranges::sort(results[i]);
            write<u32>(stream, results[i].size());
        }

        for (int i = 0; i < roamer.size(); i++)
        {
            std::ranges::sort(roamer[i]);
            write<u32>(stream, roamer[i].size());
        }

        // Write seeds
        for (int i = 0; i < entralink.size(); i++)
        {
            stream.write(reinterpret_cast<char *>(entralink[i].data()), entralink[i].size() * sizeof(u32));
        }

        for (int i = 0; i < results.size(); i++)
        {
            stream.write(reinterpret_cast<char *>(results[i].data()), results[i].size() * sizeof(u32));
        }

        for (int i = 0; i < roamer.size(); i++)
        {
            stream.write(reinterpret_cast<char *>(roamer[i].data()), roamer[i].size() * sizeof(u32));
        }
    }
}

void IVCacheSearcher::search(u32 start, u32 end)
{
    while (true)
    {
        if (cancelled.load(std::memory_order_relaxed))
        {
            return;
        }

        u64 idx = index.fetch_add(1, std::memory_order_relaxed);
        if (idx > static_cast<u64>(end))
        {
            break;
        }

        u32 seed = start + idx;

        using RNGVariant = std::variant<RNGList<u8, MTFast, 32>, RNGList<u8, MT, 32, gen>>;
        RNGVariant rngList = [&]() {
            u32 size = (maxAdvances + 5) + 32;
            if (size < 227)
            {
                return RNGVariant(std::in_place_type<RNGList<u8, MTFast, 32>>, seed, 0, size, true);
            }
            else
            {
                return RNGVariant(std::in_place_type<RNGList<u8, MT, 32, gen>>, seed);
            }
        }();

        std::visit(
            [&](auto &rng) {
                for (u32 i = 0; i <= maxAdvances + 4; i++, rng.advanceState())
                {
                    // Entralink
                    rng.advance(22);
                    u8 hp = rng.next();
                    u8 atk = rng.next();
                    u8 def = rng.next();
                    u8 spa = rng.next();
                    u8 spd = rng.next();
                    u8 spe = rng.next();
                    if (hp >= 30 && def >= 30 && spd >= 30 && (atk >= 30 || spa >= 30) && (spe <= 1 || spe >= 30))
                    {
                        std::lock_guard<std::mutex> lock(mutex);
                        entralink[i].emplace_back(seed);
                    }

                    // Normal
                    if (i <= maxAdvances + 2)
                    {
                        rng.resetState();

                        hp = rng.next();
                        atk = rng.next();
                        def = rng.next();
                        spa = rng.next();
                        spd = rng.next();
                        spe = rng.next();

                        if (hp >= 30 && def >= 30 && spd >= 30 && (atk >= 30 || spa >= 30) && (spe <= 1 || spe >= 30))
                        {
                            std::lock_guard<std::mutex> lock(mutex);
                            results[i].emplace_back(seed);
                        }
                    }

                    // Roamer
                    if (i <= maxAdvances)
                    {
                        rng.resetState();
                        rng.advance(1);

                        hp = rng.next();
                        atk = rng.next();
                        def = rng.next();
                        spd = rng.next();
                        spe = rng.next();
                        spa = rng.next();

                        if (hp >= 30 && def >= 30 && spd >= 30 && (atk >= 30 || spa >= 30) && spe >= 30)
                        {
                            std::lock_guard<std::mutex> lock(mutex);
                            roamer[i].emplace_back(seed);
                        }
                    }
                }
            },
            rngList);

        progress.fetch_add(1, std::memory_order_relaxed);
    }
}
