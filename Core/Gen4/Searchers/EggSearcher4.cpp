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

#include "EggSearcher4.hpp"
#include <Core/Enum/Method.hpp>
#include <Core/Gen4/Generators/EggGenerator4.hpp>
#include <Core/Gen4/States/EggState4.hpp>

EggSearcher4::EggSearcher4(u32 minDelay, u32 maxDelay, const Profile4 &profile) :
    Searcher(Method::None, profile), maxDelay(maxDelay), minDelay(minDelay)
{
}

void EggSearcher4::startSearch(const EggGenerator4 &generator)
{
    searching = true;
    u16 total = 0;

    for (u16 ab = 0; ab < 256; ab++)
    {
        for (u8 cd = 0; cd < 24; cd++)
        {
            for (u32 efgh = minDelay; efgh <= maxDelay; efgh++)
            {
                if (!searching)
                {
                    return;
                }

                if (total > 10000)
                {
                    progress = static_cast<int>(256 * 24 * (maxDelay - minDelay + 1));
                    return;
                }

                u32 seed = static_cast<u32>((ab << 24) | (cd << 16)) + efgh;

                auto states = generator.generate(seed, seed);
                if (!states.empty())
                {
                    std::lock_guard<std::mutex> lock(mutex);
                    results.reserve(results.capacity() + states.size());
                    for (const auto &state : states)
                    {
                        results.emplace_back(seed, state);
                    }
                }

                total += states.size();
                progress++;
            }
        }
    }
}
