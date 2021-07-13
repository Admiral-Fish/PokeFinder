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

#include "IDGenerator5.hpp"
#include <Core/RNG/LCRNG64.hpp>

IDGenerator5::IDGenerator5(u32 initialAdvances, u32 maxAdvances, const IDFilter &filter) : IDGenerator(initialAdvances, maxAdvances, filter)
{
}

std::vector<IDState5> IDGenerator5::generate(u64 seed, u32 pid, bool checkPID)
{
    std::vector<IDState5> states;

    BWRNG rng(seed);
    rng.advance(initialAdvances);

    bool pidBit = (pid >> 31) ^ (pid & 1);
    u16 psv = (pid >> 16) ^ (pid & 0xffff);
    u16 xorPSV = psv ^ 0x8000;

    //cnt starts from 1 because the game forces
    //you to enter in the name insertion screen
    //at least once, so prng advances +1 at the
    //Yes/No screen after you click A on OK
    for (u32 cnt = 1; cnt <= maxAdvances; cnt++)
    {
        u32 rand = rng.nextUInt(0xffffffff);
        u16 tid = rand & 0xffff;
        u16 sid = rand >> 16;

        IDState5 state(initialAdvances + cnt, tid, sid);
        state.setInitialAdvances(initialAdvances);

        if (filter.compare(state))
        {
            if (checkPID)
            {
                bool idBit = (tid & 1) ^ (sid & 1);

                // Check if PID will be modified by the tid/sid combo
                u16 actualPSV = (idBit ^ pidBit) ? xorPSV : psv;
                if ((actualPSV >> 3) == state.getTSV())
                {
                    state.setSeed(seed);
                    states.emplace_back(state);
                }
            }
            else
            {
                state.setSeed(seed);
                states.emplace_back(state);
            }
        }
    }

    return states;
}

void IDGenerator5::setInitialAdvances(u32 initialAdvances)
{
    this->initialAdvances = initialAdvances;
}
