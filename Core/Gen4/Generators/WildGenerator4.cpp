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

#include "WildGenerator4.hpp"
#include <Core/Enum/Encounter.hpp>
#include <Core/Enum/Lead.hpp>
#include <Core/Enum/Method.hpp>
#include <Core/Parents/Filters/FrameFilter.hpp>
#include <Core/RNG/LCRNG.hpp>
#include <Core/Util/EncounterSlot.hpp>

WildGenerator4::WildGenerator4(u32 initialFrame, u32 maxResults, u16 tid, u16 sid, u8 genderRatio, Method method,
                               const FrameFilter &filter) :
    WildGenerator(initialFrame, maxResults, tid, sid, genderRatio, method, filter)
{
}

QVector<WildFrame> WildGenerator4::generate(u32 seed) const
{
    switch (method)
    {
    case Method::MethodJ:
        return generateMethodJ(seed);
    case Method::MethodK:
        return generateMethodK(seed);
    case Method::ChainedShiny:
        return generateChainedShiny(seed);
    default:
        return QVector<WildFrame>();
    }
}

void WildGenerator4::setEncounterArea(const EncounterArea4 &encounterArea)
{
    this->encounterArea = encounterArea;
}

QVector<WildFrame> WildGenerator4::generateMethodJ(u32 seed) const
{
    QVector<WildFrame> frames;

    PokeRNG rng(seed);
    rng.advanceFrames(initialFrame - 1 + offset);

    u8 buffer = 0;
    u8 thresh = encounter == Encounter::OldRod ? 25 : encounter == Encounter::GoodRod ? 50 : encounter == Encounter::SuperRod ? 75 : 0;

    switch (lead)
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

    for (u32 cnt = 0; cnt < maxResults; cnt++, rng.nextUInt())
    {
        WildFrame frame(initialFrame + cnt);

        PokeRNG go(rng.getSeed());
        frame.setSeed(go.nextUShort()); // Blank ???

        switch (encounter)
        {
        case Encounter::Grass:
            frame.setEncounterSlot(EncounterSlot::jSlot(go.getSeed() >> 16, encounter));
            if (!filter.compareEncounterSlot(frame))
            {
                continue;
            }

            frame.setLevel(encounterArea.calcLevel(frame.getEncounterSlot()));
            break;
        case Encounter::Surfing:
            frame.setEncounterSlot(EncounterSlot::jSlot(go.getSeed() >> 16, encounter));
            if (!filter.compareEncounterSlot(frame))
            {
                continue;
            }

            frame.setLevel(encounterArea.calcLevel(frame.getEncounterSlot(), go.nextUShort()));
            break;
        case Encounter::OldRod:
        case Encounter::GoodRod:
        case Encounter::SuperRod:
            if (((go.getSeed() >> 16) / 656) >= thresh)
            {
                continue;
            }

            frame.setEncounterSlot(EncounterSlot::jSlot(go.nextUShort(), encounter));
            if (!filter.compareEncounterSlot(frame))
            {
                continue;
            }

            frame.setLevel(encounterArea.calcLevel(frame.getEncounterSlot(), go.nextUShort()));
            break;
        default:
            break;
        }

        u32 pid = 0;
        switch (lead)
        {
        case Lead::None:
            // Get hunt nature
            frame.setNature(go.nextUShort() / 0xa3e);

            if (!filter.compareNature(frame))
            {
                continue;
            }

            // Begin search for valid pid
            do
            {
                u16 low = go.nextUShort();
                u16 high = go.nextUShort();
                pid = static_cast<u32>((high << 16) | low);
            } while (pid % 25 != frame.getNature());

            break;
        case Lead::Synchronize:
            if ((go.nextUShort() >> 15) == 0) // Successful synch
            {
                frame.setNature(synchNature);
            }
            else // Failed synch
            {
                frame.setNature(go.nextUShort() / 0xa3e);
            }

            if (!filter.compareNature(frame))
            {
                continue;
            }

            // Begin search for valid pid
            do
            {
                u16 low = go.nextUShort();
                u16 high = go.nextUShort();
                pid = static_cast<u32>((high << 16) | low);
            } while (pid % 25 != frame.getNature());

            break;
        default: // Default to cover all cute charm cases
            if ((go.nextUShort() / 0x5556) != 0) // Successful cute charm
            {
                // Get nature
                frame.setNature(go.nextUShort() / 0xa3e);

                if (!filter.compareNature(frame))
                {
                    continue;
                }

                // Cute charm doesn't hunt for a valid PID, just uses buffer and target nature
                pid = buffer + frame.getNature();
            }
            else // Failed cute charm
            {
                // Get nature
                frame.setNature(go.nextUShort() / 0xa3e);

                if (!filter.compareNature(frame))
                {
                    continue;
                }

                // Begin search for valid pid
                do
                {
                    u16 low = go.nextUShort();
                    u16 high = go.nextUShort();
                    pid = static_cast<u32>((high << 16) | low);
                } while (pid % 25 != frame.getNature());
            }

            break;
        }

        frame.setPID(pid);
        frame.setAbility(pid & 1);
        frame.setGender(pid & 255, genderRatio);
        frame.setShiny(tsv, (pid >> 16) ^ (pid & 0xffff), 8);

        u16 iv1 = go.nextUShort();
        u16 iv2 = go.nextUShort();

        frame.setIVs(iv1, iv2);
        frame.calculateHiddenPower();

        if (filter.compareFrame(frame))
        {
            frames.append(frame);
        }
    }

    return frames;
}

QVector<WildFrame> WildGenerator4::generateMethodK(u32 seed) const
{
    QVector<WildFrame> frames;

    PokeRNG rng(seed);
    rng.advanceFrames(initialFrame - 1 + offset);

    u8 buffer = 0;
    u8 thresh = 0;
    u16 rate = encounterArea.getEncounterRate();
    if (encounter == Encounter::OldRod)
    {
        thresh = lead == Lead::SuctionCups ? 90 : 25;
    }
    else if (encounter == Encounter::GoodRod)
    {
        thresh = lead == Lead::SuctionCups ? 100 : 50;
    }
    else if (encounter == Encounter::SuperRod)
    {
        thresh = lead == Lead::SuctionCups ? 100 : 75;
    }

    switch (lead)
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

    for (u32 cnt = 0; cnt < maxResults; cnt++, rng.nextUInt())
    {
        WildFrame frame(initialFrame + cnt);

        PokeRNG go(rng.getSeed());
        frame.setSeed(go.nextUShort()); // Blank ???

        switch (encounter)
        {
        case Encounter::Grass:
            frame.setEncounterSlot(EncounterSlot::kSlot(go.getSeed() >> 16, encounter));
            if (!filter.compareEncounterSlot(frame))
            {
                continue;
            }

            frame.setLevel(encounterArea.calcLevel(frame.getEncounterSlot()));
            break;
        case Encounter::Surfing:
            frame.setEncounterSlot(EncounterSlot::kSlot(go.getSeed() >> 16, encounter));
            if (!filter.compareEncounterSlot(frame))
            {
                continue;
            }

            frame.setLevel(encounterArea.calcLevel(frame.getEncounterSlot(), go.nextUShort()));
            break;
        case Encounter::OldRod:
        case Encounter::GoodRod:
        case Encounter::SuperRod:
            if (((go.getSeed() >> 16) % 100) >= thresh)
            {
                continue;
            }

            frame.setEncounterSlot(EncounterSlot::kSlot(go.nextUShort(), encounter));
            if (!filter.compareEncounterSlot(frame))
            {
                continue;
            }

            frame.setLevel(encounterArea.calcLevel(frame.getEncounterSlot()));
            go.advanceFrames(1);
            break;
        case Encounter::RockSmash:
            // Blank(or maybe item) ???
            if (((go.nextUShort()) % 100) >= rate)
            {
                continue;
            }

            frame.setEncounterSlot(EncounterSlot::kSlot(go.nextUShort(), encounter));
            if (!filter.compareEncounterSlot(frame))
            {
                continue;
            }

            frame.setLevel(encounterArea.calcLevel(frame.getEncounterSlot(), go.nextUShort()));
            break;
        case Encounter::HeadButt: // TODO
        case Encounter::BugCatchingContest: // TODO
        default:
            break;
        }

        u32 pid = 0;
        switch (lead)
        {
        case Lead::None:
        case Lead::SuctionCups:
            // Get hunt nature
            frame.setNature(go.nextUShort() % 25);

            if (!filter.compareNature(frame))
            {
                continue;
            }

            // Begin search for valid pid
            do
            {
                u16 low = go.nextUShort();
                u16 high = go.nextUShort();
                pid = static_cast<u32>((high << 16) | low);
            } while (pid % 25 != frame.getNature());

            break;
        case Lead::Synchronize:
            if ((go.nextUShort() & 1) == 0) // Successful synch
            {
                frame.setNature(synchNature);
            }
            else // Failed synch
            {
                frame.setNature(go.nextUShort() % 25);
            }

            if (!filter.compareNature(frame))
            {
                continue;
            }

            // Begin search for valid pid
            do
            {
                u16 low = go.nextUShort();
                u16 high = go.nextUShort();
                pid = static_cast<u32>((high << 16) | low);
            } while (pid % 25 != frame.getNature());

            break;
        default: // Default to cover all cute charm cases
            if ((go.nextUShort() % 3) != 0) // Successfull cute charm
            {
                // Get hunt nature
                frame.setNature(go.nextUShort() % 25);

                if (!filter.compareNature(frame))
                {
                    continue;
                }

                pid = buffer + frame.getNature();
            }
            else // Failed cutecharm
            {
                // Get hunt nature
                frame.setNature(go.nextUShort() % 25);

                if (!filter.compareNature(frame))
                {
                    continue;
                }

                // Begin search for valid pid
                do
                {
                    u16 low = go.nextUShort();
                    u16 high = go.nextUShort();
                    pid = static_cast<u32>((high << 16) | low);
                } while (pid % 25 != frame.getNature());
            }

            break;
        }

        frame.setPID(pid);
        frame.setAbility(pid & 1);
        frame.setGender(pid & 255, genderRatio);
        frame.setShiny(tsv, (pid >> 16) ^ (pid & 0xffff), 8);

        u16 iv1 = go.nextUShort();
        u16 iv2 = go.nextUShort();

        frame.setIVs(iv1, iv2);
        frame.calculateHiddenPower();

        if (filter.compareFrame(frame))
        {
            frames.append(frame);
        }
    }

    return frames;
}

QVector<WildFrame> WildGenerator4::generateChainedShiny(u32 seed) const
{
    QVector<WildFrame> frames;

    PokeRNG rng(seed);
    rng.advanceFrames(initialFrame - 1 + offset);

    for (u32 cnt = 0; cnt < maxResults; cnt++, rng.nextUInt())
    {
        WildFrame frame(initialFrame + cnt);

        PokeRNG go(rng.getSeed());
        u32 originSeed = go.nextUInt(); // TODO: is this necessary

        u16 low = go.nextUShort() & 7;
        u16 high = go.nextUShort() & 7;

        for (int i = 3; i < 16; i++)
        {
            low |= (go.nextUShort() & 1) << i;
        }
        high |= (low ^ tid ^ sid) & 0xfff8;

        u16 iv1 = go.nextUShort();
        u16 iv2 = go.nextUShort();

        frame.setPID(high, low);
        frame.setAbility(low & 1);
        frame.setGender(low & 255, genderRatio);
        frame.setNature(frame.getPID() % 25);
        frame.setShiny(true);

        frame.setIVs(iv1, iv2);
        frame.calculateHiddenPower();

        if (filter.compareFrame(frame))
        {
            frame.setSeed(originSeed);
            frames.append(frame);
        }
    }

    return frames;
}
