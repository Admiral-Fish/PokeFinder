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

#include "StaticGenerator8.hpp"
#include <Core/Enum/Lead.hpp>
#include <Core/Enum/Method.hpp>
#include <Core/Parents/PersonalInfo.hpp>
#include <Core/Parents/States/State.hpp>
#include <Core/Parents/StaticTemplate.hpp>
#include <Core/RNG/RNGList.hpp>
#include <Core/RNG/Xoroshiro.hpp>
#include <Core/RNG/Xorshift.hpp>

static u32 gen(Xorshift &rng)
{
    return rng.next(0x80000000, 0x7fffffff);
}

StaticGenerator8::StaticGenerator8(u32 initialAdvances, u32 maxAdvances, u32 delay, Lead lead, const Profile8 &profile,
                                   const StateFilter &filter) :
    StaticGenerator(initialAdvances, maxAdvances, delay, Method::None, lead, profile, filter)
{
    tsv = (profile.getTID() & 0xFFF0) ^ profile.getSID();
}

std::vector<GeneratorState> StaticGenerator8::generate(u64 seed0, u64 seed1, const StaticTemplate *staticTemplate) const
{
    const PersonalInfo *info = staticTemplate->getInfo();
    RNGList<u32, Xorshift, 32, gen> rngList(seed0, seed1, initialAdvances + delay);

    std::vector<GeneratorState> states;
    for (u32 cnt = 0; cnt <= maxAdvances; cnt++, rngList.advanceState())
    {
        u32 ec = rngList.next();
        u32 sidtid = rngList.next();
        u32 pid = rngList.next();

        u16 psv = (pid >> 16) ^ (pid & 0xfff0);
        u8 shiny;
        if (staticTemplate->getShiny() == Shiny::Never)
        {
            shiny = 0;
            if ((psv ^ tsv) < 16)
            {
                pid ^= 0x10000000;
            }
        }
        else
        {
            u16 fakeXOR = (sidtid >> 16) ^ (sidtid & 0xfff0) ^ psv;
            if (fakeXOR < 16) // Force shiny
            {
                shiny = fakeXOR == 0 ? 2 : 1;

                u16 realXOR = psv ^ tsv;
                u8 realShiny = realXOR == 0 ? 2 : realXOR < 16 ? 1 : 0;

                if (realShiny != shiny)
                {
                    u16 high = (pid & 0xFFFF) ^ tsv ^ (2 - shiny);
                    pid = (high << 16) | (pid & 0xFFFF);
                }
            }
            else // Force non shiny
            {
                shiny = 0;
                if ((psv ^ tsv) < 16)
                {
                    pid ^= 0x10000000;
                }
            }
        }

        // Assign IVs set by template
        std::array<u8, 6> ivs = { 255, 255, 255, 255, 255, 255 };
        for (u8 i = 0; i < staticTemplate->getIVCount();)
        {
            u8 index = rngList.next() % 6;
            if (ivs[index] == 255)
            {
                ivs[index] = 31;
                i++;
            }
        }

        for (u8 &iv : ivs)
        {
            if (iv == 255)
            {
                iv = rngList.next() % 32;
            }
        }

        u8 ability;
        if (staticTemplate->getAbility() != 255)
        {
            ability = staticTemplate->getAbility();
        }
        else
        {
            ability = rngList.next() % 2;
        }

        u8 gender;
        switch (info->getGender())
        {
        case 255:
            gender = 2;
            break;
        case 254:
            gender = 1;
            break;
        case 0:
            gender = 0;
            break;
        default:
            if ((lead == Lead::CuteCharmF || lead == Lead::CuteCharmM) && (rngList.next() % 3) > 0)
            {
                gender = lead == Lead::CuteCharmF ? 0 : 1;
            }
            else
            {
                gender = (rngList.next() % 253) + 1 < info->getGender();
            }
            break;
        }

        u8 nature;
        if (lead <= Lead::SynchronizeEnd)
        {
            nature = toInt(lead);
        }
        else
        {
            nature = rngList.next() % 25;
        }

        GeneratorState state(initialAdvances + cnt, ec, pid, ivs, ability, gender, staticTemplate->getLevel(), nature, shiny, info);
        if (filter.compareState(static_cast<const State &>(state)))
        {
            states.emplace_back(state);
        }
    }

    return states;
}

std::vector<GeneratorState> StaticGenerator8::generateRoamer(u64 seed0, u64 seed1, const StaticTemplate *staticTemplate) const
{
    // Going to ignore most of the parameters
    // Only roamers are Cresselia/Mesprit which have identical parameters
    u8 gender = staticTemplate->getSpecie() == 488 ? 1 : 2;

    Xorshift roamer(seed0, seed1, initialAdvances + delay);

    std::vector<GeneratorState> states;
    for (u32 cnt = 0; cnt <= maxAdvances; cnt++)
    {
        u32 ec = roamer.next(0x80000000, 0x7fffffff);
        XoroshiroBDSP rng(ec);

        u32 sidtid = rng.nextUInt(0xffffffff);
        u32 pid = rng.nextUInt(0xffffffff);

        u16 psv = (pid >> 16) ^ (pid & 0xfff0);
        u16 fakeXOR = (sidtid >> 16) ^ (sidtid & 0xfff0) ^ psv;
        u8 shiny;
        if (fakeXOR < 16) // Force shiny
        {
            shiny = fakeXOR == 0 ? 2 : 1;

            u16 realXOR = psv ^ tsv;
            u8 realShiny = realXOR == 0 ? 2 : realXOR < 16 ? 1 : 0;

            if (realShiny != shiny)
            {
                u16 high = (pid & 0xFFFF) ^ tsv ^ (2 - shiny);
                pid = (high << 16) | (pid & 0xFFFF);
            }
        }
        else // Force non shiny
        {
            shiny = 0;
            if ((psv ^ tsv) < 16)
            {
                pid ^= 0x10000000;
            }
        }

        // Assign 3 31 IVs
        std::array<u8, 6> ivs = { 255, 255, 255, 255, 255, 255 };
        for (int i = 0; i < 3;)
        {
            u8 index = rng.nextUInt(6);
            if (ivs[index] == 255)
            {
                ivs[index] = 31;
                i++;
            }
        }

        for (u8 &iv : ivs)
        {
            if (iv == 255)
            {
                iv = rng.nextUInt(32);
            }
        }

        // No HA possible for roamers
        u8 ability = rng.nextUInt(2);

        u8 nature;
        if (lead <= Lead::SynchronizeEnd)
        {
            nature = toInt(lead);
        }
        else
        {
            nature = rng.nextUInt(25);
        }

        GeneratorState state(initialAdvances + cnt, ec, pid, ivs, ability, gender, staticTemplate->getLevel(), nature, shiny,
                             staticTemplate->getInfo());
        if (filter.compareState(static_cast<const State &>(state)))
        {
            states.emplace_back(state);
        }
    }

    return states;
}
