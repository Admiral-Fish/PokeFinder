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
#include <Core/Parents/Filters/StateFilter.hpp>
#include <Core/Parents/States/State.hpp>
#include <Core/RNG/LCRNG.hpp>

StaticGenerator3::StaticGenerator3(u32 initialAdvances, u32 maxAdvances, u16 tid, u16 sid, u8 genderRatio, Method method,
                                   const StateFilter &filter) :
    StaticGenerator(initialAdvances, maxAdvances, tid, sid, genderRatio, method, filter)
{
}

std::vector<State> StaticGenerator3::generate(u32 seed) const
{
    switch (method)
    {
    case Method::Method1:
    case Method::Method2:
    case Method::Method4:
        return generateMethod124(seed);
    case Method::Method1Reverse:
        return generateMethod1Reverse(seed);
    default:
        return std::vector<State>();
    }
}

std::vector<State> StaticGenerator3::generateMethod124(u32 seed) const
{
    std::vector<State> states;

    PokeRNG rng(seed);
    rng.advance(initialAdvances + offset);

    // Method 1 [SEED] [PID] [PID] [IVS] [IVS]
    // Method 2 [SEED] [PID] [PID] [BLANK] [IVS] [IVS]
    // Method 4 [SEED] [PID] [PID] [IVS] [BLANK] [IVS]

    for (u32 cnt = 0; cnt <= maxAdvances; cnt++, rng.next())
    {
        State state(cnt + initialAdvances);
        PokeRNG go(rng.getSeed());

        u16 low = go.nextUShort();
        u16 high = go.nextUShort();

        go.advance(method == Method::Method2);
        u16 iv1 = go.nextUShort();
        go.advance(method == Method::Method4);
        u16 iv2 = go.nextUShort();

        state.setPID(high, low);
        state.setAbility(low & 1);
        state.setGender(low & 255, genderRatio);
        state.setNature(state.getPID() % 25);
        state.setShiny<8>(tsv, high ^ low);

        state.setIVs(iv1, iv2);
        state.calculateHiddenPower();

        if (filter.compareState(state))
        {
            states.emplace_back(state);
        }
    }

    return states;
}

std::vector<State> StaticGenerator3::generateMethod1Reverse(u32 seed) const
{
    std::vector<State> states;

    PokeRNG rng(seed);
    rng.advance(initialAdvances + offset);

    // Method 1 Reverse [SEED] [PID] [PID] [IVS] [IVS]

    for (u32 cnt = 0; cnt <= maxAdvances; cnt++, rng.next())
    {
        State state(cnt + initialAdvances);
        PokeRNG go(rng.getSeed());

        u16 high = go.nextUShort();
        u16 low = go.nextUShort();
        u16 iv1 = go.nextUShort();
        u16 iv2 = go.nextUShort();

        state.setPID(high, low);
        state.setAbility(low & 1);
        state.setGender(low & 255, genderRatio);
        state.setNature(state.getPID() % 25);
        state.setShiny<8>(tsv, high ^ low);

        state.setIVs(iv1, iv2);
        state.calculateHiddenPower();

        if (filter.compareState(state))
        {
            states.emplace_back(state);
        }
    }

    return states;
}
