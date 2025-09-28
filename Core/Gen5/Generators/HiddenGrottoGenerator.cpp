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
#include <Core/Enum/Game.hpp>
#include <Core/Enum/Lead.hpp>
#include <Core/Enum/Method.hpp>
#include <Core/Enum/Shiny.hpp>
#include <Core/Gen5/HiddenGrottoArea.hpp>
#include <Core/Gen5/States/HiddenGrottoState.hpp>
#include <Core/Gen5/States/State5.hpp>
#include <Core/RNG/LCRNG64.hpp>
#include <Core/RNG/MT.hpp>
#include <Core/RNG/RNGList.hpp>
#include <Core/Util/Utilities.hpp>
#include <algorithm>

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

static u8 gen(MT &rng)
{
    return rng.next() >> 27;
}

HiddenGrottoSlotGenerator::HiddenGrottoSlotGenerator(u32 initialAdvances, u32 maxAdvances, u32 offset, u8 powerLevel,
                                                     const HiddenGrottoArea &encounterArea, const Profile5 &profile,
                                                     const HiddenGrottoFilter &filter) :
    Generator(initialAdvances, maxAdvances, offset, Method::None, profile, filter), encounterArea(encounterArea), powerLevel(powerLevel)
{
}

std::vector<HiddenGrottoState> HiddenGrottoSlotGenerator::generate(u64 seed) const
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

HiddenGrottoGenerator::HiddenGrottoGenerator(u32 initialAdvances, u32 maxAdvances, u32 offset, Lead lead, u8 gender,
                                             const HiddenGrottoSlot &slot, const Profile5 &profile, const StateFilter &filter) :
    Generator(initialAdvances, maxAdvances, offset, Method::None, profile, filter), slot(slot), lead(lead), gender(gender)
{
}

std::vector<State5> HiddenGrottoGenerator::generate(u64 seed, u32 initialAdvances, u32 maxAdvances) const
{
    bool bw = (profile.getVersion() & Game::BW) != Game::None;

    std::vector<std::pair<u32, std::array<u8, 6>>> ivs;

    RNGList<u8, MT, 8, gen> rngList(seed >> 32, initialAdvances + (bw ? 0 : 2));
    for (u32 cnt = 0; cnt <= maxAdvances; cnt++, rngList.advanceState())
    {
        std::array<u8, 6> iv;
        std::generate(iv.begin(), iv.end(), [&rngList] { return rngList.next(); });
        if (filter.compareIV(iv))
        {
            ivs.emplace_back(initialAdvances + cnt, iv);
        }
    }

    if (ivs.empty())
    {
        return std::vector<State5>();
    }
    else
    {
        return generate(seed, ivs);
    }
}

std::vector<State5> HiddenGrottoGenerator::generate(u64 seed, const std::vector<std::pair<u32, std::array<u8, 6>>> &ivs) const
{
    u32 advances = Utilities5::initialAdvances(seed, profile);
    BWRNG rng(seed, advances + initialAdvances);
    auto jump = rng.getJump(offset);

    u8 range = slot.getMaxLevel() - slot.getMinLevel();

    std::vector<State5> states;
    for (u32 cnt = 0; cnt <= maxAdvances; cnt++)
    {
        BWRNG go(rng, jump);

        u8 level = slot.getMinLevel() + go.nextUInt(range);

        // While cute charm is technically possible it would be overwritten by the forced gender so don't check for it
        // In theory cute charm could cause a 2nd call to be consumed but the UI doesn't allow selecting it
        bool flag = go.nextUInt(100) < 50;
        bool sync = flag && lead <= Lead::SynchronizeEnd;

        const PersonalInfo *info = slot.getInfo();

        u32 pid = Utilities5::createPID(tsv, 2, gender, Shiny::Never, true, info->getGender(), go);

        u8 ability = 2;
        if (info->getAbility(2) == 0)
        {
            ability = (pid >> 16) & 1;
        }
        u8 shiny = Utilities::getShiny<true>(pid, tsv);

        u8 nature = go.nextUInt(25);
        if (sync)
        {
            nature = toInt(lead);
        }

        u16 chatot = rng.nextUInt(0x1fff);
        for (const auto &iv : ivs)
        {
            State5 state(chatot, advances + initialAdvances + cnt, iv.first, pid, iv.second, ability, gender, level, nature, shiny, info);
            if (filter.compareState(static_cast<const State &>(state)))
            {
                states.emplace_back(state);
            }
        }
    }

    return states;
}
