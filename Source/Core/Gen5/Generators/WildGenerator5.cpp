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

#include "WildGenerator5.hpp"
#include <Core/Enum/Encounter.hpp>
#include <Core/Enum/Game.hpp>
#include <Core/Enum/Lead.hpp>
#include <Core/Gen5/States/WildState5.hpp>
#include <Core/RNG/LCRNG64.hpp>
#include <Core/RNG/MT.hpp>
#include <Core/RNG/RNGList.hpp>
#include <Core/Util/EncounterSlot.hpp>
#include <Core/Util/Utilities.hpp>
#include <algorithm>

static u8 gen(MT &rng)
{
    return rng.next() >> 27;
}

static u8 getEncounterRand(BWRNG &rng, bool bw)
{
    if (bw)
    {
        return rng.nextUInt(0x10000) / 656;
    }
    else
    {
        return rng.nextUInt(100);
    }
}

static u16 getItem(BWRNG &rng, Lead lead, const PersonalInfo *info)
{
    constexpr u8 ItemTableRange[3][3] = { { 50, 55, 56 }, { 60, 80, 85 } };

    u8 rand = rng.nextUInt(100);
    if (rand < ItemTableRange[lead == Lead::CompoundEyes ? 1 : 0][0])
    {
        return info->getItem(0);
    }
    else if (rand < ItemTableRange[lead == Lead::CompoundEyes ? 1 : 0][1])
    {
        return info->getItem(1);
    }
    else if (rand < ItemTableRange[lead == Lead::CompoundEyes ? 1 : 0][2])
    {
        return info->getItem(2);
    }
    else
    {
        return 0;
    }
}

WildGenerator5::WildGenerator5(u32 initialAdvances, u32 maxAdvances, u32 offset, Method method, Lead lead, u8 luckyPower,
                               const EncounterArea5 &area, const Profile5 &profile, const WildStateFilter &filter) :
    WildGenerator(initialAdvances, maxAdvances, offset, method, lead, area, profile, filter),
    luckyPower((profile.getVersion() & Game::BW) != Game::None ? 0 : luckyPower)
{
}

std::vector<WildState5> WildGenerator5::generate(u64 seed, u32 initialAdvances, u32 maxAdvances) const
{
    bool bw = (profile.getVersion() & Game::BW) != Game::None;

    std::vector<std::pair<u32, std::array<u8, 6>>> ivs;

    RNGList<u8, MT, 8, gen> rngList(seed >> 32, initialAdvances + (bw ? 0 : 2));
    for (u32 cnt = 0; cnt <= maxAdvances; cnt++)
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
        return std::vector<WildState5>();
    }
    else
    {
        return generate(seed, ivs);
    }
}

std::vector<WildState5> WildGenerator5::generate(u64 seed, const std::vector<std::pair<u32, std::array<u8, 6>>> &ivs) const
{
    u32 advances = Utilities5::initialAdvances(seed, profile);
    BWRNG rng(seed, advances + initialAdvances);
    auto jump = rng.getJump(offset);

    bool bw = (profile.getVersion() & Game::BW) != Game::None;
    bool id = (profile.getTID() & 1) ^ (profile.getSID() & 1);
    auto modifiedSlots = area.getSlots(lead);

    u8 shinyRolls = 1;
    if ((profile.getVersion() & Game::BW2) != Game::None)
    {
        if (profile.getShinyCharm())
        {
            shinyRolls += 2;
        }

        if (luckyPower == 3)
        {
            shinyRolls++;
        }
    }

    std::vector<WildState5> states;
    for (u32 cnt = 0; cnt <= maxAdvances; cnt++)
    {
        BWRNG go(rng, jump);

        bool cuteCharm = false;
        bool magnetStatic = false;
        bool pressure = false;
        bool sync = false;

        if (lead <= Lead::SynchronizeEnd)
        {
            sync = go.nextUInt(2);
        }
        else if (lead == Lead::CuteCharmM || lead == Lead::CuteCharmF)
        {
            cuteCharm = (go.nextUInt(0xffff) / 656) < 67;

            // Failed cute charm seems to consume another advance
            if (!cuteCharm)
            {
                go.next();
            }
        }
        else if (lead == Lead::MagnetPull || lead == Lead::Static)
        {
            magnetStatic = go.nextUInt(2);
        }
        else if (lead == Lead::Pressure)
        {
            pressure = go.nextUInt(2);
        }
        else if (lead != Lead::CompoundEyes)
        {
            go.next();
        }

        if (area.getEncounter() == Encounter::SuperRod && lead != Lead::SuctionCups && go.nextUInt(100) >= 50)
        {
            rng.next();
            continue;
        }

        bool doubleBattle = false;
        if (area.getEncounter() == Encounter::DoubleGrass && go.nextUInt(100) < 40)
        {
            doubleBattle = true;
        }

        u8 encounterSlot;
        if (magnetStatic && !modifiedSlots.empty())
        {
            encounterSlot = modifiedSlots[go.nextUInt()];
        }
        else
        {
            encounterSlot = EncounterSlot::bwSlot(getEncounterRand(go, bw), area.getEncounter(), luckyPower);
        }

        u8 level;
        if (area.getEncounter() == Encounter::Grass || area.getEncounter() == Encounter::DoubleGrass
            || area.getEncounter() == Encounter::SpecialGrass)
        {
            level = area.calculateLevel<false>(encounterSlot, go.next(), pressure);
        }
        else
        {
            level = area.calculateLevel<true>(encounterSlot, go.nextUInt(100), pressure);
        }

        // RNG calls for left encounter slot and level
        if (doubleBattle)
        {
            go.advance(2);
        }

        const Slot &slot = area.getPokemon(encounterSlot);
        const PersonalInfo *info = slot.getInfo();

        u32 pid;
        for (u8 i = 0; i < shinyRolls; i++)
        {
            pid = go.nextUInt() ^ 0x10000;
            if (cuteCharm)
            {
                pid = Utilities5::forceGender(pid, go, lead == Lead::CuteCharmF ? 0 : 1, info->getGender());
            }

            if (id ^ (pid >> 31) ^ (pid & 1))
            {
                pid ^= 0x80000000;
            }

            if (Utilities::isShiny<true>(pid, tsv))
            {
                break;
            }
        }

        u8 ability = (pid >> 16) & 1;
        u8 gender = Utilities::getGender(pid, info);
        u8 shiny = Utilities::getShiny<true>(pid, tsv);

        u8 nature = go.nextUInt(25);
        if (sync)
        {
            nature = toInt(lead);
        }

        u16 item = getItem(go, lead, info);

        u16 chatot = rng.nextUInt(0x1fff);
        for (const auto &iv : ivs)
        {
            WildState5 state(chatot, advances + initialAdvances + cnt, iv.first, pid, iv.second, ability, gender, level, nature, shiny,
                             encounterSlot, item, slot.getSpecie(), slot.getForm(), info);
            if (filter.compareState(static_cast<const WildState &>(state)))
            {
                states.emplace_back(state);
            }
        }
    }

    return states;
}
