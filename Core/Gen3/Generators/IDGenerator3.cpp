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

#include "IDGenerator3.hpp"
#include <Core/Parents/Filters/IDFilter.hpp>
#include <Core/RNG/LCRNG.hpp>

IDGenerator3::IDGenerator3(u32 initialFrame, u32 maxResults, const IDFilter &filter) : IDGenerator(initialFrame, maxResults, filter)
{
}

void IDGenerator3::setStaticTID(u16 staticTID)
{
    this->staticTID = staticTID;
}

QVector<IDFrame3> IDGenerator3::generateXDColo(u32 seed)
{
    QVector<IDFrame3> frames;

    XDRNG rng(seed);
    rng.advanceFrames(initialFrame - 1);

    for (u32 cnt = 0; cnt < maxResults; cnt++, rng.nextUInt())
    {
        XDRNG go(rng.getSeed());

        u16 tid = go.nextUShort();
        u16 sid = go.nextUShort();

        IDFrame3 frame(initialFrame + cnt, tid, sid);

        if (filter.compare(frame))
        {
            frames.append(frame);
        }
    }

    return frames;
}

QVector<IDFrame3> IDGenerator3::generateFRLGE(u32 seed)
{
    QVector<IDFrame3> frames;

    PokeRNG rng(seed);
    rng.advanceFrames(initialFrame);

    for (u32 cnt = 0; cnt < maxResults; cnt++)
    {
        u16 sid = rng.nextUShort();

        IDFrame3 frame(initialFrame + cnt, staticTID, sid);

        if (filter.compare(frame))
        {
            frames.append(frame);
        }
    }

    return frames;
}

QVector<IDFrame3> IDGenerator3::generateRS(u32 seed)
{
    QVector<IDFrame3> frames;

    PokeRNG rng(seed);
    rng.advanceFrames(initialFrame);

    for (u32 cnt = 0; cnt < maxResults; cnt++, rng.nextUInt())
    {
        PokeRNG go(rng.getSeed());

        u16 sid = go.nextUShort();
        u16 tid = go.nextUShort();

        IDFrame3 frame(initialFrame + cnt, tid, sid);

        if (filter.compare(frame))
        {
            frames.append(frame);
        }
    }

    return frames;
}
