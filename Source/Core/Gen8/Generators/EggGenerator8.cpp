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

#include "EggGenerator8.hpp"
#include <Core/Enum/Method.hpp>
#include <Core/Parents/PersonalInfo.hpp>
#include <Core/Parents/PersonalLoader.hpp>
#include <Core/Parents/States/EggState.hpp>
#include <Core/RNG/RNGList.hpp>
#include <Core/RNG/Xoroshiro.hpp>
#include <Core/RNG/Xorshift.hpp>

static u32 gen(Xorshift &rng)
{
    return rng.next(0x80000000, 0x7fffffff);
}

EggGenerator8::EggGenerator8(u32 initialAdvances, u32 maxAdvances, u32 delay, u8 compatability, const Daycare &daycare,
                             const Profile8 &profile, const StateFilter8 &filter) :
    EggGenerator(initialAdvances, maxAdvances, delay, Method::None, compatability, daycare, profile, filter),
    shinyCharm(profile.getShinyCharm())
{
}

std::vector<EggGeneratorState> EggGenerator8::generate(u64 seed0, u64 seed1) const
{
    const PersonalInfo *base = PersonalLoader::getPersonal(profile.getVersion(), daycare.getEggSpecie());
    const PersonalInfo *male;
    const PersonalInfo *female;
    if (daycare.getEggSpecie() == 29 || daycare.getEggSpecie() == 32)
    {
        male = PersonalLoader::getPersonal(profile.getVersion(), 32);
        female = PersonalLoader::getPersonal(profile.getVersion(), 29);
    }
    else if (daycare.getEggSpecie() == 313 || daycare.getEggSpecie() == 314)
    {
        male = PersonalLoader::getPersonal(profile.getVersion(), 313);
        female = PersonalLoader::getPersonal(profile.getVersion(), 314);
    }

    RNGList<u32, Xorshift, 2, gen> rngList(seed0, seed1, initialAdvances + delay);

    u8 pidRolls = 0;
    if (daycare.getMasuda())
    {
        pidRolls += 6;
    }
    if (shinyCharm)
    {
        pidRolls += 2;
    }

    // Intentionally ignoring power items
    u8 inheritanceCount = 3;
    if (daycare.getParentItem(0) == 8 || daycare.getParentItem(1) == 8)
    {
        inheritanceCount = 5;
    }

    std::vector<EggGeneratorState> states;
    for (u32 cnt = 0; cnt <= maxAdvances; cnt++, rngList.advanceState())
    {
        if ((rngList.next() % 100) < compatability)
        {
            // Sign extend seed to signed 64bit
            u64 seed = rngList.next();
            if (seed & 0x80000000)
            {
                seed |= 0xffffffff00000000;
            }

            XoroshiroBDSP rng(seed);

            // Nidoran
            // Volbeat / Illumise
            u8 gender;
            const PersonalInfo *info = base;
            if (daycare.getEggSpecie() == 29 || daycare.getEggSpecie() == 32 || daycare.getEggSpecie() == 313
                || daycare.getEggSpecie() == 314)
            {
                gender = rng.nextUInt(2);
                info = gender ? female : male;
            }
            else
            {
                switch (base->getGender())
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
                    gender = rng.nextUInt(252) + 1 < base->getGender();
                    break;
                }
            }

            u8 nature = rng.nextUInt(25);
            if (daycare.getEverstoneCount() == 2)
            {
                nature = daycare.getParentNature(rng.nextUInt(2));
            }
            else if (daycare.getParentItem(0) == 1)
            {
                nature = daycare.getParentNature(0);
            }
            else if (daycare.getParentItem(1) == 1)
            {
                nature = daycare.getParentNature(1);
            }

            // If we have a ditto acting as the female, get the ability from the other parent (this will be slot 0)
            u8 parentAbility = daycare.getParentAbility(daycare.getParentGender(1) == 3 ? 0 : 1);
            u8 ability = rng.nextUInt(100);
            if (parentAbility == 2)
            {
                ability = ability < 20 ? 0 : ability < 40 ? 1 : 2;
            }
            else if (parentAbility == 1)
            {
                ability = ability < 20 ? 0 : 1;
            }
            else
            {
                ability = ability < 80 ? 0 : 1;
            }

            // Determine inheritance
            std::array<u8, 6> inheritance = { 0, 0, 0, 0, 0, 0 };
            for (u8 i = 0; i < inheritanceCount;)
            {
                u8 index = rng.nextUInt(6);
                if (inheritance[index] == 0)
                {
                    inheritance[index] = rng.nextUInt(2) + 1;
                    i++;
                }
            }

            // Assign IVs and inheritance
            std::array<u8, 6> ivs;
            for (u8 i = 0; i < 6; i++)
            {
                u8 iv = rng.nextUInt(32);
                if (inheritance[i] == 1)
                {
                    iv = daycare.getParentIV(0, i);
                }
                else if (inheritance[i] == 2)
                {
                    iv = daycare.getParentIV(1, i);
                }
                ivs[i] = iv;
            }

            // Encryption constant
            rng.next();

            // Assign PID if we have masuda or shiny charm
            u32 pid = 0;
            u16 psv = 0;
            for (u8 roll = 0; roll < pidRolls; roll++)
            {
                pid = rng.nextUInt(0xffffffff);
                psv = (pid >> 16) ^ (pid & 0xffff);
                if ((psv ^ tsv) < 16)
                {
                    break;
                }
            }
            u8 shiny = (psv ^ tsv) < 16;

            // Ball handling check
            // Uses a rand call, maybe add later

            EggGeneratorState state(initialAdvances + cnt, pid, ivs, ability, gender, 1, nature, shiny, inheritance, info);
            if (filter.compareState(state))
            {
                states.emplace_back(state);
            }
        }
    }

    return states;
}
