/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2023 by Admiral_Fish, bumba, and EzPzStreamz
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
#include <Core/Parents/States/IDState.hpp>
#include <Core/RNG/LCRNG64.hpp>
#include <Core/Util/Utilities.hpp>

IDGenerator5::IDGenerator5(u32 initialAdvances, u32 maxAdvances, u32 pid, bool checkPID, bool checkXOR, const Profile5 &profile,
                           const IDFilter &filter) :
    IDGenerator(initialAdvances, maxAdvances, filter), profile(profile), pid(pid), checkPID(checkPID), checkXOR(checkXOR)
{
}

std::vector<IDState> IDGenerator5::generate(u64 seed) const
{
    bool pidBit = (pid >> 31) ^ (pid & 1);
    u16 psv = (pid >> 16) ^ (pid & 0xffff);

    u32 advances = Utilities5::initialAdvancesID(seed, profile.getVersion());
    BWRNG rng(seed, advances + initialAdvances);

    std::vector<IDState> states;
    for (u32 cnt = 0; cnt <= maxAdvances; cnt++)
    {
        u32 rand = rng.nextUInt(0xffffffff);
        u16 tid = rand & 0xffff;
        u16 sid = rand >> 16;
        u16 tsv = (tid ^ sid) >> 3;

        IDState state(advances + initialAdvances + cnt, tid, sid, tsv);
        if (filter.compare(state))
        {
            bool shiny = (psv >> 3) == state.getTSV();

            // Check if PID is possible with TID/SID combo if Static/Wild box is checked
            if (shiny && checkXOR) // We need to do the check only if it was shiny first
            {
                bool idbit = (tid & 1) ^ (sid & 1);
                shiny = idbit == pidBit;
            }

            if (!checkPID || shiny)
            {
                states.emplace_back(state);
            }
        }
    }

    return states;
}
