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

#include "PokeSpotGenerator.hpp"
#include <Core/Enum/Method.hpp>
#include <Core/Gen3/States/PokeSpotState.hpp>
#include <Core/Parents/EncounterArea.hpp>
#include <Core/Parents/Slot.hpp>
#include <Core/RNG/LCRNG.hpp>
#include <Core/Util/Utilities.hpp>
#include <algorithm>

// clang-format off
// See EncounterSlot.cpp computeTable() with { 50, 85, 100 }
constexpr u8 encounterTable[100] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2
};
// clang-format on

PokeSpotGenerator::PokeSpotGenerator(u32 initialAdvances, u32 maxAdvances, u32 delay, u32 initialAdvancesEncounter,
                                     u32 maxAdvancesEncounter, u32 delayEncounter, const Profile3 &profile, const WildStateFilter &filter) :
    Generator(initialAdvances, maxAdvances, delay, Method::None, profile, filter),
    delayEncounter(delayEncounter),
    initialAdvancesEncounter(initialAdvancesEncounter),
    maxAdvancesEncounter(maxAdvancesEncounter)
{
}

std::vector<PokeSpotState> PokeSpotGenerator::generate(u32 seedFood, u32 seedEncounter, const EncounterArea &encounterArea) const
{
    auto food = generateFood(seedFood, encounterArea);
    return food.empty() ? std::vector<PokeSpotState>() : generateEncounter(seedEncounter, food, encounterArea);
}

std::vector<PokeSpotState> PokeSpotGenerator::generateEncounter(u32 seed, const std::vector<PokeSpotState> &food,
                                                                const EncounterArea &encounterArea) const
{
    XDRNG rng(seed, initialAdvancesEncounter);

    std::vector<PokeSpotState> states;
    for (u32 cnt = 0; cnt <= maxAdvancesEncounter; cnt++, rng.next())
    {
        XDRNG go(rng);

        u16 levelRand = go.nextUShort();

        go.advance(2);

        u16 iv1 = go.nextUShort();
        u16 iv2 = go.nextUShort();

        std::array<u8, 6> ivs;
        ivs[0] = iv1 & 31;
        ivs[1] = (iv1 >> 5) & 31;
        ivs[2] = (iv1 >> 10) & 31;
        ivs[3] = (iv2 >> 5) & 31;
        ivs[4] = (iv2 >> 10) & 31;
        ivs[5] = iv2 & 31;

        u8 ability = go.nextUShort(2);
        if (!filter.compareIV(ivs))
        {
            continue;
        }

        for (auto state : food)
        {
            const Slot &slot = encounterArea.getPokemon(state.getEncounterSlot());
            const PersonalInfo *info = slot.getInfo();

            u8 level = encounterArea.calculateLevel(state.getEncounterSlot(), levelRand);

            state.update(initialAdvancesEncounter + cnt, ability == 1 && info->getAbility(0) != info->getAbility(1), level, ivs, info);
            if (filter.compareAbility(state.getAbility()) && filter.compareHiddenPower(state.getHiddenPower()))
            {
                states.emplace_back(state);
            }
        }
    }

    std::sort(states.begin(), states.end(), [](const PokeSpotState &left, const PokeSpotState &right) {
        if (left.getAdvances() < right.getAdvances())
        {
            return true;
        }
        else if (right.getAdvances() < left.getAdvances())
        {
            return false;
        }

        if (left.getEncounterAdvances() < right.getEncounterAdvances())
        {
            return true;
        }
        else if (right.getEncounterAdvances() < left.getEncounterAdvances())
        {
            return false;
        }

        return false;
    });

    return states;
}

std::vector<PokeSpotState> PokeSpotGenerator::generateFood(u32 seed, const EncounterArea &encounterArea) const
{
    XDRNG rng(seed, initialAdvances);

    std::vector<PokeSpotState> states;
    for (u32 cnt = 0; cnt <= maxAdvances; cnt++, rng.next())
    {
        XDRNG go(rng);

        // Check if state is a valid pokespot call
        if (go.nextUShort(3) != 0)
        {
            continue;
        }

        // Munchlax provides a skip
        if (go.nextUShort(100) < 10)
        {
            continue;
        }

        u8 encounterSlot = encounterTable[go.nextUShort(100)];
        if (!filter.compareEncounterSlot(encounterSlot))
        {
            continue;
        }

        u32 pid = go.nextUShort() << 16;
        pid |= go.nextUShort();

        if (filter.compareNature(pid % 25))
        {
            const Slot &slot = encounterArea.getPokemon(encounterSlot);
            const PersonalInfo *info = slot.getInfo();

            PokeSpotState state(initialAdvances + cnt, pid, Utilities::getGender(pid, info), Utilities::getShiny(pid, tsv), encounterSlot,
                                slot.getSpecie(), info);
            if (filter.compareGender(state.getGender()) && filter.compareShiny(state.getShiny()))
            {
                states.emplace_back(state);
            }
        }
    }

    return states;
}
