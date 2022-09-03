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

#include "StaticSearcher3.hpp"
#include <Core/Enum/Method.hpp>
#include <Core/Gen3/States/State3.hpp>
#include <Core/Parents/PersonalInfo.hpp>
#include <Core/Parents/StaticTemplate.hpp>
#include <Core/RNG/LCRNG.hpp>

StaticSearcher3::StaticSearcher3(u16 tid, u16 sid, Game version, Method method, Lead lead, const StateFilter3 &filter) :
    StaticSearcher(tid, sid, version, method, lead, filter),
    progress(0),
    cache(method),
    searching(false),
    ivAdvance(method == Method::Method2)
{
}

void StaticSearcher3::startSearch(const std::array<u8, 6> &min, const std::array<u8, 6> &max, const StaticTemplate *staticTemplate)
{
    searching = true;

    u8 level = staticTemplate->getLevel();
    const PersonalInfo *info = staticTemplate->getInfo();

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

                            auto states = search(hp, atk, def, spa, spd, spe, level, info);

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

void StaticSearcher3::cancelSearch()
{
    searching = false;
}

int StaticSearcher3::getProgress() const
{
    return progress;
}

std::vector<SearcherState3> StaticSearcher3::getResults()
{
    std::lock_guard<std::mutex> guard(mutex);
    auto data = std::move(results);
    return data;
}

std::vector<SearcherState3> StaticSearcher3::search(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe, u8 level, const PersonalInfo *info) const
{
    std::vector<SearcherState3> states;

    std::array<u8, 6> ivs = { hp, atk, def, spa, spd, spe };

    u32 seeds[6];
    int size = cache.recoverPokeRNGIV(hp, atk, def, spa, spd, spe, seeds);
    for (int i = 0; i < size; i++)
    {
        PokeRNGR rng(seeds[i]);
        rng.advance(ivAdvance);

        u16 high = rng.nextUShort();
        u16 low = rng.nextUShort();

        SearcherState3 state(rng.next(), high, low, ivs, tsv, level, info);
        if (filter.compareState(state))
        {
            states.emplace_back(state);
        }
    }

    return states;
}
