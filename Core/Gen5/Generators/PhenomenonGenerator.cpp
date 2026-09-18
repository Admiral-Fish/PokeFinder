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
#include <Core/Gen5/EncounterArea5.hpp>
#include <Core/Gen5/States/PhenomenonState.hpp>
#include <Core/Gen5/States/State5.hpp>
#include <Core/RNG/LCRNG64.hpp>
#include <Core/Util/Utilities.hpp>
#include <algorithm>

PhenomenonGenerator::PhenomenonGenerator(u32 initialAdvances, u32 maxAdvances, u32 offset, const EncounterArea5 &area,
                                         const Profile5 &profile, const PhenomenonFilter &filter) :
    Generator(initialAdvances, maxAdvances, offset, Method::None, profile, filter), area(area)
{
}

std::vector<PhenomenonState> PhenomenonGenerator::generate(u64 seed) const
{
    u32 advances = Utilities5::initialAdvances(seed, profile);
    BWRNG rng(seed, advances + initialAdvances);
    auto jump = rng.getJump(offset);

    bool bw = (profile.getVersion() & Game::BW) != Game::None;
    u16 rate = area.getPhenomenonRate();

    std::vector<PhenomenonState> states;
    for (u32 cnt = 0; cnt <= maxAdvances; cnt++)
    {
        BWRNG go(rng, jump);

        bool valid = go.nextUInt(1000) >= rate;
        u16 item = area.getItem(go, bw);

        u32 prng = rng.nextUInt();
        PhenomenonState state(prng, advances + initialAdvances + cnt, item, valid);
        if (filter.compare(state))
        {
            states.emplace_back(state);
        }
    }

    return states;
}
