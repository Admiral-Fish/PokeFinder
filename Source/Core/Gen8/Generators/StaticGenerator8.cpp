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

#include "StaticGenerator8.hpp"
#include <Core/Enum/Method.hpp>
#include <Core/RNG/RNGList.hpp>
#include <Core/RNG/Xoroshiro.hpp>
#include <Core/RNG/Xorshift.hpp>

StaticGenerator8::StaticGenerator8(u32 initialAdvances, u32 maxAdvances, u16 tid, u16 sid, u8 genderRatio,
                                           const StateFilter &filter) :
    StaticGenerator(initialAdvances, maxAdvances, tid, sid, genderRatio, Method::None, filter)
{
}

std::vector<StaticState> StaticGenerator8::generate(u64 seed0, u64 seed1) const
{
    Xorshift rng(seed0, seed1);
    rng.advance(initialAdvances + offset);

    RNGList<u32, Xorshift, 64, 0> rngList(rng);

    std::vector<StaticState> states;
    for (u32 cnt = 0; cnt <= maxAdvances; cnt++, rngList.advanceState())
    {
        StaticState state(initialAdvances + cnt);
    }

    return states;
}

std::vector<StaticState> StaticGenerator8::generateRoamer(u64 seed0, u64 seed1) const
{
    Xorshift rng(seed0, seed1);
    rng.advance(initialAdvances + offset);

    std::vector<StaticState> states;
    for (u32 cnt = 0; cnt <= maxAdvances; cnt++)
    {
        StaticState state(initialAdvances + cnt);
        XoroshiroBDSP gen(rng.next());

        // Roamers are Cresselia and Mesprit
        // Neither are shiny locked
        u32 sidtid = gen.next(0xffffffff);
        u32 pid = gen.next(0xffffffff);
        
        u16 fakeTSV = (sidtid >> 16) ^ (sidtid & 0xffff);
        u16 psv = (pid >> 16) ^ (pid & 0xffff);
        if ((fakeTSV ^ psv) < 16)
        {
            // Modify PID to trainer TID/SID

        }
        state.setPID(pid);
        state.setShiny<16>(fakeTSV, psv);

        for (int i = 0; i < 6;i++)
        {
            state.setIV(i, 255);
        }

        // Assign 3 31 IVs
        for (int i = 0; i < 3; )
        {
            u8 index = gen.next(6);
            if (state.getIV(index) == 255)
            {
                state.setIV(index, 31);
                i++;
            }
        }

        for (int i = 0; i < 6; i++)
        {
            if (state.getIV(i) == 255)
            {
                state.setIV(i, gen.next(32));
            }
        }

        // No HA possible for roamers
        state.setAbility(gen.next(2));

        if (filter.comparePID(state) && filter.compareIV(state))
        {
            states.emplace_back(state);
        }
    }

    return states;
}
