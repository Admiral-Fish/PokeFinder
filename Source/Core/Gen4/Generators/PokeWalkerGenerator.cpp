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

#include "PokeWalkerGenerator.hpp"
#include <Core/Enum/Method.hpp>
#include <Core/Parents/Filters/StateFilter.hpp>
#include <Core/RNG/LCRNG.hpp>
#include <Core/RNG/MT.hpp>

PokeWalkerGenerator::PokeWalkerGenerator(u32 initialAdvances, u32 maxAdvances, u16 tid, u16 sid, u8 genderRatio, Method method,
                                         const StateFilter &filter) :
    Generator(initialAdvances, maxAdvances, tid, sid, genderRatio, method, filter)
{
}

std::vector<PokeWalkerState> PokeWalkerGenerator::generate(u32 seed) const
{
    switch (method)
    {
    case Method::PokeWalkerPID:
        return generatePokeWalkerPID(seed);
    case Method::PokeWalkerIVs:
        return generatePokeWalkerIVs(seed);
    default:
        return std::vector<PokeWalkerState>();
    }
}

std::vector<PokeWalkerState> PokeWalkerGenerator::generatePokeWalkerPID(u32 seed) const
{
    std::vector<PokeWalkerState> states;

    MT mt(seed);
    mt.advance(initialAdvances);

    for (u32 cnt = 0; cnt <= maxAdvances; cnt++)
    {
        PokeWalkerState state(initialAdvances + cnt);

        u32 pid = mt.next();
        u32 correction = pid % 24;
        pid = ((tsv >> 8) ^ 255) << 24;
        correction -= pid % 25;
        pid += correction;
        
        state.setPID(pid);
        state.setAbility(pid & 1);
        state.setGender(pid & 255, genderRatio);
        state.setNature(pid % 25);
        state.setShiny<8>(tsv, (pid >> 16) ^ (pid & 0xffff));

        if (filter.comparePID(state))
        {
            states.emplace_back(state);
        }
    }

    return states;
}

std::vector<PokeWalkerState> PokeWalkerGenerator::generatePokeWalkerIVs(u32 seed) const
{
    std::vector<PokeWalkerState> states;

    PokeRNG rng(seed);
    rng.advance(initialAdvances);

    // Wondercard IVs [SEED] [IVS] [IVS]

    for (u32 cnt = 0; cnt <= maxAdvances; cnt += 2)
    {
        PokeWalkerState state(initialAdvances + cnt);

        u16 iv1 = rng.nextUShort();
        u16 iv2 = rng.nextUShort();

        state.setIVs(iv1, iv2);
        state.calculateHiddenPower();

        if (filter.compareIVs(state))
        {
            state.setSeed(iv1);
            states.emplace_back(state);
        }
    }

    return states;
}
