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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "EggGenerator4.hpp"
#include <Core/Enum/Method.hpp>
#include <Core/Parents/Filters/StateFilter.hpp>
#include <Core/RNG/LCRNG.hpp>
#include <Core/RNG/MT.hpp>

EggGenerator4::EggGenerator4(u32 initialAdvances, u32 maxResults, u16 tid, u16 sid, u8 genderRatio, Method method,
                             const StateFilter &filter) :
    EggGenerator(initialAdvances, maxResults, tid, sid, genderRatio, method, filter)
{
}

QVector<EggState4> EggGenerator4::generate(u32 seed) const
{
    switch (method)
    {
    case Method::Gen4Normal:
        return generateNormal(seed);
    case Method::Gen4Masuada:
        return generateMasuada(seed);
    case Method::DPPtIVs:
        return generateDPPtIVs(seed);
    case Method::HGSSIVs:
        return generateHGSSIVs(seed);
    default:
        return QVector<EggState4>();
    }
}

QVector<EggState4> EggGenerator4::generateNormal(u32 seed) const
{
    QVector<EggState4> states;

    MT mt(seed);
    mt.advance(initialAdvances);

    for (u32 cnt = 0; cnt < maxResults; cnt++)
    {
        EggState4 state(initialAdvances + cnt);

        u32 pid = mt.next();
        state.setPID(pid);
        state.setAbility(pid & 1);
        state.setGender(pid & 255, genderRatio);
        state.setNature(pid % 25);
        state.setShiny(tsv, (pid >> 16) ^ (pid & 0xffff), 8);

        if (filter.comparePID(state))
        {
            states.append(state);
        }
    }

    return states;
}

QVector<EggState4> EggGenerator4::generateMasuada(u32 seed) const
{
    QVector<EggState4> states;

    MT mt(seed);
    mt.advance(initialAdvances);

    for (u32 cnt = 0; cnt < maxResults; cnt++)
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

            pid = pid * 0x6c078965 + 1; // Advance with ARNG
        }

        state.setPID(pid);
        state.setAbility(pid & 1);
        state.setGender(pid & 255, genderRatio);
        state.setNature(pid % 25);
        state.setShiny(tsv, (pid >> 16) ^ (pid & 0xffff), 8);

        if (filter.comparePID(state))
        {
            states.append(state);
        }
    }

    return states;
}

QVector<EggState4> EggGenerator4::generateDPPtIVs(u32 seed) const
{
    QVector<EggState4> states;

    PokeRNG rng(seed);
    rng.advance(initialAdvances);

    for (u32 cnt = 0; cnt < maxResults; cnt++, rng.next())
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

        setInheritance(state, inh, par, true);
        state.calculateHiddenPower();

        if (filter.compareIVs(state))
        {
            state.setSeed(iv1);
            states.append(state);
        }
    }
    return states;
}

QVector<EggState4> EggGenerator4::generateHGSSIVs(u32 seed) const
{
    QVector<EggState4> states;

    PokeRNG rng(seed);
    rng.advance(initialAdvances);

    for (u32 cnt = 0; cnt < maxResults; cnt++, rng.next())
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

        setInheritance(state, inh, par, false);
        state.calculateHiddenPower();

        if (filter.compareIVs(state))
        {
            state.setSeed(iv1);
            states.append(state);
        }
    }
    return states;
}

void EggGenerator4::setInheritance(EggState4 &state, const u16 *inh, const u16 *par, bool broken) const
{
    u8 available[6] = { 0, 1, 2, 3, 4, 5 };
    for (u8 i = 0; i < 3; i++)
    {
        u8 stat = available[inh[i] % (6 - i)];
        u8 parent = par[i] & 1;

        switch (stat)
        {
        case 0:
            state.setIVs(0, parent == 0 ? parent1.at(0) : parent2.at(0));
            state.setInheritance(0, parent + 1);
            break;
        case 1:
            state.setIVs(1, parent == 0 ? parent1.at(1) : parent2.at(1));
            state.setInheritance(1, parent + 1);
            break;
        case 2:
            state.setIVs(2, parent == 0 ? parent1.at(2) : parent2.at(2));
            state.setInheritance(2, parent + 1);
            break;
        case 3:
            state.setIVs(5, parent == 0 ? parent1.at(5) : parent2.at(5));
            state.setInheritance(5, parent + 1);
            break;
        case 4:
            state.setIVs(3, parent == 0 ? parent1.at(3) : parent2.at(3));
            state.setInheritance(3, parent + 1);
            break;
        case 5:
            state.setIVs(4, parent == 0 ? parent1.at(4) : parent2.at(4));
            state.setInheritance(4, parent + 1);
            break;
        }

        // Avoids repeat IV inheritance
        // In DPPt this doesn't work properly
        for (u8 j = broken ? i : stat; j < 5 - i; j++)
        {
            available[j] = available[j + 1];
        }
    }
}
