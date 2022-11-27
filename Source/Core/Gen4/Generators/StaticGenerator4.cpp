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

#include "StaticGenerator4.hpp"
#include <Core/Enum/Lead.hpp>
#include <Core/Enum/Method.hpp>
#include <Core/Gen4/States/State4.hpp>
#include <Core/Gen4/StaticTemplate4.hpp>
#include <Core/Parents/PersonalInfo.hpp>
#include <Core/RNG/LCRNG.hpp>

static u8 getGender(u32 pid, const PersonalInfo *info)
{
    switch (info->getGender())
    {
    case 255: // Genderless
        return 2;
        break;
    case 254: // Female
        return 1;
        break;
    case 0: // Male
        return 0;
        break;
    default: // Random gender
        return (pid & 255) < info->getGender();
        break;
    }
}

static u8 getShiny(u32 pid, u16 tsv)
{
    u16 psv = (pid >> 16) ^ (pid & 0xffff);
    if (tsv == psv)
    {
        return 2; // Square
    }
    else if ((tsv ^ psv) < 8)
    {
        return 1; // Star
    }
    else
    {
        return 0;
    }
}

static bool isShiny(u32 pid, u16 tsv)
{
    u16 psv = ((pid >> 16) & (pid & 0xffff));
    return (psv ^ tsv) < 8;
}

StaticGenerator4::StaticGenerator4(u32 initialAdvances, u32 maxAdvances, u32 offset, u16 tid, u16 sid, Game version, Method method,
                                   Lead lead, const StateFilter4 &filter) :
    StaticGenerator<StateFilter4>(initialAdvances, maxAdvances, offset, tid, sid, version, method, lead, filter)
{
}

std::vector<GeneratorState4> StaticGenerator4::generate(u32 seed, const StaticTemplate4 *staticTemplate) const
{
    switch (method)
    {
    case Method::Method1:
        return generateMethod1(seed, staticTemplate);
    case Method::MethodJ:
        return generateMethodJ(seed, staticTemplate);
    case Method::MethodK:
        return generateMethodK(seed, staticTemplate);
    default:
        return std::vector<GeneratorState4>();
    }
}

std::vector<GeneratorState4> StaticGenerator4::generateMethod1(u32 seed, const StaticTemplate4 *staticTemplate) const
{
    std::vector<GeneratorState4> states;
    const PersonalInfo *info = staticTemplate->getInfo();

    PokeRNG rng(seed, initialAdvances + offset);
    for (u32 cnt = 0; cnt <= maxAdvances; cnt++)
    {
        PokeRNG go(rng.getSeed());

        u32 pid;
        if (staticTemplate->getShiny() == Shiny::Always)
        {
            u16 low = go.nextUShort(8);
            u16 high = go.nextUShort(8);

            for (int i = 3; i < 16; i++)
            {
                low |= go.nextUShort(2) << i;
            }
            high |= (low ^ tsv) & 0xfff8;
            pid = (high << 16) | low;
        }
        else
        {
            pid = go.nextUShort();
            pid |= go.nextUShort() << 16;

            if (staticTemplate->getShiny() == Shiny::Never)
            {
                while (isShiny(pid, tsv))
                {
                    pid = ARNG(pid).next();
                }
            }
        }

        u16 iv1 = go.nextUShort();
        u16 iv2 = go.nextUShort();
        std::array<u8, 6> ivs;
        ivs[0] = iv1 & 31;
        ivs[1] = (iv1 >> 5) & 31;
        ivs[2] = (iv1 >> 10) & 31;
        ivs[3] = (iv2 >> 5) & 31;
        ivs[4] = (iv2 >> 10) & 31;
        ivs[5] = iv2 & 31;

        GeneratorState4 state(rng.nextUShort(), initialAdvances + cnt, pid, ivs, pid & 1, getGender(pid, info), staticTemplate->getLevel(),
                              pid % 25, getShiny(pid, tsv), info);
        if (filter.compareState(state))
        {
            states.emplace_back(state);
        }
    }

    return states;
}

std::vector<GeneratorState4> StaticGenerator4::generateMethodJ(u32 seed, const StaticTemplate4 *staticTemplate) const
{
    std::vector<GeneratorState4> states;
    const PersonalInfo *info = staticTemplate->getInfo();

    bool cuteCharmFlag = false;
    u8 buffer = 0;
    if (lead == Lead::CuteCharmF)
    {
        buffer = 25 * ((info->getGender() / 25) + 1);
    }

    PokeRNG rng(seed, initialAdvances + offset);
    for (u32 cnt = 0; cnt <= maxAdvances; cnt++)
    {
        PokeRNG go(rng.getSeed());

        if (lead == Lead::CuteCharmM || lead == Lead::CuteCharmF)
        {
            switch (info->getGender())
            {
            case 0:
            case 254:
            case 255:
                cuteCharmFlag = false;
                break;
            default:
                cuteCharmFlag = go.nextUShort<false>(3) != 0;
                break;
            }
        }

        u8 nature;
        if (lead <= Lead::SynchronizeEnd)
        {
            nature = go.nextUShort<false>(2) == 0 ? toInt(lead) : go.nextUShort<false>(25);
        }
        else
        {
            nature = go.nextUShort<false>(25);
        }

        u32 pid;
        if (cuteCharmFlag)
        {
            pid = buffer + nature;
        }
        else
        {
            do
            {
                u16 low = go.nextUShort();
                u16 high = go.nextUShort();
                pid = (high << 16) | low;
            } while (pid % 25 != nature);
        }

        u16 iv1 = go.nextUShort();
        u16 iv2 = go.nextUShort();
        std::array<u8, 6> ivs;
        ivs[0] = iv1 & 31;
        ivs[1] = (iv1 >> 5) & 31;
        ivs[2] = (iv1 >> 10) & 31;
        ivs[3] = (iv2 >> 5) & 31;
        ivs[4] = (iv2 >> 10) & 31;
        ivs[5] = iv2 & 31;

        GeneratorState4 state(rng.nextUShort(), initialAdvances + cnt, pid, ivs, pid & 1, getGender(pid, info), staticTemplate->getLevel(),
                              pid % 25, getShiny(pid, tsv), info);
        if (filter.compareState(state))
        {
            states.emplace_back(state);
        }
    }

    return states;
}

std::vector<GeneratorState4> StaticGenerator4::generateMethodK(u32 seed, const StaticTemplate4 *staticTemplate) const
{
    std::vector<GeneratorState4> states;
    const PersonalInfo *info = staticTemplate->getInfo();

    bool cuteCharmFlag = false;
    u8 buffer = 0;
    if (lead == Lead::CuteCharmF)
    {
        buffer = 25 * ((info->getGender() / 25) + 1);
    }

    PokeRNG rng(seed, initialAdvances + offset);
    for (u32 cnt = 0; cnt <= maxAdvances; cnt++)
    {
        PokeRNG go(rng.getSeed());

        u8 nature;
        if (lead == Lead::CuteCharmM || lead == Lead::CuteCharmF)
        {
            switch (info->getGender())
            {
            case 0:
            case 254:
            case 255:
                cuteCharmFlag = false;
                break;
            default:
                cuteCharmFlag = go.nextUShort(3) != 0;
                break;
            }
        }

        if (lead <= Lead::SynchronizeEnd)
        {
            nature = go.nextUShort(2) == 0 ? toInt(lead) : go.nextUShort(25);
        }
        else
        {
            nature = go.nextUShort(25);
        }

        u32 pid;
        if (cuteCharmFlag)
        {
            pid = buffer + nature;
        }
        else
        {
            do
            {
                u16 low = go.nextUShort();
                u16 high = go.nextUShort();
                pid = (high << 16) | low;
            } while (pid % 25 != nature);
        }

        u16 iv1 = go.nextUShort();
        u16 iv2 = go.nextUShort();
        std::array<u8, 6> ivs;
        ivs[0] = iv1 & 31;
        ivs[1] = (iv1 >> 5) & 31;
        ivs[2] = (iv1 >> 10) & 31;
        ivs[3] = (iv2 >> 5) & 31;
        ivs[4] = (iv2 >> 10) & 31;
        ivs[5] = iv2 & 31;

        GeneratorState4 state(rng.nextUShort(), initialAdvances + cnt, pid, ivs, pid & 1, getGender(pid, info), staticTemplate->getLevel(),
                              pid % 25, getShiny(pid, tsv), info);
        if (filter.compareState(state))
        {
            states.emplace_back(state);
        }
    }

    return states;
}
