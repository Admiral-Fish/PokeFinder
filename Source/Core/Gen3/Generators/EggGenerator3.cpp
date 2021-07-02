/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2021 by Admiral_Fish, bumba, and EzPzStreamz
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
#include <Core/Parents/Filters/StateFilter.hpp>
#include <Core/RNG/LCRNG.hpp>
#include <algorithm>

template <bool broken>
void setInheritance(const Daycare &daycare, EggState3 &state, const u16 *inh, const u16 *par)
{
    constexpr u8 order[6] = { 0, 1, 2, 5, 3, 4 };

    if constexpr (broken)
    {
        // Avoid repeat IV inheritance doesn't work in Emerald
        constexpr u8 available1[6] = { 0, 1, 2, 3, 4, 5 };
        constexpr u8 available2[5] = { 1, 2, 3, 4, 5 };
        constexpr u8 available3[4] = { 1, 3, 4, 5 };

        u8 stat = available1[inh[0] % 6];
        u8 parent = par[0] & 1;
        state.setIVs(order[stat], daycare.getParentIV(parent, order[stat]));
        state.setInheritance(order[stat], parent + 1);

        stat = available2[inh[1] % 5];
        parent = par[1] & 1;
        state.setIVs(order[stat], daycare.getParentIV(parent, order[stat]));
        state.setInheritance(order[stat], parent + 1);

        stat = available1[inh[2] % 4];
        parent = par[2] & 1;
        state.setIVs(order[stat], daycare.getParentIV(parent, order[stat]));
        state.setInheritance(order[stat], parent + 1);
    }
    else
    {
        u8 available[6] = { 0, 1, 2, 3, 4, 5 };
        auto avoid = [&available](u8 stat, u8 i)
        {
            for (u8 j = stat; j < 5 - i; j++)
            {
                available[j] = available[j + 1];
            }
        };

        u8 stat = available[inh[0] % 6];
        u8 parent = par[0] & 1;
        state.setIVs(order[stat], daycare.getParentIV(parent, order[stat]));
        state.setInheritance(order[stat], parent + 1);

        avoid(stat, 0);

        stat = available[inh[1] % 5];
        parent = par[1] & 1;
        state.setIVs(order[stat], daycare.getParentIV(parent, order[stat]));
        state.setInheritance(order[stat], parent + 1);

        avoid(stat, 1);

        stat = available[inh[2] % 4];
        parent = par[2] & 1;
        state.setIVs(order[stat], daycare.getParentIV(parent, order[stat]));
        state.setInheritance(order[stat], parent + 1);
    }
}

EggGenerator3::EggGenerator3(u32 initialAdvances, u32 maxAdvances, u16 tid, u16 sid, u8 genderRatio, Method method,
                             const StateFilter &filter, const Daycare &daycare) :
    EggGenerator(initialAdvances, maxAdvances, tid, sid, genderRatio, method, filter, daycare)
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
    case Method::RSBred:
    case Method::FRLGBred:
        iv1 = 1;
        iv2 = 0;
        inh = 1;
        // par = 0;
        break;
    case Method::RSBredSplit:
    case Method::FRLGBredSplit:
        iv1 = 0;
        iv2 = 1;
        inh = 1;
        // par = 0;
        break;
    case Method::RSBredAlternate:
    case Method::FRLGBredAlternate:
        iv1 = 1;
        iv2 = 0;
        inh = 2;
        // par = 0;
        break;
    case Method::FRLGBredMixed:
        iv1 = 0;
        iv2 = 0;
        inh = 2;
        // par = 0;
        break;
    default:
        break;
    }
}

std::vector<EggState3> EggGenerator3::generate(u32 seed, u32 seed2) const
{
    switch (method)
    {
    case Method::EBredPID:
        return generateEmeraldPID();
    case Method::EBred:
    case Method::EBredSplit:
    case Method::EBredAlternate:
        return generateEmeraldIVs();
    case Method::RSBred:
    case Method::RSBredAlternate:
    case Method::RSBredSplit:
    case Method::FRLGBredSplit:
    case Method::FRLGBred:
    case Method::FRLGBredAlternate:
    case Method::FRLGBredMixed:
    {
        auto lower = generateLower(seed);
        return lower.empty() ? std::vector<EggState3>() : generateUpper(seed2, lower);
    }
    default:
        return std::vector<EggState3>();
    }
}

void EggGenerator3::setInitialAdvancesPickup(u32 value)
{
    initialAdvancesPickup = value;
}

void EggGenerator3::setMaxAdvancesPickup(u32 value)
{
    maxAdvancesPickup = value;
}

void EggGenerator3::setCalibration(u8 value)
{
    calibration = value;
}

void EggGenerator3::setMinRedraw(u8 value)
{
    minRedraw = value;
}

void EggGenerator3::setMaxRedraw(u8 value)
{
    maxRedraw = value;
}

void EggGenerator3::setCompatability(u8 value)
{
    compatability = value;
}

std::vector<EggState3> EggGenerator3::generateEmeraldPID() const
{
    std::vector<EggState3> states;

    // Determine if female parent is holding everstone
    bool everstone = false;
    u8 parent;
    for (u8 i = 0; i < 2; i++)
    {
        if (daycare.getParentGender(i) == 1)
        {
            if (daycare.getParentItem(i) == 1)
            {
                parent = i;
                everstone = true;
            }
        }
    }

    for (u8 i = 0; i < 2; i++)
    {
        if (daycare.getParentGender(i) == 3)
        {
            if (daycare.getParentItem(i) == 1)
            {
                parent = i;
                everstone = true;
            }
        }
    }

    PokeRNG rng(0);
    rng.advance(initialAdvances);

    u32 val = initialAdvances + 1;
    for (u32 cnt = 0; cnt <= maxAdvances; cnt++, val++, rng.next())
    {
        PokeRNG comp(rng.getSeed());
        if (((comp.nextUShort() * 100) / 0xFFFF) < compatability)
        {
            for (u16 redraw = minRedraw; redraw <= maxRedraw; redraw++)
            {
                PokeRNG go(comp.getSeed());

                u16 offset = calibration + 3 * redraw;
                EggState3 state(cnt + initialAdvances - offset);

                bool flag = everstone ? (go.nextUShort() >> 15) == 0 : false;

                PokeRNG trng((val - offset) & 0xFFFF);

                u32 pid = 0;
                if (!flag)
                {
                    pid = ((go.nextUShort() % 0xFFFE) + 1) | (trng.next() & 0xFFFF0000);
                    state.setNature(pid % 25);
                }
                else
                {
                    u8 i = 2;
                    do
                    {
                        // VBlank at 17 from starting PID generation
                        // Adjusted i value is 19
                        // Skip at this point since spread is unlikely to occur
                        if (i == 19)
                        {
                            break;
                        }

                        pid = go.nextUShort() | (trng.next() & 0xFFFF0000);
                        i++;
                    } while (pid % 25 != daycare.getParentNature(parent));

                    if (i == 19)
                    {
                        continue;
                    }
                    state.setNature(daycare.getParentNature(parent));
                }

                state.setPID(pid);
                state.setAbility(pid & 1);
                state.setGender(pid & 255, genderRatio);
                state.setShiny<8>(tsv, (pid >> 16) ^ (pid & 0xffff));

                if (filter.comparePID(state))
                {
                    state.setRedraw(redraw);
                    states.emplace_back(state);
                }
            }
        }
    }

    std::sort(states.begin(), states.end(),
              [](const EggState3 &state1, const EggState3 &state2) { return state1.getAdvances() < state2.getAdvances(); });

    return states;
}

std::vector<EggState3> EggGenerator3::generateEmeraldIVs() const
{
    std::vector<EggState3> states;

    PokeRNG rng(0);
    rng.advance(initialAdvances);

    for (u32 cnt = 0; cnt <= maxAdvances; cnt++, rng.next())
    {
        EggState3 state(cnt + initialAdvances);
        PokeRNG go(rng.getSeed());

        go.advance(this->iv1);
        u16 iv1 = go.nextUShort();
        go.advance(this->iv2);
        u16 iv2 = go.nextUShort();
        state.setIVs(iv1, iv2);

        go.advance(this->inh);
        u16 inh1 = go.nextUShort();
        u16 inh2 = go.nextUShort();
        u16 inh3 = go.nextUShort();
        u16 inh[3] = { inh1, inh2, inh3 };

        // go.advance(this->par);
        u16 par1 = go.nextUShort();
        u16 par2 = go.nextUShort();
        u16 par3 = go.nextUShort();
        u16 par[3] = { par1, par2, par3 };

        setInheritance<true>(daycare, state, inh, par);
        state.calculateHiddenPower();

        if (filter.compareIVs(state))
        {
            states.emplace_back(state);
        }
    }

    return states;
}

std::vector<std::pair<u32, u16>> EggGenerator3::generateLower(u32 seed) const
{
    std::vector<std::pair<u32, u16>> states;

    PokeRNG rng(seed);
    rng.advance(initialAdvances);

    for (u32 cnt = 0; cnt <= maxAdvances; cnt++, rng.next())
    {
        PokeRNG go(rng.getSeed());
        if (((go.nextUShort() * 100) / 0xFFFF) < compatability)
        {
            u16 pid = (go.nextUShort() % 0xFFFE) + 1;

            // TODO: decide on filtering for ability/gender
            states.emplace_back(cnt + initialAdvances, pid);
        }
    }

    return states;
}

std::vector<EggState3> EggGenerator3::generateUpper(u32 seed, const std::vector<std::pair<u32, u16>> &lower) const
{
    std::vector<EggState3> upper;

    PokeRNG rng(seed);
    rng.advance(initialAdvancesPickup);

    for (u32 cnt = 0; cnt <= maxAdvancesPickup; cnt++, rng.next())
    {
        EggState3 state;
        PokeRNG go(rng.getSeed());

        state.setPID(go.nextUShort());

        go.advance(this->iv1);
        u16 iv1 = go.nextUShort();
        go.advance(this->iv2);
        u16 iv2 = go.nextUShort();
        state.setIVs(iv1, iv2);

        go.advance(this->inh);
        u16 inh1 = go.nextUShort();
        u16 inh2 = go.nextUShort();
        u16 inh3 = go.nextUShort();
        u16 inh[3] = { inh1, inh2, inh3 };

        // go.advance(this->par);
        u16 par1 = go.nextUShort();
        u16 par2 = go.nextUShort();
        u16 par3 = go.nextUShort();
        u16 par[3] = { par1, par2, par3 };

        setInheritance<false>(daycare, state, inh, par);
        state.calculateHiddenPower();

        if (filter.compareIVs(state))
        {
            state.setPickupAdvance(cnt + initialAdvancesPickup);
            upper.emplace_back(state);
        }
    }

    std::vector<EggState3> states;
    for (const auto &low : lower)
    {
        for (auto up : upper)
        {
            up.setPID(up.getPID(), low.second);
            up.setAbility(low.second & 1);
            up.setGender(low.second & 255, genderRatio);
            up.setNature(up.getPID() % 25);
            up.setShiny<8>(tsv, (up.getPID() >> 16) ^ (up.getPID() & 0xffff));

            if (filter.comparePID(up))
            {
                up.setAdvances(low.first);
                states.emplace_back(up);
            }
        }
    }
    return states;
}
