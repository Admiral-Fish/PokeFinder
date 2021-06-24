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

StationaryGenerator4::StationaryGenerator4(u32 initialAdvances, u32 maxAdvances, u16 tid, u16 sid, u8 genderRatio, Method method,
                                           const StateFilter &filter) :
    StationaryGenerator(initialAdvances, maxAdvances, tid, sid, genderRatio, method, filter)
{
}

std::vector<State> StationaryGenerator4::generate(u32 seed) const
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
        return std::vector<State>();
    }
}

std::vector<State> StationaryGenerator4::generateMethod1(u32 seed) const
{
    std::vector<State> states;

    PokeRNG rng(seed);
    rng.advance(initialAdvances + offset);

    // Method 1 [SEED] [PID] [PID] [IVS] [IVS]
    for (u32 cnt = 0; cnt <= maxAdvances; cnt++, rng.next())
    {
        State state(initialAdvances + cnt);
        PokeRNG go(rng.getSeed());

        u16 low = go.nextUShort();
        u16 high = go.nextUShort();

        u16 iv1 = go.nextUShort();
        u16 iv2 = go.nextUShort();

        state.setPID(high, low);
        state.setShiny<8>(tsv, high ^ low);
        state.setAbility(low & 1);
        state.setGender(low & 255, genderRatio);
        state.setNature(state.getPID() % 25);

        state.setIVs(iv1, iv2);
        state.calculateHiddenPower();

        if (filter.compareState(state))
        {
            state.setSeed(low);
            states.emplace_back(state);
        }
    }

    return states;
}

std::vector<State> StationaryGenerator4::generateMethodJ(u32 seed) const
{
    std::vector<State> states;

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

    for (u32 cnt = 0; cnt <= maxAdvances; cnt++, rng.next())
    {
        State state(initialAdvances + cnt);
        PokeRNG go(rng.getSeed());

        u16 first = go.nextUShort();
        u32 pid = 0;
        switch (lead)
        {
        case Lead::None:
            // Get hunt nature
            state.setNature(first / 0xa3e);
            if (!filter.compareNature(state))
            {
                continue;
            }

            // Begin search for valid pid
            do
            {
                u16 low = go.nextUShort();
                u16 high = go.nextUShort();
                pid = (high << 16) | low;
            } while (pid % 25 != state.getNature());

            break;
        case Lead::Synchronize:
            if ((first >> 15) == 0) // Successful synch
            {
                state.setNature(synchNature);
            }
            else // Failed synch
            {
                state.setNature(go.nextUShort() / 0xa3e);
            }

            if (!filter.compareNature(state))
            {
                continue;
            }

            // Begin search for valid pid
            do
            {
                u16 low = go.nextUShort();
                u16 high = go.nextUShort();
                pid = (high << 16) | low;
            } while (pid % 25 != state.getNature());

            break;
        default: // Default to cover all cute charm cases
            if ((first / 0x5556) != 0) // Successful cute charm
            {
                // Get nature
                state.setNature(go.nextUShort() / 0xa3e);

                if (!filter.compareNature(state))
                {
                    continue;
                }

                // Cute charm doesn't hunt for a valid PID, just uses buffer and target nature
                pid = buffer + state.getNature();
            }
            else // Failed cute charm
            {
                // Get nature
                state.setNature(go.nextUShort() / 0xa3e);

                if (!filter.compareNature(state))
                {
                    continue;
                }

                // Begin search for valid pid
                do
                {
                    u16 low = go.nextUShort();
                    u16 high = go.nextUShort();
                    pid = (high << 16) | low;
                } while (pid % 25 != state.getNature());
            }

            break;
        }

        state.setAbility(pid & 1);
        state.setGender(pid & 255, genderRatio);
        state.setPID(pid);
        state.setShiny<8>(tsv, (pid >> 16) ^ (pid & 0xffff));

        u16 iv1 = go.nextUShort();
        u16 iv2 = go.nextUShort();

        state.setIVs(iv1, iv2);
        state.calculateHiddenPower();

        if (filter.compareState(state))
        {
            state.setSeed(first);
            states.emplace_back(state);
        }
    }

    return states;
}

std::vector<State> StationaryGenerator4::generateMethodK(u32 seed) const
{
    std::vector<State> states;

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

    for (u32 cnt = 0; cnt <= maxAdvances; cnt++, rng.next())
    {
        State state(initialAdvances + cnt);
        PokeRNG go(rng.getSeed());

        u16 first = go.nextUShort();
        u32 pid = 0;
        switch (lead)
        {
        case Lead::None:
            // Get hunt nature
            state.setNature(first % 25);

            if (!filter.compareNature(state))
            {
                continue;
            }

            // Begin search for valid pid
            do
            {
                u16 low = go.nextUShort();
                u16 high = go.nextUShort();
                pid = (high << 16) | low;
            } while (pid % 25 != state.getNature());

            break;
        case Lead::Synchronize:
            if ((first & 1) == 0) // Successful synch
            {
                state.setNature(synchNature);
            }
            else // Failed synch
            {
                state.setNature(go.nextUShort() % 25);
            }

            if (!filter.compareNature(state))
            {
                continue;
            }

            // Begin search for valid pid
            do
            {
                u16 low = go.nextUShort();
                u16 high = go.nextUShort();
                pid = (high << 16) | low;
            } while (pid % 25 != state.getNature());

            break;
        default: // Default to cover all cute charm cases
            if ((first % 3) != 0) // Successfull cute charm
            {
                // Get hunt nature
                state.setNature(go.nextUShort() % 25);

                if (!filter.compareNature(state))
                {
                    continue;
                }

                pid = buffer + state.getNature();
            }
            else // Failed cutecharm
            {
                // Get hunt nature
                state.setNature(go.nextUShort() % 25);

                if (!filter.compareNature(state))
                {
                    continue;
                }

                // Begin search for valid pid
                do
                {
                    u16 low = go.nextUShort();
                    u16 high = go.nextUShort();
                    pid = (high << 16) | low;
                } while (pid % 25 != state.getNature());
            }

            break;
        }

        state.setAbility(pid & 1);
        state.setGender(pid & 255, genderRatio);
        state.setPID(pid);
        state.setShiny<8>(tsv, (pid >> 16) ^ (pid & 0xffff));

        u16 iv1 = go.nextUShort();
        u16 iv2 = go.nextUShort();

        state.setIVs(iv1, iv2);
        state.calculateHiddenPower();

        if (filter.compareState(state))
        {
            state.setSeed(first);
            states.emplace_back(state);
        }
    }

    return states;
}

std::vector<State> StationaryGenerator4::generateWonderCardIVs(u32 seed) const
{
    std::vector<State> states;

    PokeRNG rng(seed);
    rng.advance(initialAdvances + offset);

    // Wondercard IVs [SEED] [IVS] [IVS]

    for (u32 cnt = 0; cnt <= maxAdvances; cnt++, rng.next())
    {
        State state(initialAdvances + cnt);
        PokeRNG go(rng.getSeed());

        u16 iv1 = go.nextUShort();
        u16 iv2 = go.nextUShort();

        state.setIVs(iv1, iv2);
        state.calculateHiddenPower();

        if (filter.compareIVs(state))
        {
            state.setSeed(iv1);
            states.emplace_back(state);
        }
    }

    return states;
}
