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

#include "EggGenerator4.hpp"
#include <Core/Enum/Game.hpp>
#include <Core/Enum/Method.hpp>
#include <Core/Gen4/States/EggState4.hpp>
#include <Core/Parents/PersonalLoader.hpp>
#include <Core/RNG/LCRNG.hpp>
#include <Core/RNG/MT.hpp>
#include <Core/Util/Utilities.hpp>
#include <algorithm>

static bool compare(const EggGeneratorState4 &left, const EggGeneratorState4 &right)
{
    if (left.getAdvances() < right.getAdvances())
    {
        return true;
    }
    else if (right.getAdvances() < left.getAdvances())
    {
        return false;
    }

    if (left.getPickupAdvances() < right.getPickupAdvances())
    {
        return true;
    }
    else if (right.getPickupAdvances() < left.getPickupAdvances())
    {
        return false;
    }

    return false;
}

/**
 * @brief Inherits IVs from the parents
 * In DPPt this is bugged to first remove HP followed by Defense which means it is less likely to inherit those and allows the
 * possibility of repeat inheritance.
 *
 * In HGSS it works as expected with no repeat inheritance.
 *
 * @param daycare Daycare information
 * @param ivs Pokemon IVs to set
 * @param inheritance Pokemon inheritance values to set
 * @param inh Rand inheritance values
 * @param par Rand parent values
 * @param dppt Whether the game version is DPPt
 */
static void setInheritance(const Daycare &daycare, std::array<u8, 6> &ivs, std::array<u8, 6> &inheritance, const u8 *inh, const u8 *par,
                           bool dppt)
{
    if (dppt)
    {
        constexpr u8 available1[6] = { 0, 1, 2, 5, 3, 4 };
        constexpr u8 available2[5] = { 1, 2, 5, 3, 4 };
        constexpr u8 available3[4] = { 1, 5, 3, 4 };

        u8 stat = available1[inh[0]];
        ivs[stat] = daycare.getParentIV(par[0], stat);
        inheritance[stat] = par[0] + 1;

        stat = available2[inh[1]];
        ivs[stat] = daycare.getParentIV(par[1], stat);
        inheritance[stat] = par[1] + 1;

        stat = available3[inh[2]];
        ivs[stat] = daycare.getParentIV(par[2], stat);
        inheritance[stat] = par[2] + 1;
    }
    else
    {
        constexpr u8 order[6] = { 0, 1, 2, 5, 3, 4 };

        u8 available[6] = { 0, 1, 2, 3, 4, 5 };
        auto avoid = [&available](u8 index, u8 max) {
            for (u8 i = index; i < max; i++)
            {
                available[i] = available[i + 1];
            }
        };

        u8 stat = available[inh[0]];
        ivs[order[stat]] = daycare.getParentIV(par[0], order[stat]);
        inheritance[order[stat]] = par[0] + 1;

        avoid(inh[0], 5);

        stat = available[inh[1]];
        ivs[order[stat]] = daycare.getParentIV(par[1], order[stat]);
        inheritance[order[stat]] = par[1] + 1;

        avoid(inh[1], 4);

        stat = available[inh[2]];
        ivs[order[stat]] = daycare.getParentIV(par[2], order[stat]);
        inheritance[order[stat]] = par[2] + 1;
    }
}

EggGenerator4::EggGenerator4(u32 initialAdvances, u32 maxAdvances, u32 offset, u32 initialAdvancesPickup, u32 maxAdvancesPickup,
                             u32 offsetPickup, const Daycare &daycare, const Profile4 &profile, const StateFilter &filter) :
    EggGenerator(initialAdvances, maxAdvances, offset, Method::None, 0, daycare, profile, filter),

    initialAdvancesPickup(initialAdvancesPickup),
    maxAdvancesPickup(maxAdvancesPickup),
    offsetPickup(offsetPickup)
{
}

std::vector<EggGeneratorState4> EggGenerator4::generate(u32 seedHeld, u32 seedPickup) const
{
    auto held = generateHeld(seedHeld);
    if (!held.empty())
    {
        return generatePickup(seedPickup, held);
    }
    return held;
}

std::vector<EggGeneratorState4> EggGenerator4::generateHeld(u32 seed) const
{
    const PersonalInfo *base = PersonalLoader::getPersonal(profile.getVersion(), daycare.getEggSpecie());
    const PersonalInfo *male = nullptr;
    if (daycare.getEggSpecie() == 29) // Nidoran
    {
        male = PersonalLoader::getPersonal(profile.getVersion(), 32);
    }
    else if (daycare.getEggSpecie() == 314) // Illumise
    {
        male = PersonalLoader::getPersonal(profile.getVersion(), 313);
    }

    MT mt(seed, initialAdvances + offset);

    std::vector<EggGeneratorState4> states;
    for (u32 cnt = 0; cnt <= maxAdvances; cnt++)
    {
        u32 pid = mt.next();
        if (daycare.getMasuda())
        {
            ARNG rng(pid);
            for (int i = 0; i < 4; i++)
            {
                if (Utilities::getShiny<true>(pid, tsv))
                {
                    break;
                }
                pid = rng.next();
            }
        }

        const PersonalInfo *info = base;
        if (male && (pid & 0x8000))
        {
            info = male;
        }

        EggGeneratorState4 state(initialAdvances + cnt, pid, Utilities::getGender(pid, info), Utilities::getShiny<true>(pid, tsv), info);
        if (filter.compareAbility(state.getAbility()) && filter.compareGender(state.getGender()) && filter.compareNature(state.getNature())
            && filter.compareShiny(state.getShiny()))
        {
            states.emplace_back(state);
        }
    }

    return states;
}

std::vector<EggGeneratorState4> EggGenerator4::generatePickup(u32 seed, const std::vector<EggGeneratorState4> &held) const
{
    const PersonalInfo *base = PersonalLoader::getPersonal(profile.getVersion(), daycare.getEggSpecie());
    const PersonalInfo *male = nullptr;
    if (daycare.getEggSpecie() == 29) // Nidoran
    {
        male = PersonalLoader::getPersonal(profile.getVersion(), 32);
    }
    else if (daycare.getEggSpecie() == 314) // Illumise
    {
        male = PersonalLoader::getPersonal(profile.getVersion(), 313);
    }

    PokeRNG rng(seed, initialAdvancesPickup);
    auto jump = rng.getJump(offsetPickup);

    std::vector<EggGeneratorState4> states;
    for (u32 cnt = 0; cnt <= maxAdvancesPickup; cnt++)
    {
        PokeRNG go(rng, jump);

        u16 iv1 = go.nextUShort();
        u16 iv2 = go.nextUShort();

        std::array<u8, 6> ivs;
        ivs[0] = iv1 & 31;
        ivs[1] = (iv1 >> 5) & 31;
        ivs[2] = (iv1 >> 10) & 31;
        ivs[3] = (iv2 >> 5) & 31;
        ivs[4] = (iv2 >> 10) & 31;
        ivs[5] = iv2 & 31;

        u8 inh[3];
        inh[0] = go.nextUShort(6);
        inh[1] = go.nextUShort(5);
        inh[2] = go.nextUShort(4);

        u8 par[3];
        par[0] = go.nextUShort(2);
        par[1] = go.nextUShort(2);
        par[2] = go.nextUShort(2);

        std::array<u8, 6> inheritance = { 0, 0, 0, 0, 0, 0 };
        setInheritance(daycare, ivs, inheritance, inh, par, (profile.getVersion() & Game::DPPt) != Game::None);

        u16 prng = rng.nextUShort();
        for (auto state : held)
        {
            const PersonalInfo *info = base;
            if (male && (state.getPID() & 0x8000))
            {
                info = male;
            }

            state.update(prng, initialAdvancesPickup + cnt, ivs, inheritance, info);
            if (filter.compareHiddenPower(state.getHiddenPower()) && filter.compareIV(state.getIVs()))
            {
                states.emplace_back(state);
            }
        }
    }

    std::sort(states.begin(), states.end(), compare);
    return states;
}
