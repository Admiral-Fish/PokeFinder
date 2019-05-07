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

#include "Generator5.hpp"

Generator5::Generator5()
{
    maxResults = 100000;
    initialFrame = 1;
    initialSeed = 0;
    tid = 12345;
    sid = 54321;
    psv = tid ^ sid;
}

Generator5::Generator5(u32 maxResults, u32 initialFrame, u32 initialSeed, u16 tid, u16 sid, u32 offset, Method type)
{
    this->maxResults = maxResults;
    this->initialFrame = initialFrame;
    this->initialSeed = initialSeed;
    this->tid = tid;
    this->sid = sid;
    psv = tid ^ sid;
    this->offset = offset;
    frameType = type;
}

QVector<Frame5> Generator5::generate(const FrameCompare &compare) const
{
    switch (frameType)
    {
        case Method5IVs:
            switch (encounterType)
            {
                case Encounter::Roamer:
                    return generateRoamerIVs(compare);
                default:
                    return generateIVs(compare);
            }
        case Method5CGear:
            switch (encounterType)
            {
                case Encounter::Roamer:
                    return generateRoamerCGear(compare);
                default:
                    return generateCGear(compare);
            }
        case Method5:
            switch (leadType)
            {

            }
        default:
            return QVector<Frame5>();
    }
}

QVector<Frame5> Generator5::generateRoamerIVs(const FrameCompare &compare) const
{
    QVector<Frame5> frames;
    Frame5 frame(tid, sid, psv);

    MersenneTwister mt(initialSeed, initialFrame - 1 + offset);
    QVector<u16> rngList(maxResults + 7);
    for (u16 &x : rngList)
    {
        x = mt.nextUInt() >> 27;
    }

    for (u32 cnt = 0; cnt < maxResults; cnt++)
    {
        frame.setIVs(rngList.at(1), rngList.at(2), rngList.at(3), rngList.at(6), rngList.at(4), rngList.at(5));
        if (compare.compareIVs(frame))
        {
            frames.append(frame);
        }
    }

    return frames;
}

QVector<Frame5> Generator5::generateIVs(const FrameCompare &compare) const
{
    QVector<Frame5> frames;
    Frame5 frame = Frame5(tid, sid, psv);

    MersenneTwister mt(initialSeed, initialFrame - 1 + offset);
    QVector<u16> rngList(maxResults + 6);
    for (u16 &x : rngList)
    {
        x = mt.nextUInt() >> 27;
    }

    for (u32 cnt = 0; cnt < maxResults; cnt++)
    {
        frame.setIVs(rngList.at(0), rngList.at(1), rngList.at(2), rngList.at(3), rngList.at(4), rngList.at(5));
        if (compare.compareIVs(frame))
        {
            frames.append(frame);
        }
    }

    return frames;
}

QVector<Frame5> Generator5::generateRoamerCGear(const FrameCompare &compare) const
{
    QVector<Frame5> frames;
    Frame5 frame = Frame5(tid, sid, psv);

    MersenneTwister mt(initialSeed, initialFrame - 1 + offset);
    mt.advanceFrames(2); // Skip first two frames

    QVector<u16> rngList(maxResults + 7);
    for (u16 &x : rngList)
    {
        x = mt.nextUInt() >> 27;
    }

    for (u32 cnt = 0; cnt < maxResults; cnt++)
    {
        frame.setIVs(rngList.at(1), rngList.at(2), rngList.at(3), rngList.at(6), rngList.at(4), rngList.at(5));
        if (compare.compareIVs(frame))
        {
            frames.append(frame);
        }
    }

    return frames;
}

QVector<Frame5> Generator5::generateCGear(const FrameCompare &compare) const
{
    QVector<Frame5> frames;
    Frame5 frame = Frame5(tid, sid, psv);

    MersenneTwister mt(initialSeed, initialFrame - 1 + offset);
    mt.advanceFrames(2); // Skip first two frames

    QVector<u16> rngList(maxResults + 6);
    for (u16 &x : rngList)
    {
        x = mt.nextUInt() >> 27;
    }

    for (u32 cnt = 0; cnt < maxResults; cnt++)
    {
        frame.setIVs(rngList.at(0), rngList.at(1), rngList.at(2), rngList.at(3), rngList.at(4), rngList.at(5));
        if (compare.compareIVs(frame))
        {
            frames.append(frame);
        }
    }

    return frames;
}
