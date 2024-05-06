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
    StaticSearcher(method, Lead::None, profile, filter), ivAdvance(method == Method::Method2)
{
}

void StaticSearcher3::startSearch(const std::array<u8, 6> &min, const std::array<u8, 6> &max, const StaticTemplate *staticTemplate, const bool buggedRoamer)
{
    searching = true;

    for (u8 hp = min[0]; hp <= max[0]; hp++)
    {
        for (u8 atk = buggedRoamer ? min[1] & 7 : min[1]; atk <= (buggedRoamer ? max[1] & 7 : max[1]); atk++)
        {
            for (u8 def = buggedRoamer ? 0 : min[2]; def <= (buggedRoamer ? 31 : max[2]); def++)
            {
                for (u8 spa = buggedRoamer ? 0 : min[3]; spa <= (buggedRoamer ? 31 : max[3]); spa++)
                {
                    for (u8 spd = buggedRoamer ? 0 : min[4]; spd <= (buggedRoamer ? 31 : max[4]); spd++)
                    {
                        for (u8 spe = buggedRoamer ? 0 : min[5]; spe <= (buggedRoamer ? 31 : max[5]); spe++)
                        {
                            if (!searching)
                            {
                                return;
                            }

                            auto states = search(hp, atk, def, spa, spd, spe, staticTemplate, buggedRoamer);

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

void buggedRoamerIVsMatcher(std::array<u8, 6> &ivs, const bool buggedRoamer)
{
    if (buggedRoamer)
    {
        ivs[1] &= 7;
        for (int i = 2; i < 6; i++)
        {
            ivs[i] = 0;
        }
    }
}

std::vector<SearcherState> StaticSearcher3::search(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe,
                                                   const StaticTemplate *staticTemplate, const bool buggedRoamer) const
{
    std::vector<SearcherState> states;
    std::array<u8, 6> ivs = { hp, atk, def, spa, spd, spe };
    buggedRoamerIVsMatcher(ivs, buggedRoamer);
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
                            Utilities::getShiny<true>(pid, tsv), info);
        if (filter.compareState(state))
        {
            states.emplace_back(state);
        }
    }

    return states;
}
