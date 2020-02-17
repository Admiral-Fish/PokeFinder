/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2019 by Admiral_Fish, bumba, and EzPzStreamz
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

#include "StationaryGenerator5.hpp"
#include <Core/Enum/Encounter.hpp>
#include <Core/Enum/Method.hpp>
#include <Core/Parents/Filters/FrameFilter.hpp>
#include <Core/RNG/MTRNG.hpp>
#include <Core/RNG/RNGList.hpp>

Generator5::Generator5(u32 initialFrame, u32 maxResults, u16 tid, u16 sid, u8 genderRatio, Method method)
    : StationaryGenerator(initialFrame, maxResults, tid, sid, genderRatio, method)
{
    tsv = (tid ^ sid) >> 3;
}

QVector<StationaryFrame> Generator5::generate(const FrameFilter &compare) const
{
    switch (method)
    {
    case Method::Method5IVs:
        switch (encounter)
        {
        case Encounter::Roamer:
            return generateRoamerIVs(compare);
        default:
            return generateIVs(compare);
        }
    case Method::Method5CGear:
        switch (encounter)
        {
        case Encounter::Roamer:
            return generateRoamerCGear(compare);
        default:
            return generateCGear(compare);
        }
    // case Method::Method5:
    //    break;
    default:
        return QVector<StationaryFrame>();
    }
}

QVector<StationaryFrame> Generator5::generateRoamerIVs(const FrameFilter &filter) const
{
    QVector<StationaryFrame> frames;

    MersenneTwister rng(static_cast<u32>(seed));
    rng.advanceFrames(initialFrame - 1 + offset);

    RNGList<u8> rngList([&rng] { return rng.nextUInt() >> 27; });

    for (u32 cnt = 0; cnt < maxResults; cnt++, rngList.advanceState())
    {
        StationaryFrame frame(initialFrame + cnt);

        rngList.getValue(); // Blank ???
        u8 hp = rngList.getValue();
        u8 atk = rngList.getValue();
        u8 def = rngList.getValue();
        u8 spd = rngList.getValue();
        u8 spe = rngList.getValue();
        u8 spa = rngList.getValue();

        frame.setIVs(hp, atk, def, spa, spd, spe);
        frame.calculateHiddenPower();

        if (filter.compareIVs(frame))
        {
            frames.append(frame);
        }
    }

    return frames;
}

QVector<StationaryFrame> Generator5::generateIVs(const FrameFilter &filter) const
{
    QVector<StationaryFrame> frames;

    MersenneTwister rng(static_cast<u32>(seed));
    rng.advanceFrames(initialFrame - 1 + offset);

    RNGList<u8> rngList([&rng] { return rng.nextUInt() >> 27; });

    for (u32 cnt = 0; cnt < maxResults; cnt++, rngList.advanceState())
    {
        StationaryFrame frame(initialFrame + cnt);

        u8 hp = rngList.getValue();
        u8 atk = rngList.getValue();
        u8 def = rngList.getValue();
        u8 spa = rngList.getValue();
        u8 spd = rngList.getValue();
        u8 spe = rngList.getValue();

        frame.setIVs(hp, atk, def, spa, spd, spe);
        frame.calculateHiddenPower();

        if (filter.compareIVs(frame))
        {
            frames.append(frame);
        }
    }

    return frames;
}

QVector<StationaryFrame> Generator5::generateRoamerCGear(const FrameFilter &filter) const
{
    QVector<StationaryFrame> frames;

    MersenneTwister rng(static_cast<u32>(seed));
    rng.advanceFrames(initialFrame - 1 + offset);
    rng.advanceFrames(2); // Skip first two frames

    RNGList<u8> rngList([&rng] { return rng.nextUInt() >> 27; });

    for (u32 cnt = 0; cnt < maxResults; cnt++)
    {
        StationaryFrame frame(initialFrame + cnt);

        rngList.getValue(); // Blank ???
        u8 hp = rngList.getValue();
        u8 atk = rngList.getValue();
        u8 def = rngList.getValue();
        u8 spd = rngList.getValue();
        u8 spe = rngList.getValue();
        u8 spa = rngList.getValue();

        frame.setIVs(hp, atk, def, spa, spd, spe);
        frame.calculateHiddenPower();

        if (filter.compareIVs(frame))
        {
            frames.append(frame);
        }
    }

    return frames;
}

QVector<StationaryFrame> Generator5::generateCGear(const FrameFilter &filter) const
{
    QVector<StationaryFrame> frames;

    MersenneTwister rng(static_cast<u32>(seed));
    rng.advanceFrames(initialFrame - 1 + offset);
    rng.advanceFrames(2); // Skip first two frames

    RNGList<u8> rngList([&rng] { return rng.nextUInt(); });

    for (u32 cnt = 0; cnt < maxResults; cnt++)
    {
        StationaryFrame frame(initialFrame + cnt);

        u8 hp = rngList.getValue();
        u8 atk = rngList.getValue();
        u8 def = rngList.getValue();
        u8 spa = rngList.getValue();
        u8 spd = rngList.getValue();
        u8 spe = rngList.getValue();

        frame.setIVs(hp, atk, def, spa, spd, spe);
        frame.calculateHiddenPower();

        if (filter.compareIVs(frame))
        {
            frames.append(frame);
        }
    }

    return frames;
}
