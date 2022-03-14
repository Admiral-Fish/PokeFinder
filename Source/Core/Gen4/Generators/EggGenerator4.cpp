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

#include "EggGenerator4.hpp"
#include <Core/Enum/Method.hpp>
#include <Core/Gen4/States/EggState4.hpp>
#include <Core/RNG/LCRNG.hpp>
#include <Core/RNG/MT.hpp>

template <bool broken>
void setInheritance(const Daycare &daycare, EggState4 &state, const u16 *inh, const u16 *par)
{
    constexpr u8 order[6] = { 0, 1, 2, 5, 3, 4 };

    if constexpr (broken)
    {
        // Avoid repeat IV inheritance doesn't work in DPPt
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

        stat = available3[inh[2] % 4];
        parent = par[2] & 1;
        state.setIVs(order[stat], daycare.getParentIV(parent, order[stat]));
        state.setInheritance(order[stat], parent + 1);
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

EggGenerator4::EggGenerator4(u32 initialAdvances, u32 maxAdvances, u16 tid, u16 sid, u8 genderRatio, Method method,
                             const StateFilter &filter, const Daycare &daycare) :
    EggGenerator(initialAdvances, maxAdvances, tid, sid, genderRatio, method, filter, daycare)
{
}

std::vector<EggState4> EggGenerator4::generate(u32 seed) const
{
    switch (method)
    {
    case Method::Gen4Normal:
        return generateNormal(seed);
    case Method::Gen4Masuda:
        return generateMasuda(seed);
    case Method::DPPtIVs:
        return generateDPPtIVs(seed);
    case Method::HGSSIVs:
        return generateHGSSIVs(seed);
    default:
        return std::vector<EggState4>();
    }
}

std::vector<EggState4> EggGenerator4::generateNormal(u32 seed) const
{
    std::vector<EggState4> states;

    MT mt(seed);
    mt.advance(initialAdvances);

    for (u32 cnt = 0; cnt <= maxAdvances; cnt++)
    {
        EggState4 state(initialAdvances + cnt);

        u32 pid = mt.next();
        state.setPID(pid);
        state.setAbility(pid & 1);
        state.setGender(pid & 255, genderRatio);
        state.setNature(pid % 25);
        state.setShiny<8>(tsv, (pid >> 16) ^ (pid & 0xffff));

        if (filter.comparePID(state))
        {
            states.emplace_back(state);
        }
    }

    return states;
}

std::vector<EggState4> EggGenerator4::generateMasuda(u32 seed) const
{
    std::vector<EggState4> states;

    MT mt(seed);
    mt.advance(initialAdvances);

    for (u32 cnt = 0; cnt <= maxAdvances; cnt++)
    {
        EggState4 state(initialAdvances + cnt);

        u32 pid = mt.next();
        for (int i = 0; i <= 3; i++)
        {
            u16 val = (pid >> 16) ^ (pid & 0xFFFF);

            if ((val ^ tsv) < 8)
            {
                break;
            }

            pid = ARNG(pid).next();
        }

        state.setPID(pid);
        state.setAbility(pid & 1);
        state.setGender(pid & 255, genderRatio);
        state.setNature(pid % 25);
        state.setShiny<8>(tsv, (pid >> 16) ^ (pid & 0xffff));

        if (filter.comparePID(state))
        {
            states.emplace_back(state);
        }
    }

    return states;
}

std::vector<EggState4> EggGenerator4::generateDPPtIVs(u32 seed) const
{
    std::vector<EggState4> states;

    PokeRNG rng(seed);
    rng.advance(initialAdvances);

    for (u32 cnt = 0; cnt <= maxAdvances; cnt++, rng.next())
    {
        EggState4 state(initialAdvances + cnt);
        PokeRNG go(rng.getSeed());

        u16 iv1 = go.nextUShort();
        u16 iv2 = go.nextUShort();
        state.setIVs(iv1, iv2);

        u16 inh1 = go.nextUShort();
        u16 inh2 = go.nextUShort();
        u16 inh3 = go.nextUShort();
        u16 inh[3] = { inh1, inh2, inh3 };

        u16 par1 = go.nextUShort();
        u16 par2 = go.nextUShort();
        u16 par3 = go.nextUShort();
        u16 par[3] = { par1, par2, par3 };

        setInheritance<true>(daycare, state, inh, par);
        state.calculateHiddenPower();

        if (filter.compareIVs(state))
        {
            state.setSeed(iv1);
            states.emplace_back(state);
        }
    }
    return states;
}

std::vector<EggState4> EggGenerator4::generateHGSSIVs(u32 seed) const
{
    std::vector<EggState4> states;

    PokeRNG rng(seed);
    rng.advance(initialAdvances);

    for (u32 cnt = 0; cnt <= maxAdvances; cnt++, rng.next())
    {
        EggState4 state(initialAdvances + cnt);
        PokeRNG go(rng.getSeed());

        u16 iv1 = go.nextUShort();
        u16 iv2 = go.nextUShort();
        state.setIVs(iv1, iv2);

        u16 inh1 = go.nextUShort();
        u16 inh2 = go.nextUShort();
        u16 inh3 = go.nextUShort();
        u16 inh[3] = { inh1, inh2, inh3 };

        u16 par1 = go.nextUShort();
        u16 par2 = go.nextUShort();
        u16 par3 = go.nextUShort();
        u16 par[3] = { par1, par2, par3 };

        setInheritance<false>(daycare, state, inh, par);
        state.calculateHiddenPower();

        if (filter.compareIVs(state))
        {
            state.setSeed(iv1);
            states.emplace_back(state);
        }
    }
    return states;
}
