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

#include "Searcher4.hpp"
#include <Core/RNG/LCRNG.hpp>
#include <Core/Util/EncounterSlot.hpp>

static const QVector<u8> genderThreshHolds = { 0, 0x96, 0xC8, 0x4B, 0x32 };

Searcher4::Searcher4()
{
    tid = 12345;
    sid = 54321;
    psv = tid ^ sid;
}

Searcher4::Searcher4(u16 tid, u16 sid, u8 genderRatio, u32 minDelay, u32 maxDelay, u32 minFrame, u32 maxFrame, const FrameCompare &compare, Method method)
{
    this->tid = tid;
    this->sid = sid;
    psv = tid ^ sid;
    this->genderRatio = genderRatio;
    this->minDelay = minDelay;
    this->maxDelay = maxDelay;
    this->minFrame = minFrame;
    this->maxFrame = maxFrame;
    this->compare = compare;
    frameType = method;
    cache.switchCache(method);
}

void Searcher4::setEncounter(const EncounterArea4 &value)
{
    encounter = value;
}

QVector<Frame4> Searcher4::search(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe) const
{
    QVector<Frame4> frames;
    switch (frameType)
    {
        case Method::Method1:
            frames = searchMethod1(hp, atk, def, spa, spd, spe);
            break;
        case Method::MethodJ:
            switch (leadType)
            {
                case Lead::None:
                    frames = searchMethodJ(hp, atk, def, spa, spd, spe);
                    break;
                case Lead::Synchronize:
                    frames = searchMethodJSynch(hp, atk, def, spa, spd, spe);
                    break;
                case Lead::CuteCharm:
                    frames = searchMethodJCuteCharm(hp, atk, def, spa, spd, spe);
                    break;
                case Lead::Search:
                    frames = searchMethodJSearch(hp, atk, def, spa, spd, spe);
                    break;
                default:
                    break;
            }
            break;
        case Method::MethodK:
            switch (leadType)
            {
                case Lead::None:
                    frames = searchMethodK(hp, atk, def, spa, spd, spe);
                    break;
                case Lead::Synchronize:
                    frames = searchMethodKSynch(hp, atk, def, spa, spd, spe);
                    break;
                case Lead::CuteCharm:
                    frames = searchMethodKCuteCharm(hp, atk, def, spa, spd, spe);
                    break;
                case Lead::SuctionCups:
                    frames = searchMethodKSuctionCups(hp, atk, def, spa, spd, spe);
                    break;
                case Lead::Search:
                    frames = searchMethodKSearch(hp, atk, def, spa, spd, spe);
                    break;
                default:
                    break;
            }
            break;
        case Method::ChainedShiny:
            frames = searchChainedShiny(hp, atk, def, spa, spd, spe);
            break;
        case Method::WondercardIVs:
            frames = searchWondercardIVs(hp, atk, def, spa, spd, spe);
            break;
        default:
            break;
    }
    return searchInitialSeeds(frames);
}

QVector<Frame4> Searcher4::searchMethod1(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe) const
{
    QVector<Frame4> frames;

    Frame4 frame(tid, sid, psv);
    frame.setIVs(hp, atk, def, spa, spd, spe);

    if (!compare.compareHiddenPower(frame))
    {
        return frames;
    }

    u32 first = (hp | (atk << 5) | (def << 10)) << 16;
    u32 second = (spe | (spa << 5) | (spd << 10)) << 16;

    QVector<u32> seeds = cache.recoverLower16BitsIV(first, second);
    for (const auto &seed : seeds)
    {
        // Setup normal frame
        PokeRNGR rng(seed);
        u16 high = rng.nextUShort();
        u16 low = rng.nextUShort();

        frame.setPID(high, low, genderRatio);
        frame.setSeed(rng.nextUInt());
        if (compare.comparePID(frame))
        {
            frames.append(frame);
        }

        // Setup XORed frame
        frame.xorFrame();
        if (compare.comparePID(frame))
        {
            frame.setSeed(frame.getSeed() ^ 0x80000000);
            frames.append(frame);
        }
    }

    return frames;
}

QVector<Frame4> Searcher4::searchMethodJ(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe) const
{
    QVector<Frame4> frames;

    Frame4 frame(tid, sid, psv);
    frame.setIVs(hp, atk, def, spa, spd, spe);

    if (!compare.compareHiddenPower(frame))
    {
        return frames;
    }

    u32 first = (hp | (atk << 5) | (def << 10)) << 16;
    u32 second = (spe | (spa << 5) | (spd << 10)) << 16;

    QVector<u32> seeds = cache.recoverLower16BitsIV(first, second);
    u8 thresh = encounterType == Encounter::OldRod ? 25 : encounterType == Encounter::GoodRod ? 50 : encounterType == Encounter::SuperRod ? 75 : 0;

    for (const auto &val : seeds)
    {
        // Setup normal frame
        PokeRNGR rng(val);
        u16 high = rng.nextUShort();
        u16 low = rng.nextUShort();

        frame.setPID(high, low, genderRatio);
        u32 seed = rng.nextUInt();

        for (const bool &flag : { false, true })
        {
            if (flag)
            {
                frame.xorFrame();
                seed ^= 0x80000000;
            }

            if (!compare.comparePID(frame))
            {
                continue;
            }

            PokeRNGR testRNG(seed);
            u32 testPID, slot, nibble;
            u16 nextRNG = seed >> 16;
            u16 nextRNG2 = testRNG.nextUShort();

            do
            {
                bool skip = false;

                if ((nextRNG / 0xa3e) == frame.getNature())
                {
                    switch (encounterType)
                    {
                        case Encounter::Grass:
                            slot = testRNG.getSeed();
                            frame.setEncounterSlot(EncounterSlot::jSlot(slot >> 16, encounterType));
                            frame.setLevel(encounter.calcLevel(frame.getEncounterSlot()));
                            frame.setSeed(slot * 0xeeb9eb65 + 0xa3561a1);
                            break;
                        case Encounter::Surfing:
                            slot = testRNG.getSeed() * 0xeeb9eb65 + 0xa3561a1;
                            frame.setEncounterSlot(EncounterSlot::jSlot(slot >> 16, encounterType));
                            frame.setLevel(encounter.calcLevel(frame.getEncounterSlot(), testRNG.getSeed() >> 16));
                            frame.setSeed(slot * 0xeeb9eb65 + 0xa3561a1);
                            break;
                        case Encounter::OldRod:
                        case Encounter::GoodRod:
                        case Encounter::SuperRod:
                            slot = testRNG.getSeed() * 0xeeb9eb65 + 0xa3561a1;
                            nibble = slot * 0xeeb9eb65 + 0xa3561a1;
                            if (((nibble >> 16) / 656) < thresh)
                            {
                                frame.setEncounterSlot(EncounterSlot::jSlot(slot >> 16, encounterType));
                                frame.setLevel(encounter.calcLevel(frame.getEncounterSlot(), testRNG.getSeed() >> 16));
                                frame.setSeed(nibble * 0xeeb9eb65 + 0xa3561a1);
                            }
                            else
                            {
                                skip = true;
                            }
                            break;
                        case Encounter::Stationary:
                        default:
                            frame.setSeed(testRNG.getSeed());
                            break;
                    }

                    if (!skip && (encounterType == Encounter::Stationary || compare.compareSlot(frame)))
                    {
                        frames.append(frame);
                    }
                }

                testPID = (nextRNG << 16) | nextRNG2;
                nextRNG = testRNG.nextUShort();
                nextRNG2 = testRNG.nextUShort();
            }
            while (testPID % 25 != frame.getNature());
        }
    }

    return frames;
}

QVector<Frame4> Searcher4::searchMethodJSynch(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe) const
{
    QVector<Frame4> frames;

    Frame4 frame(tid, sid, psv);
    frame.setLeadType(Lead::Synchronize);
    frame.setIVs(hp, atk, def, spa, spd, spe);

    if (!compare.compareHiddenPower(frame))
    {
        return frames;
    }

    u32 first = (hp | (atk << 5) | (def << 10)) << 16;
    u32 second = (spe | (spa << 5) | (spd << 10)) << 16;

    QVector<u32> seeds = cache.recoverLower16BitsIV(first, second);
    u8 thresh = encounterType == Encounter::OldRod ? 25 : encounterType == Encounter::GoodRod ? 50 : encounterType == Encounter::SuperRod ? 75 : 0;

    for (const auto &val : seeds)
    {
        // Setup normal frame
        PokeRNGR rng(val);
        u16 high = rng.nextUShort();
        u16 low = rng.nextUShort();

        frame.setPID(high, low, genderRatio);
        u32 seed = rng.nextUInt();

        for (const bool &flag : { false, true })
        {
            if (flag)
            {
                frame.xorFrame();
                seed ^= 0x80000000;
            }

            if (!compare.comparePID(frame))
            {
                continue;
            }

            PokeRNGR testRNG(seed);
            u32 testPID, slot, nibble;
            u16 nextRNG = seed >> 16;
            u16 nextRNG2 = testRNG.nextUShort();

            do
            {
                bool skip = false;

                // Successful synch
                if ((nextRNG >> 15) == 0)
                {
                    switch (encounterType)
                    {
                        case Encounter::Grass:
                            slot = testRNG.getSeed();
                            frame.setEncounterSlot(EncounterSlot::jSlot(slot >> 16, encounterType));
                            frame.setLevel(encounter.calcLevel(frame.getEncounterSlot()));
                            frame.setSeed(slot * 0xeeb9eb65 + 0xa3561a1);
                            break;
                        case Encounter::Surfing:
                            slot = testRNG.getSeed() * 0xeeb9eb65 + 0xa3561a1;
                            frame.setEncounterSlot(EncounterSlot::jSlot(slot >> 16, encounterType));
                            frame.setLevel(encounter.calcLevel(frame.getEncounterSlot(), testRNG.getSeed() >> 16));
                            frame.setSeed(slot * 0xeeb9eb65 + 0xa3561a1);
                            break;
                        case Encounter::OldRod:
                        case Encounter::GoodRod:
                        case Encounter::SuperRod:
                            slot = testRNG.getSeed() * 0xeeb9eb65 + 0xa3561a1;
                            nibble = slot * 0xeeb9eb65 + 0xa3561a1;
                            if (((nibble >> 16) / 656) < thresh)
                            {
                                frame.setEncounterSlot(EncounterSlot::jSlot(slot >> 16, encounterType));
                                frame.setLevel(encounter.calcLevel(frame.getEncounterSlot(), testRNG.getSeed() >> 16));
                                frame.setSeed(nibble * 0xeeb9eb65 + 0xa3561a1);
                            }
                            else
                            {
                                skip = true;
                            }
                            break;
                        case Encounter::Stationary:
                        default:
                            frame.setSeed(testRNG.getSeed());
                            break;
                    }
                }
                // Failed Synch
                else if ((nextRNG2 >> 15) == 1 && (nextRNG / 0xa3e) == frame.getNature())
                {
                    switch (encounterType)
                    {
                        case Encounter::Grass:
                            slot = testRNG.getSeed() * 0xeeb9eb65 + 0xa3561a1;
                            frame.setEncounterSlot(EncounterSlot::jSlot(slot >> 16, encounterType));
                            frame.setLevel(encounter.calcLevel(frame.getEncounterSlot()));
                            frame.setSeed(slot * 0xeeb9eb65 + 0xa3561a1);
                            break;
                        case Encounter::Surfing:
                            slot = testRNG.getSeed() * 0xdc6c95d9 + 0x4d3cb126;
                            frame.setEncounterSlot(EncounterSlot::jSlot(slot >> 16, encounterType));
                            frame.setLevel(encounter.calcLevel(frame.getEncounterSlot(), testRNG.getSeed() >> 16));
                            frame.setSeed(slot * 0xeeb9eb65 + 0xa3561a1);
                            break;
                        case Encounter::OldRod:
                        case Encounter::GoodRod:
                        case Encounter::SuperRod:
                            slot = testRNG.getSeed() * 0xeeb9eb65 + 0xa3561a1;
                            nibble = slot * 0xeeb9eb65 + 0xa3561a1;
                            if (((nibble >> 16) / 656) < thresh)
                            {
                                frame.setEncounterSlot(EncounterSlot::jSlot(slot >> 16, encounterType));
                                frame.setLevel(encounter.calcLevel(frame.getEncounterSlot(), slot >> 16));
                                frame.setSeed(nibble * 0xeeb9eb65 + 0xa3561a1);
                            }
                            else
                            {
                                skip = true;
                            }
                            break;
                        case Encounter::Stationary:
                        default:
                            frame.setSeed(testRNG.getSeed() * 0xeeb9eb65 + 0xa3561a1);
                            break;
                    }
                }

                if (!skip && (encounterType == Encounter::Stationary || compare.compareSlot(frame)))
                {
                    frames.append(frame);
                }

                testPID = (nextRNG << 16) | nextRNG2;
                nextRNG = testRNG.nextUShort();
                nextRNG2 = testRNG.nextUShort();
            }
            while (testPID % 25 != frame.getNature());
        }
    }

    return frames;
}

QVector<Frame4> Searcher4::searchMethodJCuteCharm(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe) const
{
    QVector<Frame4> frames;

    Frame4 frame(tid, sid, psv);
    frame.setIVs(hp, atk, def, spa, spd, spe);

    if (!compare.compareHiddenPower(frame))
    {
        return frames;
    }

    u32 first = (hp | (atk << 5) | (def << 10)) << 16;
    u32 second = (spe | (spa << 5) | (spd << 10)) << 16;

    QVector<u32> seeds = cache.recoverLower16BitsIV(first, second);
    u8 thresh = encounterType == Encounter::OldRod ? 25 : encounterType == Encounter::GoodRod ? 50 : encounterType == Encounter::SuperRod ? 75 : 0;

    for (const auto &val : seeds)
    {
        // Setup normal frame
        PokeRNGR rng(val);
        u16 high = rng.nextUShort();
        u16 low = rng.nextUShort();
        u32 seed = rng.nextUInt();
        u32 nibble, slot;

        for (const bool &flag : { false, true })
        {
            if (flag)
            {
                high ^= 0x8000;
                low ^= 0x8000;
                seed ^= 0x80000000;
            }

            if ((low / 0x5556) != 0)
            {
                switch (encounterType)
                {
                    case Encounter::Grass:
                        slot = seed;
                        frame.setEncounterSlot(EncounterSlot::jSlot(slot >> 16, encounterType));
                        frame.setLevel(encounter.calcLevel(frame.getEncounterSlot()));
                        frame.setSeed(slot * 0xeeb9eb65 + 0xa3561a1);
                        break;
                    case Encounter::Surfing:
                        slot = seed * 0xeeb9eb65 + 0xa3561a1;
                        frame.setEncounterSlot(EncounterSlot::jSlot(slot >> 16, encounterType));
                        frame.setLevel(encounter.calcLevel(frame.getEncounterSlot(), seed >> 16));
                        frame.setSeed(slot * 0xeeb9eb65 + 0xa3561a1);
                        break;
                    case Encounter::OldRod:
                    case Encounter::GoodRod:
                    case Encounter::SuperRod:
                        slot = seed * 0xeeb9eb65 + 0xa3561a1;
                        nibble = slot * 0xeeb9eb65 + 0xa3561a1;
                        if ((nibble >> 16) / 656 < thresh)
                        {
                            frame.setEncounterSlot(EncounterSlot::jSlot(slot >> 16, encounterType));
                            frame.setLevel(encounter.calcLevel(frame.getEncounterSlot(), seed >> 16));
                            frame.setSeed(nibble * 0xeeb9eb65 + 0xa3561a1);
                        }
                        else
                        {
                            continue;
                        }
                        break;
                    case Encounter::Stationary:
                    default:
                        frame.setSeed(seed);
                        break;
                }

                u8 choppedPID = high / 0xa3e;
                for (const auto &buffer : genderThreshHolds)
                {
                    switch (buffer)
                    {
                        case 0x0:
                            frame.setLeadType(Lead::CuteCharmFemale);
                            break;
                        case 0x96:
                            frame.setLeadType(Lead::CuteCharm50M);
                            break;
                        case 0xC8:
                            frame.setLeadType(Lead::CuteCharm25M);
                            break;
                        case 0x4B:
                            frame.setLeadType(Lead::CuteCharm75M);
                            break;
                        case 0x32:
                            frame.setLeadType(Lead::CuteCharm875M);
                            break;
                        default:
                            frame.setLeadType(Lead::CuteCharm);
                            break;
                    }

                    frame.setPID(choppedPID + buffer, genderRatio);
                    if (!compare.comparePID(frame))
                    {
                        continue;
                    }

                    if (encounterType == Encounter::Stationary || compare.compareSlot(frame))
                    {
                        frames.append(frame);
                    }
                }
            }
        }
    }

    return frames;
}

QVector<Frame4> Searcher4::searchMethodJSearch(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe) const
{
    QVector<Frame4> frames;

    Frame4 frame(tid, sid, psv);
    frame.setIVs(hp, atk, def, spa, spd, spe);

    if (!compare.compareHiddenPower(frame))
    {
        return frames;
    }

    u32 first = (hp | (atk << 5) | (def << 10)) << 16;
    u32 second = (spe | (spa << 5) | (spd << 10)) << 16;

    QVector<u32> seeds = cache.recoverLower16BitsIV(first, second);
    u8 thresh = encounterType == Encounter::OldRod ? 25 : encounterType == Encounter::GoodRod ? 50 : encounterType == Encounter::SuperRod ? 75 : 0;
    u8 adjustedThresh = encounterType == Encounter::OldRod ? 90 : encounterType == Encounter::GoodRod ? 100 : encounterType == Encounter::SuperRod ? 100 : 0;

    for (const auto &val : seeds)
    {
        // Setup normal frame
        PokeRNGR rng(val);
        u16 high = rng.nextUShort();
        u16 low = rng.nextUShort();
        u32 seed = rng.nextUInt();

        for (const bool &flag : { false, true })
        {
            if (flag)
            {
                low ^= 0x8000;
                high ^= 0x8000;
                seed ^= 0x80000000;
            }

            PokeRNGR testRNG(seed);
            u32 testPID, slot = 0, nibble = 0;
            u16 nextRNG = seed >> 16;
            u16 nextRNG2 = testRNG.nextUShort();

            frame.setPID(high, low, genderRatio);
            if (compare.comparePID(frame))
            {
                do
                {
                    bool skip = false;

                    // Normal
                    if ((nextRNG / 0xa3e) == frame.getNature())
                    {
                        frame.setLeadType(Lead::None);
                        switch (encounterType)
                        {
                            case Encounter::Grass:
                                slot = testRNG.getSeed();
                                frame.setEncounterSlot(EncounterSlot::jSlot(slot >> 16, encounterType));
                                frame.setLevel(encounter.calcLevel(frame.getEncounterSlot()));
                                frame.setSeed(slot * 0xeeb9eb65 + 0xa3561a1);
                                break;
                            case Encounter::Surfing:
                                slot = testRNG.getSeed() * 0xeeb9eb65 + 0xa3561a1;
                                frame.setEncounterSlot(EncounterSlot::jSlot(slot >> 16, encounterType));
                                frame.setLevel(encounter.calcLevel(frame.getEncounterSlot(), testRNG.getSeed() >> 16));
                                frame.setSeed(slot * 0xeeb9eb65 + 0xa3561a1);
                                break;
                            case Encounter::OldRod:
                            case Encounter::GoodRod:
                            case Encounter::SuperRod:
                                slot = testRNG.getSeed();
                                nibble = slot * 0xeeb9eb65 + 0xa3561a1;
                                if (((nibble >> 16) / 656) < adjustedThresh)
                                {
                                    if (((nibble >> 16) / 656) >= thresh)
                                        frame.setLeadType(Lead::SuctionCups);
                                    frame.setEncounterSlot(EncounterSlot::jSlot(slot >> 16, encounterType));
                                    frame.setLevel(encounter.calcLevel(frame.getEncounterSlot(), slot >> 16));
                                    frame.setSeed(nibble * 0xeeb9eb65 + 0xa3561a1);
                                }
                                else
                                {
                                    skip = true;
                                }
                                break;
                            case Stationary:
                            default:
                                frame.setSeed(testRNG.getSeed());
                                break;
                        }

                        if (!skip && (encounterType == Encounter::Stationary || compare.compareSlot(frame)))
                        {
                            frames.append(frame);
                        }

                        // Failed synch
                        if ((nextRNG2 >> 15) == 1)
                        {
                            frame.setLeadType(Lead::Synchronize);
                            u32 level;
                            switch (encounterType)
                            {
                                case Encounter::Grass:
                                    slot = frame.getSeed();
                                    frame.setEncounterSlot(EncounterSlot::jSlot(slot >> 16, encounterType));
                                    frame.setLevel(encounter.calcLevel(frame.getEncounterSlot()));
                                    frame.setSeed(slot * 0xeeb9eb65 + 0xa3561a1);
                                    break;
                                case Encounter::Surfing:
                                    level = slot;
                                    slot = frame.getSeed();
                                    frame.setEncounterSlot(EncounterSlot::jSlot(slot >> 16, encounterType));
                                    frame.setLevel(encounter.calcLevel(frame.getEncounterSlot(), level >> 16));
                                    frame.setSeed(slot * 0xeeb9eb65 + 0xa3561a1);
                                    break;
                                case Encounter::OldRod:
                                case Encounter::GoodRod:
                                case Encounter::SuperRod:
                                    slot = nibble;
                                    nibble = frame.getSeed();
                                    if (((nibble >> 16) / 656) < thresh)
                                    {
                                        frame.setEncounterSlot(EncounterSlot::jSlot(slot >> 16, encounterType));
                                        frame.setLevel(encounter.calcLevel(frame.getEncounterSlot(), slot >> 16));
                                        frame.setSeed(nibble * 0xeeb9eb65 + 0xa3561a1);
                                    }
                                    else
                                    {
                                        skip = true;
                                    }
                                    break;
                                case Encounter::Stationary:
                                default:
                                    frame.setSeed(testRNG.getSeed() * 0xeeb9eb65 + 0xa3561a1);
                                    break;
                            }

                            if (!skip && (encounterType == Encounter::Stationary || compare.compareSlot(frame)))
                            {
                                frames.append(frame);
                            }
                        }
                    }
                    // Successful synch
                    else if ((nextRNG >> 15) == 0)
                    {
                        frame.setLeadType(Lead::Synchronize);
                        switch (encounterType)
                        {
                            case Encounter::Grass:
                                slot = testRNG.getSeed();
                                frame.setEncounterSlot(EncounterSlot::jSlot(slot >> 16, encounterType));
                                frame.setLevel(encounter.calcLevel(frame.getEncounterSlot()));
                                frame.setSeed(slot * 0xeeb9eb65 + 0xa3561a1);
                                break;
                            case Encounter::Surfing:
                                slot = testRNG.getSeed() * 0xeeb9eb65 + 0xa3561a1;
                                frame.setEncounterSlot(EncounterSlot::jSlot(slot >> 16, encounterType));
                                frame.setLevel(encounter.calcLevel(frame.getEncounterSlot(), testRNG.getSeed() >> 16));
                                frame.setSeed(slot * 0xeeb9eb65 + 0xa3561a1);
                                break;
                            case Encounter::OldRod:
                            case Encounter::GoodRod:
                            case Encounter::SuperRod:
                                slot = testRNG.getSeed();
                                nibble = slot * 0xeeb9eb65 + 0xa3561a1;
                                if (((nibble >> 16) / 656) < thresh)
                                {
                                    frame.setEncounterSlot(EncounterSlot::jSlot(slot >> 16, encounterType));
                                    frame.setLevel(encounter.calcLevel(frame.getEncounterSlot(), slot >> 16));
                                    frame.setSeed(nibble * 0xeeb9eb65 + 0xa3561a1);
                                }
                                else
                                {
                                    skip = true;
                                }
                                break;
                            case Encounter::Stationary:
                            default:
                                frame.setSeed(testRNG.getSeed());
                                break;
                        }

                        if (!skip && (encounterType == Encounter::Stationary || compare.compareSlot(frame)))
                        {
                            frames.append(frame);
                        }
                    }

                    testPID = (nextRNG << 16) | nextRNG2;
                    nextRNG = testRNG.nextUShort();
                    nextRNG2 = testRNG.nextUShort();
                }
                while (testPID % 25 != frame.getNature());
            }

            if (low / 0x5556 != 0)
            {
                u32 slot = 0;
                bool skipFrame = false;

                switch (encounterType)
                {
                    case Encounter::Grass:
                        slot = seed;
                        frame.setEncounterSlot(EncounterSlot::jSlot(slot >> 16, encounterType));
                        frame.setLevel(encounter.calcLevel(frame.getEncounterSlot()));
                        frame.setSeed(slot * 0xeeb9eb65 + 0xa3561a1);
                        break;
                    case Encounter::Surfing:
                        slot = seed * 0xeeb9eb65 + 0xa3561a1;
                        frame.setEncounterSlot(EncounterSlot::jSlot(slot >> 16, encounterType));
                        frame.setLevel(encounter.calcLevel(frame.getEncounterSlot(), seed >> 16));
                        frame.setSeed(slot * 0xeeb9eb65 + 0xa3561a1);
                        break;
                    case Encounter::OldRod:
                    case Encounter::GoodRod:
                    case Encounter::SuperRod:
                        slot = seed;
                        nibble = slot * 0xeeb9eb65 + 0xa3561a1;
                        if ((nibble >> 16) / 656 < thresh)
                        {
                            frame.setEncounterSlot(EncounterSlot::jSlot(slot >> 16, encounterType));
                            frame.setLevel(encounter.calcLevel(frame.getEncounterSlot(), slot >> 16));
                            frame.setSeed(nibble * 0xeeb9eb65 + 0xa3561a1);
                        }
                        else
                        {
                            continue;
                        }
                        break;
                    case Encounter::Stationary:
                    default:
                        frame.setSeed(seed);
                        break;
                }

                u8 choppedPID = high / 0xA3E;
                if (!skipFrame)
                {
                    for (const auto &buffer : genderThreshHolds)
                    {
                        switch (buffer)
                        {
                            case 0x0:
                                frame.setLeadType(Lead::CuteCharmFemale);
                                break;
                            case 0x96:
                                frame.setLeadType(Lead::CuteCharm50M);
                                break;
                            case 0xC8:
                                frame.setLeadType(Lead::CuteCharm25M);
                                break;
                            case 0x4B:
                                frame.setLeadType(Lead::CuteCharm75M);
                                break;
                            case 0x32:
                                frame.setLeadType(Lead::CuteCharm875M);
                                break;
                            default:
                                frame.setLeadType(Lead::CuteCharm);
                                break;
                        }

                        frame.setPID(buffer + choppedPID, genderRatio);
                        if (!compare.comparePID(frame))
                        {
                            continue;
                        }

                        frame.setEncounterSlot(EncounterSlot::jSlot(slot >> 16, encounterType));
                        if (encounterType == Encounter::Stationary || compare.compareSlot(frame))
                        {
                            frames.append(frame);
                        }
                    }
                }
            }
        }
    }

    return frames;
}

QVector<Frame4> Searcher4::searchMethodK(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe) const
{
    QVector<Frame4> frames;

    Frame4 frame(tid, sid, psv);
    frame.setIVs(hp, atk, def, spa, spd, spe);

    if (!compare.compareHiddenPower(frame))
    {
        return frames;
    }

    u32 first = (hp | (atk << 5) | (def << 10)) << 16;
    u32 second = (spe | (spa << 5) | (spd << 10)) << 16;

    QVector<u32> seeds = cache.recoverLower16BitsIV(first, second);
    u8 thresh = encounterType == Encounter::OldRod ? 25 : encounterType == Encounter::GoodRod ? 50 : encounterType == Encounter::SuperRod ? 75 : 0;
    u8 rock = encounter.getEncounterRate();

    for (const auto &val : seeds)
    {
        // Setup normal frame
        PokeRNGR rng(val);
        u16 high = rng.nextUShort();
        u16 low = rng.nextUShort();

        frame.setPID(high, low, genderRatio);
        u32 seed = rng.nextUInt();

        for (const bool &flag : { false, true })
        {
            if (flag)
            {
                frame.xorFrame();
                seed ^= 0x80000000;
            }

            if (!compare.comparePID(frame))
            {
                continue;
            }

            PokeRNGR testRNG(seed);
            u32 testPID, slot, nibble;
            u16 nextRNG = seed >> 16;
            u16 nextRNG2 = testRNG.nextUShort();

            do
            {
                bool skip = false;

                if ((nextRNG % 25) == frame.getNature())
                {
                    switch (encounterType)
                    {
                        case Encounter::Grass:
                            slot = testRNG.getSeed();
                            frame.setEncounterSlot(EncounterSlot::kSlot(slot >> 16, encounterType));
                            frame.setLevel(encounter.calcLevel(frame.getEncounterSlot()));
                            frame.setSeed(slot * 0xeeb9eb65 + 0xa3561a1);
                            break;
                        case Encounter::Surfing:
                            slot = testRNG.getSeed() * 0xeeb9eb65 + 0xa3561a1;
                            frame.setEncounterSlot(EncounterSlot::kSlot(slot >> 16, encounterType));
                            frame.setLevel(encounter.calcLevel(frame.getEncounterSlot(), testRNG.getSeed() >> 16));
                            frame.setSeed(slot * 0xeeb9eb65 + 0xa3561a1);
                            break;
                        case Encounter::OldRod:
                        case Encounter::GoodRod:
                        case Encounter::SuperRod:
                            slot = testRNG.getSeed() * 0xeeb9eb65 + 0xa3561a1;
                            nibble = slot * 0xeeb9eb65 + 0xa3561a1;
                            if (((nibble >> 16) % 100) < thresh)
                            {
                                frame.setEncounterSlot(EncounterSlot::kSlot(slot >> 16, encounterType));
                                frame.setLevel(encounter.calcLevel(frame.getEncounterSlot()));
                                frame.setSeed(nibble * 0xeeb9eb65 + 0xa3561a1);
                            }
                            else
                            {
                                skip = true;
                            }
                            break;
                        case Encounter::RockSmash:
                            slot = testRNG.getSeed() * 0xeeb9eb65 + 0xa3561a1;
                            nibble = slot * 0xeeb9eb65 + 0xa3561a1;
                            if (((nibble >> 16) % 100) < rock)
                            {
                                frame.setEncounterSlot(EncounterSlot::kSlot(slot >> 16, encounterType));
                                frame.setLevel(encounter.calcLevel(frame.getEncounterSlot(), testRNG.getSeed() >> 16));
                                nibble = nibble * 0xeeb9eb65 + 0xa3561a1; // Blank(or maybe item)
                                frame.setSeed(nibble * 0xeeb9eb65 + 0xa3561a1);
                            }
                            else
                            {
                                skip = true;
                            }
                            break;
                        case Encounter::Stationary:
                        default:
                            frame.setSeed(testRNG.getSeed());
                            break;
                    }

                    if (!skip && (encounterType == Encounter::Stationary || compare.compareSlot(frame)))
                    {
                        frames.append(frame);
                    }
                }

                testPID = (nextRNG << 16) | nextRNG2;
                nextRNG = testRNG.nextUShort();
                nextRNG2 = testRNG.nextUShort();
            }
            while (testPID % 25 != frame.getNature());
        }
    }

    return frames;
}

QVector<Frame4> Searcher4::searchMethodKSynch(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe) const
{
    QVector<Frame4> frames;

    Frame4 frame(tid, sid, psv);
    frame.setLeadType(Lead::Synchronize);
    frame.setIVs(hp, atk, def, spa, spd, spe);

    if (!compare.compareHiddenPower(frame))
    {
        return frames;
    }

    u32 first = (hp | (atk << 5) | (def << 10)) << 16;
    u32 second = (spe | (spa << 5) | (spd << 10)) << 16;

    QVector<u32> seeds = cache.recoverLower16BitsIV(first, second);
    u8 thresh = encounterType == Encounter::OldRod ? 25 : encounterType == Encounter::GoodRod ? 50 : encounterType == Encounter::SuperRod ? 75 : 0;
    u8 rock = encounter.getEncounterRate();

    for (const auto &val : seeds)
    {
        // Setup normal frame
        PokeRNGR rng(val);
        u16 high = rng.nextUShort();
        u16 low = rng.nextUShort();

        frame.setPID(high, low, genderRatio);
        u32 seed = rng.nextUInt();

        for (const bool &flag : { false, true })
        {
            if (flag)
            {
                frame.xorFrame();
                seed ^= 0x80000000;
            }

            if (!compare.comparePID(frame))
            {
                continue;
            }

            PokeRNGR testRNG(seed);
            u32 testPID, slot, nibble;
            u16 nextRNG = seed >> 16;
            u16 nextRNG2 = testRNG.nextUShort();

            do
            {
                bool skip = false;

                // Successful synch
                if ((nextRNG & 1) == 0)
                {
                    switch (encounterType)
                    {
                        case Encounter::Grass:
                            slot = testRNG.getSeed();
                            frame.setEncounterSlot(EncounterSlot::kSlot(slot >> 16, encounterType));
                            frame.setLevel(encounter.calcLevel(frame.getEncounterSlot()));
                            frame.setSeed(slot * 0xeeb9eb65 + 0xa3561a1);
                            break;
                        case Encounter::Surfing:
                            slot = testRNG.getSeed() * 0xeeb9eb65 + 0xa3561a1;
                            frame.setEncounterSlot(EncounterSlot::kSlot(slot >> 16, encounterType));
                            frame.setLevel(encounter.calcLevel(frame.getEncounterSlot(), testRNG.getSeed() >> 16));
                            frame.setSeed(slot * 0xeeb9eb65 + 0xa3561a1);
                            break;
                        case Encounter::OldRod:
                        case Encounter::GoodRod:
                        case Encounter::SuperRod:
                            slot = testRNG.getSeed() * 0xeeb9eb65 + 0xa3561a1;;
                            nibble = slot * 0xeeb9eb65 + 0xa3561a1;
                            if (((nibble >> 16) % 100) < thresh)
                            {
                                frame.setEncounterSlot(EncounterSlot::kSlot(slot >> 16, encounterType));
                                frame.setLevel(encounter.calcLevel(frame.getEncounterSlot()));
                                frame.setSeed(nibble * 0xeeb9eb65 + 0xa3561a1);
                            }
                            else
                            {
                                skip = true;
                            }
                            break;
                        case Encounter::RockSmash:
                            slot = testRNG.getSeed() * 0xeeb9eb65 + 0xa3561a1;
                            nibble = slot * 0xeeb9eb65 + 0xa3561a1;
                            if (((nibble >> 16) % 100) < rock)
                            {
                                frame.setEncounterSlot(EncounterSlot::kSlot(slot >> 16, encounterType));
                                frame.setLevel(encounter.calcLevel(frame.getEncounterSlot(), testRNG.getSeed() >> 16));
                                nibble = nibble * 0xeeb9eb65 + 0xa3561a1; // Blank(or maybe item)
                                frame.setSeed(nibble * 0xeeb9eb65 + 0xa3561a1);
                            }
                            else
                            {
                                skip = true;
                            }
                            break;
                        case Encounter::Stationary:
                        default:
                            frame.setSeed(testRNG.getSeed());
                            break;
                    }

                    if (!skip && (encounterType == Encounter::Stationary || compare.compareSlot(frame)))
                    {
                        frames.append(frame);
                    }
                }
                // Failed Synch
                else if ((nextRNG2 & 1) == 1 && (nextRNG % 25) == frame.getNature())
                {
                    switch (encounterType)
                    {
                        case Encounter::Grass:
                            slot = testRNG.getSeed() * 0xeeb9eb65 + 0xa3561a1;
                            frame.setEncounterSlot(EncounterSlot::kSlot(slot >> 16, encounterType));
                            frame.setLevel(encounter.calcLevel(frame.getEncounterSlot()));
                            frame.setSeed(slot * 0xeeb9eb65 + 0xa3561a1);
                            break;
                        case Encounter::Surfing:
                            slot = testRNG.getSeed() * 0xdc6c95d9 + 0x4d3cb126;
                            frame.setEncounterSlot(EncounterSlot::kSlot(slot >> 16, encounterType));
                            frame.setLevel(encounter.calcLevel(frame.getEncounterSlot(), testRNG.getSeed() >> 16));
                            frame.setSeed(slot * 0xeeb9eb65 + 0xa3561a1);
                            break;
                        case Encounter::OldRod:
                        case Encounter::GoodRod:
                        case Encounter::SuperRod:
                            slot = testRNG.getSeed() * 0xdc6c95d9 + 0x4d3cb126;
                            nibble = slot * 0xeeb9eb65 + 0xa3561a1;
                            if (((nibble >> 16) % 100) < thresh)
                            {
                                frame.setEncounterSlot(EncounterSlot::kSlot(slot >> 16, encounterType));
                                frame.setLevel(encounter.calcLevel(frame.getEncounterSlot()));
                                frame.setSeed(nibble * 0xeeb9eb65 + 0xa3561a1);
                            }
                            else
                            {
                                skip = true;
                            }
                            break;
                        case Encounter::RockSmash:
                            slot = testRNG.getSeed() * 0xdc6c95d9 + 0x4d3cb126;
                            nibble = slot * 0xeeb9eb65 + 0xa3561a1;
                            if (((nibble >> 16) % 100) < rock)
                            {
                                frame.setEncounterSlot(EncounterSlot::kSlot(slot >> 16, encounterType));
                                frame.setLevel(encounter.calcLevel(frame.getEncounterSlot(), testRNG.getSeed() >> 16));
                                nibble = nibble * 0xeeb9eb65 + 0xa3561a1; // Blank(or maybe item)
                                frame.setSeed(nibble * 0xeeb9eb65 + 0xa3561a1);
                            }
                            else
                            {
                                skip = true;
                            }
                            break;
                        case Encounter::Stationary:
                        default:
                            frame.setSeed(testRNG.getSeed() * 0xeeb9eb65 + 0xa3561a1);
                            break;
                    }

                    if (!skip && (encounterType == Encounter::Stationary || compare.compareSlot(frame)))
                    {
                        frames.append(frame);
                    }
                }

                testPID = (nextRNG << 16) | nextRNG2;
                nextRNG = testRNG.nextUShort();
                nextRNG2 = testRNG.nextUShort();
            }
            while (testPID % 25 != frame.getNature());
        }
    }

    return frames;
}

QVector<Frame4> Searcher4::searchMethodKCuteCharm(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe) const
{
    QVector<Frame4> frames;

    Frame4 frame(tid, sid, psv);
    frame.setIVs(hp, atk, def, spa, spd, spe);

    if (!compare.compareHiddenPower(frame))
    {
        return frames;
    }

    u32 first = (hp | (atk << 5) | (def << 10)) << 16;
    u32 second = (spe | (spa << 5) | (spd << 10)) << 16;

    QVector<u32> seeds = cache.recoverLower16BitsIV(first, second);
    u8 thresh = encounterType == Encounter::OldRod ? 25 : encounterType == Encounter::GoodRod ? 50 : encounterType == Encounter::SuperRod ? 75 : 0;
    u8 rock = encounter.getEncounterRate();

    for (const auto &val : seeds)
    {
        // Setup normal frame
        PokeRNGR rng(val);
        u16 high = rng.nextUShort();
        u16 low = rng.nextUShort();
        u32 seed = rng.nextUInt();
        u32 nibble, slot;

        for (const bool &flag : { false, true })
        {
            if (flag)
            {
                high ^= 0x8000;
                low ^= 0x8000;
                seed ^= 0x80000000;
            }

            if ((low % 3) != 0)
            {
                switch (encounterType)
                {
                    case Encounter::Grass:
                        slot = seed;
                        frame.setEncounterSlot(EncounterSlot::kSlot(slot >> 16, encounterType));
                        frame.setLevel(encounter.calcLevel(frame.getEncounterSlot()));
                        frame.setSeed(slot * 0xeeb9eb65 + 0xa3561a1);
                        break;
                    case Encounter::Surfing:
                        slot = seed * 0xeeb9eb65 + 0xa3561a1;
                        frame.setEncounterSlot(EncounterSlot::kSlot(slot >> 16, encounterType));
                        frame.setLevel(encounter.calcLevel(frame.getEncounterSlot(), seed >> 16));
                        frame.setSeed(slot * 0xeeb9eb65 + 0xa3561a1);
                        break;
                    case Encounter::OldRod:
                    case Encounter::GoodRod:
                    case Encounter::SuperRod:
                        slot = seed * 0xeeb9eb65 + 0xa3561a1;;
                        nibble = slot * 0xeeb9eb65 + 0xa3561a1;
                        if ((nibble >> 16) % 100 < thresh)
                        {
                            frame.setEncounterSlot(EncounterSlot::kSlot(slot >> 16, encounterType));
                            frame.setLevel(encounter.calcLevel(frame.getEncounterSlot()));
                            frame.setSeed(nibble * 0xeeb9eb65 + 0xa3561a1);
                        }
                        else
                        {
                            continue;
                        }
                        break;
                    case Encounter::RockSmash:
                        slot = seed * 0xeeb9eb65 + 0xa3561a1;
                        nibble = slot * 0xeeb9eb65 + 0xa3561a1;
                        if (((nibble >> 16) % 100) < rock)
                        {
                            frame.setEncounterSlot(EncounterSlot::kSlot(slot >> 16, encounterType));
                            frame.setLevel(encounter.calcLevel(frame.getEncounterSlot(), seed >> 16));
                            nibble = nibble * 0xeeb9eb65 + 0xa3561a1; // Blank(or maybe item)
                            frame.setSeed(nibble * 0xeeb9eb65 + 0xa3561a1);
                        }
                        else
                        {
                            continue;
                        }
                        break;
                    case Encounter::Stationary:
                    default:
                        frame.setSeed(seed);
                        break;
                }

                u8 choppedPID = high % 25;
                for (const auto &buffer : genderThreshHolds)
                {
                    switch (buffer)
                    {
                        case 0x0:
                            frame.setLeadType(Lead::CuteCharmFemale);
                            break;
                        case 0x96:
                            frame.setLeadType(Lead::CuteCharm50M);
                            break;
                        case 0xC8:
                            frame.setLeadType(Lead::CuteCharm25M);
                            break;
                        case 0x4B:
                            frame.setLeadType(Lead::CuteCharm75M);
                            break;
                        case 0x32:
                            frame.setLeadType(Lead::CuteCharm875M);
                            break;
                        default:
                            frame.setLeadType(Lead::CuteCharm);
                            break;
                    }

                    frame.setPID(choppedPID + buffer, genderRatio);
                    if (!compare.comparePID(frame))
                    {
                        continue;
                    }

                    if (encounterType == Encounter::Stationary || compare.compareSlot(frame))
                    {
                        frames.append(frame);
                    }
                }
            }
        }
    }

    return frames;
}

QVector<Frame4> Searcher4::searchMethodKSuctionCups(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe) const
{
    QVector<Frame4> frames;

    Frame4 frame(tid, sid, psv);
    frame.setIVs(hp, atk, def, spa, spd, spe);

    if (!compare.compareHiddenPower(frame))
    {
        return frames;
    }

    u32 first = (hp | (atk << 5) | (def << 10)) << 16;
    u32 second = (spe | (spa << 5) | (spd << 10)) << 16;

    QVector<u32> seeds = cache.recoverLower16BitsIV(first, second);
    u8 thresh = encounterType == Encounter::OldRod ? 25 : encounterType == Encounter::GoodRod ? 50 : encounterType == Encounter::SuperRod ? 75 : 0;
    u8 adjustedThresh = encounterType == Encounter::OldRod ? 90 : encounterType == Encounter::GoodRod ? 100 : encounterType == Encounter::SuperRod ? 100 : 0;
    u8 rock = encounter.getEncounterRate();

    for (const auto &val : seeds)
    {
        // Setup normal frame
        PokeRNGR rng(val);
        u16 high = rng.nextUShort();
        u16 low = rng.nextUShort();

        frame.setPID(high, low, genderRatio);
        u32 seed = rng.nextUInt();

        for (const bool &flag : { false, true })
        {
            if (flag)
            {
                frame.xorFrame();
                seed ^= 0x80000000;
            }

            if (!compare.comparePID(frame))
            {
                continue;
            }

            PokeRNGR testRNG(seed);
            u32 testPID, slot, nibble;
            u16 nextRNG = seed >> 16;
            u16 nextRNG2 = testRNG.nextUShort();

            do
            {
                bool skip = false;

                if ((nextRNG % 25) == frame.getNature())
                {
                    frame.setLeadType(Lead::None);
                    switch (encounterType)
                    {
                        case Encounter::Grass:
                            slot = testRNG.getSeed();
                            frame.setEncounterSlot(EncounterSlot::kSlot(slot >> 16, encounterType));
                            frame.setLevel(encounter.calcLevel(frame.getEncounterSlot()));
                            frame.setSeed(slot * 0xeeb9eb65 + 0xa3561a1);
                            break;
                        case Encounter::Surfing:
                            slot = testRNG.getSeed() * 0xeeb9eb65 + 0xa3561a1;
                            frame.setEncounterSlot(EncounterSlot::kSlot(slot >> 16, encounterType));
                            frame.setLevel(encounter.calcLevel(frame.getEncounterSlot(), testRNG.getSeed() >> 16));
                            frame.setSeed(slot * 0xeeb9eb65 + 0xa3561a1);
                            break;
                        case Encounter::OldRod:
                        case Encounter::GoodRod:
                        case Encounter::SuperRod:
                            slot = testRNG.getSeed() * 0xeeb9eb65 + 0xa3561a1;;
                            nibble = slot * 0xeeb9eb65 + 0xa3561a1;
                            if (((nibble >> 16) % 100) < adjustedThresh)
                            {
                                if (((nibble >> 16) % 100) >= thresh)
                                {
                                    frame.setLeadType(Lead::SuctionCups);
                                }
                                frame.setEncounterSlot(EncounterSlot::kSlot(slot >> 16, encounterType));
                                frame.setLevel(encounter.calcLevel(frame.getEncounterSlot()));
                                frame.setSeed(nibble * 0xeeb9eb65 + 0xa3561a1);
                            }
                            else
                            {
                                skip = true;
                            }
                            break;
                        case Encounter::RockSmash:
                            slot = seed * 0xeeb9eb65 + 0xa3561a1;
                            nibble = slot * 0xeeb9eb65 + 0xa3561a1;
                            if (((nibble >> 16) % 100) < rock)
                            {
                                frame.setEncounterSlot(EncounterSlot::kSlot(slot >> 16, encounterType));
                                frame.setLevel(encounter.calcLevel(frame.getEncounterSlot(), seed >> 16));
                                nibble = nibble * 0xeeb9eb65 + 0xa3561a1; // Blank(or maybe item)
                                frame.setSeed(nibble * 0xeeb9eb65 + 0xa3561a1);
                            }
                            else
                            {
                                skip = true;
                            }
                            break;
                        case Encounter::Stationary:
                        default:
                            frame.setSeed(testRNG.getSeed());
                            break;
                    }

                    if (!skip && (encounterType == Encounter::Stationary || compare.compareSlot(frame)))
                    {
                        frames.append(frame);
                    }
                }

                testPID = (nextRNG << 16) | nextRNG2;
                nextRNG = testRNG.nextUShort();
                nextRNG2 = testRNG.nextUShort();
            }
            while (testPID % 25 != frame.getNature());
        }
    }

    return frames;
}

QVector<Frame4> Searcher4::searchMethodKSearch(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe) const
{
    QVector<Frame4> frames;

    Frame4 frame(tid, sid, psv);
    frame.setIVs(hp, atk, def, spa, spd, spe);

    if (!compare.compareHiddenPower(frame))
    {
        return frames;
    }

    u32 first = (hp | (atk << 5) | (def << 10)) << 16;
    u32 second = (spe | (spa << 5) | (spd << 10)) << 16;

    QVector<u32> seeds = cache.recoverLower16BitsIV(first, second);
    u8 thresh = encounterType == Encounter::OldRod ? 25 : encounterType == Encounter::GoodRod ? 50 : encounterType == Encounter::SuperRod ? 75 : 0;
    u8 adjustedThresh = encounterType == Encounter::OldRod ? 90 : encounterType == Encounter::GoodRod ? 100 : encounterType == Encounter::SuperRod ? 100 : 0;
    u8 rock = encounter.getEncounterRate();

    for (const auto &val : seeds)
    {
        // Setup normal frame
        PokeRNGR rng(val);
        u16 high = rng.nextUShort();
        u16 low = rng.nextUShort();
        u32 seed = rng.nextUInt();

        for (const bool &flag : { false, true })
        {
            if (flag)
            {
                low ^= 0x8000;
                high ^= 0x8000;
                seed ^= 0x80000000;
            }

            PokeRNGR testRNG(seed);
            u32 testPID, slot = 0, nibble = 0;
            u16 nextRNG = seed >> 16;
            u16 nextRNG2 = testRNG.nextUShort();

            frame.setPID(high, low, genderRatio);
            if (compare.comparePID(frame))
            {
                do
                {
                    bool skip = false;

                    // Normal
                    if ((nextRNG % 25) == frame.getNature())
                    {
                        frame.setLeadType(Lead::None);
                        switch (encounterType)
                        {
                            case Encounter::Grass:
                                slot = testRNG.getSeed();
                                frame.setEncounterSlot(EncounterSlot::kSlot(slot >> 16, encounterType));
                                frame.setLevel(encounter.calcLevel(frame.getEncounterSlot()));
                                frame.setSeed(slot * 0xeeb9eb65 + 0xa3561a1);
                                break;
                            case Encounter::Surfing:
                                slot = testRNG.getSeed() * 0xeeb9eb65 + 0xa3561a1;
                                frame.setEncounterSlot(EncounterSlot::kSlot(slot >> 16, encounterType));
                                frame.setLevel(encounter.calcLevel(frame.getEncounterSlot(), testRNG.getSeed() >> 16));
                                frame.setSeed(slot * 0xeeb9eb65 + 0xa3561a1);
                                break;
                            case Encounter::OldRod:
                            case Encounter::GoodRod:
                            case Encounter::SuperRod:
                                slot = testRNG.getSeed() * 0xeeb9eb65 + 0xa3561a1;;
                                nibble = slot * 0xeeb9eb65 + 0xa3561a1;
                                if (((nibble >> 16) % 100) < adjustedThresh)
                                {
                                    if (((nibble >> 16) % 100) >= thresh)
                                    {
                                        frame.setLeadType(Lead::SuctionCups);
                                    }
                                    frame.setEncounterSlot(EncounterSlot::kSlot(slot >> 16, encounterType));
                                    frame.setLevel(encounter.calcLevel(frame.getEncounterSlot()));
                                    frame.setSeed(nibble * 0xeeb9eb65 + 0xa3561a1);
                                }
                                else
                                {
                                    skip = true;
                                }
                                break;
                            case Encounter::RockSmash:
                                slot = seed * 0xeeb9eb65 + 0xa3561a1;
                                nibble = slot * 0xeeb9eb65 + 0xa3561a1;
                                if (((nibble >> 16) % 100) < rock)
                                {
                                    frame.setEncounterSlot(EncounterSlot::kSlot(slot >> 16, encounterType));
                                    frame.setLevel(encounter.calcLevel(frame.getEncounterSlot(), seed >> 16));
                                    nibble = nibble * 0xeeb9eb65 + 0xa3561a1; // Blank(or maybe item)
                                    frame.setSeed(nibble * 0xeeb9eb65 + 0xa3561a1);
                                }
                                else
                                {
                                    skip = true;
                                }
                                break;
                            case Encounter::Stationary:
                            default:
                                frame.setSeed(testRNG.getSeed());
                                break;
                        }

                        if (!skip && (encounterType == Encounter::Stationary || compare.compareSlot(frame)))
                        {
                            frames.append(frame);
                        }

                        // Failed synch
                        if ((nextRNG2 & 1) == 1)
                        {
                            frame.setLeadType(Lead::Synchronize);
                            u32 level;
                            switch (encounterType)
                            {
                                case Encounter::Grass:
                                    slot = frame.getSeed();
                                    frame.setEncounterSlot(EncounterSlot::kSlot(slot >> 16, encounterType));
                                    frame.setLevel(encounter.calcLevel(frame.getEncounterSlot()));
                                    frame.setSeed(slot * 0xeeb9eb65 + 0xa3561a1);
                                    break;
                                case Encounter::Surfing:
                                    level = slot;
                                    slot = frame.getSeed();
                                    frame.setEncounterSlot(EncounterSlot::kSlot(slot >> 16, encounterType));
                                    frame.setLevel(encounter.calcLevel(frame.getEncounterSlot(), level >> 16));
                                    frame.setSeed(slot * 0xeeb9eb65 + 0xa3561a1);
                                    break;
                                case Encounter::OldRod:
                                case Encounter::GoodRod:
                                case Encounter::SuperRod:
                                    slot = nibble;
                                    nibble = frame.getSeed();
                                    if (((nibble >> 16) / 656) < thresh)
                                    {
                                        frame.setEncounterSlot(EncounterSlot::kSlot(slot >> 16, encounterType));
                                        frame.setLevel(encounter.calcLevel(frame.getEncounterSlot()));
                                        frame.setSeed(nibble * 0xeeb9eb65 + 0xa3561a1);
                                    }
                                    else
                                    {
                                        skip = true;
                                    }
                                    break;
                                case Encounter::RockSmash:
                                    slot = nibble;
                                    nibble = frame.getSeed();
                                    if (((nibble >> 16) % 100) < rock)
                                    {
                                        frame.setEncounterSlot(EncounterSlot::kSlot(slot >> 16, encounterType));
                                        frame.setLevel(encounter.calcLevel(frame.getEncounterSlot(), seed >> 16));
                                        nibble = nibble * 0xeeb9eb65 + 0xa3561a1; // Blank(or maybe item)
                                        frame.setSeed(nibble * 0xeeb9eb65 + 0xa3561a1);
                                    }
                                    else
                                    {
                                        skip = true;
                                    }
                                    break;
                                case Encounter::Stationary:
                                default:
                                    frame.setSeed(testRNG.getSeed() * 0xeeb9eb65 + 0xa3561a1);
                                    break;
                            }

                            if (!skip && (encounterType == Encounter::Stationary || compare.compareSlot(frame)))
                            {
                                frames.append(frame);
                            }
                        }
                    }
                    // Successful synch
                    else if ((nextRNG & 1) == 0)
                    {
                        frame.setLeadType(Lead::Synchronize);
                        switch (encounterType)
                        {
                            case Encounter::Grass:
                                slot = testRNG.getSeed();
                                frame.setEncounterSlot(EncounterSlot::kSlot(slot >> 16, encounterType));
                                frame.setLevel(encounter.calcLevel(frame.getEncounterSlot()));
                                frame.setSeed(slot * 0xeeb9eb65 + 0xa3561a1);
                                break;
                            case Encounter::Surfing:
                                slot = testRNG.getSeed() * 0xeeb9eb65 + 0xa3561a1;
                                frame.setEncounterSlot(EncounterSlot::kSlot(slot >> 16, encounterType));
                                frame.setLevel(encounter.calcLevel(frame.getEncounterSlot(), testRNG.getSeed() >> 16));
                                frame.setSeed(slot * 0xeeb9eb65 + 0xa3561a1);
                                break;
                            case Encounter::OldRod:
                            case Encounter::GoodRod:
                            case Encounter::SuperRod:
                                slot = testRNG.getSeed() * 0xeeb9eb65 + 0xa3561a1;;
                                nibble = slot * 0xeeb9eb65 + 0xa3561a1;
                                if (((nibble >> 16) % 100) < thresh)
                                {
                                    frame.setEncounterSlot(EncounterSlot::kSlot(slot >> 16, encounterType));
                                    frame.setLevel(encounter.calcLevel(frame.getEncounterSlot()));
                                    frame.setSeed(nibble * 0xeeb9eb65 + 0xa3561a1);
                                }
                                else
                                {
                                    skip = true;
                                }
                                break;
                            case Encounter::RockSmash:
                                slot = seed * 0xeeb9eb65 + 0xa3561a1;
                                nibble = slot * 0xeeb9eb65 + 0xa3561a1;
                                if (((nibble >> 16) % 100) < rock)
                                {
                                    frame.setEncounterSlot(EncounterSlot::kSlot(slot >> 16, encounterType));
                                    frame.setLevel(encounter.calcLevel(frame.getEncounterSlot(), seed >> 16));
                                    nibble = nibble * 0xeeb9eb65 + 0xa3561a1; // Blank(or maybe item)
                                    frame.setSeed(nibble * 0xeeb9eb65 + 0xa3561a1);
                                }
                                else
                                {
                                    skip = true;
                                }
                                break;
                            case Encounter::Stationary:
                            default:
                                frame.setSeed(testRNG.getSeed());
                                break;
                        }

                        if (!skip && (encounterType == Encounter::Stationary || compare.compareSlot(frame)))
                        {
                            frames.append(frame);
                        }
                    }

                    testPID = (nextRNG << 16) | nextRNG2;
                    nextRNG = testRNG.nextUShort();
                    nextRNG2 = testRNG.nextUShort();
                }
                while (testPID % 25 != frame.getNature());
            }

            if ((low % 3) != 0)
            {
                u32 slot = 0;
                bool skipFrame = false;

                switch (encounterType)
                {
                    case Encounter::Grass:
                        slot = seed;
                        frame.setEncounterSlot(EncounterSlot::kSlot(slot >> 16, encounterType));
                        frame.setLevel(encounter.calcLevel(frame.getEncounterSlot()));
                        frame.setSeed(slot * 0xeeb9eb65 + 0xa3561a1);
                        break;
                    case Encounter::Surfing:
                        slot = seed * 0xeeb9eb65 + 0xa3561a1;
                        frame.setEncounterSlot(EncounterSlot::kSlot(slot >> 16, encounterType));
                        frame.setLevel(encounter.calcLevel(frame.getEncounterSlot(), seed >> 16));
                        frame.setSeed(slot * 0xeeb9eb65 + 0xa3561a1);
                        break;
                    case Encounter::OldRod:
                    case Encounter::GoodRod:
                    case Encounter::SuperRod:
                        slot = seed * 0xeeb9eb65 + 0xa3561a1;;
                        nibble = slot * 0xeeb9eb65 + 0xa3561a1;
                        if (((nibble >> 16) % 100) < thresh)
                        {
                            frame.setEncounterSlot(EncounterSlot::kSlot(slot >> 16, encounterType));
                            frame.setLevel(encounter.calcLevel(frame.getEncounterSlot()));
                            frame.setSeed(nibble * 0xeeb9eb65 + 0xa3561a1);
                        }
                        else
                        {
                            continue;
                        }
                        break;
                    case Encounter::RockSmash:
                        slot = seed * 0xeeb9eb65 + 0xa3561a1;
                        nibble = slot * 0xeeb9eb65 + 0xa3561a1;
                        if (((nibble >> 16) % 100) < rock)
                        {
                            frame.setEncounterSlot(EncounterSlot::kSlot(slot >> 16, encounterType));
                            frame.setLevel(encounter.calcLevel(frame.getEncounterSlot(), seed >> 16));
                            nibble = nibble * 0xeeb9eb65 + 0xa3561a1; // Blank(or maybe item)
                            frame.setSeed(nibble * 0xeeb9eb65 + 0xa3561a1);
                        }
                        else
                        {
                            continue;
                        }
                        break;
                    case Encounter::Stationary:
                    default:
                        frame.setSeed(seed);
                        break;
                }

                u8 choppedPID = high % 25;
                if (!skipFrame)
                {
                    for (const auto &buffer : genderThreshHolds)
                    {
                        switch (buffer)
                        {
                            case 0x0:
                                frame.setLeadType(Lead::CuteCharmFemale);
                                break;
                            case 0x96:
                                frame.setLeadType(Lead::CuteCharm50M);
                                break;
                            case 0xC8:
                                frame.setLeadType(Lead::CuteCharm25M);
                                break;
                            case 0x4B:
                                frame.setLeadType(Lead::CuteCharm75M);
                                break;
                            case 0x32:
                                frame.setLeadType(Lead::CuteCharm875M);
                                break;
                            default:
                                frame.setLeadType(Lead::CuteCharm);
                                break;
                        }

                        frame.setPID(buffer + choppedPID, genderRatio);
                        if (!compare.comparePID(frame))
                        {
                            continue;
                        }

                        frame.setEncounterSlot(EncounterSlot::jSlot(slot >> 16, encounterType));
                        if (encounterType == Encounter::Stationary || compare.compareSlot(frame))
                        {
                            frames.append(frame);
                        }
                    }
                }
            }
        }
    }

    return frames;
}

QVector<Frame4> Searcher4::searchChainedShiny(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe) const
{
    QVector<Frame4> frames;

    Frame4 frame(tid, sid, psv);
    frame.setIVs(hp, atk, def, spa, spd, spe);

    if (!compare.compareHiddenPower(frame))
    {
        return frames;
    }

    u32 first = (hp | (atk << 5) | (def << 10)) << 16;
    u32 second = (spe | (spa << 5) | (spd << 10)) << 16;

    QVector<u32> seeds = cache.recoverLower16BitsIV(first, second);

    QVector<u16> calls(15);
    u16 low, high;

    for (const auto &seed : seeds)
    {
        PokeRNGR rng(seed);

        for (u16 &x : calls)
        {
            x = rng.nextUShort();
        }

        low = chainedPIDLow(calls);
        high = chainedPIDHigh(calls[13], low, tid, sid);
        frame.setPID(high, low, genderRatio);

        if (compare.comparePID(frame))
        {
            rng.nextUInt();
            frame.setSeed(rng.nextUInt());
            frames.append(frame);

            // Sister spread shares PID
            frame.setSeed(frame.getSeed() ^ 0x80000000);
            frames.append(frame);
        }
    }

    return frames;
}

QVector<Frame4> Searcher4::searchWondercardIVs(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe) const
{
    QVector<Frame4> frames;

    Frame4 frame(tid, sid, psv);
    frame.setIVs(hp, atk, def, spa, spd, spe);

    if (!compare.compareHiddenPower(frame))
    {
        return frames;
    }

    u32 first = (hp | (atk << 5) | (def << 10)) << 16;
    u32 second = (spe | (spa << 5) | (spd << 10)) << 16;

    QVector<u32> seeds = cache.recoverLower16BitsIV(first, second);
    for (const auto &seed : seeds)
    {
        // Setup normal frame
        PokeRNGR rng(seed);
        frame.setSeed(rng.nextUInt());
        frames.append(frame);

        // Setup XORed frame
        frame.setSeed(frame.getSeed() ^ 0x80000000);
        frames.append(frame);
    }

    return frames;
}

QVector<Frame4> Searcher4::searchInitialSeeds(const QVector<Frame4> &results) const
{
    QVector<Frame4> frames;

    for (Frame4 result : results)
    {
        PokeRNGR rng(result.getSeed(), minFrame - 1);
        u32 test = rng.getSeed();

        for (u32 cnt = minFrame; cnt <= maxFrame; cnt++)
        {
            u8 hour = (test >> 16) & 0xFF;
            u16 delay = test & 0xFFFF;

            // Check if seed matches a valid gen 4 format
            if (hour < 24 && delay >= minDelay && delay <= maxDelay)
            {
                result.setSeed(test);
                result.setFrame(cnt);
                frames.append(result);
            }

            test = rng.nextUInt();
        }
    }

    return frames;
}


u16 Searcher4::chainedPIDLow(const QVector<u16> &calls) const
{
    return (calls.at(14) & 7) | ((calls.at(12) & 1) << 3) | ((calls.at(11) & 1) << 4) | ((calls.at(10) & 1) << 5) | ((calls.at(9) & 1) << 6) |
           ((calls.at(8) & 1) << 7) | ((calls.at(7) & 1) << 8) | ((calls.at(6) & 1) << 9) | ((calls.at(5) & 1) << 10) | ((calls.at(4) & 1) << 11) |
           ((calls.at(3) & 1) << 12) | ((calls.at(2) & 1) << 13) | ((calls.at(1) & 1) << 14) | ((calls.at(0) & 1) << 15);
}

u16 Searcher4::chainedPIDHigh(u16 high, u16 low, u16 tid, u16 sid) const
{
    return (((low ^ tid ^ sid) & 0xFFF8) | (high & 7));
}
