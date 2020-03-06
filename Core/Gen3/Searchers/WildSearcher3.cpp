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

#include "WildSearcher3.hpp"
#include <Core/Enum/Method.hpp>
#include <Core/RNG/LCRNG.hpp>
#include <Core/Util/EncounterSlot.hpp>

WildSearcher3::WildSearcher3(u16 tid, u16 sid, u8 genderRatio, Method method, const FrameFilter &filter) :
    WildSearcher(tid, sid, genderRatio, method, filter), cache(method), searching(false), progress(0)
{
}

void WildSearcher3::setEncounterArea(const EncounterArea3 &encounterArea)
{
    this->encounterArea = encounterArea;
}

void WildSearcher3::startSearch(const QVector<u8> &min, const QVector<u8> &max)
{
    searching = true;

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

void WildSearcher3::cancelSearch()
{
    searching = false;
}

QVector<WildFrame> WildSearcher3::getResults()
{
    std::lock_guard<std::mutex> guard(mutex);
    auto data(results);
    results.clear();
    return data;
}

int WildSearcher3::getProgress() const
{
    return progress;
}

QVector<WildFrame> WildSearcher3::search(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe) const
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
        // Setup normal frame
        PokeRNGR rng(val);
        rng.advanceFrames(method == Method::Method2);

        u16 high = rng.nextUShort();
        u16 low = rng.nextUShort();

        frame.setPID(high, low);
        frame.setAbility(low & 1);
        frame.setGender(low & 255, genderRatio);
        frame.setNature(frame.getPID() % 25);
        frame.setShiny(tsv, high ^ low, 8);

        u32 seed = rng.nextUInt();

        // Use for loop to check both normal and sister spread
        for (const bool flag : { false, true })
        {
            if (flag)
            {
                frame.setPID(frame.getPID() ^ 0x80008000);
                frame.setNature(frame.getPID() % 25);
                seed ^= 0x80000000;
            }

            if (!filter.comparePID(frame))
            {
                continue;
            }

            PokeRNGR testRNG(seed);
            u32 testPID;
            u32 slot;
            u16 nextRNG = seed >> 16;
            u16 nextRNG2 = testRNG.nextUShort();

            do
            {
                switch (lead)
                {
                case Lead::None:
                    if ((nextRNG % 25) == frame.getNature())
                    {
                        frame.setLead(Lead::None);
                        slot = testRNG.getSeed() * 0xeeb9eb65 + 0xa3561a1;
                        frame.setSeed(slot * 0xdc6c95d9 + 0x4d3cb126);
                        frame.setEncounterSlot(EncounterSlot::hSlot(slot >> 16, encounter));
                        if (filter.compareEncounterSlot(frame))
                        {
                            frame.setLevel(encounterArea.calcLevel(frame.getEncounterSlot(), testRNG.getSeed() >> 16));
                            frames.append(frame);
                        }
                    }
                    break;
                case Lead::Synchronize:
                    // Successful synch
                    if ((nextRNG & 1) == 0)
                    {
                        frame.setLead(Lead::Synchronize);
                        slot = testRNG.getSeed() * 0xeeb9eb65 + 0xa3561a1;
                        frame.setSeed(slot * 0xdc6c95d9 + 0x4d3cb126);
                        frame.setEncounterSlot(EncounterSlot::hSlot(slot >> 16, encounter));
                        if (filter.compareEncounterSlot(frame))
                        {
                            frame.setLevel(encounterArea.calcLevel(frame.getEncounterSlot(), testRNG.getSeed() >> 16));
                            frames.append(frame);
                        }
                    }
                    // Failed synch
                    else if ((nextRNG2 & 1) == 1 && (nextRNG % 25) == frame.getNature())
                    {
                        frame.setLead(Lead::Synchronize);
                        slot = testRNG.getSeed() * 0xdc6c95d9 + 0x4d3cb126;
                        frame.setSeed(slot * 0xdc6c95d9 + 0x4d3cb126);
                        frame.setEncounterSlot(EncounterSlot::hSlot(slot >> 16, encounter));
                        if (filter.compareEncounterSlot(frame))
                        {
                            frame.setLevel(encounterArea.calcLevel(frame.getEncounterSlot(), testRNG.getSeed() >> 16));
                            frames.append(frame);
                        }
                    }
                    break;
                case Lead::CuteCharm:
                    if ((nextRNG % 25) == frame.getNature() && (nextRNG2 % 3) > 0)
                    {
                        frame.setLead(Lead::CuteCharm);
                        slot = testRNG.getSeed() * 0xdc6c95d9 + 0x4d3cb126;
                        frame.setSeed(slot * 0xdc6c95d9 + 0x4d3cb126);
                        frame.setEncounterSlot(EncounterSlot::hSlot(slot >> 16, encounter));
                        if (filter.compareEncounterSlot(frame))
                        {
                            frame.setLevel(encounterArea.calcLevel(frame.getEncounterSlot(), testRNG.getSeed() >> 16));
                            frames.append(frame);
                        }
                    }
                    break;
                case Lead::Search:
                default:
                    // Normal
                    if ((nextRNG % 25) == frame.getNature())
                    {
                        frame.setLead(Lead::None);
                        slot = testRNG.getSeed() * 0xeeb9eb65 + 0xa3561a1;
                        frame.setSeed(slot * 0xdc6c95d9 + 0x4d3cb126);
                        frame.setEncounterSlot(EncounterSlot::hSlot(slot >> 16, encounter));
                        if (filter.compareEncounterSlot(frame))
                        {
                            frame.setLevel(encounterArea.calcLevel(frame.getEncounterSlot(), testRNG.getSeed() >> 16));
                            frames.append(frame);
                        }

                        slot = testRNG.getSeed() * 0xdc6c95d9 + 0x4d3cb126;
                        frame.setSeed(slot * 0xdc6c95d9 + 0x4d3cb126);
                        frame.setEncounterSlot(EncounterSlot::hSlot(slot >> 16, encounter));
                        if (filter.compareEncounterSlot(frame))
                        {
                            frame.setLevel(encounterArea.calcLevel(frame.getEncounterSlot(), testRNG.getSeed() >> 16));

                            // Failed synch
                            if ((nextRNG2 & 1) == 1 && (nextRNG % 25) == frame.getNature())
                            {
                                frame.setLead(Lead::Synchronize);
                                frames.append(frame);
                            }

                            // Cute Charm
                            if ((nextRNG2 % 3) > 0)
                            {
                                frame.setLead(Lead::CuteCharm);
                                frames.append(frame);
                            }
                        }
                    }
                    // Successful Synch
                    else if ((nextRNG & 1) == 0)
                    {
                        frame.setLead(Lead::Synchronize);
                        slot = testRNG.getSeed() * 0xeeb9eb65 + 0xa3561a1;
                        frame.setSeed(slot * 0xdc6c95d9 + 0x4d3cb126);
                        frame.setEncounterSlot(EncounterSlot::hSlot(slot >> 16, encounter));
                        if (filter.compareEncounterSlot(frame))
                        {
                            frame.setLevel(encounterArea.calcLevel(frame.getEncounterSlot(), testRNG.getSeed() >> 16));
                            frames.append(frame);
                        }
                    }
                    break;
                }

                testPID = (nextRNG << 16) | nextRNG2;
                nextRNG = testRNG.nextUShort();
                nextRNG2 = testRNG.nextUShort();
            } while ((testPID % 25) != frame.getNature());
        }
    }

    // RSE rock smash is dependent on origin seed for encounter check
    if (encounter == Encounter::RockSmash)
    {
        u16 rate = encounterArea.getEncounterRate() * 16;

        // 2880 means FRLG which is not dependent on origin seed for encounter check
        if (rate != 2880)
        {
            for (int i = 0; i < frames.size();)
            {
                u32 check = frames.at(i).getSeed() * 0x41c64e6d + 0x6073;

                if (((check >> 16) % 2880) >= rate)
                {
                    frames.erase(frames.begin() + i);
                }
                else
                {
                    frames[i].setSeed(frames.at(i).getSeed() * 0xeeb9eb65 + 0xa3561a1);
                    i++;
                }
            }
        }
    }

    return frames;
}
