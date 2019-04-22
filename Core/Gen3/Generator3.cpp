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

#include "Generator3.hpp"
#include <Core/RNG/LCRNG.hpp>
#include <Core/Util/EncounterSlot.hpp>

Generator3::Generator3()
{
    maxResults = 100000;
    initialFrame = 1;
    initialSeed = 0;
    tid = 12345;
    sid = 54321;
    psv = tid ^ sid;
}

Generator3::Generator3(u32 maxResults, u32 initialFrame, u32 initialSeed, u16 tid, u16 sid, u32 offset, u8 genderRatio)
{
    this->maxResults = maxResults;
    this->initialFrame = initialFrame;
    this->initialSeed = initialSeed;
    this->tid = tid;
    this->sid = sid;
    psv = tid ^ sid;
    this->offset = offset;
    this->genderRatio = genderRatio;
}

QVector<Frame3> Generator3::generate(const FrameCompare &compare) const
{
    switch (frameType)
    {
        case Method::Method1:
        case Method::Method2:
        case Method::Method4:
            return generateMethod124(compare);
        case Method::Method1Reverse:
            return generateMethod1Reverse(compare);
        case Method::MethodH1:
        case Method::MethodH2:
        case Method::MethodH4:
            switch (leadType)
            {
                case Lead::None:
                    return generateMethodH124(compare);
                case Lead::Synchronize:
                    return generateMethodH124Synch(compare);
                // case Lead::CuteCharm:
                // Set to default to avoid compiler warning message
                default:
                    return generateMethodH124CuteCharm(compare);
            }
        case Method::XDColo:
            return generateMethodXDColo(compare);
        case Method::Channel:
            return generateMethodChannel(compare);
        default:
            return QVector<Frame3>();
    }
}

void Generator3::setup(Method method)
{
    frameType = method;
    if (frameType == Method::Method1 || frameType == Method::MethodH1)
    {
        iv1 = frameType == Method::MethodH1 ? 1 : 2;
        iv2 = frameType == Method::MethodH1 ? 2 : 3;
    }
    else if (frameType == Method::Method2 || frameType == Method::MethodH2)
    {
        iv1 = frameType == Method::MethodH2 ? 2 : 3;
        iv2 = frameType == Method::MethodH2 ? 3 : 4;
    }
    else if (frameType == Method::Method4 || frameType == Method::MethodH4)
    {
        iv1 = frameType == Method::MethodH4 ? 1 : 2;
        iv2 = frameType == Method::MethodH4 ? 3 : 4;
    }
}

void Generator3::setEncounter(const EncounterArea3 &value)
{
    encounter = value;
}

QVector<Frame3> Generator3::generateMethodChannel(const FrameCompare &compare) const
{
    QVector<Frame3> frames;
    Frame3 frame(tid, sid, psv);

    XDRNG rng(initialSeed, initialFrame - 1 + offset);
    QVector<u16> rngList(maxResults + 12);
    for (u16 &x : rngList)
    {
        x = rng.nextUShort();
    }

    // Method Channel [SEED] [SID] [PID] [PID] [BERRY] [GAME ORIGIN] [OT GENDER] [IV] [IV] [IV] [IV] [IV] [IV]

    for (u32 cnt = 0; cnt < maxResults; cnt++)
    {
        frame.setIDs(40122, rngList.at(cnt), 40122 ^ rngList.at(cnt));

        if ((rngList.at(cnt + 2) > 7 ? 0 : 1) != (rngList.at(cnt + 1) ^ 40122 ^ rngList.at(cnt)))
        {
            frame.setPID(rngList.at(cnt + 2), rngList.at(cnt + 1) ^ 0x8000, genderRatio);
        }
        else
        {
            frame.setPID(rngList.at(cnt + 2), rngList.at(cnt + 1), genderRatio);
        }

        frame.setIVs(rngList.at(cnt + 6) >> 11, rngList.at(cnt + 7) >> 11, rngList.at(cnt + 8) >> 11,
                     rngList.at(cnt + 10) >> 11, rngList.at(cnt + 11) >> 11, rngList.at(cnt + 9) >> 11);

        if (compare.compareFrame(frame))
        {
            frame.setFrame(cnt + initialFrame);
            frames.append(frame);
        }
    }

    return frames;
}

QVector<Frame3> Generator3::generateMethodH124(const FrameCompare &compare) const
{
    QVector<Frame3> frames;
    Frame3 frame(tid, sid, psv);

    PokeRNG rng(initialSeed, initialFrame - 1 + offset);
    u32 max = initialFrame + maxResults;
    u32 pid, hunt = 0;
    u16 pid1, pid2, val1, val2;

    u16 rate = encounter.getEncounterRate() * 16;
    bool rock = rate == 2880;

    for (u32 cnt = initialFrame; cnt < max; cnt++)
    {
        PokeRNG go(rng.nextUInt());
        hunt = 2;

        switch (encounterType)
        {
            case Encounter::RockSmash:
                if (!rock)
                {
                    go.nextUInt();
                }
                if (((go.getSeed() >> 16) % 2880) >= rate)
                {
                    continue;
                }

                frame.setEncounterSlot(EncounterSlot::hSlot(go.nextUShort(), encounterType));
                if (!compare.compareSlot(frame))
                {
                    continue;
                }

                frame.setLevel(encounter.calcLevel(frame.getEncounterSlot(), go.nextUShort()));
                break;
            case Encounter::SafariZone:
                frame.setEncounterSlot(EncounterSlot::hSlot(go.getSeed() >> 16, encounterType));
                if (!compare.compareSlot(frame))
                {
                    continue;
                }

                frame.setLevel(encounter.calcLevel(frame.getEncounterSlot()));
                go.advanceFrames(2);
                break;
            case Encounter::Grass:
                frame.setEncounterSlot(EncounterSlot::hSlot(go.nextUShort(), encounterType));
                if (!compare.compareSlot(frame))
                {
                    continue;
                }

                frame.setLevel(encounter.calcLevel(frame.getEncounterSlot()));
                go.advanceFrames(1);
                break;
            case Encounter::Surfing:
            case Encounter::OldRod:
            case Encounter::GoodRod:
            case Encounter::SuperRod:
                frame.setEncounterSlot(EncounterSlot::hSlot(go.nextUShort(), encounterType));
                if (!compare.compareSlot(frame))
                {
                    continue;
                }

                frame.setLevel(encounter.calcLevel(frame.getEncounterSlot(), go.nextUShort()));
                break;
            default:
                break;
        }

        // Method H relies on grabbing a hunt nature and generating PIDs until the PID nature matches the hunt nature
        // Grab the hunt nature
        frame.setNature(go.nextUShort() % 25);
        if (!compare.compareNature(frame))
        {
            continue;
        }

        // Now search for a Method 124 PID that matches our hunt nature
        do
        {
            pid2 = go.nextUShort();
            pid1 = go.nextUShort();
            pid = (pid1 << 16) | pid2;
            hunt += 2;
        }
        while (pid % 25 != frame.getNature());

        frame.setPID(pid, genderRatio);

        // Valid PID is found now time to generate IVs
        if (frameType == Method::MethodH1)
        {
            val1 = go.nextUShort();
            val2 = go.nextUShort();
        }
        else if (frameType == Method::MethodH2)
        {
            go.nextUInt();
            val1 = go.nextUShort();
            val2 = go.nextUShort();
        }
        else
        {
            val1 = go.nextUShort();
            go.nextUInt();
            val2 = go.nextUShort();
        }
        frame.setIVs(val1, val2);

        if (compare.compareFrame(frame))
        {
            frame.setFrame(cnt);
            frame.setOccidentary(hunt + cnt);
            frames.append(frame);
        }
    }

    return frames;
}

QVector<Frame3> Generator3::generateMethodH124Synch(const FrameCompare &compare) const
{
    QVector<Frame3> frames;
    Frame3 frame(tid, sid, psv);

    PokeRNG rng(initialSeed, initialFrame - 1 + offset);
    u32 max = initialFrame + maxResults;
    u32 pid, hunt = 0;
    u16 pid1, pid2, val1, val2;

    u16 rate = encounter.getEncounterRate() * 16;
    bool rock = rate == 2880;

    for (u32 cnt = initialFrame; cnt < max; cnt++)
    {
        PokeRNG go(rng.nextUInt());
        hunt = 2;

        switch (encounterType)
        {
            case Encounter::RockSmash:
                if (!rock)
                {
                    go.nextUInt();
                }
                if (((go.getSeed() >> 16) % 2880) >= rate)
                {
                    continue;
                }

                frame.setEncounterSlot(EncounterSlot::hSlot(go.nextUShort(), encounterType));
                if (!compare.compareSlot(frame))
                {
                    continue;
                }

                frame.setLevel(encounter.calcLevel(frame.getEncounterSlot(), go.nextUShort()));
                break;
            case Encounter::SafariZone:
                frame.setEncounterSlot(EncounterSlot::hSlot(go.getSeed() >> 16, encounterType));
                if (!compare.compareSlot(frame))
                {
                    continue;
                }

                frame.setLevel(encounter.calcLevel(frame.getEncounterSlot()));
                go.advanceFrames(2);
                break;
            case Encounter::Grass:
                frame.setEncounterSlot(EncounterSlot::hSlot(go.nextUShort(), encounterType));
                if (!compare.compareSlot(frame))
                {
                    continue;
                }

                frame.setLevel(encounter.calcLevel(frame.getEncounterSlot()));
                go.advanceFrames(1);
                break;
            case Encounter::Surfing:
            case Encounter::OldRod:
            case Encounter::GoodRod:
            case Encounter::SuperRod:
                frame.setEncounterSlot(EncounterSlot::hSlot(go.nextUShort(), encounterType));
                if (!compare.compareSlot(frame))
                {
                    continue;
                }

                frame.setLevel(encounter.calcLevel(frame.getEncounterSlot(), go.nextUShort()));
                break;
            default:
                break;
        }

        // Method H relies on grabbing a hunt nature and generating PIDs until the PID nature matches the hunt nature
        // Check by seeing if frame can synch
        if ((go.nextUShort() & 1) == 0) // Frame is synchable so set nature to synch nature
        {
            frame.setNature(synchNature);
        }
        else // Synch failed so grab hunt nature from next RNG call
        {
            frame.setNature(go.nextUShort() % 25);
            hunt++;
        }

        if (!compare.compareNature(frame))
        {
            continue;
        }

        // Now search for a Method 124 PID that matches our hunt nature
        do
        {
            pid2 = go.nextUShort();
            pid1 = go.nextUShort();
            pid = (pid1 << 16) | pid2;
            hunt += 2;
        }
        while (pid % 25 != frame.getNature());

        frame.setPID(pid, genderRatio);

        // Valid PID is found now time to generate IVs
        if (frameType == Method::MethodH1)
        {
            val1 = go.nextUShort();
            val2 = go.nextUShort();
        }
        else if (frameType == Method::MethodH2)
        {
            go.nextUInt();
            val1 = go.nextUShort();
            val2 = go.nextUShort();
        }
        else
        {
            val1 = go.nextUShort();
            go.nextUInt();
            val2 = go.nextUShort();
        }
        frame.setIVs(val1, val2);

        if (compare.compareFrame(frame))
        {
            frame.setFrame(cnt);
            frame.setOccidentary(hunt + cnt);
            frames.append(frame);
        }
    }

    return frames;
}

QVector<Frame3> Generator3::generateMethodH124CuteCharm(const FrameCompare &compare) const
{
    QVector<Frame3> frames;
    Frame3 frame(tid, sid, psv);

    PokeRNG rng(initialSeed, initialFrame - 1 + offset);
    u32 max = initialFrame + maxResults;
    u32 pid, hunt = 0;
    u16 pid1, pid2, val1, val2;

    bool (*cuteCharm)(u32);
    switch (leadType)
    {
        case Lead::CuteCharm125F:
            cuteCharm = &Generator3::cuteCharm125F;
            break;
        case Lead::CuteCharm875M:
            cuteCharm = &Generator3::cuteCharm875M;
            break;
        case Lead::CuteCharm25F:
            cuteCharm = &Generator3::cuteCharm25F;
            break;
        case Lead::CuteCharm75M:
            cuteCharm = &Generator3::cuteCharm75M;
            break;
        case Lead::CuteCharm50F:
            cuteCharm = &Generator3::cuteCharm50F;
            break;
        case Lead::CuteCharm50M:
            cuteCharm = &Generator3::cuteCharm50M;
            break;
        case Lead::CuteCharm75F:
            cuteCharm = &Generator3::cuteCharm75F;
            break;
        case Lead::CuteCharm25M:
        default:
            cuteCharm = &Generator3::cuteCharm25F;
            break;
    }

    u16 rate = encounter.getEncounterRate() * 16;
    bool rock = rate == 2880;

    for (u32 cnt = initialFrame; cnt < max; cnt++)
    {
        PokeRNG go(rng.nextUInt());
        hunt = 3;

        switch (encounterType)
        {
            case Encounter::RockSmash:
                if (!rock)
                {
                    go.nextUInt();
                }
                if (((go.getSeed() >> 16)  % 2880) >= rate)
                {
                    continue;
                }

                frame.setEncounterSlot(EncounterSlot::hSlot(go.nextUShort(), encounterType));
                if (!compare.compareSlot(frame))
                {
                    continue;
                }

                frame.setLevel(encounter.calcLevel(frame.getEncounterSlot(), go.nextUShort()));
                break;
            case Encounter::SafariZone:
                frame.setEncounterSlot(EncounterSlot::hSlot(go.getSeed() >> 16, encounterType));
                if (!compare.compareSlot(frame))
                {
                    continue;
                }

                frame.setLevel(encounter.calcLevel(frame.getEncounterSlot()));
                go.advanceFrames(2);
                break;
            case Encounter::Grass:
                frame.setEncounterSlot(EncounterSlot::hSlot(go.nextUShort(), encounterType));
                if (!compare.compareSlot(frame))
                {
                    continue;
                }

                frame.setLevel(encounter.calcLevel(frame.getEncounterSlot()));
                go.advanceFrames(1);
                break;
            case Encounter::Surfing:
            case Encounter::OldRod:
            case Encounter::GoodRod:
            case Encounter::SuperRod:
                frame.setEncounterSlot(EncounterSlot::hSlot(go.nextUShort(), encounterType));
                if (!compare.compareSlot(frame))
                {
                    continue;
                }

                frame.setLevel(encounter.calcLevel(frame.getEncounterSlot(), go.nextUShort()));
                break;
            default:
                break;
        }

        // Check if cute charm will effect frame
        if (go.nextUShort() % 3 > 0)
        {
            // Call next RNG to determine hunt nature
            frame.setNature(go.nextUShort() % 25);
            if (!compare.compareNature(frame))
            {
                continue;
            }

            // Now search for a Method 124 PID that matches our hunt nature and cute charm
            do
            {
                pid2 = go.nextUShort();
                pid1 = go.nextUShort();
                pid = (pid1 << 16) | pid2;
                hunt += 2;
            }
            while (pid % 25 != frame.getNature() || !cuteCharm(pid));
        }
        else
        {
            // Call next RNG to determine hunt nature
            frame.setNature(go.nextUShort() % 25);
            if (!compare.compareNature(frame))
            {
                continue;
            }

            // Cute charm failed
            // Only search for a Method 124 PID that matches our hunt nature
            do
            {
                pid2 = go.nextUShort();
                pid1 = go.nextUShort();
                pid = (pid1 << 16) | pid2;
                hunt += 2;
            }
            while (pid % 25 != frame.getNature());
        }

        frame.setPID(pid, genderRatio);

        // Valid PID is found now time to generate IVs
        if (frameType == Method::MethodH1)
        {
            val1 = go.nextUShort();
            val2 = go.nextUShort();
        }
        else if (frameType == Method::MethodH2)
        {
            go.nextUInt();
            val1 = go.nextUShort();
            val2 = go.nextUShort();
        }
        else
        {
            val1 = go.nextUShort();
            go.nextUInt();
            val2 = go.nextUShort();
        }
        frame.setIVs(val1, val2);

        if (compare.compareFrame(frame))
        {
            frame.setFrame(cnt);
            frame.setOccidentary(hunt + cnt);
            frames.append(frame);
        }
    }

    return frames;
}

QVector<Frame3> Generator3::generateMethodXDColo(const FrameCompare &compare) const
{
    QVector<Frame3> frames;
    Frame3 frame(tid, sid, psv);

    XDRNG rng(initialSeed, initialFrame - 1 + offset);
    QVector<u16> rngList(maxResults + 5);
    for (u16 &x : rngList)
    {
        x = rng.nextUShort();
    }

    // Method XD/Colo [SEED] [IVS] [IVS] [BLANK] [PID] [PID]

    for (u32 cnt = 0; cnt < maxResults; cnt++)
    {
        frame.setPID(rngList.at(cnt + 4), rngList.at(cnt + 3), genderRatio);
        frame.setIVs(rngList.at(cnt), rngList.at(cnt + 1));

        if (compare.compareFrame(frame))
        {
            frame.setFrame(cnt + initialFrame);
            frames.append(frame);
        }
    }

    return frames;
}

QVector<Frame3> Generator3::generateMethod124(const FrameCompare &compare) const
{
    QVector<Frame3> frames;
    Frame3 frame(tid, sid, psv);

    PokeRNG rng(initialSeed, initialFrame - 1 + offset);
    QVector<u16> rngList(maxResults + 5);
    for (u16 &x : rngList)
    {
        x = rng.nextUShort();
    }

    // Method 1 [SEED] [PID] [PID] [IVS] [IVS]
    // Method 2 [SEED] [PID] [PID] [BLANK] [IVS] [IVS]
    // Method 4 [SEED] [PID] [PID] [IVS] [BLANK] [IVS]

    for (u32 cnt = 0; cnt < maxResults; cnt++)
    {
        frame.setPID(rngList.at(cnt), rngList.at(cnt + 1), genderRatio);
        frame.setIVs(rngList.at(cnt + iv1), rngList.at(cnt + iv2));

        if (compare.compareFrame(frame))
        {
            frame.setFrame(cnt + initialFrame);
            frames.append(frame);
        }
    }

    return frames;
}

QVector<Frame3> Generator3::generateMethod1Reverse(const FrameCompare &compare) const
{
    QVector<Frame3> frames;
    Frame3 frame(tid, sid, psv);

    PokeRNG rng(initialSeed, initialFrame - 1 + offset);
    QVector<u16> rngList(maxResults + 4);
    for (u16 &x : rngList)
    {
        x = rng.nextUShort();
    }

    // Method 1 Reverse [SEED] [PID] [PID] [IVS] [IVS]

    for (u32 cnt = 0; cnt < maxResults; cnt++)
    {
        frame.setPID(rngList.at(cnt + 1), rngList.at(cnt), genderRatio);
        frame.setIVs(rngList.at(cnt + 2), rngList.at(cnt + 3));

        if (compare.compareFrame(frame))
        {
            frame.setFrame(cnt + initialFrame);
            frames.append(frame);
        }
    }

    return frames;
}
