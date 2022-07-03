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
#include <Core/Parents/States/IDState.hpp>
#include <Core/RNG/LCRNG.hpp>

IDGenerator3::IDGenerator3(u64 initialAdvances, u64 maxAdvances, const IDFilter &filter) : IDGenerator(initialAdvances, maxAdvances, filter)
{
}

std::vector<IDState> IDGenerator3::generateXDColo(u32 seed)
{
    std::vector<IDState> states;

    XDRNG rng(seed, initialAdvances);
    for (u64 cnt = 0; cnt <= maxAdvances; cnt++, rng.next())
    {
        XDRNG go(rng);

        u16 tid = go.nextUShort();
        u16 sid = go.nextUShort();
        u16 tsv = (tid ^ sid) >> 3;

        IDState state(initialAdvances + cnt, tid, sid, tsv);
        if (filter.compare(state))
        {
            states.emplace_back(state);
        }
    }

    return states;
}

std::vector<IDState> IDGenerator3::generateFRLGE(u16 tid)
{
    std::vector<IDState> states;

    PokeRNG rng(tid, initialAdvances);
    for (u64 cnt = 0; cnt <= maxAdvances; cnt++)
    {
        u16 sid = rng.nextUShort();
        u16 tsv = (tid ^ sid) >> 3;

        IDState state(initialAdvances + cnt, tid, sid, tsv);
        if (filter.compare(state))
        {
            states.emplace_back(state);
        }
    }

    return states;
}

std::vector<IDState> IDGenerator3::generateRS(u16 seed)
{
    std::vector<IDState> states;

    PokeRNG rng(seed, initialAdvances);
    for (u64 cnt = 0; cnt <= maxAdvances; cnt++, rng.next())
    {
        PokeRNG go(rng);

        u16 tid = go.nextUShort();
        u16 sid = go.nextUShort();
        u16 tsv = (tid ^ sid) >> 3;

        IDState state(initialAdvances + cnt, tid, sid, tsv);
        if (filter.compare(state))
        {
            states.emplace_back(state);
        }
    }

    return states;
}
