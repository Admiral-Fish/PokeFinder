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
#include <Core/Enum/PassPower.hpp>
#include <Core/Gen5/States/State5.hpp>
#include <Core/RNG/LCRNG64.hpp>
#include <Core/RNG/MT.hpp>
#include <Core/RNG/RNGList.hpp>
#include <Core/Util/Utilities.hpp>
#include <variant>

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

StaticGenerator5::StaticGenerator5(u32 initialAdvances, u32 maxAdvances, u32 offset, Method method, Lead lead, PassPower luckyPower,
                                   const StaticTemplate5 &staticTemplate, const Profile5 &profile, const StateFilter &filter) :
    StaticGenerator(initialAdvances, maxAdvances, offset, method, lead, staticTemplate, profile, filter),
    luckyPower((profile.getVersion() & Game::BW) != Game::None ? PassPower::None : luckyPower)
{
    if (staticTemplate.getCurtis())
    {
        tsv = 54118;
    }
    else if (staticTemplate.getYancy())
    {
        tsv = 10303;
    }
}

std::vector<State5> StaticGenerator5::generate(u64 seed, u32 initialAdvances, u32 maxAdvances) const
{
    bool bw = (profile.getVersion() & Game::BW) != Game::None;
    u32 initial = initialAdvances + (bw ? 0 : 2) + ((staticTemplate.getEgg() || staticTemplate.getRoamer()) ? 1 : 0);

    using RNGVariant = std::variant<RNGList<u8, MTFast, 8>, RNGList<u8, MT, 8, gen>>;
    RNGVariant rngList = [&]() {
        u32 size = initial + (maxAdvances + 1) + 8;
        if (size < 227)
        {
            return RNGVariant(std::in_place_type<RNGList<u8, MTFast, 8>>, seed >> 32, initial, size, true);
        }
        else
        {
            return RNGVariant(std::in_place_type<RNGList<u8, MT, 8, gen>>, seed >> 32, initial);
        }
    }();

    std::vector<std::pair<u32, std::array<u8, 6>>> ivs;
    std::visit(
        [&](auto &rng) {
            for (u32 cnt = 0; cnt <= maxAdvances; cnt++, rng.advanceState())
            {
                std::array<u8, 6> iv;

                iv[0] = rng.next();
                iv[1] = rng.next();
                iv[2] = rng.next();

                if (staticTemplate.getRoamer())
                {
                    iv[4] = rng.next();
                    iv[5] = rng.next();
                    iv[3] = rng.next();
                }
                else
                {
                    iv[3] = rng.next();
                    iv[4] = rng.next();
                    iv[5] = rng.next();
                }

                if (filter.compareIV(iv) && filter.compareHiddenPower(iv))
                {
                    ivs.emplace_back(initialAdvances + cnt, iv);
                }
            }
        },
        rngList);

    if (ivs.empty())
    {
        return std::vector<State5>();
    }
    else
    {
        return generate(seed, ivs);
    }
}

std::vector<State5> StaticGenerator5::generate(u64 seed, const std::vector<std::pair<u32, std::array<u8, 6>>> &ivs) const
{
    if (staticTemplate.getWild())
    {
        return generateWild(seed, ivs);
    }
    else
    {
        return generateNonWild(seed, ivs);
    }
}

std::vector<State5> StaticGenerator5::generateNonWild(u64 seed, const std::vector<std::pair<u32, std::array<u8, 6>>> &ivs) const
{
    u32 advances = Utilities5::initialAdvances(seed, profile);
    BWRNG rng(seed, advances + initialAdvances);
    auto jump = rng.getJump(offset);
    const PersonalInfo *info = staticTemplate.getInfo();

    std::vector<State5> states;
    for (u32 cnt = 0; cnt <= maxAdvances; cnt++)
    {
        BWRNG go(rng, jump);

        u32 pid;
        if (staticTemplate.getEgg())
        {
            pid = go.nextUInt();
            // Temp TID/SID
            go.nextUInt();
        }
        else if (staticTemplate.getRoamer())
        {
            pid = go.nextUInt();
        }
        else
        {
            pid = Utilities5::createPID(tsv, staticTemplate.getAbility(), staticTemplate.getGender(), staticTemplate.getShiny(), false,
                                        info->getGender(), go);
        }

        u8 ability = staticTemplate.getAbility() == 2 ? 2 : (pid >> 16) & 1;
        u8 gender = Utilities::getGender(pid, info);
        u8 shiny = Utilities::getShiny<true>(pid, tsv);
        u8 nature = go.nextUInt(25);

        // IVs have already been pre-filtered by this point
        // Only filter by the other data once before creating results
        if (filter.compare(ability, gender, nature, shiny))
        {
            u32 prng = rng.nextUInt();
            for (const auto &iv : ivs)
            {
                states.emplace_back(prng, advances + initialAdvances + cnt, iv.first, pid, iv.second, ability, gender,
                                    staticTemplate.getLevel(), nature, shiny, info);
            }
        }
    }

    return states;
}

std::vector<State5> StaticGenerator5::generateWild(u64 seed, const std::vector<std::pair<u32, std::array<u8, 6>>> &ivs) const
{
    u32 advances = Utilities5::initialAdvances(seed, profile);
    BWRNG rng(seed, advances + initialAdvances);
    auto jump = rng.getJump(offset);
    const PersonalInfo *info = staticTemplate.getInfo();

    bool bw = (profile.getVersion() & Game::BW) != Game::None;

    u8 shinyRolls = 1;
    if ((profile.getVersion() & Game::BW2) != Game::None)
    {
        if (profile.getShinyCharm())
        {
            shinyRolls += 2;
        }

        if (luckyPower == PassPower::Level3)
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

        // If the pokemon has a fixed gender ratio don't let anything override it
        u8 gender = 255;
        if (!info->getFixedGender())
        {
            gender = staticTemplate.getGender();

            // Only override the gender with cutecharm if the template doesn't have a forced gender
            if (cuteCharm && gender == 255)
            {
                gender = lead == Lead::CuteCharmF ? 0 : 1;
            }
        }

        u32 pid;
        for (u8 i = 0; i < shinyRolls; i++)
        {
            pid = Utilities5::createPID(tsv, 2, gender, staticTemplate.getShiny(), true, info->getGender(), go);
            if (Utilities::isShiny<true>(pid, tsv))
            {
                break;
            }
        }

        u8 ability = staticTemplate.getAbility() == 2 ? 2 : (pid >> 16) & 1;
        gender = Utilities::getGender(pid, info);
        u8 shiny = Utilities::getShiny<true>(pid, tsv);

        u8 nature = go.nextUInt(25);
        if (sync)
        {
            nature = toInt(lead);
        }

        // IVs have already been pre-filtered by this point
        // Only filter by the other data once before creating results
        if (filter.compare(ability, gender, nature, shiny))
        {
            u32 prng = rng.nextUInt();
            for (const auto &iv : ivs)
            {
                states.emplace_back(prng, advances + initialAdvances + cnt, iv.first, pid, iv.second, ability, gender,
                                    staticTemplate.getLevel(), nature, shiny, info);
            }
        }
    }

    return states;
}
