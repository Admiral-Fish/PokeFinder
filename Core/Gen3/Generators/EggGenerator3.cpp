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
#include <Core/Parents/Filters/FrameFilter.hpp>
#include <Core/RNG/LCRNG.hpp>

EggGenerator3::EggGenerator3(u32 initialFrame, u32 maxResults, u16 tid, u16 sid, u8 genderRatio, Method method, const FrameFilter &filter) :
    EggGenerator(initialFrame, maxResults, tid, sid, genderRatio, method, filter)
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

QVector<EggFrame3> EggGenerator3::generate(u32 seed, u32 seed2) const
{
    switch (method)
    {
    case Method::EBredPID:
        return generateEmeraldPID(seed);
    case Method::EBred:
    case Method::EBredSplit:
    case Method::EBredAlternate:
        return generateEmeraldIVs(seed);
    case Method::RSBred:
    case Method::RSBredAlternate:
    case Method::RSBredSplit:
    case Method::FRLGBredSplit:
    case Method::FRLGBred:
    case Method::FRLGBredAlternate:
    {
        auto lower = generateLower(seed);
        return lower.isEmpty() ? QVector<EggFrame3>() : generateUpper(seed2, lower);
    }
    default:
        return QVector<EggFrame3>();
    }
}

void EggGenerator3::setInitialFramePickup(u32 value)
{
    initialFramePickup = value;
}

void EggGenerator3::setMaxResultsPickup(u32 value)
{
    maxResultsPickup = value;
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

void EggGenerator3::setEverstone(bool value)
{
    everstone = value;
}

QVector<EggFrame3> EggGenerator3::generateEmeraldPID(u32 seed) const
{
    QVector<EggFrame3> frames;

    PokeRNG rng(seed);
    rng.advanceFrames(initialFrame - 1);

    u32 val = initialFrame;
    for (u32 cnt = 0; cnt < maxResults; cnt++, val++, rng.nextUInt())
    {
        for (u8 redraw = minRedraw; redraw <= maxRedraw; redraw++)
        {
            PokeRNG go(rng.getSeed());

            if (((go.nextUShort() * 100) / 0xFFFF) < compatability)
            {
                u16 offset = calibration + 3 * redraw;
                EggFrame3 frame(cnt + initialFrame - offset);

                bool flag = everstone ? (go.nextUShort() >> 15) == 0 : false;

                PokeRNG trng((val - offset) & 0xFFFF);

                u32 pid = 0;
                if (!flag)
                {
                    pid = ((go.nextUShort() % 0xFFFE) + 1) | (trng.nextUInt() & 0xFFFF0000);
                    frame.setNature(pid % 25);
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

                        pid = go.nextUShort() | (trng.nextUInt() & 0xFFFF0000);
                        i++;
                    } while (pid % 25 != everstoneNature);

                    if (i == 19)
                    {
                        continue;
                    }
                    frame.setNature(everstoneNature);
                }

                frame.setPID(pid);
                frame.setAbility(pid & 1);
                frame.setGender(pid & 255, genderRatio);
                frame.setShiny(tsv, (pid >> 16) ^ (pid & 0xffff), 8);

                if (filter.comparePID(frame))
                {
                    frame.setRedraw(redraw);
                    frames.append(frame);
                }
            }
        }
    }

    std::sort(frames.begin(), frames.end(),
              [](const EggFrame3 &frame1, const EggFrame3 &frame2) { return frame1.getFrame() < frame2.getFrame(); });

    return frames;
}

QVector<EggFrame3> EggGenerator3::generateEmeraldIVs(u32 seed) const
{
    QVector<EggFrame3> frames;

    PokeRNG rng(seed);
    rng.advanceFrames(initialFrame - 1);

    for (u32 cnt = 0; cnt < maxResults; cnt++, rng.nextUShort())
    {
        EggFrame3 frame(cnt + initialFrame);
        PokeRNG go(rng.getSeed());

        go.advanceFrames(this->iv1);
        u16 iv1 = go.nextUShort();
        go.advanceFrames(this->iv2);
        u16 iv2 = go.nextUShort();
        frame.setIVs(iv1, iv2);

        go.advanceFrames(this->inh);
        u16 inh1 = go.nextUShort();
        u16 inh2 = go.nextUShort();
        u16 inh3 = go.nextUShort();
        u16 inh[3] = { inh1, inh2, inh3 };

        // go.advanceFrames(this->par);
        u16 par1 = go.nextUShort();
        u16 par2 = go.nextUShort();
        u16 par3 = go.nextUShort();
        u16 par[3] = { par1, par2, par3 };

        setInheritance(frame, inh, par, true);
        frame.calculateHiddenPower();

        if (filter.compareIVs(frame))
        {
            frames.append(frame);
        }
    }

    return frames;
}

QVector<QPair<u32, u16>> EggGenerator3::generateLower(u32 seed) const
{
    QVector<QPair<u32, u16>> frames;

    PokeRNG rng(seed);
    rng.advanceFrames(initialFrame - 1);

    for (u32 cnt = 0; cnt < maxResults; cnt++, rng.nextUInt())
    {
        PokeRNG go(rng.getSeed());
        if (((go.nextUShort() * 100) / 0xFFFF) < compatability)
        {
            u16 pid = (go.nextUShort() % 0xFFFE) + 1;

            // TODO: decide on filtering for ability/gender
            frames.append(qMakePair(cnt + initialFrame, pid));
        }
    }

    return frames;
}

QVector<EggFrame3> EggGenerator3::generateUpper(u32 seed, const QVector<QPair<u32, u16>> &lower) const
{
    QVector<EggFrame3> upper;

    PokeRNG rng(seed);
    rng.advanceFrames(initialFramePickup - 1);

    for (u32 cnt = 0; cnt < maxResultsPickup; cnt++, rng.nextUInt())
    {
        EggFrame3 frame;
        PokeRNG go(rng.getSeed());

        frame.setPID(go.nextUShort());

        go.advanceFrames(this->iv1);
        u16 iv1 = go.nextUShort();
        rng.advanceFrames(this->iv2);
        u16 iv2 = go.nextUShort();
        frame.setIVs(iv1, iv2);

        go.advanceFrames(this->inh);
        u16 inh1 = go.nextUShort();
        u16 inh2 = go.nextUShort();
        u16 inh3 = go.nextUShort();
        u16 inh[3] = { inh1, inh2, inh3 };

        // go.advanceFrames(this->par);
        u16 par1 = go.nextUShort();
        u16 par2 = go.nextUShort();
        u16 par3 = go.nextUShort();
        u16 par[3] = { par1, par2, par3 };

        setInheritance(frame, inh, par, false);
        frame.calculateHiddenPower();

        if (filter.compareIVs(frame))
        {
            frame.setPickupFrame(cnt + initialFramePickup);
            upper.append(frame);
        }
    }

    QVector<EggFrame3> frames;
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
                up.setFrame(low.first);
                frames.append(up);
            }
        }
    }
    return frames;
}

void EggGenerator3::setInheritance(EggFrame3 &frame, const u16 *inh, const u16 *par, bool broken) const
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
        // In Emerald this doesn't work properly
        for (u8 j = broken ? i : stat; j < 5 - i; j++)
        {
            available[j] = available[j + 1];
        }
    }
}
