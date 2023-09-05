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

WildGenerator5::WildGenerator5(u32 initialAdvances, u32 maxAdvances, u32 delay, Method method, Lead lead, const EncounterArea5 &area,
                               const Profile5 &profile, const WildStateFilter &filter) :
    WildGenerator(initialAdvances, maxAdvances, delay, method, lead, area, profile, filter)
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
    auto jump = rng.getJump(delay);

    bool bw = (profile.getVersion() & Game::BW) != Game::None;
    bool id = (profile.getTID() & 1) ^ (profile.getSID() & 1);

    std::vector<WildState5> states;
    for (u32 cnt = 0; cnt <= maxAdvances; cnt++)
    {
        BWRNG go(rng, jump);

        bool sync = false;
        bool cuteCharm = false;

        if (lead <= Lead::SynchronizeEnd)
        {
            sync = go.nextUInt(2);
        }
        else if (lead == Lead::CuteCharmM || lead == Lead::CuteCharmF)
        {
            cuteCharm = (go.nextUInt(0xffff) / 656) < 67;
        }
        else if (lead != Lead::CompoundEyes)
        {
            go.advance(1);
        }

        u8 encounterSlot = EncounterSlot::bwSlot(getEncounterRand(go, bw), area.getEncounter());
        u8 level = area.calculateLevel(encounterSlot, go, lead == Lead::Pressure);

        const Slot &slot = area.getPokemon(encounterSlot);
        const PersonalInfo *info = slot.getInfo();

        u32 pid = go.nextUInt() ^ 0x10000;
        if (cuteCharm)
        {
            pid = Utilities5::forceGender(pid, go, lead == Lead::CuteCharmF ? 0 : 1, info->getGender());
        }

        if (id ^ (pid >> 31) ^ (pid & 1))
        {
            pid ^= 0x80000000;
        }

        u8 nature = go.nextUInt(25);
        if (sync)
        {
            nature = toInt(lead);
        }

        u16 item = getItem(go, lead, info);

        u16 chatot = rng.nextUInt(0x1fff);
        for (const auto &iv : ivs)
        {
            WildState5 state(chatot, advances + initialAdvances + cnt, iv.first, pid, iv.second, (pid >> 16) & 1,
                             Utilities::getGender(pid, info), level, nature, Utilities::getShiny(pid, tsv), encounterSlot, item,
                             slot.getSpecie(), slot.getForm(), info);
            if (filter.compareState(static_cast<const WildState &>(state)))
            {
                states.emplace_back(state);
            }
        }
    }

    return states;
}
