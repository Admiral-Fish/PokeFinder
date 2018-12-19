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

#include "Egg3.hpp"

Egg3::Egg3()
{
    maxResults = 100000;
    initialFrame = 1;
    seed = 0;
    tid = 12345;
    sid = 54321;
    psv = tid ^ sid;
}

Egg3::Egg3(u32 maxFrame, u32 initialFrame, u16 tid, u16 sid, Method method, u32 seed)
{
    maxResults = maxFrame;
    this->initialFrame = initialFrame;
    this->seed = seed;
    this->tid = tid;
    this->sid = sid;
    psv = tid ^ sid;
    frameType = method;
}

QVector<Frame3> Egg3::generate(const FrameCompare &compare) const
{
    switch (frameType)
    {
        case Method::EBredPID:
            return generateEmeraldPID(compare);
        case Method::EBred:
            return generateEmerald(compare);
        case Method::EBredSplit:
            return generateEmeraldSplit(compare);
        case Method::EBredAlternate:
            return generateEmeraldAlternate(compare);
        case Method::RSBred:
        case Method::FRLGBred:
            {
                QVector<Frame3> lower = generateLower(compare);
                return lower.isEmpty() ? lower : generateUpper(lower, compare);
            }
        default:
            return QVector<Frame3>();
    }
}

void Egg3::setParents(const QVector<u8> &parent1, const QVector<u8> &parent2)
{
    this->parent1 = parent1;
    this->parent2 = parent2;
}

void Egg3::setMinRedraw(const u8 &value)
{
    minRedraw = value;
}

void Egg3::setMaxRedraw(const u8 &value)
{
    maxRedraw = value;
}

void Egg3::setCompatability(const int &value)
{
    compatability = value;
}

void Egg3::setCalibration(const u8 &value)
{
    calibration = value;
}

void Egg3::setEverstone(bool value)
{
    everstone = value;
}

void Egg3::setMinPickup(const u32 &value)
{
    minPickup = value;
}

void Egg3::setMaxPickup(const u32 &value)
{
    maxPickup = value;
}

u32 Egg3::getSeed() const
{
    return seed;
}

void Egg3::setSeed(const u32 &value)
{
    seed = value;
}

QVector<Frame3> Egg3::generateEmeraldPID(const FrameCompare &compare) const
{
    QVector<Frame3> frames;
    Frame3 frame = Frame3(tid, sid, psv);
    frame.setGenderRatio(compare.getGenderRatio());

    u32 i;
    u32 pid = 0;

    PokeRNG rng(seed, initialFrame - 1);
    auto *rngArray = new u16[maxResults + 19];
    for (u32 x = 0; x < maxResults + 19; x++)
    {
        rngArray[x] = rng.nextUShort();
    }

    u32 val = initialFrame;

    u32 max = maxResults - initialFrame + 1;
    for (u32 cnt = 0; cnt < max; cnt++, val++)
    {
        for (u8 redraw = minRedraw; redraw <= maxRedraw; redraw++)
        {
            if (((rngArray[cnt] * 100) / 0xFFFF) >= compatability)
            {
                continue;
            }

            u16 offset = calibration + 3 * redraw;

            i = 1;

            bool flag = everstone ? (rngArray[cnt + i++] >> 15) == 0 : false;

            PokeRNG trng((val - offset) & 0xFFFF);

            if (!flag)
            {
                // Lower PID
                pid = (rngArray[i + cnt] % 0xFFFE) + 1;

                // Upper PID
                pid |= trng.nextUInt() & 0xFFFF0000;

                frame.setPID(pid, pid >> 16, pid & 0xFFFF);
                frame.setNature(pid % 25);
            }
            else
            {
                do
                {
                    // VBlank at 17 from starting PID generation
                    // Adjusted i value is 19
                    // Skip at this point since spread is unlikely to occur
                    if (i == 19)
                    {
                        break;
                    }

                    // generate lower
                    pid = rngArray[cnt + i++];

                    // generate upper
                    pid |= trng.nextUInt() & 0xFFFF0000;
                }
                while (pid % 25 != everstoneNature);

                if (i == 19)
                {
                    continue;
                }

                frame.setPID(pid, pid >> 16, pid & 0xFFFF);
                frame.setNature(everstoneNature);
            }

            if (compare.comparePID(frame))
            {
                frame.setFrame(cnt + initialFrame - offset);
                frame.setOccidentary(redraw);
                frames.append(frame);
            }
        }
    }

    std::sort(frames.begin(), frames.end(), [](const Frame3 & frame1, const Frame3 & frame2)
    {
        return frame1.getFrame() < frame2.getFrame();
    });

    delete[] rngArray;
    return frames;
}

QVector<Frame3> Egg3::generateEmerald(const FrameCompare &compare) const
{
    QVector<Frame3> frames;
    Frame3 frame = Frame3(tid, sid, psv);

    PokeRNG rng(seed, initialFrame - 1);
    auto *rngArray = new u16[maxResults + 13];
    for (u32 x = 0; x < maxResults + 13; x++)
    {
        rngArray[x] = rng.nextUShort();
    }

    u16 inh1, inh2, inh3, par1, par2, par3;

    u32 max = maxResults - initialFrame + 1;
    for (u32 cnt = 0; cnt < max; cnt++)
    {
        inh1 = rngArray[7 + cnt];
        inh2 = rngArray[8 + cnt];
        inh3 = rngArray[9 + cnt];
        par1 = rngArray[10 + cnt];
        par2 = rngArray[11 + cnt];
        par3 = rngArray[12 + cnt];
        frame.setInheritance(rngArray[4 + cnt], rngArray[5 + cnt], par1, par2, par3, inh1,
                             inh2, inh3, parent1, parent2, true);

        if (compare.compareIVs(frame))
        {
            frame.setFrame(cnt + initialFrame);
            frames.append(frame);
        }
    }

    delete[] rngArray;
    return frames;
}

QVector<Frame3> Egg3::generateEmeraldSplit(const FrameCompare &compare) const
{
    QVector<Frame3> frames;
    Frame3 frame = Frame3(tid, sid, psv);

    PokeRNG rng(seed, initialFrame - 1);
    auto *rngArray = new u16[maxResults + 14];
    for (u32 x = 0; x < maxResults + 14; x++)
    {
        rngArray[x] = rng.nextUShort();
    }

    u16 inh1, inh2, inh3, par1, par2, par3;

    u32 max = maxResults - initialFrame + 1;
    for (u32 cnt = 0; cnt < max; cnt++)
    {
        inh1 = rngArray[8 + cnt];
        inh2 = rngArray[9 + cnt];
        inh3 = rngArray[10 + cnt];
        par1 = rngArray[11 + cnt];
        par2 = rngArray[12 + cnt];
        par3 = rngArray[13 + cnt];
        frame.setInheritance(rngArray[4 + cnt], rngArray[6 + cnt], par1, par2, par3, inh1,
                             inh2, inh3, parent1, parent2, true);

        if (compare.compareIVs(frame))
        {
            frame.setFrame(cnt + initialFrame);
            frames.append(frame);
        }
    }

    delete[] rngArray;
    return frames;
}

QVector<Frame3> Egg3::generateEmeraldAlternate(const FrameCompare &compare) const
{
    QVector<Frame3> frames;
    Frame3 frame = Frame3(tid, sid, psv);

    PokeRNG rng(seed, initialFrame - 1);
    auto *rngArray = new u16[maxResults + 14];
    for (u32 x = 0; x < maxResults + 14; x++)
    {
        rngArray[x] = rng.nextUShort();
    }

    u16 inh1, inh2, inh3, par1, par2, par3;

    u32 max = maxResults - initialFrame + 1;
    for (u32 cnt = 0; cnt < max; cnt++)
    {
        inh1 = rngArray[8 + cnt];
        inh2 = rngArray[9 + cnt];
        inh3 = rngArray[10 + cnt];
        par1 = rngArray[11 + cnt];
        par2 = rngArray[12 + cnt];
        par3 = rngArray[13 + cnt];
        frame.setInheritance(rngArray[4 + cnt], rngArray[5 + cnt], par1, par2, par3, inh1,
                             inh2, inh3, parent1, parent2, true);

        if (compare.compareIVs(frame))
        {
            frame.setFrame(cnt + initialFrame);
            frames.append(frame);
        }
    }

    delete[] rngArray;
    return frames;
}

QVector<Frame3> Egg3::generateLower(const FrameCompare &compare) const
{
    QVector<Frame3> frames;
    Frame3 frame = Frame3(tid, sid, psv);

    PokeRNG rng(seed, initialFrame - 1);
    auto *rngArray = new u16[maxResults + 2];
    for (u32 x = 0; x < maxResults + 2; x++)
    {
        rngArray[x] = rng.nextUShort();
    }

    u32 max = maxResults - initialFrame + 1;
    for (u32 cnt = 0; cnt < max; cnt++)
    {
        if (((rngArray[cnt] * 100) / 0xFFFF) >= compatability)
        {
            continue;
        }

        frame.setPID((rngArray[1 + cnt] % 0xFFFE) + 1);

        if (compare.compareGender(frame))
        {
            frame.setFrame(cnt + initialFrame);
            frames.append(frame);
        }
    }

    delete[] rngArray;
    return frames;
}

QVector<Frame3> Egg3::generateUpper(const QVector<Frame3> &lower, const FrameCompare &compare) const
{
    QVector<Frame3> upper;
    Frame3 frame = Frame3(tid, sid, psv);

    PokeRNG rng(seed, minPickup - 1);
    auto *rngArray = new u16[maxResults + 14];
    for (u32 x = 0; x < maxResults + 14; x++)
    {
        rngArray[x] = rng.nextUShort();
    }

    u16 inh1, inh2, inh3, par1, par2, par3;

    u32 max = maxPickup - minPickup + 1;
    for (u32 cnt = 0; cnt < max; cnt++)
    {
        frame.setPID(rngArray[3 + cnt]);

        inh1 = rngArray[8 + cnt];
        inh2 = rngArray[9 + cnt];
        inh3 = rngArray[10 + cnt];
        par1 = rngArray[11 + cnt];
        par2 = rngArray[12 + cnt];
        par3 = rngArray[13 + cnt];

        frame.setInheritance(rngArray[5 + cnt], rngArray[6 + cnt], par1, par2, par3, inh1,
                             inh2, inh3, parent1, parent2);

        if (compare.compareIVs(frame))
        {
            frame.setOccidentary(cnt + minPickup);
            upper.append(frame);
        }
    }

    delete[] rngArray;

    QVector<Frame3> frames;
    for (const auto &low : lower)
    {
        for (auto up : upper)
        {
            up.setPID(low.getPID(), up.getPID());
            if (compare.comparePID(up))
            {
                up.setFrame(low.getFrame());
                frames.append(up);
            }
        }
    }
    return frames;
}
