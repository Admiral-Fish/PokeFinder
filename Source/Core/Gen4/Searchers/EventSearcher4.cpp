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

#include "EventSearcher4.hpp"
#include <Core/Enum/Method.hpp>
#include <Core/Gen4/States/State4.hpp>
#include <Core/Parents/PersonalLoader.hpp>
#include <Core/Parents/Searchers/StaticSearcher.hpp>
#include <Core/RNG/LCRNG.hpp>
#include <Core/RNG/LCRNGReverse.hpp>

EventSearcher4::EventSearcher4(u32 minAdvance, u32 maxAdvance, u32 minDelay, u32 maxDelay, const Profile4 &profile,
                               const StateFilter &filter) :
    Searcher(Method::None, profile), maxAdvance(maxAdvance), minAdvance(minAdvance), maxDelay(maxDelay), minDelay(minDelay), filter(filter)
{
}

void EventSearcher4::startSearch(const std::array<u8, 6> &min, const std::array<u8, 6> &max, u16 species, u8 nature, u8 level)
{
    searching = true;

    for (u8 hp = min[0]; hp <= max[0]; hp++)
    {
        for (u8 atk = min[1]; atk <= max[1]; atk++)
        {
            for (u8 def = min[2]; def <= max[2]; def++)
            {
                for (u8 spa = min[3]; spa <= max[3]; spa++)
                {
                    for (u8 spd = min[4]; spd <= max[4]; spd++)
                    {
                        for (u8 spe = min[5]; spe <= max[5]; spe++)
                        {
                            if (!searching)
                            {
                                return;
                            }

                            auto states = search(hp, atk, def, spa, spd, spe, species, nature, level);

                            std::lock_guard<std::mutex> guard(mutex);
                            results.insert(results.end(), states.begin(), states.end());
                            progress++;
                        }
                    }
                }
            }
        }
    }
}

std::vector<SearcherState4> EventSearcher4::search(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe, u16 species, u8 nature, u8 level) const
{
    std::vector<SearcherState4> states;
    std::array<u8, 6> ivs = { hp, atk, def, spa, spd, spe };
    const PersonalInfo *info = PersonalLoader::getPersonal(profile.getVersion(), species);

    SearcherState4 state(0, 0, ivs, 0, 0, level, nature, 0, info);
    if (!filter.compareHiddenPower(state.getHiddenPower()))
    {
        return states;
    }

    u32 seeds[6];
    int size = LCRNGReverse::recoverPokeRNGIV(hp, atk, def, spa, spd, spe, seeds, Method::Method1);
    for (int i = 0; i < size; i++)
    {
        PokeRNGR rng(seeds[i]);
        state.setSeed(rng.next());
        states.emplace_back(state);

        state.setSeed(state.getSeed() ^ 0x80000000);
        states.emplace_back(state);
    }

    return searchInitialSeeds(states);
}

std::vector<SearcherState4> EventSearcher4::searchInitialSeeds(const std::vector<SearcherState4> &results) const
{
    std::vector<SearcherState4> states;

    for (SearcherState4 result : results)
    {
        PokeRNGR rng(result.getSeed(), minAdvance);
        u32 seed = rng.getSeed();
        for (u32 cnt = minAdvance; cnt <= maxAdvance; cnt++)
        {
            u8 hour = (seed >> 16) & 0xFF;
            u16 delay = seed & 0xFFFF;

            // Check if seed matches a valid gen 4 format
            if (hour < 24 && delay >= minDelay && delay <= maxDelay)
            {
                result.setSeed(seed);
                result.setAdvances(cnt);
                states.emplace_back(result);
            }

            seed = rng.next();
        }
    }

    return states;
}
