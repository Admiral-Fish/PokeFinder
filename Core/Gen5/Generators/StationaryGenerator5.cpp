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

#include "StationaryGenerator5.hpp"
#include <Core/Enum/Encounter.hpp>
#include <Core/Enum/Lead.hpp>
#include <Core/Enum/Method.hpp>
#include <Core/Parents/Filters/FrameFilter.hpp>
#include <Core/RNG/LCRNG64.hpp>
#include <Core/RNG/MTRNG.hpp>
#include <Core/RNG/RNGList.hpp>

StationaryGenerator5::StationaryGenerator5(u32 initialFrame, u32 maxResults, u16 tid, u16 sid, u8 genderRatio, Method method,
                                           Encounter encounter, const FrameFilter &filter) :
    StationaryGenerator(initialFrame, maxResults, tid, sid, genderRatio, method, filter),
    idBit((tid & 1) ^ (sid & 1)),
    encounter(encounter)
{
    switch (method)
    {
    case Method::Method5IVs:
        switch (encounter)
        {
        case Encounter::Roamer:
            initializeMT(7);
            break;
        default:
            initializeMT(6);
            break;
        }
        break;
    case Method::Method5CGear:
        switch (encounter)
        {
        case Encounter::Roamer:
            initializeMT(9);
            break;
        default:
            initializeMT(8);
            break;
        }
        break;
    default:
        break;
    }
}

StationaryGenerator5::~StationaryGenerator5()
{
    delete mt;
}

QVector<StationaryFrame5> StationaryGenerator5::generate(u64 seed) const
{
    switch (method)
    {
    case Method::Method5IVs:
        switch (encounter)
        {
        case Encounter::Roamer:
            return generateRoamerIVs(seed);
        default:
            return generateIVs(seed);
        }
    case Method::Method5CGear:
        switch (encounter)
        {
        case Encounter::Roamer:
            return generateRoamerCGear(seed);
        default:
            return generateCGear(seed);
        }
    // case Method::Method5:
    //    break;
    default:
        return QVector<StationaryFrame5>();
    }
}

QVector<StationaryFrame5> StationaryGenerator5::generateRoamerIVs(u64 seed) const
{
    QVector<StationaryFrame5> frames;

    mt->setSeed(static_cast<u32>(seed), initialFrame - 1 + offset);
    RNGList<u8> rngList([this] { return mt->next() >> 27; });

    for (u32 cnt = 0; cnt < maxResults; cnt++, rngList.advanceState())
    {
        StationaryFrame5 frame;

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
            frame.setIVFrame(initialFrame + cnt);
            frames.append(frame);
        }
    }

    return frames;
}

QVector<StationaryFrame5> StationaryGenerator5::generateIVs(u64 seed) const
{
    QVector<StationaryFrame5> frames;

    mt->setSeed(static_cast<u32>(seed), initialFrame - 1 + offset);
    RNGList<u8> rngList([this] { return mt->next() >> 27; });

    for (u32 cnt = 0; cnt < maxResults; cnt++, rngList.advanceState())
    {
        StationaryFrame5 frame;

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
            frame.setIVFrame(initialFrame + cnt);
            frames.append(frame);
        }
    }

    return frames;
}

QVector<StationaryFrame5> StationaryGenerator5::generateRoamerCGear(u64 seed) const
{
    QVector<StationaryFrame5> frames;

    mt->setSeed(static_cast<u32>(seed), initialFrame - 1 + offset);
    mt->advanceFrames(2); // Skip first two frames

    RNGList<u8> rngList([this] { return mt->next() >> 27; });

    for (u32 cnt = 0; cnt < maxResults; cnt++, rngList.advanceState())
    {
        StationaryFrame5 frame;

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
            frame.setIVFrame(initialFrame + cnt);
            frames.append(frame);
        }
    }

    return frames;
}

QVector<StationaryFrame5> StationaryGenerator5::generateCGear(u64 seed) const
{
    QVector<StationaryFrame5> frames;

    mt->setSeed(static_cast<u32>(seed), initialFrame - 1 + offset);
    mt->advanceFrames(2); // Skip first two frames

    RNGList<u8> rngList([this] { return mt->next() >> 27; });

    for (u32 cnt = 0; cnt < maxResults; cnt++, rngList.advanceState())
    {
        StationaryFrame5 frame;

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
            frame.setIVFrame(initialFrame + cnt);
            frames.append(frame);
        }
    }

    return frames;
}

QVector<StationaryFrame5> StationaryGenerator5::generateStationary(u64 seed) const
{
    QVector<StationaryFrame5> frames;

    BWRNG rng(seed);
    rng.advanceFrames(initialFrame - 1 + offset);

    for (u32 cnt = 0; cnt < maxResults; cnt++)
    {
        StationaryFrame5 frame(initialFrame + cnt);
        BWRNG go(rng.getSeed());

        u32 pid;
        if (lead == Lead::Synchronize)
        {
            bool synch = (go.nextUInt() >> 1) == 1;
            pid = go.nextUInt() ^ 0x10000;

            if (synch)
            {
                frame.setNature(synchNature);
            }
            else
            {
                frame.setNature(static_cast<u8>(go.nextUInt(25)));
            }
        }
        else if (lead == Lead::CuteCharm)
        {
            bool charm = (go.nextUInt(0xffff) / 656) < 67;
            pid = go.nextUInt() ^ 0x10000;

            if (!charm)
            {
                frame.setNature(static_cast<u8>(go.nextUInt(25)));
            }
            else
            {
                go.nextULong(); // TODO modify pid
                frame.setNature(static_cast<u8>(go.nextUInt(25)));
            }
        }
        else if (lead == Lead::CompoundEyes)
        {
            pid = go.nextUInt();
            frame.setNature(static_cast<u8>(go.nextUInt(25)));
        }
        else if (lead == Lead::Search)
        {
            // TODO
        }
        else // No lead
        {
            go.advanceFrames(1);
            pid = go.nextUInt();
            frame.setNature(static_cast<u8>(go.nextUInt(25)));
        }

        u8 val = idBit ^ (pid & 1) ^ (pid >> 31);
        if (val == 1)
        {
            pid ^= 0x80000000;
        }

        frame.setPID(pid);
        frame.setAbility((pid >> 16) & 1);
        frame.setGender(pid & 255, genderRatio);
        frame.setShiny(tsv, (pid >> 16) ^ (pid & 0xffff), 8);

        if (filter.comparePID(frame))
        {
            frames.append(frame);
        }
    }

    return frames;
}

QVector<StationaryFrame5> StationaryGenerator5::generateRoamer(u64 seed)
{
    QVector<StationaryFrame5> frames;

    BWRNG rng(seed);
    rng.advanceFrames(initialFrame - 1 + offset);

    for (u32 cnt = 0; cnt < maxResults; cnt++)
    {
        StationaryFrame5 frame(initialFrame + cnt);
        BWRNG go(rng.getSeed());

        u32 pid = go.nextUInt();
        frame.setNature(static_cast<u8>(go.nextUInt(25)));

        frame.setPID(pid);
        frame.setAbility((pid >> 16) & 1);
        frame.setGender(pid & 255, genderRatio);
        frame.setShiny(tsv, (pid >> 16) ^ (pid & 0xffff), 8);

        if (filter.comparePID(frame))
        {
            frames.append(frame);
        }
    }

    return frames;
}

QVector<StationaryFrame5> StationaryGenerator5::generateGift(u64 seed)
{
    QVector<StationaryFrame5> frames;

    BWRNG rng(seed);
    rng.advanceFrames(initialFrame - 1 + offset);

    for (u32 cnt = 0; cnt < maxResults; cnt++)
    {
        StationaryFrame5 frame(initialFrame + cnt);
        BWRNG go(rng.getSeed());

        u32 pid = go.nextUInt() ^ 0x10000;
        frame.setNature(static_cast<u8>(go.nextUInt(25)));

        frame.setPID(pid);
        frame.setAbility((pid >> 16) & 1);
        frame.setGender(pid & 255, genderRatio);
        frame.setShiny(tsv, (pid >> 16) ^ (pid & 0xffff), 8);

        if (filter.comparePID(frame))
        {
            frames.append(frame);
        }
    }

    return frames;
}

QVector<StationaryFrame5> StationaryGenerator5::generateEntraLink(u64 seed)
{
    QVector<StationaryFrame5> frames;

    BWRNG rng(seed);
    rng.advanceFrames(initialFrame - 1 + offset);

    for (u32 cnt = 0; cnt < maxResults; cnt++)
    {
        StationaryFrame5 frame(initialFrame + cnt);
        BWRNG go(rng.getSeed());

        // TODO
    }

    return frames;
}

QVector<StationaryFrame5> StationaryGenerator5::generateLarvestaEgg(u64 seed)
{
    QVector<StationaryFrame5> frames;

    BWRNG rng(seed);
    rng.advanceFrames(initialFrame - 1 + offset);

    for (u32 cnt = 0; cnt < maxResults; cnt++)
    {
        StationaryFrame5 frame(initialFrame + cnt);
        BWRNG go(rng.getSeed());

        u32 pid = go.nextUInt();
        go.advanceFrames(1);
        frame.setNature(static_cast<u8>(go.nextUInt(25)));

        frame.setPID(pid);
        frame.setAbility((pid >> 16) & 1);
        frame.setGender(pid & 255, genderRatio);
        frame.setShiny(tsv, (pid >> 16) ^ (pid & 0xffff), 8);

        if (filter.comparePID(frame))
        {
            frames.append(frame);
        }
    }

    return frames;
}

QVector<StationaryFrame5> StationaryGenerator5::generateHiddenGrotto(u64 seed)
{
    QVector<StationaryFrame5> frames;

    BWRNG rng(seed);
    rng.advanceFrames(initialFrame - 1 + offset);

    for (u32 cnt = 0; cnt < maxResults; cnt++)
    {
        StationaryFrame5 frame(initialFrame + cnt);
        BWRNG go(rng.getSeed());

        // TODO
    }

    return frames;
}

void StationaryGenerator5::initializeMT(u8 num)
{
    try
    {
        mt = new MersenneTwisterFast(initialFrame + maxResults + offset + num);
    }
    catch (const std::runtime_error &e)
    {
        (void)e;
        mt = new MersenneTwister();
    }
}
