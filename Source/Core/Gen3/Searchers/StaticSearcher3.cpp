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

#include "StaticSearcher3.hpp"
#include <Core/Enum/Lead.hpp>
#include <Core/Enum/Method.hpp>
#include <Core/Parents/PersonalInfo.hpp>
#include <Core/Parents/States/State.hpp>
#include <Core/Parents/StaticTemplate.hpp>
#include <Core/RNG/LCRNG.hpp>
#include <Core/RNG/LCRNGReverse.hpp>
#include <Core/Util/Utilities.hpp>

StaticSearcher3::StaticSearcher3(Method method, const Profile3 &profile, const StateFilter &filter) :
    StaticSearcher(method, Lead::None, profile, filter), progress(0), ivAdvance(method == Method::Method2), searching(false)
{
}

void StaticSearcher3::startSearch(const std::array<u8, 6> &min, const std::array<u8, 6> &max, const StaticTemplate *staticTemplate)
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

                            auto states = search(hp, atk, def, spa, spd, spe, staticTemplate);

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

std::vector<SearcherState> StaticSearcher3::getResults()
{
    std::lock_guard<std::mutex> guard(mutex);
    auto data = std::move(results);
    return data;
}

std::vector<SearcherState> StaticSearcher3::search(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe,
                                                   const StaticTemplate *staticTemplate) const
{
    std::vector<SearcherState> states;
    std::array<u8, 6> ivs = { hp, atk, def, spa, spd, spe };
    const PersonalInfo *info = staticTemplate->getInfo();

    u32 seeds[6];
    int size = LCRNGReverse::recoverPokeRNGIV(hp, atk, def, spa, spd, spe, seeds, method);
    for (int i = 0; i < size; i++)
    {
        PokeRNGR rng(seeds[i]);
        if (ivAdvance)
        {
            rng.next();
        }

        u32 pid = rng.nextUShort() << 16;
        pid |= rng.nextUShort();

        u8 nature = pid % 25;
        if (!filter.compareNature(nature))
        {
            continue;
        }

        SearcherState state(rng.next(), pid, ivs, pid & 1, Utilities::getGender(pid, info), staticTemplate->getLevel(), nature,
                            Utilities::getShiny(pid, tsv), info);
        if (filter.compareState(state))
        {
            states.emplace_back(state);
        }
    }

    return states;
}
