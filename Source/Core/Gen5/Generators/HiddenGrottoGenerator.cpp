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

#include "HiddenGrottoGenerator.hpp"
#include <Core/RNG/LCRNG64.hpp>
#include <Core/Util/Utilities.hpp>

// Game has all of these + 1, removed for simplicity
constexpr u32 grottoSlots[11] = { 0, 4, 19, 20, 24, 34, 59, 60, 64, 74, 99 };

HiddenGrottoGenerator::HiddenGrottoGenerator(u32 initialAdvances, u32 maxAdvances, u8 genderRatio, const HiddenGrottoFilter &filter) :
    initialAdvances(initialAdvances), maxAdvances(maxAdvances), genderRatio(genderRatio), filter(filter)
{
}

std::vector<HiddenGrottoState> HiddenGrottoGenerator::generate(u64 seed, bool memory)
{
    std::vector<HiddenGrottoState> states;

    BWRNG rng(seed);
    u32 initialAdvancesBW2 = Utilities::initialAdvancesBW2(seed, memory);
    rng.advance(initialAdvancesBW2);

    for (u32 cnt = 0; cnt <= maxAdvances; cnt++, rng.next())
    {
        BWRNG go(rng.getSeed());

        if (go.nextUInt(100) < 5)
        {
            u8 group = go.nextUInt(4);

            // Game does slot + 1, removed for simplicity
            u8 slotRand = go.nextUInt(100);
            u8 slot = 0;
            while (slotRand > grottoSlots[slot])
            {
                slot++;
            }

            u8 gender = go.nextUInt(100) < genderRatio;

            HiddenGrottoState state(initialAdvances + cnt, group, slot, gender);
            if (filter.compareState(state))
            {
                states.emplace_back(state);
            }
        }
    }

    return states;
}
