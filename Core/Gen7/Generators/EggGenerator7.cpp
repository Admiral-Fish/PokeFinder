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

#include "EggGenerator7.hpp"
#include <Core/Enum/Method.hpp>
#include <Core/Parents/PersonalInfo.hpp>
#include <Core/Parents/PersonalLoader.hpp>
#include <Core/Parents/States/EggState.hpp>
#include <Core/RNG/RNGList.hpp>
#include <Core/RNG/TinyMT.hpp>
#include <Core/Util/Utilities.hpp>

EggGenerator7::EggGenerator7(u32 initialAdvances, u32 maxAdvances, u32 offset, const Daycare &daycare, const Profile7 &profile,
                             const StateFilter &filter) :
    EggGenerator(initialAdvances, maxAdvances, offset, Method::None, daycare, profile, filter), shinyCharm(profile.getShinyCharm())
{
}

std::vector<EggGeneratorState> EggGenerator7::generate(u32 seed0, u32 seed1, u32 seed2, u32 seed3) const
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

    RNGList<u32, TinyMT, 64> rngList(seed0, seed1, seed2, seed3, initialAdvances + offset);

    u8 pidRolls = 0;
    if (daycare.getMasuda())
    {
        pidRolls += 6;
    }
    if (shinyCharm)
    {
        pidRolls += 2;
    }

    u8 inheritanceCount = 3;
    if (daycare.getParentItem(0) == 8 || daycare.getParentItem(1) == 8)
    {
        inheritanceCount = 5;
    }

    std::vector<EggGeneratorState> states;
    for (u32 cnt = 0; cnt <= maxAdvances; cnt++, rngList.advanceState())
    {
        // Nidoran
        // Volbeat / Illumise
        u8 gender;
        const PersonalInfo *info = base;
        if (daycare.getEggSpecie() == 29 || daycare.getEggSpecie() == 32 || daycare.getEggSpecie() == 313 || daycare.getEggSpecie() == 314)
        {
            gender = rngList.next(2);
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
                gender = rngList.next(252) < base->getGender();
                break;
            }
        }

        u8 nature = rngList.next(25);
        if (daycare.getEverstoneCount() == 2)
        {
            nature = daycare.getParentNature(rngList.next(2));
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
        u8 ability = rngList.next(100);
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

        // Power Items
        u8 inherit = 0;
        std::array<u8, 6> inheritance = { 0, 0, 0, 0, 0, 0 };
        u8 powerItem = daycare.getPowerItemCount();
        if (powerItem != 0)
        {
            inherit = 1;
            if (powerItem == 2)
            {
                u8 parent = rngList.next(2);
                u8 item = daycare.getParentItem(parent);

                inheritance[item - 2] = parent + 1;
            }
            else
            {
                u8 parent = (daycare.getParentItem(0) >= 2 && daycare.getParentItem(0) <= 7) ? 0 : 1;
                u8 item = daycare.getParentItem(parent);

                inheritance[item - 2] = parent + 1;
            }
        }

        // Determine inheritance
        for (; inherit < inheritanceCount;)
        {
            u8 index = rngList.next(6);
            if (inheritance[index] == 0)
            {
                inheritance[index] = rngList.next(2) + 1;
                inherit++;
            }
        }

        // Assign IVs and inheritance
        std::array<u8, 6> ivs;
        for (u8 i = 0; i < 6; i++)
        {
            u8 iv = rngList.next(32);
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

        u32 ec = rngList.next();

        // Assign PID if we have masuda or shiny charm
        u32 pid = 0;
        for (u8 roll = 0; roll < pidRolls; roll++)
        {
            pid = rngList.next();
            if (Utilities::isShiny<false>(pid, tsv))
            {
                break;
            }
        }

        // Ball handling check
        // Uses a rand call, maybe add later

        EggGeneratorState state(initialAdvances + cnt, ec, pid, ivs, ability, gender, 1, nature, Utilities::getShiny<false>(pid, tsv),
                                inheritance, info);
        if (filter.compare(static_cast<const State &>(state)))
        {
            states.emplace_back(state);
        }
    }

    return states;
}
