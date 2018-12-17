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

#include "Generator4.hpp"

Generator4::Generator4()
{
    maxResults = 100000;
    initialFrame = 1;
    initialSeed = 0;
    tid = 12345;
    sid = 54321;
    psv = tid ^ sid;
}

Generator4::Generator4(u32 maxResults, u32 initialFrame, u32 initialSeed, u16 tid, u16 sid, u32 offset, Method type)
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

void Generator4::setEncounter(const EncounterArea4 &value)
{
    encounter = value;
}

QVector<Frame4> Generator4::generate(const FrameCompare &compare)
{
    switch (frameType)
    {
        case Method::Method1:
            return generateMethod1(compare);
        case Method::MethodJ:
            switch (leadType)
            {
                case Lead::None:
                    return generateMethodJ(compare);
                case Lead::Synchronize:
                    return generateMethodJSynch(compare);
                // Default to cover all cute charm cases
                default:
                    return generateMethodJCuteCharm(compare);
            }
        case Method::MethodK:
            switch (leadType)
            {
                case Lead::None:
                case Lead::SuctionCups:
                    return generateMethodK(compare);
                case Synchronize:
                    return generateMethodKSynch(compare);
                // Default to cover all cute charm cases
                default:
                    return generateMethodKCuteCharm(compare);
            }
        case Method::ChainedShiny:
            return generateChainedShiny(compare);
        case Method::WondercardIVs:
            return generateWondercardIVs(compare);
        default:
            return QVector<Frame4>();
    }
}

QVector<Frame4> Generator4::generateMethod1(FrameCompare compare)
{
    QVector<Frame4> frames;
    Frame4 frame = Frame4(tid, sid, psv);
    frame.setGenderRatio(compare.getGenderRatio());

    PokeRNG rng(initialSeed, initialFrame - 1 + offset);
    auto *rngArray = new u16[maxResults + 4];
    for (u32 i = 0; i < maxResults + 4; i++)
    {
        rngArray[i] = rng.nextUShort();
    }

    // Method 1 [SEED] [PID] [PID] [IVS] [IVS]

    for (u32 cnt = 0; cnt < maxResults; cnt++)
    {
        frame.setPID(rngArray[cnt], rngArray[1 + cnt]);
        frame.setIVs(rngArray[2 + cnt], rngArray[3 + cnt]);

        if (compare.compareFrame(frame))
        {
            frame.setSeed(rngArray[cnt]);
            frame.setFrame(cnt + initialFrame);
            frames.append(frame);
        }
    }

    delete[] rngArray;
    return frames;
}

QVector<Frame4> Generator4::generateMethodJ(FrameCompare compare)
{
    QVector<Frame4> frames;
    Frame4 frame = Frame4(tid, sid, psv);
    frame.setGenderRatio(compare.getGenderRatio());

    PokeRNG rng(initialSeed, initialFrame - 1 + offset);
    u32 max = initialFrame + maxResults;
    u32 pid, hunt = 0;
    u16 pid1, pid2;

    u8 thresh = encounterType == Encounter::OldRod ? 25 : encounterType == Encounter::GoodRod ? 50 : encounterType == Encounter::SuperRod ? 75 : 0;

    for (u32 cnt = initialFrame; cnt < max; cnt++)
    {
        PokeRNG go(rng.nextUInt());
        frame.setSeed(go.getSeed());

        switch (encounterType)
        {
            case Encounter::Grass:
                frame.setEncounterSlot(EncounterSlot::jSlot(go.getSeed() >> 16, encounterType));
                if (!compare.compareSlot(frame))
                {
                    continue;
                }

                frame.setLevel(encounter.calcLevel(frame.getEncounterSlot()));
                hunt = 0;
                break;
            case Encounter::Surfing:
                frame.setEncounterSlot(EncounterSlot::jSlot(go.getSeed() >> 16, encounterType));
                if (!compare.compareSlot(frame))
                {
                    continue;
                }

                frame.setLevel(encounter.calcLevel(frame.getEncounterSlot(), go.nextUShort()));
                hunt = 1;
                break;
            case Encounter::OldRod:
            case Encounter::GoodRod:
            case Encounter::SuperRod:
                if (((go.getSeed() >> 16) / 656) >= thresh)
                {
                    continue;
                }

                frame.setEncounterSlot(EncounterSlot::jSlot(go.nextUShort(), encounterType));
                if (!compare.compareSlot(frame))
                {
                    continue;
                }

                frame.setLevel(encounter.calcLevel(frame.getEncounterSlot(), go.nextUShort()));
                hunt = 1;
                break;
            default:
                break;
        }

        // Get hunt nature
        frame.setNature(go.nextUShort() / 0xa3e);

        if (!compare.compareNature(frame))
        {
            continue;
        }

        // Begin search for valid pid
        do
        {
            pid1 = go.nextUShort();
            pid2 = go.nextUShort();
            pid = (pid2 << 16) | pid1;
            hunt += 2;
        }
        while (pid % 25 != frame.getNature());

        frame.setPID(pid, pid2, pid1);

        u16 val1 = go.nextUShort();
        u16 val2 = go.nextUShort();

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

QVector<Frame4> Generator4::generateMethodJSynch(FrameCompare compare)
{
    QVector<Frame4> frames;
    Frame4 frame = Frame4(tid, sid, psv);
    frame.setGenderRatio(compare.getGenderRatio());

    PokeRNG rng(initialSeed, initialFrame - 1 + offset);
    u32 max = initialFrame + maxResults;
    u32 pid, hunt = 0;
    u16 pid1, pid2;

    u8 thresh = encounterType == Encounter::OldRod ? 25 : encounterType == Encounter::GoodRod ? 50 : encounterType == Encounter::SuperRod ? 75 : 0;

    for (u32 cnt = initialFrame; cnt < max; cnt++)
    {
        PokeRNG go(rng.nextUInt());
        frame.setSeed(go.getSeed());

        switch (encounterType)
        {
            case Encounter::Grass:
                frame.setEncounterSlot(EncounterSlot::jSlot(go.getSeed() >> 16, encounterType));
                if (!compare.compareSlot(frame))
                {
                    continue;
                }

                frame.setLevel(encounter.calcLevel(frame.getEncounterSlot()));
                hunt = 1;
                break;
            case Encounter::Surfing:
                frame.setEncounterSlot(EncounterSlot::jSlot(go.getSeed() >> 16, encounterType));
                if (!compare.compareSlot(frame))
                {
                    continue;
                }

                frame.setLevel(encounter.calcLevel(frame.getEncounterSlot(), go.nextUShort()));
                hunt = 2;
                break;
            case Encounter::OldRod:
            case Encounter::GoodRod:
            case Encounter::SuperRod:
                if (((go.getSeed() >> 16) / 656) >= thresh)
                {
                    continue;
                }

                frame.setEncounterSlot(EncounterSlot::jSlot(go.nextUShort(), encounterType));
                if (!compare.compareSlot(frame))
                {
                    continue;
                }

                frame.setLevel(encounter.calcLevel(frame.getEncounterSlot(), go.nextUShort()));
                hunt = 2;
                break;
            default:
                break;
        }


        if ((go.nextUShort() >> 15) == 0) // Successful synch
        {
            frame.setNature(synchNature);
        }
        else // Failed synch
        {
            frame.setNature(go.nextUShort() / 0xa3e);
        }

        if (!compare.compareNature(frame))
        {
            continue;
        }

        // Begin search for valid pid
        do
        {
            pid1 = go.nextUShort();
            pid2 = go.nextUShort();
            pid = (pid2 << 16) | pid1;
            hunt += 2;
        }
        while (pid % 25 != frame.getNature());

        frame.setPID(pid, pid2, pid1);

        u16 val1 = go.nextUShort();
        u16 val2 = go.nextUShort();

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

QVector<Frame4> Generator4::generateMethodJCuteCharm(FrameCompare compare)
{
    QVector<Frame4> frames;
    Frame4 frame = Frame4(tid, sid, psv);
    frame.setGenderRatio(compare.getGenderRatio());

    PokeRNG rng(initialSeed, initialFrame - 1 + offset);
    u32 max = initialFrame + maxResults;
    u32 pid, hunt = 0;
    u16 pid1, pid2;

    u8 buffer = 0;
    u8 thresh = encounterType == Encounter::OldRod ? 25 : encounterType == Encounter::GoodRod ? 50 : encounterType == Encounter::SuperRod ? 75 : 0;

    switch (leadType)
    {
        case Lead::CuteCharmFemale:
            buffer = 0;
            break;
        case Lead::CuteCharm25M:
            buffer = 0xC8;
            break;
        case Lead::CuteCharm50M:
            buffer = 0x96;
            break;
        case Lead::CuteCharm75M:
            buffer = 0x4B;
            break;
        case Lead::CuteCharm875M:
            buffer = 0x32;
            break;
        default:
            break;
    }

    for (u32 cnt = initialFrame; cnt < max; cnt++)
    {
        PokeRNG go(rng.nextUInt());
        frame.setSeed(go.getSeed());

        switch (encounterType)
        {
            case Encounter::Grass:
                frame.setEncounterSlot(EncounterSlot::jSlot(go.getSeed() >> 16, encounterType));
                if (!compare.compareSlot(frame))
                {
                    continue;
                }

                frame.setLevel(encounter.calcLevel(frame.getEncounterSlot()));
                hunt = 1;
                break;
            case Encounter::Surfing:
                frame.setEncounterSlot(EncounterSlot::jSlot(go.getSeed() >> 16, encounterType));
                if (!compare.compareSlot(frame))
                {
                    continue;
                }

                frame.setLevel(encounter.calcLevel(frame.getEncounterSlot(), go.nextUShort()));
                hunt = 2;
                break;
            case Encounter::OldRod:
            case Encounter::GoodRod:
            case Encounter::SuperRod:
                if (((go.getSeed() >> 16) / 656) >= thresh)
                {
                    continue;
                }

                frame.setEncounterSlot(EncounterSlot::jSlot(go.nextUShort(), encounterType));
                if (!compare.compareSlot(frame))
                {
                    continue;
                }

                frame.setLevel(encounter.calcLevel(frame.getEncounterSlot(), go.nextUShort()));
                hunt = 2;
                break;
            default:
                break;
        }


        if ((go.nextUShort() / 0x5556) != 0) // Successful cute charm
        {
            // Get nature
            frame.setNature(go.nextUShort() / 0xa3e);

            if (!compare.compareNature(frame))
            {
                continue;
            }

            // Cute charm doesn't hunt for a valid PID, just uses buffer and target nature
            frame.setPID(buffer + frame.getNature(), 0, buffer + frame.getNature());
            frame.setOccidentary(cnt);
        }
        else // Failed cute charm
        {
            // Get nature
            frame.setNature(go.nextUShort() / 0xa3e);

            if (!compare.compareNature(frame))
            {
                continue;
            }

            // Begin search for valid pid
            do
            {
                pid1 = go.nextUShort();
                pid2 = go.nextUShort();
                pid = (pid2 << 16) | pid1;
                hunt += 2;
            }
            while (pid % 25 != frame.getNature());

            frame.setPID(pid, pid2, pid1);
            frame.setOccidentary(hunt + cnt);
        }

        u16 val1 = go.nextUShort();
        u16 val2 = go.nextUShort();

        frame.setIVs(val1, val2);

        if (compare.compareFrame(frame))
        {
            frame.setFrame(cnt);
            frames.append(frame);
        }
    }

    return frames;
}

QVector<Frame4> Generator4::generateMethodK(FrameCompare compare)
{
    QVector<Frame4> frames;
    Frame4 frame = Frame4(tid, sid, psv);
    frame.setGenderRatio(compare.getGenderRatio());

    PokeRNG rng(initialSeed, initialFrame - 1 + offset);
    u32 max = initialFrame + maxResults;
    u32 pid, hunt = 0;
    u16 pid1, pid2;

    u8 thresh = 0;
    if (encounterType == Encounter::OldRod)
    {
        thresh = leadType == Lead::SuctionCups ? 90 : 25;
    }
    else if (encounterType == Encounter::GoodRod)
    {
        thresh = leadType == Lead::SuctionCups ? 100 : 50;
    }
    else if (encounterType == Encounter::SuperRod)
    {
        thresh = leadType == Lead::SuctionCups ? 100 : 75;
    }

    u8 rate = encounter.getEncounterRate();

    for (u32 cnt = initialFrame; cnt < max; cnt++)
    {
        PokeRNG go(rng.nextUInt());
        frame.setSeed(go.getSeed());

        switch (encounterType)
        {
            case Encounter::Grass:
                frame.setEncounterSlot(EncounterSlot::kSlot(go.getSeed() >> 16, encounterType));
                if (!compare.compareSlot(frame))
                {
                    continue;
                }

                frame.setLevel(encounter.calcLevel(frame.getEncounterSlot()));
                hunt = 1;
                break;
            case Encounter::Surfing:
                frame.setEncounterSlot(EncounterSlot::kSlot(go.getSeed() >> 16, encounterType));
                if (!compare.compareSlot(frame))
                {
                    continue;
                }

                frame.setLevel(encounter.calcLevel(frame.getEncounterSlot(), go.nextUShort()));
                hunt = 2;
                break;
            case Encounter::OldRod:
            case Encounter::GoodRod:
            case Encounter::SuperRod:
                if (((go.getSeed() >> 16) % 100) >= thresh)
                {
                    continue;
                }

                frame.setEncounterSlot(EncounterSlot::kSlot(go.nextUShort(), encounterType));
                if (!compare.compareSlot(frame))
                {
                    continue;
                }

                frame.setLevel(encounter.calcLevel(frame.getEncounterSlot()));
                go.advanceFrames(1);
                hunt = 3;
                break;
            case Encounter::RockSmash:
                // Blank(or maybe item) ???
                if (((go.nextUShort()) % 100) >= rate)
                {
                    continue;
                }

                frame.setEncounterSlot(EncounterSlot::kSlot(go.nextUShort(), encounterType));
                if (!compare.compareSlot(frame))
                {
                    continue;
                }

                frame.setLevel(encounter.calcLevel(frame.getEncounterSlot(), go.nextUShort()));
                hunt = 2;
                break;
            case Encounter::HeadButt: // TODO
            case Encounter::BugCatchingContest: // TODO
            default:
                break;
        }

        // Get hunt nature
        frame.setNature(go.nextUShort() % 25);

        if (!compare.compareNature(frame))
        {
            continue;
        }

        // Begin search for valid pid
        do
        {
            pid1 = go.nextUShort();
            pid2 = go.nextUShort();
            pid = (pid2 << 16) | pid1;
            hunt += 2;
        }
        while (pid % 25 != frame.getNature());

        frame.setPID(pid, pid2, pid1);

        u16 val1 = go.nextUShort();
        u16 val2 = go.nextUShort();

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

QVector<Frame4> Generator4::generateMethodKSynch(FrameCompare compare)
{
    QVector<Frame4> frames;
    Frame4 frame = Frame4(tid, sid, psv);
    frame.setGenderRatio(compare.getGenderRatio());

    PokeRNG rng(initialSeed, initialFrame - 1 + offset);
    u32 max = initialFrame + maxResults;
    u32 pid, hunt = 0;
    u16 pid1, pid2;

    u8 thresh = encounterType == Encounter::OldRod ? 25 : encounterType == Encounter::GoodRod ? 50 : encounterType == Encounter::SuperRod ? 75 : 0;
    u8 rock = encounter.getEncounterRate();

    for (u32 cnt = initialFrame; cnt < max; cnt++)
    {
        PokeRNG go(rng.nextUInt());
        frame.setSeed(go.getSeed());

        switch (encounterType)
        {
            case Encounter::Grass:
                frame.setEncounterSlot(EncounterSlot::kSlot(go.getSeed() >> 16, encounterType));
                if (!compare.compareSlot(frame))
                {
                    continue;
                }

                frame.setLevel(encounter.calcLevel(frame.getEncounterSlot()));
                hunt = 2;
                break;
            case Encounter::Surfing:
                frame.setEncounterSlot(EncounterSlot::kSlot(go.getSeed() >> 16, encounterType));
                if (!compare.compareSlot(frame))
                {
                    continue;
                }

                frame.setLevel(encounter.calcLevel(frame.getEncounterSlot(), go.nextUShort()));
                hunt = 3;
                break;
            case Encounter::OldRod:
            case Encounter::GoodRod:
            case Encounter::SuperRod:
                if (((go.getSeed() >> 16) % 100) >= thresh)
                {
                    continue;
                }

                frame.setEncounterSlot(EncounterSlot::kSlot(go.nextUShort(), encounterType));
                if (!compare.compareSlot(frame))
                {
                    continue;
                }

                frame.setLevel(encounter.calcLevel(frame.getEncounterSlot()));
                go.advanceFrames(1);
                hunt = 4;
                break;
            case Encounter::RockSmash:
                // Blank(or maybe item) ???
                if (((go.nextUShort()) % 100) >= rock)
                {
                    continue;
                }

                frame.setEncounterSlot(EncounterSlot::kSlot(go.nextUShort(), encounterType));
                if (!compare.compareSlot(frame))
                {
                    continue;
                }

                frame.setLevel(encounter.calcLevel(frame.getEncounterSlot(), go.nextUShort()));
                hunt = 3;
                break;
            case Encounter::HeadButt: // TODO
            case Encounter::BugCatchingContest: // TODO
            default:
                break;
        }


        if ((go.nextUShort() & 1) == 0) // Successful synch
        {
            frame.setNature(synchNature);
        }
        else // Failed synch
        {
            frame.setNature(go.nextUShort() % 25);
        }

        if (!compare.compareNature(frame))
        {
            continue;
        }

        // Begin search for valid pid
        do
        {
            pid1 = go.nextUShort();
            pid2 = go.nextUShort();
            pid = (pid2 << 16) | pid1;
        }
        while (pid % 25 != frame.getNature());

        frame.setPID(pid, pid2, pid1);

        u16 val1 = go.nextUShort();
        u16 val2 = go.nextUShort();

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

QVector<Frame4> Generator4::generateMethodKCuteCharm(FrameCompare compare)
{
    QVector<Frame4> frames;
    Frame4 frame = Frame4(tid, sid, psv);
    frame.setGenderRatio(compare.getGenderRatio());

    PokeRNG rng(initialSeed, initialFrame - 1 + offset);
    u32 max = initialFrame + maxResults;
    u32 pid, hunt = 0;
    u16 pid1, pid2;

    u8 buffer = 0;
    u8 thresh = encounterType == Encounter::OldRod ? 25 : encounterType == Encounter::GoodRod ? 50 : encounterType == Encounter::SuperRod ? 75 : 0;
    u8 rock = encounter.getEncounterRate();

    switch (leadType)
    {
        case Lead::CuteCharmFemale:
            buffer = 0;
            break;
        case Lead::CuteCharm25M:
            buffer = 0xC8;
            break;
        case Lead::CuteCharm50M:
            buffer = 0x96;
            break;
        case Lead::CuteCharm75M:
            buffer = 0x4B;
            break;
        case Lead::CuteCharm875M:
            buffer = 0x32;
            break;
        default:
            break;
    }

    for (u32 cnt = initialFrame; cnt < max; cnt++)
    {
        PokeRNG go(rng.nextUInt());
        frame.setSeed(go.getSeed());

        switch (encounterType)
        {
            case Encounter::Grass:
                frame.setEncounterSlot(EncounterSlot::kSlot(go.getSeed() >> 16, encounterType));
                if (!compare.compareSlot(frame))
                {
                    continue;
                }

                frame.setLevel(encounter.calcLevel(frame.getEncounterSlot()));
                hunt = 2;
                break;
            case Encounter::Surfing:
                frame.setEncounterSlot(EncounterSlot::kSlot(go.getSeed() >> 16, encounterType));
                if (!compare.compareSlot(frame))
                {
                    continue;
                }

                frame.setLevel(encounter.calcLevel(frame.getEncounterSlot(), go.nextUShort()));
                hunt = 3;
                break;
            case Encounter::OldRod:
            case Encounter::GoodRod:
            case Encounter::SuperRod:
                if (((go.getSeed() >> 16) % 100) >= thresh)
                {
                    continue;
                }

                frame.setEncounterSlot(EncounterSlot::kSlot(go.nextUShort(), encounterType));
                if (!compare.compareSlot(frame))
                {
                    continue;
                }

                frame.setLevel(encounter.calcLevel(frame.getEncounterSlot()));
                go.advanceFrames(1);
                hunt = 4;
                break;
            case Encounter::RockSmash:
                // Blank(or maybe item) ???
                if (((go.nextUShort()) % 100) >= rock)
                {
                    continue;
                }

                frame.setEncounterSlot(EncounterSlot::kSlot(go.nextUShort(), encounterType));
                if (!compare.compareSlot(frame))
                {
                    continue;
                }

                frame.setLevel(encounter.calcLevel(frame.getEncounterSlot(), go.nextUShort()));
                hunt = 3;
                break;
            case Encounter::HeadButt: // TODO
            case Encounter::BugCatchingContest: // TODO
            default:
                break;
        }

        if ((go.nextUShort() % 3) != 0) // Successfull cute charm
        {
            // Get hunt nature
            frame.setNature(go.nextUShort() % 25);

            if (!compare.compareNature(frame))
            {
                continue;
            }

            frame.setPID(buffer + frame.getNature(), 0, buffer + frame.getNature());
            frame.setOccidentary(cnt);
        }
        else // Failed cutecharm
        {
            // Get hunt nature
            frame.setNature(go.nextUShort() % 25);

            if (!compare.compareNature(frame))
            {
                continue;
            }

            // Begin search for valid pid
            do
            {
                pid1 = go.nextUShort();
                pid2 = go.nextUShort();
                pid = (pid2 << 16) | pid1;
            }
            while (pid % 25 != frame.getNature());

            frame.setPID(pid, pid2, pid1);
            frame.setOccidentary(hunt + cnt);
        }

        u16 val1 = go.nextUShort();
        u16 val2 = go.nextUShort();

        frame.setIVs(val1, val2);

        if (compare.compareFrame(frame))
        {
            frame.setFrame(cnt);
            frames.append(frame);
        }
    }

    return frames;
}

QVector<Frame4> Generator4::generateChainedShiny(FrameCompare compare)
{
    QVector<Frame4> frames;
    Frame4 frame = Frame4(tid, sid, psv);
    frame.setGenderRatio(compare.getGenderRatio());

    PokeRNG rng(initialSeed, initialFrame - 1 + offset);
    auto *rngArray = new u16[maxResults + 18];
    for (u32 i = 0; i < maxResults + 18; i++)
    {
        rngArray[i] = rng.nextUShort();
    }

    u16 low, high;

    for (u32 cnt = initialFrame; cnt < maxResults; cnt++)
    {
        low = chainedPIDLow(rngArray[1 + cnt], rngArray[15 + cnt], rngArray[14 + cnt], rngArray[13 + cnt], rngArray[12 + cnt], rngArray[11 + cnt], rngArray[10 + cnt],
                            rngArray[9 + cnt], rngArray[8 + cnt], rngArray[7 + cnt], rngArray[6 + cnt], rngArray[5 + cnt], rngArray[4 + cnt], rngArray[3 + cnt]);
        high = chainedPIDHigh(rngArray[2 + cnt], low, tid, sid);

        frame.setPID(low, high);
        frame.setIVs(rngArray[16 + cnt], rngArray[17 + cnt]);

        if (compare.compareFrame(frame))
        {
            frame.setSeed(rngArray[cnt]);
            frame.setFrame(cnt + initialFrame);
            frames.append(frame);
        }
    }

    delete[] rngArray;
    return frames;
}

QVector<Frame4> Generator4::generateWondercardIVs(FrameCompare compare)
{
    QVector<Frame4> frames;
    Frame4 frame = Frame4(tid, sid, psv);

    PokeRNG rng(initialSeed, initialFrame - 1 + offset);
    auto *rngArray = new u16[maxResults + 2];
    for (u32 i = 0; i < maxResults + 2; i++)
    {
        rngArray[i] = rng.nextUShort();
    }

    // Wondercard IVs [SEED] [IVS] [IVS]

    for (u32 cnt = 0; cnt < maxResults; cnt++)
    {
        frame.setIVs(rngArray[cnt], rngArray[1 + cnt]);

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

u16 Generator4::chainedPIDLow(u16 low, u16 call1, u16 call2, u16 call3, u16 call4, u16 call5, u16 call6, u16 call7, u16 call8, u16 call9, u16 call10, u16 call11, u16 call12, u16 call13)
{
    return (low & 7) | (call13 & 1) << 3 | (call12 & 1) << 4 | (call11 & 1) << 5 | (call10 & 1) << 6 |
           (call9 & 1) << 7 | (call8 & 1) << 8 | (call7 & 1) << 9 | (call6 & 1) << 10 | (call5 & 1) << 11 |
           (call4 & 1) << 12 | (call3 & 1) << 13 | (call2 & 1) << 14 | (call1 & 1) << 15;
}

u16 Generator4::chainedPIDHigh(u16 high, u16 low, u16 tid, u16 sid)
{
    return (((low ^ tid ^ sid) & 0xFFF8) | (high & 7));
}
