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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301,
 * USA.
 */

#include "EggGenerator3.hpp"
#include <Core/Enum/Method.hpp>
#include <Core/Gen3/States/EggState3.hpp>
#include <Core/Parents/PersonalInfo.hpp>
#include <Core/Parents/PersonalLoader.hpp>
#include <Core/RNG/LCRNG.hpp>
#include <Core/Util/Utilities.hpp>
#include <algorithm>

static bool compare(const EggState3 &left, const EggState3 &right)
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
 * In Emerald this is bugged to first remove HP followed by Defense which means it is less likely to inherit those and allows the
 * possibility of repeat inheritance.
 *
 * In RS/FRLG it does not remove an index but the value at an index so it can also lead to repeat inheritance.
 *
 * @tparam emerald Whether the game version is Emerald
 * @param daycare Daycare information
 * @param ivs Pokemon IVs to set
 * @param inheritance Pokemon inheritance values to set
 * @param inh Rand inheritance values
 * @param par Rand parent values
 */
template <bool emerald>
static void setInheritance(const Daycare &daycare, std::array<u8, 6> &ivs, std::array<u8, 6> &inheritance, const u8 *inh, const u8 *par)
{
    constexpr u8 order[6] = { 0, 1, 2, 5, 3, 4 };

    if constexpr (emerald)
    {
        constexpr u8 available1[6] = { 0, 1, 2, 3, 4, 5 };
        constexpr u8 available2[5] = { 1, 2, 3, 4, 5 };
        constexpr u8 available3[4] = { 1, 3, 4, 5 };

        u8 stat = available1[inh[0]];
        ivs[order[stat]] = daycare.getParentIV(par[0], order[stat]);
        inheritance[order[stat]] = par[0] + 1;

        stat = available2[inh[1]];
        ivs[order[stat]] = daycare.getParentIV(par[1], order[stat]);
        inheritance[order[stat]] = par[1] + 1;

        stat = available3[inh[2]];
        ivs[order[stat]] = daycare.getParentIV(par[2], order[stat]);
        inheritance[order[stat]] = par[2] + 1;
    }
    else
    {
        u8 available[6] = { 0, 1, 2, 3, 4, 5 };
        auto avoid = [&available](u8 stat, u8 i) {
            for (u8 j = stat; j < 5 - i; j++)
            {
                available[j] = available[j + 1];
            }
        };

        u8 stat = available[inh[0]];
        ivs[order[stat]] = daycare.getParentIV(par[0], order[stat]);
        inheritance[order[stat]] = par[0] + 1;

        avoid(stat, 0);

        stat = available[inh[1]];
        ivs[order[stat]] = daycare.getParentIV(par[1], order[stat]);
        inheritance[order[stat]] = par[1] + 1;

        avoid(stat, 1);

        stat = available[inh[2]];
        ivs[order[stat]] = daycare.getParentIV(par[2], order[stat]);
        inheritance[order[stat]] = par[2] + 1;
    }
}

EggGenerator3::EggGenerator3(u32 initialAdvances, u32 maxAdvances, u32 delay, u32 initialAdvancesPickup, u32 maxAdvancesPickup,
                             u32 delayPickup, u8 calibration, u8 minRedraw, u8 maxRedraw, Method method, u8 compatability,
                             const Daycare &daycare, const Profile3 &profile, const StateFilter &filter) :
    EggGenerator(initialAdvances, maxAdvances, delay, method, compatability, daycare, profile, filter),
    delayPickup(delayPickup),
    initialAdvancesPickup(initialAdvancesPickup),
    maxAdvancesPickup(maxAdvancesPickup),
    calibration(calibration),
    maxRedraw(maxRedraw),
    minRedraw(minRedraw)
{
    switch (method)
    {
    case Method::EBred:
        iv1 = 0;
        iv2 = 0;
        inh = 1;
        // par = 0;
        break;
    case Method::EBredSplit:
        iv1 = 0;
        iv2 = 1;
        inh = 1;
        // par = 0;
        break;
    case Method::EBredAlternate:
        iv1 = 0;
        iv2 = 0;
        inh = 2;
        // par = 0;
        break;
    case Method::RSFRLGBred:
        iv1 = 1;
        iv2 = 0;
        inh = 1;
        // par = 0;
        break;
    case Method::RSFRLGBredSplit:
        iv1 = 0;
        iv2 = 1;
        inh = 1;
        // par = 0;
        break;
    case Method::RSFRLGBredAlternate:
        iv1 = 1;
        iv2 = 0;
        inh = 2;
        // par = 0;
        break;
    case Method::RSFRLGBredMixed:
        iv1 = 0;
        iv2 = 0;
        inh = 2;
        // par = 0;
        break;
    default:
        break;
    }
}

std::vector<EggState3> EggGenerator3::generate(u32 seedHeld, u32 seedPickup) const
{
    switch (method)
    {
    case Method::EBred:
    case Method::EBredSplit:
    case Method::EBredAlternate:
    {
        auto held = generateEmeraldHeld();
        return held.empty() ? held : generateEmeraldPickup(held);
    }
    case Method::RSFRLGBredSplit:
    case Method::RSFRLGBred:
    case Method::RSFRLGBredAlternate:
    case Method::RSFRLGBredMixed:
    {
        auto held = generateRSFRLGHeld(seedHeld);
        return held.empty() ? held : generateRSFRLGPickup(seedPickup, held);
    }
    default:
        return std::vector<EggState3>();
    }
}

std::vector<EggState3> EggGenerator3::generateEmeraldHeld() const
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

    // Determine if female parent is holding everstone
    u8 parent;
    for (u8 i = 0; i < 2; i++)
    {
        if (daycare.getParentGender(i) == 1)
        {
            parent = i;
        }
    }

    for (u8 i = 0; i < 2; i++)
    {
        if (daycare.getParentGender(i) == 3)
        {
            parent = i;
        }
    }

    bool everstone = daycare.getParentItem(parent) == 1;

    PokeRNG rng(0, initialAdvances + delay);
    u32 val = initialAdvances + delay + 1;

    std::vector<EggState3> states;
    for (u32 cnt = 0; cnt <= maxAdvances; cnt++, val++)
    {
        if (((rng.nextUShort() * 100) / 0xffff) >= compatability)
        {
            continue;
        }

        for (u16 redraw = minRedraw; redraw <= maxRedraw; redraw++)
        {
            PokeRNG go(rng);

            u16 offset = calibration + 3 * redraw;
            bool flag = everstone ? (go.nextUShort() >> 15) == 0 : false;
            PokeRNG trng((val - offset) & 0xffff);

            u32 pid;
            if (!flag)
            {
                pid = (go.nextUShort(0xfffe) + 1) | (trng.next() & 0xffff0000);
            }
            else
            {
                u8 i = 0;
                do
                {
                    // VBlank at 17 from starting PID generation
                    // Skip at this point since spread is unlikely to occur
                    if (i == 17)
                    {
                        break;
                    }

                    i++;
                    pid = go.nextUShort() | (trng.next() & 0xffff0000);
                } while (pid % 25 != daycare.getParentNature(parent));

                if (i == 17)
                {
                    continue;
                }
            }

            const PersonalInfo *info = base;
            if (male && (pid & 0x8000))
            {
                info = male;
            }

            EggState3 state(initialAdvances + cnt - offset, redraw, pid, Utilities::getGender(pid, info), Utilities::getShiny(pid, tsv),
                            info);
            if (filter.compareAbility(state.getAbility()) && filter.compareGender(state.getGender()))
            {
                states.emplace_back(state);
            }
        }
    }

    return states;
}

std::vector<EggState3> EggGenerator3::generateEmeraldPickup(const std::vector<EggState3> &held) const
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

    PokeRNG rng(0, initialAdvancesPickup + delayPickup);

    std::vector<EggState3> states;
    for (u32 cnt = 0; cnt <= maxAdvancesPickup; cnt++, rng.next())
    {
        PokeRNG go(rng);

        go.advance(this->iv1);
        u16 iv1 = go.nextUShort();
        go.advance(this->iv2);
        u16 iv2 = go.nextUShort();

        std::array<u8, 6> ivs;
        ivs[0] = iv1 & 31;
        ivs[1] = (iv1 >> 5) & 31;
        ivs[2] = (iv1 >> 10) & 31;
        ivs[3] = (iv2 >> 5) & 31;
        ivs[4] = (iv2 >> 10) & 31;
        ivs[5] = iv2 & 31;

        go.advance(this->inh);
        u8 inh[3];
        inh[0] = go.nextUShort(6);
        inh[1] = go.nextUShort(5);
        inh[2] = go.nextUShort(4);

        // go.advance(this->par);
        u8 par[3];
        par[0] = go.nextUShort(2);
        par[1] = go.nextUShort(2);
        par[2] = go.nextUShort(2);

        std::array<u8, 6> inheritance = { 0, 0, 0, 0, 0, 0 };
        setInheritance<true>(daycare, ivs, inheritance, inh, par);

        for (auto state : held)
        {
            const PersonalInfo *info = base;
            if (male && (state.getPID() & 0x8000))
            {
                info = male;
            }

            state.update(initialAdvancesPickup + cnt, ivs, inheritance, info);
            if (filter.compareHiddenPower(state.getHiddenPower()) && filter.compareNature(state.getNature())
                && filter.compareShiny(state.getShiny()) && filter.compareIV(state.getIVs()))
            {
                states.emplace_back(state);
            }
        }
    }

    std::sort(states.begin(), states.end(), compare);
    return states;
}

std::vector<EggState3> EggGenerator3::generateRSFRLGHeld(u32 seed) const
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

    PokeRNG rng(seed, initialAdvances);

    std::vector<EggState3> states;
    for (u32 cnt = 0; cnt <= maxAdvances; cnt++, rng.next())
    {
        PokeRNG go(rng);
        if (((go.nextUShort() * 100) / 0xffff) < compatability)
        {
            u16 pid = go.nextUShort(0xfffe) + 1;
            const PersonalInfo *info = base;
            if (male && (pid & 0x8000))
            {
                info = male;
            }

            EggState3 state(initialAdvances + cnt, pid, Utilities::getGender(pid, info), info);
            if (filter.compareAbility(state.getAbility()) && filter.compareGender(state.getGender()))
            {
                states.emplace_back(state);
            }
        }
    }

    return states;
}

std::vector<EggState3> EggGenerator3::generateRSFRLGPickup(u32 seed, const std::vector<EggState3> &held) const
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

    PokeRNG rng(seed, initialAdvancesPickup + delayPickup);

    std::vector<EggState3> states;
    for (u32 cnt = 0; cnt <= maxAdvancesPickup; cnt++, rng.next())
    {
        PokeRNG go(rng);

        u32 high = go.nextUShort() << 16;

        go.advance(this->iv1);
        u16 iv1 = go.nextUShort();
        go.advance(this->iv2);
        u16 iv2 = go.nextUShort();

        std::array<u8, 6> ivs;
        ivs[0] = iv1 & 31;
        ivs[1] = (iv1 >> 5) & 31;
        ivs[2] = (iv1 >> 10) & 31;
        ivs[3] = (iv2 >> 5) & 31;
        ivs[4] = (iv2 >> 10) & 31;
        ivs[5] = iv2 & 31;

        go.advance(this->inh);
        u8 inh[3];
        inh[0] = go.nextUShort(6);
        inh[1] = go.nextUShort(5);
        inh[2] = go.nextUShort(4);

        // go.advance(this->par);
        u8 par[3];
        par[0] = go.nextUShort(2);
        par[1] = go.nextUShort(2);
        par[2] = go.nextUShort(2);

        std::array<u8, 6> inheritance = { 0, 0, 0, 0, 0, 0 };
        setInheritance<false>(daycare, ivs, inheritance, inh, par);

        for (auto state : held)
        {
            u32 pid = high | state.getPID();
            const PersonalInfo *info = base;
            if (male && (pid & 0x8000))
            {
                info = male;
            }

            state.update(initialAdvancesPickup + cnt, pid, Utilities::getShiny(pid, tsv), ivs, inheritance, info);
            if (filter.compareHiddenPower(state.getHiddenPower()) && filter.compareNature(state.getNature())
                && filter.compareShiny(state.getShiny()) && filter.compareIV(state.getIVs()))
            {
                states.emplace_back(state);
            }
        }
    }

    std::sort(states.begin(), states.end(), compare);
    return states;
}
