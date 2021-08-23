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

HiddenGrottoGenerator::HiddenGrottoGenerator(u32 initialAdvances, u32 maxAdvances, u8 genderRatio, u8 powerLevel,
                                             const HiddenGrottoFilter &filter) :
    initialAdvances(initialAdvances), maxAdvances(maxAdvances), genderRatio(genderRatio), powerLevel(powerLevel), filter(filter)
{
}

void HiddenGrottoGenerator::setInitialAdvances(u32 initialAdvances)
{
    this->initialAdvances = initialAdvances;
}

std::vector<HiddenGrottoState> HiddenGrottoGenerator::generate(u64 seed) const
{
    std::vector<HiddenGrottoState> states;

    BWRNG rng(seed);
    rng.advance(initialAdvances);

    for (u32 cnt = 0; cnt <= maxAdvances; cnt++, rng.next())
    {
        BWRNG go(rng.getSeed());
        u32 seed = go.nextUInt(0x1FFF);
        if (go.nextUInt(100) < powerLevel)
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

            HiddenGrottoState state(seed, initialAdvances + cnt, group, slot, gender);
            if (filter.compareState(state))
            {
                states.emplace_back(state);
            }
        }
    }

    return states;
}
