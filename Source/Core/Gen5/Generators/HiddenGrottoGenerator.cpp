/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2022 by Admiral_Fish, bumba, and EzPzStreamz
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

// Game has all of these + 1, removed for simplicity
constexpr u32 grottoSlots[11] = { 0, 4, 19, 20, 24, 34, 59, 60, 64, 74, 99 };

HiddenGrottoGenerator::HiddenGrottoGenerator(u32 initialAdvances, u32 maxAdvances, u32 delay, u8 powerLevel,
                                             const HiddenGrottoArea &encounterArea, const Profile5 &profile,
                                             const HiddenGrottoFilter &filter) :
    Generator(initialAdvances, maxAdvances, delay, Method::None, profile, filter), encounterArea(encounterArea), powerLevel(powerLevel)
{
}

std::vector<HiddenGrottoState> HiddenGrottoGenerator::generate(u64 seed) const
{
    u32 advances = Utilities5::initialAdvancesBW2(seed, profile.getMemoryLink());
    BWRNG rng(seed, advances + initialAdvances + delay);

    std::vector<HiddenGrottoState> states;
    for (u32 cnt = 0; cnt <= maxAdvances; cnt++)
    {
        BWRNG go(rng.getSeed());
        u16 prng = rng.nextUInt(0x1fff);
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
