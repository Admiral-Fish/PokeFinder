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

#include "IDGenerator3.hpp"
#include <Core/Parents/Filters/IDFilter.hpp>
#include <Core/RNG/LCRNG.hpp>

IDGenerator3::IDGenerator3(u32 initialAdvances, u32 maxAdvances, const IDFilter &filter) : IDGenerator(initialAdvances, maxAdvances, filter)
{
}

std::vector<IDState3> IDGenerator3::generateXDColo(u32 seed)
{
    std::vector<IDState3> states;

    XDRNG rng(seed);
    rng.advance(initialAdvances);

    for (u32 cnt = 0; cnt <= maxAdvances; cnt++, rng.next())
    {
        XDRNG go(rng.getSeed());

        u16 tid = go.nextUShort();
        u16 sid = go.nextUShort();

        IDState3 state(initialAdvances + cnt, tid, sid);

        if (filter.compare(state))
        {
            states.emplace_back(state);
        }
    }

    return states;
}

std::vector<IDState3> IDGenerator3::generateFRLGE(u16 tid)
{
    std::vector<IDState3> states;

    PokeRNG rng(tid);
    rng.advance(initialAdvances);

    for (u32 cnt = 0; cnt <= maxAdvances; cnt++)
    {
        u16 sid = rng.nextUShort();

        IDState3 state(initialAdvances + cnt, tid, sid);
        if (filter.compare(state))
        {
            states.emplace_back(state);
        }
    }

    return states;
}

std::vector<IDState3> IDGenerator3::generateRS(u32 seed)
{
    std::vector<IDState3> states;

    PokeRNG rng(seed);
    rng.advance(initialAdvances);

    for (u32 cnt = 0; cnt <= maxAdvances; cnt++, rng.next())
    {
        PokeRNG go(rng.getSeed());

        u16 sid = go.nextUShort();
        u16 tid = go.nextUShort();

        IDState3 state(initialAdvances + cnt, tid, sid);

        if (filter.compare(state))
        {
            states.emplace_back(state);
        }
    }

    return states;
}
