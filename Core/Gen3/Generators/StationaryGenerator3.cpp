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

#include "StationaryGenerator3.hpp"
#include <Core/Enum/Method.hpp>
#include <Core/Parents/Filters/FrameFilter.hpp>
#include <Core/RNG/LCRNG.hpp>

StationaryGenerator3::StationaryGenerator3(u32 initialFrame, u32 maxResults, u16 tid, u16 sid, u8 genderRatio, Method method) :
    StationaryGenerator(initialFrame, maxResults, tid, sid, genderRatio, method)
{
    tsv = (tid ^ sid) >> 3;
}

QVector<Frame> StationaryGenerator3::generate(u32 seed, const FrameFilter &filter) const
{
    switch (method)
    {
    case Method::Method1:
    case Method::Method2:
    case Method::Method4:
        return generateMethod124(seed, filter);
    case Method::Method1Reverse:
        return generateMethod1Reverse(seed, filter);
    default:
        return QVector<Frame>();
    }
}

QVector<Frame> StationaryGenerator3::generateMethod124(u32 seed, const FrameFilter &filter) const
{
    QVector<Frame> frames;

    PokeRNG rng(seed);
    rng.advanceFrames(initialFrame - 1 + offset);

    // Method 1 [SEED] [PID] [PID] [IVS] [IVS]
    // Method 2 [SEED] [PID] [PID] [BLANK] [IVS] [IVS]
    // Method 4 [SEED] [PID] [PID] [IVS] [BLANK] [IVS]

    for (u32 cnt = 0; cnt < maxResults; cnt++, rng.nextUInt())
    {
        Frame frame(cnt + initialFrame);
        PokeRNG go(rng.getSeed());

        u16 low = go.nextUShort();
        u16 high = go.nextUShort();

        go.advanceFrames(method == Method::Method2);
        u16 iv1 = go.nextUShort();
        go.advanceFrames(method == Method::Method4);
        u16 iv2 = go.nextUShort();

        frame.setPID(high, low);
        frame.setAbility(low & 1);
        frame.setGender(low & 255, genderRatio);
        frame.setNature(frame.getPID() % 25);
        frame.setShiny(tsv, (high ^ low) >> 3);

        frame.setIVs(iv1, iv2);
        frame.calculateHiddenPower();

        if (filter.compareFrame(frame))
        {
            frames.append(frame);
        }
    }

    return frames;
}

QVector<Frame> StationaryGenerator3::generateMethod1Reverse(u32 seed, const FrameFilter &filter) const
{
    QVector<Frame> frames;

    PokeRNG rng(seed);
    rng.advanceFrames(initialFrame - 1 + offset);

    // Method 1 Reverse [SEED] [PID] [PID] [IVS] [IVS]

    for (u32 cnt = 0; cnt < maxResults; cnt++, rng.nextUInt())
    {
        Frame frame(cnt + initialFrame);
        PokeRNG go(rng.getSeed());

        u16 high = go.nextUShort();
        u16 low = go.nextUShort();
        u16 iv1 = go.nextUShort();
        u16 iv2 = go.nextUShort();

        frame.setPID(high, low);
        frame.setAbility(low & 1);
        frame.setGender(low & 255, genderRatio);
        frame.setNature(frame.getPID() % 25);
        frame.setShiny(tsv, (high ^ low) >> 3);

        frame.setIVs(iv1, iv2);
        frame.calculateHiddenPower();

        if (filter.compareFrame(frame))
        {
            frames.append(frame);
        }
    }

    return frames;
}
