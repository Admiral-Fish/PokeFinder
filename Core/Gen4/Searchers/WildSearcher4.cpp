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

#include "WildSearcher4.hpp"
#include <Core/Enum/Encounter.hpp>
#include <Core/Enum/Lead.hpp>
#include <Core/Enum/Method.hpp>
#include <Core/RNG/LCRNG.hpp>
#include <Core/Util/EncounterSlot.hpp>

constexpr u8 genderThreshHolds[5] = { 0, 0x96, 0xC8, 0x4B, 0x32 };

WildSearcher4::WildSearcher4(u16 tid, u16 sid, u8 genderRatio, Method method, const FrameFilter &filter) :
    WildSearcher(tid, sid, genderRatio, method, filter), cache(method), searching(false), progress(0)
{
}

void WildSearcher4::setEncounterArea(const EncounterArea4 &encounterArea)
{
    this->encounterArea = encounterArea;
}

void WildSearcher4::setDelay(u32 minDelay, u32 maxDelay)
{
    this->minDelay = minDelay;
    this->maxDelay = maxDelay;
}

void WildSearcher4::setFrame(u32 minFrame, u32 maxFrame)
{
    this->minFrame = minFrame;
    this->maxFrame = maxFrame;
}

void WildSearcher4::startSearch(const QVector<u8> &min, const QVector<u8> &max)
{
    searching = true;

    thresh = encounter == Encounter::OldRod ? 25 : encounter == Encounter::GoodRod ? 50 : encounter == Encounter::SuperRod ? 75 : 0;
    suctionCupThresh
        = encounter == Encounter::OldRod ? 90 : encounter == Encounter::GoodRod ? 100 : encounter == Encounter::SuperRod ? 100 : 0;
    rock = encounterArea.getEncounterRate();

    for (u8 hp = min.at(0); hp <= max.at(0); hp++)
    {
        for (u8 atk = min.at(1); atk <= max.at(1); atk++)
        {
            for (u8 def = min.at(2); def <= max.at(2); def++)
            {
                for (u8 spa = min.at(3); spa <= max.at(3); spa++)
                {
                    for (u8 spd = min.at(4); spd <= max.at(4); spd++)
                    {
                        for (u8 spe = min.at(5); spe <= max.at(5); spe++)
                        {
                            if (!searching)
                            {
                                return;
                            }

                            auto frames = search(hp, atk, def, spa, spd, spe);

                            std::lock_guard<std::mutex> guard(mutex);
                            results.append(frames);
                            progress++;
                        }
                    }
                }
            }
        }
    }
}

void WildSearcher4::cancelSearch()
{
    searching = false;
}

QVector<WildFrame> WildSearcher4::getResults()
{
    std::lock_guard<std::mutex> guard(mutex);
    auto data(results);
    results.clear();
    return data;
}

int WildSearcher4::getProgress() const
{
    return progress;
}

QVector<WildFrame> WildSearcher4::search(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe) const
{
    QVector<WildFrame> frames;

    if (method == Method::MethodJ)
    {
        frames = searchMethodJ(hp, atk, def, spa, spd, spe);
    }
    else if (method == Method::MethodK)
    {
        frames = searchMethodK(hp, atk, def, spa, spd, spe);
    }
    else if (method == Method::ChainedShiny)
    {
        frames = searchChainedShiny(hp, atk, def, spa, spd, spe);
    }

    return searchInitialSeeds(frames);
}

QVector<WildFrame> WildSearcher4::searchMethodJ(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe) const
{
    QVector<WildFrame> frames;

    WildFrame frame;
    frame.setIVs(hp, atk, def, spa, spd, spe);
    frame.calculateHiddenPower();

    if (!filter.compareHiddenPower(frame))
    {
        return frames;
    }

    auto seeds = cache.recoverLower16BitsIV(hp, atk, def, spa, spd, spe);
    for (const u32 val : seeds)
    {
        PokeRNGR rng(val);
        u16 high = rng.nextUShort();
        u16 low = rng.nextUShort();
        u32 seed = rng.nextUInt();

        frame.setPID(high, low);

        for (const bool flag : { false, true })
        {
            if (flag)
            {
                frame.setPID(frame.getPID() ^ 0x80008000);
                seed ^= 0x80000000;
            }

            if (lead == Lead::CuteCharm)
            {
                frames.append(cuteCharmMethodJ(frame, seed));
            }
            else
            {
                if (lead == Lead::Search)
                {
                    frames.append(cuteCharmMethodJ(frame, seed));
                }

                frame.setAbility(low & 1);
                frame.setGender(low & 255, genderRatio);
                frame.setNature(frame.getPID() % 25);
                frame.setShiny(tsv, high ^ low, 8);

                if (!filter.comparePID(frame))
                {
                    continue;
                }

                if (lead == Lead::None)
                {
                    frames.append(normalMethodJ(frame, seed));
                }
                else if (lead == Lead::Synchronize)
                {
                    frames.append(synchMethodJ(frame, seed));
                }
                else if (lead == Lead::Search)
                {
                    frames.append(normalMethodJ(frame, seed));
                    frames.append(synchMethodJ(frame, seed));
                }
            }
        }
    }

    return frames;
}

QVector<WildFrame> WildSearcher4::searchMethodK(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe) const
{
    QVector<WildFrame> frames;

    WildFrame frame;
    frame.setIVs(hp, atk, def, spa, spd, spe);
    frame.calculateHiddenPower();

    if (!filter.compareHiddenPower(frame))
    {
        return frames;
    }

    auto seeds = cache.recoverLower16BitsIV(hp, atk, def, spa, spd, spe);
    for (const u32 val : seeds)
    {
        PokeRNGR rng(val);
        u16 high = rng.nextUShort();
        u16 low = rng.nextUShort();
        u32 seed = rng.nextUInt();

        frame.setPID(high, low);

        for (const bool flag : { false, true })
        {
            if (flag)
            {
                frame.setPID(frame.getPID() ^ 0x80008000);
                seed ^= 0x80000000;
            }

            if (lead == Lead::CuteCharm)
            {
                frames.append(cuteCharmMethodK(frame, seed));
            }
            else
            {
                if (lead == Lead::Search)
                {
                    frames.append(cuteCharmMethodK(frame, seed));
                }

                frame.setAbility(low & 1);
                frame.setGender(low & 255, genderRatio);
                frame.setNature(frame.getPID() % 25);
                frame.setShiny(tsv, high ^ low, 8);

                if (!filter.comparePID(frame))
                {
                    continue;
                }

                if (lead == Lead::None)
                {
                    frames.append(normalMethodK(frame, seed));
                }
                else if (lead == Lead::Synchronize)
                {
                    frames.append(synchMethodK(frame, seed));
                }
                else if (lead == Lead::Search)
                {
                    frames.append(normalMethodK(frame, seed));
                    frames.append(synchMethodK(frame, seed));
                }
            }
        }
    }

    return frames;
}

QVector<WildFrame> WildSearcher4::searchChainedShiny(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe) const
{
    QVector<WildFrame> frames;

    WildFrame frame;
    frame.setIVs(hp, atk, def, spa, spd, spe);
    frame.calculateHiddenPower();

    if (!filter.compareHiddenPower(frame))
    {
        return frames;
    }

    auto seeds = cache.recoverLower16BitsIV(hp, atk, def, spa, spd, spe);
    for (const auto &seed : seeds)
    {
        PokeRNGR rng(seed);

        u16 low = 0;
        for (int i = 15; i > 2; i--)
        {
            low |= (rng.nextUShort() & 1) << i;
        }
        u16 high = rng.nextUShort() & 7;
        low |= rng.nextUShort() & 7;
        high |= (tid ^ sid ^ low) & 0xfff8;

        frame.setPID(high, low);
        frame.setAbility(low & 1);
        frame.setGender(low & 255, genderRatio);
        frame.setNature(frame.getPID() % 25);
        frame.setShiny(true);

        if (filter.comparePID(frame))
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

QVector<WildFrame> WildSearcher4::searchInitialSeeds(const QVector<WildFrame> &results) const
{
    QVector<WildFrame> frames;

    for (WildFrame result : results)
    {
        PokeRNGR rng(result.getSeed());
        rng.advanceFrames(minFrame - 1);

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

QVector<WildFrame> WildSearcher4::normalMethodJ(WildFrame frame, u32 seed) const
{
    QVector<WildFrame> frames;
    frame.setLead(Lead::None);

    PokeRNGR rng(seed);
    u32 pid;
    u16 nextRNG = seed >> 16;
    u16 nextRNG2 = rng.nextUShort();

    do
    {
        if ((nextRNG / 0xa3e) == frame.getNature())
        {
            if (encounterMethodJ(frame, rng.getSeed()))
            {
                frames.append(frame);
            }
        }

        pid = static_cast<u32>((nextRNG << 16) | nextRNG2);
        nextRNG = rng.nextUShort();
        nextRNG2 = rng.nextUShort();
    } while (pid % 25 != frame.getNature());

    return frames;
}

QVector<WildFrame> WildSearcher4::synchMethodJ(WildFrame frame, u32 seed) const
{
    QVector<WildFrame> frames;
    frame.setLead(Lead::Synchronize);

    PokeRNGR rng(seed);
    u32 pid;
    u16 nextRNG = seed >> 16;
    u16 nextRNG2 = rng.nextUShort();

    do
    {
        if ((nextRNG >> 15) == 0)
        {
            if (encounterMethodJ(frame, rng.getSeed()))
            {
                frames.append(frame);
            }
        }
        else if ((nextRNG2 >> 15) == 1 && (nextRNG / 0xa3e) == frame.getNature())
        {
            if (encounterMethodJ(frame, rng.getSeed() * 0xeeb9eb65 + 0xa3561a1))
            {
                frames.append(frame);
            }
        }

        pid = static_cast<u32>((nextRNG << 16) | nextRNG2);
        nextRNG = rng.nextUShort();
        nextRNG2 = rng.nextUShort();
    } while (pid % 25 != frame.getNature());

    return frames;
}

QVector<WildFrame> WildSearcher4::cuteCharmMethodJ(WildFrame frame, u32 seed) const
{
    QVector<WildFrame> frames;

    PokeRNGR rng(seed);
    u16 high = frame.getPID() >> 16;
    u16 low = frame.getPID() & 0xffff;

    if ((low / 0x5556) != 0)
    {
        if (encounterMethodJ(frame, seed))
        {
            u8 choppedPID = high / 0xa3e;
            u8 nature = choppedPID % 25;
            for (const u8 buffer : genderThreshHolds)
            {
                if (buffer == 0)
                {
                    frame.setLead(Lead::CuteCharmFemale);
                }
                else if (buffer == 0x96)
                {
                    frame.setLead(Lead::CuteCharm50M);
                }
                else if (buffer == 0xc8)
                {
                    frame.setLead(Lead::CuteCharm25M);
                }
                else if (buffer == 0x4b)
                {
                    frame.setLead(Lead::CuteCharm75M);
                }
                else if (buffer == 0x32)
                {
                    frame.setLead(Lead::CuteCharm875M);
                }

                u8 pid = choppedPID + buffer;
                frame.setPID(pid);
                frame.setAbility(pid & 1);
                frame.setGender(pid, genderRatio);
                frame.setNature(nature);
                frame.setShiny(tsv, pid, 8);

                if (filter.comparePID(frame))
                {
                    frames.append(frame);
                }
            }
        }
    }

    return frames;
}

bool WildSearcher4::encounterMethodJ(WildFrame &frame, u32 seed) const
{
    PokeRNGR rng(seed);

    switch (encounter)
    {
    case Encounter::Grass:
        frame.setEncounterSlot(EncounterSlot::jSlot(seed >> 16, encounter));
        frame.setLevel(encounterArea.calcLevel(frame.getEncounterSlot()));
        frame.setSeed(rng.nextUInt());
        break;
    case Encounter::Surfing:
        frame.setEncounterSlot(EncounterSlot::jSlot(rng.nextUShort(), encounter));
        frame.setLevel(encounterArea.calcLevel(frame.getEncounterSlot(), seed >> 16));
        frame.setSeed(rng.nextUInt());
        break;
    case Encounter::OldRod:
    case Encounter::GoodRod:
    case Encounter::SuperRod:
    {
        u16 slot = rng.nextUShort();
        if ((rng.nextUShort() / 656) < thresh)
        {
            frame.setEncounterSlot(EncounterSlot::jSlot(slot, encounter));
            frame.setLevel(encounterArea.calcLevel(frame.getEncounterSlot(), seed >> 16));
            frame.setSeed(rng.nextUInt());
        }
        else
        {
            return false;
        }
        break;
    }
    default:
        break;
    }

    return filter.compareEncounterSlot(frame);
}

QVector<WildFrame> WildSearcher4::normalMethodK(WildFrame frame, u32 seed) const
{
    QVector<WildFrame> frames;

    PokeRNGR rng(seed);
    u32 pid;
    u16 nextRNG = seed >> 16;
    u16 nextRNG2 = rng.nextUShort();

    do
    {
        if ((nextRNG % 25) == frame.getNature())
        {
            frame.setLead(Lead::None);
            if (encounterMethodK(frame, rng.getSeed()))
            {
                frames.append(frame);
            }
        }

        pid = static_cast<u32>((nextRNG << 16) | nextRNG2);
        nextRNG = rng.nextUShort();
        nextRNG2 = rng.nextUShort();
    } while (pid % 25 != frame.getNature());

    return frames;
}

QVector<WildFrame> WildSearcher4::synchMethodK(WildFrame frame, u32 seed) const
{
    QVector<WildFrame> frames;
    frame.setLead(Lead::Synchronize);

    PokeRNGR rng(seed);
    u32 pid;
    u16 nextRNG = seed >> 16;
    u16 nextRNG2 = rng.nextUShort();

    do
    {
        if ((nextRNG & 1) == 0)
        {
            if (encounterMethodK(frame, rng.getSeed()))
            {
                frames.append(frame);
            }
        }
        else if ((nextRNG2 & 1) == 1 && (nextRNG / 0xa3e) == frame.getNature())
        {
            if (encounterMethodK(frame, rng.getSeed() * 0xeeb9eb65 + 0xa3561a1))
            {
                frames.append(frame);
            }
        }

        pid = static_cast<u32>((nextRNG << 16) | nextRNG2);
        nextRNG = rng.nextUShort();
        nextRNG2 = rng.nextUShort();
    } while (pid % 25 != frame.getNature());

    return frames;
}

QVector<WildFrame> WildSearcher4::cuteCharmMethodK(WildFrame frame, u32 seed) const
{
    QVector<WildFrame> frames;

    PokeRNGR rng(seed);
    u16 high = frame.getPID() >> 16;
    u16 low = frame.getPID() & 0xffff;

    if ((low % 3) != 0)
    {
        if (encounterMethodK(frame, seed))
        {
            u8 choppedPID = high % 25;
            u8 nature = choppedPID % 25;
            for (const u8 buffer : genderThreshHolds)
            {
                if (buffer == 0)
                {
                    frame.setLead(Lead::CuteCharmFemale);
                }
                else if (buffer == 0x96)
                {
                    frame.setLead(Lead::CuteCharm50M);
                }
                else if (buffer == 0xc8)
                {
                    frame.setLead(Lead::CuteCharm25M);
                }
                else if (buffer == 0x4b)
                {
                    frame.setLead(Lead::CuteCharm75M);
                }
                else if (buffer == 0x32)
                {
                    frame.setLead(Lead::CuteCharm875M);
                }

                u8 pid = choppedPID + buffer;
                frame.setPID(pid);
                frame.setAbility(pid & 1);
                frame.setGender(pid, genderRatio);
                frame.setNature(nature);
                frame.setShiny(tsv, pid, 8);

                if (filter.comparePID(frame))
                {
                    frames.append(frame);
                }
            }
        }
    }

    return frames;
}

bool WildSearcher4::encounterMethodK(WildFrame &frame, u32 seed) const
{
    PokeRNGR rng(seed);

    switch (encounter)
    {
    case Encounter::Grass:
        frame.setEncounterSlot(EncounterSlot::kSlot(seed >> 16, encounter));
        frame.setLevel(encounterArea.calcLevel(frame.getEncounterSlot()));
        frame.setSeed(rng.nextUInt());
        break;
    case Encounter::Surfing:
        frame.setEncounterSlot(EncounterSlot::kSlot(rng.nextUShort(), encounter));
        frame.setLevel(encounterArea.calcLevel(frame.getEncounterSlot(), seed >> 16));
        frame.setSeed(rng.nextUInt());
        break;
    case Encounter::OldRod:
    case Encounter::GoodRod:
    case Encounter::SuperRod:
    {
        u16 slot = rng.nextUShort();
        u16 nibble = rng.nextUShort() % 100;
        if (nibble < thresh)
        {
            frame.setEncounterSlot(EncounterSlot::kSlot(slot, encounter));
            frame.setLevel(encounterArea.calcLevel(frame.getEncounterSlot()));
            frame.setSeed(rng.nextUInt());
        }
        else if (nibble < suctionCupThresh && frame.getLead() == Lead::None)
        {
            frame.setLead(Lead::SuctionCups);
            frame.setEncounterSlot(EncounterSlot::kSlot(slot, encounter));
            frame.setLevel(encounterArea.calcLevel(frame.getEncounterSlot()));
            frame.setSeed(rng.nextUInt());
        }
        else
        {
            return false;
        }
    }
    break;
    case Encounter::RockSmash:
    {
        u16 slot = rng.nextUShort();
        if ((rng.nextUShort() % 100) < rock)
        {
            frame.setEncounterSlot(EncounterSlot::kSlot(slot, encounter));
            frame.setLevel(encounterArea.calcLevel(frame.getEncounterSlot(), seed >> 16));
            rng.advanceFrames(1); // Blank (maybe item)
            frame.setSeed(rng.nextUInt());
        }
        else
        {
            return false;
        }
    }
    break;
    default:
        break;
    }

    return true;
}
