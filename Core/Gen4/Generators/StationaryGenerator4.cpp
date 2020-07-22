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
#include <Core/Parents/Filters/StateFilter.hpp>
#include <Core/RNG/LCRNG.hpp>

StationaryGenerator4::StationaryGenerator4(u32 initialAdvances, u32 maxResults, u16 tid, u16 sid, u8 genderRatio, Method method,
                                           const StateFilter &filter) :
    StationaryGenerator(initialAdvances, maxResults, tid, sid, genderRatio, method, filter)
{
}

QVector<State> StationaryGenerator4::generate(u32 seed) const
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
        return QVector<State>();
    }
}

QVector<State> StationaryGenerator4::generateMethod1(u32 seed) const
{
    QVector<State> states;

    PokeRNG rng(seed);
    rng.advance(initialAdvances + offset);

    // Method 1 [SEED] [PID] [PID] [IVS] [IVS]
    for (u32 cnt = 0; cnt < maxResults; cnt++, rng.next())
    {
        State currentState(initialAdvances + cnt);
        PokeRNG go(rng.getSeed());

        u16 low = go.nextUShort();
        u16 high = go.nextUShort();

        u16 iv1 = go.nextUShort();
        u16 iv2 = go.nextUShort();

        currentState.setPID(high, low);
        currentState.setShiny(tsv, high ^ low, 8);
        currentState.setAbility(low & 1);
        currentState.setGender(low & 255, genderRatio);
        currentState.setNature(currentState.getPID() % 25);

        currentState.setIVs(iv1, iv2);
        currentState.calculateHiddenPower();

        if (filter.compareState(currentState))
        {
            currentState.setSeed(low);
            states.append(currentState);
        }
    }

    return states;
}

QVector<State> StationaryGenerator4::generateMethodJ(u32 seed) const
{
    QVector<State> states;

    PokeRNG rng(seed);
    rng.advance(initialAdvances + offset);

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

    for (u32 cnt = 0; cnt < maxResults; cnt++, rng.next())
    {
        State currentState(initialAdvances + cnt);

        PokeRNG go(rng.getSeed());
        currentState.setSeed(go.getSeed());

        u32 pid = 0;
        switch (lead)
        {
        case Lead::None:
            // Get hunt nature
            currentState.setNature(go.nextUShort() / 0xa3e);
            if (!filter.compareNature(currentState))
            {
                continue;
            }

            // Begin search for valid pid
            do
            {
                u16 low = go.nextUShort();
                u16 high = go.nextUShort();
                pid = (high << 16) | low;
            } while (pid % 25 != currentState.getNature());
            currentState.setPID(pid);

            break;
        case Lead::Synchronize:
            if ((go.nextUShort() >> 15) == 0) // Successful synch
            {
                currentState.setNature(synchNature);
            }
            else // Failed synch
            {
                currentState.setNature(go.nextUShort() / 0xa3e);
            }

            if (!filter.compareNature(currentState))
            {
                continue;
            }

            // Begin search for valid pid
            do
            {
                u16 low = go.nextUShort();
                u16 high = go.nextUShort();
                pid = (high << 16) | low;
            } while (pid % 25 != currentState.getNature());
            currentState.setPID(pid);

            break;
        default: // Default to cover all cute charm cases
            if ((go.nextUShort() / 0x5556) != 0) // Successful cute charm
            {
                // Get nature
                currentState.setNature(go.nextUShort() / 0xa3e);

                if (!filter.compareNature(currentState))
                {
                    continue;
                }

                // Cute charm doesn't hunt for a valid PID, just uses buffer and target nature
                currentState.setPID(buffer + currentState.getNature());
            }
            else // Failed cute charm
            {
                // Get nature
                currentState.setNature(go.nextUShort() / 0xa3e);

                if (!filter.compareNature(currentState))
                {
                    continue;
                }

                // Begin search for valid pid
                do
                {
                    u16 low = go.nextUShort();
                    u16 high = go.nextUShort();
                    pid = (high << 16) | low;
                } while (pid % 25 != currentState.getNature());
                currentState.setPID(pid);
            }

            break;
        }

        currentState.setAbility(pid & 1);
        currentState.setGender(pid & 255, genderRatio);
        currentState.setShiny(tsv, (pid >> 16) ^ (pid & 0xffff), 8);

        u16 iv1 = go.nextUShort();
        u16 iv2 = go.nextUShort();

        currentState.setIVs(iv1, iv2);
        currentState.calculateHiddenPower();

        if (filter.compareState(currentState))
        {
            states.append(currentState);
        }
    }

    return states;
}

QVector<State> StationaryGenerator4::generateMethodK(u32 seed) const
{
    QVector<State> states;

    PokeRNG rng(seed);
    rng.advance(initialAdvances + offset);

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

    for (u32 cnt = 0; cnt < maxResults; cnt++, rng.next())
    {
        State currentState(initialAdvances + cnt);

        PokeRNG go(rng.getSeed());
        currentState.setSeed(go.getSeed());

        u32 pid = 0;
        switch (lead)
        {
        case Lead::None:
            // Get hunt nature
            currentState.setNature(go.nextUShort() % 25);

            if (!filter.compareNature(currentState))
            {
                continue;
            }

            // Begin search for valid pid
            do
            {
                u16 low = go.nextUShort();
                u16 high = go.nextUShort();
                pid = (high << 16) | low;
            } while (pid % 25 != currentState.getNature());
            currentState.setPID(pid);

            break;
        case Lead::Synchronize:
            if ((go.nextUShort() & 1) == 0) // Successful synch
            {
                currentState.setNature(synchNature);
            }
            else // Failed synch
            {
                currentState.setNature(go.nextUShort() % 25);
            }

            if (!filter.compareNature(currentState))
            {
                continue;
            }

            // Begin search for valid pid
            do
            {
                u16 low = go.nextUShort();
                u16 high = go.nextUShort();
                pid = (high << 16) | low;
            } while (pid % 25 != currentState.getNature());
            currentState.setPID(pid);

            break;
        default: // Default to cover all cute charm cases
            if ((go.nextUShort() % 3) != 0) // Successfull cute charm
            {
                // Get hunt nature
                currentState.setNature(go.nextUShort() % 25);

                if (!filter.compareNature(currentState))
                {
                    continue;
                }

                currentState.setPID(buffer + currentState.getNature());
            }
            else // Failed cutecharm
            {
                // Get hunt nature
                currentState.setNature(go.nextUShort() % 25);

                if (!filter.compareNature(currentState))
                {
                    continue;
                }

                // Begin search for valid pid
                do
                {
                    u16 low = go.nextUShort();
                    u16 high = go.nextUShort();
                    pid = (high << 16) | low;
                } while (pid % 25 != currentState.getNature());

                currentState.setPID(pid);
            }

            break;
        }

        currentState.setAbility(pid & 1);
        currentState.setGender(pid & 255, genderRatio);
        currentState.setShiny(tsv, (pid >> 16) ^ (pid & 0xffff), 8);

        u16 iv1 = go.nextUShort();
        u16 iv2 = go.nextUShort();

        currentState.setIVs(iv1, iv2);
        currentState.calculateHiddenPower();

        if (filter.compareState(currentState))
        {
            states.append(currentState);
        }
    }

    return states;
}

QVector<State> StationaryGenerator4::generateWonderCardIVs(u32 seed) const
{
    QVector<State> states;

    PokeRNG rng(seed);
    rng.advance(initialAdvances + offset);

    // Wondercard IVs [SEED] [IVS] [IVS]

    for (u32 cnt = 0; cnt < maxResults; cnt++, rng.next())
    {
        State currentState(initialAdvances + cnt);
        PokeRNG go(rng.getSeed());

        u16 iv1 = go.nextUShort();
        u16 iv2 = go.nextUShort();

        currentState.setIVs(iv1, iv2);
        currentState.calculateHiddenPower();

        if (filter.compareIVs(currentState))
        {
            currentState.setSeed(iv1);
            states.append(currentState);
        }
    }

    return states;
}
