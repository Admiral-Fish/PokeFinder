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

#include "StaticGenerator3.hpp"
#include <Core/Enum/Method.hpp>
#include <Core/Gen3/States/State3.hpp>
#include <Core/Parents/StaticTemplate.hpp>
#include <Core/RNG/LCRNG.hpp>

StaticGenerator3::StaticGenerator3(u32 initialAdvances, u32 maxAdvances, u32 offset, u16 tid, u16 sid, Game version, Method method,
                                   Lead lead, const StateFilter3 &filter) :
    StaticGenerator(initialAdvances, maxAdvances, offset, tid, sid, version, method, lead, filter)
{
}

std::vector<GeneratorState3> StaticGenerator3::generate(u32 seed, const StaticTemplate *staticTemplate) const
{
    std::vector<GeneratorState3> states;
    const PersonalInfo *info = staticTemplate->getInfo();

    PokeRNG rng(seed);
    rng.advance(initialAdvances + offset);

    for (u32 cnt = 0; cnt <= maxAdvances; cnt++, rng.next())
    {
        PokeRNG go(rng.getSeed());

        u16 low = go.nextUShort();
        u16 high = go.nextUShort();

        u16 iv1 = go.nextUShort();
        go.advance(method == Method::Method4);
        u16 iv2 = go.nextUShort();

        GeneratorState3 state(initialAdvances + cnt, high, low, iv1, iv2, tsv, staticTemplate->getLevel(), info);
        if (filter.compareState(state))
        {
            states.emplace_back(state);
        }
    }

    return states;
}
