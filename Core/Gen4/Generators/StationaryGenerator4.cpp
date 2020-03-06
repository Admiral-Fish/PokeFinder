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

#include "StationaryGenerator4.hpp"
#include <Core/Enum/Lead.hpp>
#include <Core/Enum/Method.hpp>
#include <Core/Parents/Filters/FrameFilter.hpp>
#include <Core/RNG/LCRNG.hpp>

StationaryGenerator4::StationaryGenerator4(u32 initialFrame, u32 maxResults, u16 tid, u16 sid, u8 genderRatio, Method method,
                                           const FrameFilter &filter) :
    StationaryGenerator(initialFrame, maxResults, tid, sid, genderRatio, method, filter)
{
}

QVector<Frame> StationaryGenerator4::generate(u32 seed) const
{
    switch (method)
    {
    case Method::Method1:
        return generateMethod1(seed);
    case Method::MethodJ:
        return generateMethodJ(seed);
    case Method::MethodK:
        return generateMethodK(seed);
    case Method::WondercardIVs:
        return generateWonderCardIVs(seed);
    default:
        return QVector<Frame>();
    }
}

QVector<Frame> StationaryGenerator4::generateMethod1(u32 seed) const
{
    QVector<Frame> frames;

    PokeRNG rng(seed);
    rng.advanceFrames(initialFrame - 1 + offset);

    // Method 1 [SEED] [PID] [PID] [IVS] [IVS]
    for (u32 cnt = 0; cnt < maxResults; cnt++, rng.nextUInt())
    {
        Frame frame(initialFrame + cnt);
        PokeRNG go(rng.getSeed());

        u16 low = go.nextUShort();
        u16 high = go.nextUShort();

        u16 iv1 = go.nextUShort();
        u16 iv2 = go.nextUShort();

        frame.setPID(high, low);
        frame.setShiny(tsv, high ^ low, 8);
        frame.setAbility(low & 1);
        frame.setGender(low & 255, genderRatio);
        frame.setNature(frame.getPID() % 25);

        frame.setIVs(iv1, iv2);
        frame.calculateHiddenPower();

        if (filter.compareFrame(frame))
        {
            frame.setSeed(low);
            frames.append(frame);
        }
    }

    return frames;
}

QVector<Frame> StationaryGenerator4::generateMethodJ(u32 seed) const
{
    QVector<Frame> frames;

    PokeRNG rng(seed);
    rng.advanceFrames(initialFrame - 1 + offset);

    u8 buffer = 0;
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
        Frame frame(initialFrame + cnt);

        PokeRNG go(rng.getSeed());
        frame.setSeed(go.nextUShort());

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
                pid = (high << 16) | low;
            } while (pid % 25 != frame.getNature());
            frame.setPID(pid);

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
                pid = (high << 16) | low;
            } while (pid % 25 != frame.getNature());
            frame.setPID(pid);

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
                frame.setPID(buffer + frame.getNature());
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
                    pid = (high << 16) | low;
                } while (pid % 25 != frame.getNature());
                frame.setPID(pid);
            }

            break;
        }

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

QVector<Frame> StationaryGenerator4::generateMethodK(u32 seed) const
{
    QVector<Frame> frames;

    PokeRNG rng(seed);
    rng.advanceFrames(initialFrame - 1 + offset);

    u8 buffer = 0;
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
        Frame frame(initialFrame + cnt);
        PokeRNG go(rng.getSeed());

        frame.setSeed(go.nextUShort());

        u32 pid = 0;
        switch (lead)
        {
        case Lead::None:
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
                pid = (high << 16) | low;
            } while (pid % 25 != frame.getNature());
            frame.setPID(pid);

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
                pid = (high << 16) | low;
            } while (pid % 25 != frame.getNature());
            frame.setPID(pid);

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

                frame.setPID(buffer + frame.getNature());
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
                    pid = (high << 16) | low;
                } while (pid % 25 != frame.getNature());

                frame.setPID(pid);
            }

            break;
        }

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

QVector<Frame> StationaryGenerator4::generateWonderCardIVs(u32 seed) const
{
    QVector<Frame> frames;

    PokeRNG rng(seed);
    rng.advanceFrames(initialFrame - 1 + offset);

    // Wondercard IVs [SEED] [IVS] [IVS]

    for (u32 cnt = 0; cnt < maxResults; cnt++, rng.nextUInt())
    {
        Frame frame(initialFrame + cnt);
        PokeRNG go(rng.getSeed());

        u16 iv1 = go.nextUShort();
        u16 iv2 = go.nextUShort();

        frame.setIVs(iv1, iv2);
        frame.calculateHiddenPower();

        if (filter.compareIVs(frame))
        {
            frame.setSeed(iv1);
            frames.append(frame);
        }
    }

    return frames;
}
