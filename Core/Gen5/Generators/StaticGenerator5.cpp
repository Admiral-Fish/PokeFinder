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

static u8 gen(MT &rng)
{
    return rng.next() >> 27;
}

static u8 getPercentRand(BWRNG &rng, bool bw)
{
    if (bw)
    {
        return rng.nextUInt(0xffff) / 656;
    }
    else
    {
        return rng.nextUInt(100);
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

    RNGList<u8, MT, 8, gen> rngList(seed >> 32, initialAdvances + (bw ? 0 : 2) + (staticTemplate.getRoamer() ? 1 : 0));
    for (u32 cnt = 0; cnt <= maxAdvances; cnt++, rngList.advanceState())
    {
        std::array<u8, 6> iv;

        iv[0] = rngList.next();
        iv[1] = rngList.next();
        iv[2] = rngList.next();

        if (staticTemplate.getRoamer())
        {
            iv[4] = rngList.next();
            iv[5] = rngList.next();
            iv[3] = rngList.next();
        }
        else
        {
            iv[3] = rngList.next();
            iv[4] = rngList.next();
            iv[5] = rngList.next();
        }

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

    u8 shinyRolls = 1;
    if (staticTemplate.getShiny() == Shiny::Random && (profile.getVersion() & Game::BW2) != Game::None)
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

    bool wild = staticTemplate.getEvent() || staticTemplate.getLegend() || staticTemplate.getStationary();

    std::vector<State5> states;
    for (u32 cnt = 0; cnt <= maxAdvances; cnt++)
    {
        BWRNG go(rng, jump);

        bool cuteCharm = false;
        bool sync = false;

        if (wild)
        {
            // Failed cute charm continues to check for other leads
            if ((lead == Lead::CuteCharmM || lead == Lead::CuteCharmF) && getPercentRand(go, bw) < 67)
            {
                cuteCharm = true;
            }
            else
            {
                bool flag = getPercentRand(go, bw) >= 50;
                if (lead <= Lead::SynchronizeEnd)
                {
                    sync = flag;
                }
            }
        }

        u32 pid;
        for (u8 i = 0; i < shinyRolls; i++)
        {
            if (staticTemplate.getEgg() || staticTemplate.getRoamer())
            {
                pid = go.nextUInt();
            }
            else
            {
                u8 gender = 255;

                // If the pokemon has a fixed gender ratio don't let anything override it
                if (!info->getFixedGender())
                {
                    gender = staticTemplate.getGender();

                    // Only override the gender with cutecharm if the template doesn't have a forced gender
                    if (cuteCharm && gender == 255)
                    {
                        gender = lead == Lead::CuteCharmF ? 0 : 1;
                    }
                }

                pid = Utilities5::createPID(tsv, staticTemplate.getAbility(), gender, staticTemplate.getShiny(), wild,
                                            info->getGender(), go);
            }

            if (Utilities::isShiny<true>(pid, tsv))
            {
                break;
            }
        }

        u8 ability = staticTemplate.getAbility() == 2 ? 2 : (pid >> 16) & 1;
        u8 gender = Utilities::getGender(pid, info);
        u8 shiny = Utilities::getShiny<true>(pid, tsv);

        u8 nature = go.nextUInt(25);
        if (sync)
        {
            nature = toInt(lead);
        }

        u16 chatot = rng.nextUInt(0x1fff);
        for (const auto &iv : ivs)
        {
            State5 state(chatot, advances + initialAdvances + cnt, iv.first, pid, iv.second, ability, gender, staticTemplate.getLevel(),
                         nature, shiny, info);
            if (filter.compareState(static_cast<const State &>(state)))
            {
                states.emplace_back(state);
            }
        }
    }

    return states;
}
