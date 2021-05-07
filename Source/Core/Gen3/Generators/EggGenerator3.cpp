/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2020 by Admiral_Fish, bumba, and EzPzStreamz
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
                state.setShiny(tsv, (pid >> 16) ^ (pid & 0xffff), 8);

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

        setInheritance(state, inh, par, true);
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
        rng.advance(this->iv2);
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

        setInheritance(state, inh, par, false);
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
            up.setShiny(tsv, (up.getPID() >> 16) ^ (up.getPID() & 0xffff), 8);

            if (filter.comparePID(up))
            {
                up.setAdvances(low.first);
                states.emplace_back(up);
            }
        }
    }
    return states;
}

void EggGenerator3::setInheritance(EggState3 &state, const u16 *inh, const u16 *par, bool broken) const
{
    u8 available[6] = { 0, 1, 2, 3, 4, 5 };
    for (u8 i = 0; i < 3; i++)
    {
        u8 stat = available[inh[i] % (6 - i)];
        u8 parent = par[i] & 1;

        switch (stat)
        {
        case 0:
            state.setIVs(0, daycare.getParentIV(parent, 0));
            state.setInheritance(0, parent + 1);
            break;
        case 1:
            state.setIVs(1, daycare.getParentIV(parent, 1));
            state.setInheritance(1, parent + 1);
            break;
        case 2:
            state.setIVs(2, daycare.getParentIV(parent, 2));
            state.setInheritance(2, parent + 1);
            break;
        case 3:
            state.setIVs(5, daycare.getParentIV(parent, 5));
            state.setInheritance(5, parent + 1);
            break;
        case 4:
            state.setIVs(3, daycare.getParentIV(parent, 3));
            state.setInheritance(3, parent + 1);
            break;
        case 5:
            state.setIVs(4, daycare.getParentIV(parent, 4));
            state.setInheritance(4, parent + 1);
            break;
        }

        // Avoids repeat IV inheritance
        // In Emerald this doesn't work properly
        for (u8 j = broken ? i : stat; j < 5 - i; j++)
        {
            available[j] = available[j + 1];
        }
    }
}
