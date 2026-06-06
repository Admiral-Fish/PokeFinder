/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2024 by Admiral_Fish, bumba, and EzPzStreamz
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

#include "PhenomenonGenerator.hpp"
#include <Core/Enum/Game.hpp>
#include <Core/Enum/Lead.hpp>
#include <Core/Enum/Method.hpp>
#include <Core/Enum/Shiny.hpp>
#include <Core/Gen5/PhenomenonArea.hpp>
#include <Core/Gen5/States/PhenomenonState.hpp>
#include <Core/Gen5/States/State5.hpp>
#include <Core/RNG/LCRNG64.hpp>
#include <Core/Util/Utilities.hpp>
#include <algorithm>

PhenomenonGenerator::PhenomenonGenerator(u32 initialAdvances, u32 maxAdvances, u32 offset, const PhenomenonArea &area,
                                         const Profile5 &profile, const PhenomenonFilter &filter) :
    Generator(initialAdvances, maxAdvances, offset, Method::None, profile, filter), area(area)
{
}

std::vector<PhenomenonState> PhenomenonGenerator::generate(u64 seed) const
{
    u32 advances = Utilities5::initialAdvancesBW2(seed, profile.getMemoryLink());
    BWRNG rng(seed, advances + initialAdvances);
    auto jump = rng.getJump(offset);

    u16 rate = area.getRate();

    std::vector<PhenomenonState> states;
    for (u32 cnt = 0; cnt <= maxAdvances; cnt++)
    {
        BWRNG go(rng, jump);
        u16 prng = rng.nextUInt(0x1fff);

        if (go.nextUInt(1000) < rate)
        {
            PhenomenonState state(prng, advances + initialAdvances + cnt);
            if (filter.compareState(state))
            {
                states.emplace_back(state);
            }
        }
        else
        {
            u16 item = area.getItem(go);
            PhenomenonState state(prng, advances + initialAdvances + cnt, item);
            if (filter.compareState(state))
            {
                states.emplace_back(state);
            }
        }
    }

    return states;
}
