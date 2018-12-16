/*
 * This file is part of PokéFinder
 * Copyright (C) 2017 by Admiral_Fish, bumba, and EzPzStreamz
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

#include "Egg4.hpp"

Egg4::Egg4()
{
    maxResults = 100000;
    initialFrame = 1;
    seed = 0;
    tid = 12345;
    sid = 54321;
    psv = tid ^ sid;
}

Egg4::Egg4(u32 maxFrame, u32 initialFrame, u16 tid, u16 sid, Method method, u32 seed)
{
    maxResults = maxFrame;
    this->initialFrame = initialFrame;
    this->seed = seed;
    this->tid = tid;
    this->sid = sid;
    psv = tid ^ sid;
    frameType = method;
}

void Egg4::setParents(const QVector<u8> &parent1, const QVector<u8> &parent2)
{
    this->parent1 = parent1;
    this->parent2 = parent2;
}

QVector<Frame4> Egg4::generate(const FrameCompare &compare)
{
    switch (frameType)
    {
        case Method::Gen4Normal:
            return generatePID(compare);
        case Method::Gen4Masuada:
            return generatePIDMasuada(compare);
        case Method::DPPtIVs:
            return generateIVsDPPt(compare);
        case Method::HGSSIVs:
            return generateIVsHGSS(compare);
        default:
            return QVector<Frame4>();
    }
}

QVector<Frame4> Egg4::generatePID(FrameCompare compare)
{
    QVector<Frame4> frames;
    Frame4 frame = Frame4(tid, sid, psv);
    frame.setGenderRatio(compare.getGenderRatio());
    frame.setInitialSeed(seed);

    MersenneTwister mt(seed, initialFrame - 1);

    for (u32 cnt = initialFrame; cnt <= maxResults; cnt++)
    {
        frame.setPID(mt.nextUInt());

        if (compare.comparePID(frame))
        {
            frame.setFrame(cnt);
            frames.append(frame);
        }
    }

    return frames;
}

QVector<Frame4> Egg4::generatePIDMasuada(FrameCompare compare)
{
    QVector<Frame4> frames;
    Frame4 frame = Frame4(tid, sid, psv);
    frame.setGenderRatio(compare.getGenderRatio());
    frame.setInitialSeed(seed);

    MersenneTwister mt(seed, initialFrame - 1);

    for (u32 cnt = initialFrame; cnt <= maxResults; cnt++)
    {
        u32 pid = mt.nextUInt();

        for (int i = 0; i <= 3; i++)
        {
            u16 val = (pid >> 16) ^ (pid & 0xFFFF);

            if ((val ^ psv) < 8)
            {
                break;
            }

            pid = pid * 0x6c078965 + 1; // Advance with ARNG
        }

        frame.setPID(pid);

        if (compare.comparePID(frame))
        {
            frame.setFrame(cnt);
            frames.append(frame);
        }
    }

    return frames;
}

QVector<Frame4> Egg4::generateIVsDPPt(FrameCompare compare)
{
    QVector<Frame4> frames;
    Frame4 frame = Frame4(tid, sid, psv);
    frame.setInitialSeed(seed);

    PokeRNG rng(seed, initialFrame - 1);
    auto *rngArray = new u16[maxResults + 8];
    for (u32 x = 0; x < maxResults + 8; x++)
    {
        rngArray[x] = rng.nextUShort();
    }

    u16 inh1, inh2, inh3, par1, par2, par3;

    for (u32 cnt = 0; cnt < maxResults; cnt++)
    {
        inh1 = rngArray[2 + cnt];
        inh2 = rngArray[3 + cnt];
        inh3 = rngArray[4 + cnt];
        par1 = rngArray[5 + cnt];
        par2 = rngArray[6 + cnt];
        par3 = rngArray[7 + cnt];

        frame.setInheritanceDPPt(rngArray[cnt], rngArray[1 + cnt], par1 & 1, par2 & 1, par3 & 1, HABCDS[inh1 % 6],
                                 ABCDS[inh2 % 5], ACDS[inh3 & 3], parent1, parent2);

        if (compare.compareIVs(frame))
        {
            frame.setSeed(rngArray[cnt]);
            frame.setFrame(cnt + initialFrame);
            frames.append(frame);
        }
    }

    delete[] rngArray;
    return frames;
}

QVector<Frame4> Egg4::generateIVsHGSS(FrameCompare compare)
{
    QVector<Frame4> frames;
    Frame4 frame = Frame4(tid, sid, psv);
    frame.setInitialSeed(seed);

    PokeRNG rng(seed, initialFrame - 1);
    auto *rngArray = new u16[maxResults + 8];
    for (u32 x = 0; x < maxResults + 8; x++)
    {
        rngArray[x] = rng.nextUShort();
    }

    u16 inh1, inh2, inh3, par1, par2, par3;

    for (u32 cnt = 0; cnt < maxResults; cnt++)
    {
        inh1 = rngArray[2 + cnt];
        inh2 = rngArray[3 + cnt];
        inh3 = rngArray[4 + cnt];
        par1 = rngArray[5 + cnt];
        par2 = rngArray[6 + cnt];
        par3 = rngArray[7 + cnt];

        frame.setInheritanceHGSS(rngArray[cnt], rngArray[1 + cnt], par1, par2, par3, inh1,
                                 inh2, inh3, parent1, parent2);

        if (compare.compareIVs(frame))
        {
            frame.setSeed(rngArray[cnt]);
            frame.setFrame(cnt + initialFrame);
            frames.append(frame);
        }
    }

    delete[] rngArray;
    return frames;
}
