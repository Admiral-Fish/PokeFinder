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

#include "WildGenerator8.hpp"
#include <Core/Enum/Method.hpp>
#include <Core/RNG/RNGList.hpp>
#include <Core/RNG/Xorshift.hpp>

WildGenerator8::WildGenerator8(u32 initialAdvances, u32 maxAdvances, u16 tid, u16 sid, u8 genderRatio, const StateFilter &filter) :
    WildGenerator(initialAdvances, maxAdvances, tid, sid, genderRatio, Method::Null, filter)
{
}

std::vector<WildState> WildGenerator8::generate(u64 seed0, u64 seed1) const
{
    Xorshift rng(seed0, seed1);
    rng.advance(initialAdvances + offset);

    RNGList<u32, Xorshift, 2, 0> rngList(rng);

    std::vector<WildState> states;
    for (u32 cnt = 0; cnt <= maxAdvances; cnt++, rngList.advanceState())
    {
        
    }

    return states;
}
