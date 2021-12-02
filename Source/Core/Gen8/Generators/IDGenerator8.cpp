/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2021 by Admiral_Fish, bumba, and EzPzStreamz
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

#include "IDGenerator8.hpp"
#include <Core/RNG/Xorshift.hpp>

IDGenerator8::IDGenerator8(u32 initialAdvances, u32 maxAdvances, const IDFilter8 &filter) : IDGenerator(initialAdvances, maxAdvances, filter)
{
    this->filter = filter;
}

std::vector<IDState8> IDGenerator8::generate(u64 seed0, u64 seed1)
{
    Xorshift rng(seed0, seed1);
    rng.advance(initialAdvances);

    std::vector<IDState8> states;
    for (u32 cnt = 0; cnt < maxAdvances; cnt++)
    {
        u32 sidtid = rng.next();
        u16 tid = sidtid & 0xffff;
        u16 sid = sidtid >> 16;
        u32 g8tid = sidtid % 1000000;

        IDState8 state(initialAdvances + cnt, tid, sid, g8tid);
        if (filter.compare(state))
        {
            states.emplace_back(state);
        }
    }

    return states;
}
