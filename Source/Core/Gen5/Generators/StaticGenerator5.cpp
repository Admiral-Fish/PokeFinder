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

#include "StaticGenerator5.hpp"
#include <Core/Enum/Lead.hpp>
#include <Core/Gen5/States/State5.hpp>
#include <Core/RNG/LCRNG64.hpp>
#include <Core/RNG/MT.hpp>
#include <Core/RNG/RNGList.hpp>
#include <Core/Util/Utilities.hpp>
#include <algorithm>

static u8 gen(MT &rng)
{
    return rng.next() >> 27;
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

StaticGenerator5::StaticGenerator5(u32 initialAdvances, u32 maxAdvances, u32 offset, Method method, Lead lead, u8 luckyPower,
                                   const StaticTemplate5 &staticTemplate, const Profile5 &profile, const StateFilter &filter) :
    StaticGenerator(initialAdvances, maxAdvances, offset, method, lead, staticTemplate, profile, filter),
    luckyPower((profile.getVersion() & Game::BW) != Game::None ? 0 : luckyPower)
{
}

std::vector<State5> StaticGenerator5::generate(u64 seed, u32 initialAdvances, u32 maxAdvances) const
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
        return std::vector<State5>();
    }
    else
    {
        return generate(seed, ivs);
    }

    return std::vector<State5>();
}

std::vector<State5> StaticGenerator5::generate(u64 seed, const std::vector<std::pair<u32, std::array<u8, 6>>> &ivs) const
{
    u32 advances = Utilities5::initialAdvances(seed, profile);
    BWRNG rng(seed, advances + initialAdvances);
    auto jump = rng.getJump(offset);
    const PersonalInfo *info = staticTemplate.getInfo();

    bool bw = (profile.getVersion() & Game::BW) != Game::None;
    bool id = (profile.getTID() & 1) ^ (profile.getSID() & 1);

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

    std::vector<State5> states;
    for (u32 cnt = 0; cnt <= maxAdvances; cnt++)
    {
        BWRNG go(rng, jump);

        bool cuteCharm = false;
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
        else if (lead != Lead::CompoundEyes)
        {
            go.next();
        }

        u32 pid;
        if (staticTemplate.getShiny() == Shiny::Never)
        {
        }
        else if (staticTemplate.getShiny() == Shiny::Always)
        {
        }
        else
        {
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
        /*for (const auto &iv : ivs)
        {
            State5 state(chatot, advances + initialAdvances + cnt, iv.first, pid, iv.second, ability, gender, staticTemplate.getLevel(),
                         nature, shiny, item, staticTemplate.getSpecie(), staticTemplate.getForm(), info);
            if (filter.compareState(static_cast<const State &>(state)))
            {
                states.emplace_back(state);
            }
        }*/
    }

    return std::vector<State5>();
}
