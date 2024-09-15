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

#include "HiddenGrottoGenerator.hpp"
#include <Core/Enum/Method.hpp>
#include <Core/Gen5/HiddenGrottoArea.hpp>
#include <Core/Gen5/States/HiddenGrottoState.hpp>
#include <Core/RNG/LCRNG64.hpp>
#include <Core/Util/Utilities.hpp>

// clang-format off
// See EncounterSlot.cpp computeTable() with { 1, 5, 20, 21, 25, 35, 60, 61, 65, 75, 100 }
constexpr u8 encounterTable[100] = {
    0,
    1, 1, 1, 1,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    3,
    4, 4, 4, 4,
    5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
    6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
    7,
    8, 8, 8, 8,
    9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
    10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10
};
// clang-format on

HiddenGrottoGenerator::HiddenGrottoGenerator(u32 initialAdvances, u32 maxAdvances, u32 offset, u8 powerLevel,
                                             const HiddenGrottoArea &encounterArea, const Profile5 &profile,
                                             const HiddenGrottoFilter &filter) :
    Generator(initialAdvances, maxAdvances, offset, Method::None, profile, filter), encounterArea(encounterArea), powerLevel(powerLevel)
{
}

std::vector<HiddenGrottoState> HiddenGrottoGenerator::generate(u64 seed) const
{
    u32 advances = Utilities5::initialAdvancesBW2(seed, profile.getMemoryLink());
    BWRNG rng(seed, advances + initialAdvances);
    auto jump = rng.getJump(offset);

    std::vector<HiddenGrottoState> states;
    for (u32 cnt = 0; cnt <= maxAdvances; cnt++)
    {
        BWRNG go(rng, jump);
        u16 prng = rng.nextUInt(0x1fff);
        if (go.nextUInt(100) < powerLevel)
        {
            u8 group = go.nextUInt(4);
            u8 slot = encounterTable[go.nextUInt(100)];
            if (slot < 3) // Pokemon
            {
                const auto &pokemon = encounterArea.getPokemon(group, slot);
                u8 gender = go.nextUInt(100) < pokemon.getGender();
                HiddenGrottoState state(prng, advances + initialAdvances + cnt, group, slot, pokemon.getSpecie(), gender);
                if (filter.compareState(state))
                {
                    states.emplace_back(state);
                }
            }
            else if (slot < 7) // Item
            {
                u16 item = encounterArea.getItem(group, slot - 3);
                HiddenGrottoState state(prng, advances + initialAdvances + cnt, group, slot, item);
                if (filter.compareState(state))
                {
                    states.emplace_back(state);
                }
            }
            else // Hidden item
            {
                u16 item = encounterArea.getHiddenItem(group, slot - 7);
                HiddenGrottoState state(prng, advances + initialAdvances + cnt, group, slot, item);
                if (filter.compareState(state))
                {
                    states.emplace_back(state);
                }
            }
        }
    }

    return states;
}
