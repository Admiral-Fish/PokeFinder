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
#include <Core/Parents/Filters/FrameFilter.hpp>
#include <Core/RNG/LCRNG.hpp>
#include <Core/RNG/MTRNG.hpp>

EggGenerator4::EggGenerator4(u32 initialFrame, u32 maxResults, u16 tid, u16 sid, u8 genderRatio, Method method, const FrameFilter &filter) :
    EggGenerator(initialFrame, maxResults, tid, sid, genderRatio, method, filter)
{
}

QVector<EggFrame4> EggGenerator4::generate(u32 seed) const
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
        return QVector<EggFrame4>();
    }
}

QVector<EggFrame4> EggGenerator4::generateNormal(u32 seed) const
{
    QVector<EggFrame4> frames;

    MersenneTwister mt(seed);
    mt.advanceFrames(initialFrame - 1);

    for (u32 cnt = 0; cnt < maxResults; cnt++)
    {
        EggFrame4 frame(initialFrame + cnt);

        u32 pid = mt.nextUInt();
        frame.setPID(pid);
        frame.setAbility(pid & 1);
        frame.setGender(pid & 255, genderRatio);
        frame.setNature(pid % 25);
        frame.setShiny(tsv, (pid >> 16) ^ (pid & 0xffff), 8);

        if (filter.comparePID(frame))
        {
            frames.append(frame);
        }
    }

    return frames;
}

QVector<EggFrame4> EggGenerator4::generateMasuada(u32 seed) const
{
    QVector<EggFrame4> frames;

    MersenneTwister mt(seed);
    mt.advanceFrames(initialFrame - 1);

    for (u32 cnt = 0; cnt < maxResults; cnt++)
    {
        EggFrame4 frame(initialFrame + cnt);

        u32 pid = mt.nextUInt();
        for (int i = 0; i <= 3; i++)
        {
            u16 val = (pid >> 16) ^ (pid & 0xFFFF);

            if ((val ^ tsv) < 8)
            {
                break;
            }

            pid = pid * 0x6c078965 + 1; // Advance with ARNG
        }

        frame.setPID(pid);
        frame.setAbility(pid & 1);
        frame.setGender(pid & 255, genderRatio);
        frame.setNature(pid % 25);
        frame.setShiny(tsv, (pid >> 16) ^ (pid & 0xffff), 8);

        if (filter.comparePID(frame))
        {
            frames.append(frame);
        }
    }

    return frames;
}

QVector<EggFrame4> EggGenerator4::generateDPPtIVs(u32 seed) const
{
    QVector<EggFrame4> frames;

    PokeRNG rng(seed);
    rng.advanceFrames(initialFrame - 1);

    for (u32 cnt = 0; cnt < maxResults; cnt++, rng.nextUInt())
    {
        EggFrame4 frame(initialFrame + cnt);
        PokeRNG go(rng.getSeed());

        u16 iv1 = go.nextUShort();
        u16 iv2 = go.nextUShort();
        frame.setIVs(iv1, iv2);

        u16 inh1 = go.nextUShort();
        u16 inh2 = go.nextUShort();
        u16 inh3 = go.nextUShort();
        u16 inh[3] = { inh1, inh2, inh3 };

        u16 par1 = go.nextUShort();
        u16 par2 = go.nextUShort();
        u16 par3 = go.nextUShort();
        u16 par[3] = { par1, par2, par3 };

        setInheritance(frame, inh, par, true);
        frame.calculateHiddenPower();

        if (filter.compareIVs(frame))
        {
            frame.setSeed(rng.getSeed());
            frames.append(frame);
        }
    }
    return frames;
}

QVector<EggFrame4> EggGenerator4::generateHGSSIVs(u32 seed) const
{
    QVector<EggFrame4> frames;

    PokeRNG rng(seed);
    rng.advanceFrames(initialFrame - 1);

    for (u32 cnt = 0; cnt < maxResults; cnt++, rng.nextUInt())
    {
        EggFrame4 frame(initialFrame + cnt);
        PokeRNG go(rng.getSeed());

        u16 iv1 = go.nextUShort();
        u16 iv2 = go.nextUShort();
        frame.setIVs(iv1, iv2);

        u16 inh1 = go.nextUShort();
        u16 inh2 = go.nextUShort();
        u16 inh3 = go.nextUShort();
        u16 inh[3] = { inh1, inh2, inh3 };

        u16 par1 = go.nextUShort();
        u16 par2 = go.nextUShort();
        u16 par3 = go.nextUShort();
        u16 par[3] = { par1, par2, par3 };

        setInheritance(frame, inh, par, false);
        frame.calculateHiddenPower();

        if (filter.compareIVs(frame))
        {
            frame.setSeed(rng.getSeed());
            frames.append(frame);
        }
    }
    return frames;
}

void EggGenerator4::setInheritance(EggFrame4 &frame, const u16 *inh, const u16 *par, bool broken) const
{
    u8 available[6] = { 0, 1, 2, 3, 4, 5 };
    for (u8 i = 0; i < 3; i++)
    {
        u8 stat = available[inh[i] % (6 - i)];
        u8 parent = par[i] & 1;

        switch (stat)
        {
        case 0:
            frame.setIVs(0, parent == 0 ? parent1.at(0) : parent2.at(0));
            frame.setInheritance(0, parent + 1);
            break;
        case 1:
            frame.setIVs(1, parent == 0 ? parent1.at(1) : parent2.at(1));
            frame.setInheritance(1, parent + 1);
            break;
        case 2:
            frame.setIVs(2, parent == 0 ? parent1.at(2) : parent2.at(2));
            frame.setInheritance(2, parent + 1);
            break;
        case 3:
            frame.setIVs(5, parent == 0 ? parent1.at(5) : parent2.at(5));
            frame.setInheritance(5, parent + 1);
            break;
        case 4:
            frame.setIVs(3, parent == 0 ? parent1.at(3) : parent2.at(3));
            frame.setInheritance(3, parent + 1);
            break;
        case 5:
            frame.setIVs(4, parent == 0 ? parent1.at(4) : parent2.at(4));
            frame.setInheritance(4, parent + 1);
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
