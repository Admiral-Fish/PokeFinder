/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2021 by Admiral_Fish, bumba, and EzPzStreamz
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

#include "UnownGenerator4.hpp"
#include <Core/Enum/Encounter.hpp>
#include <Core/Enum/Lead.hpp>
#include <Core/Enum/Method.hpp>
#include <Core/Parents/Filters/StateFilter.hpp>
#include <Core/RNG/LCRNG.hpp>
#include <Core/Util/EncounterSlot.hpp>

UnownGenerator4::UnownGenerator4(u32 initialAdvances, u32 maxAdvances, u16 tid, u16 sid, u8 genderRatio, Method method,
                               const StateFilter &filter) :
    WildGenerator(initialAdvances, maxAdvances, tid, sid, genderRatio, method, filter)
{
}

std::vector<UnownState4> UnownGenerator4::generate(u32 seed) const
{
    switch (method)
    {
    case Method::MethodJ:
        return generateMethodJ(seed);
    default:
        return std::vector<UnownState4>();
    }
}

void UnownGenerator4::setEncounterArea(const u8 encounterArea)
{
    this->encounterArea = encounterArea;
}

std::vector<UnownState4> UnownGenerator4::generateMethodJ(u32 seed) const
{
    std::vector<UnownState4> states;

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
        UnownState4 state(initialAdvances + cnt);
        
        u32 occidentary = initialAdvances + cnt;
        PokeRNG go(rng.getSeed(), &occidentary);

        u16 first = go.nextUShort<true>(); // Encounter slot call, nibble call for fishing

        // state.setEncounterSlot(EncounterSlot::jSlot(first, encounter));
        // if (!filter.compareEncounterSlot(state))
        // {
            // continue;
        // }

        // state.setLevel(encounterArea.calcLevel(state.getEncounterSlot()));


        u32 pid = 0;
        switch (lead)
        {
        case Lead::None:
            // Get hunt nature
            state.setNature(go.nextUShort<true>() / 0xa3e);

            if (!filter.compareNature(state))
            {
                continue;
            }

            // Begin search for valid pid
            do
            {
                u16 low = go.nextUShort<true>();
                u16 high = go.nextUShort<true>();
                pid = static_cast<u32>((high << 16) | low);
            } while (pid % 25 != state.getNature());

            break;
        case Lead::Synchronize:
            if ((go.nextUShort<true>() >> 15) == 0) // Successful synch
            {
                state.setNature(synchNature);
            }
            else // Failed synch
            {
                state.setNature(go.nextUShort<true>() / 0xa3e);
            }

            if (!filter.compareNature(state))
            {
                continue;
            }

            // Begin search for valid pid
            do
            {
                u16 low = go.nextUShort<true>();
                u16 high = go.nextUShort<true>();
                pid = static_cast<u32>((high << 16) | low);
            } while (pid % 25 != state.getNature());

            break;
        default: // Default to cover all cute charm cases
            if ((go.nextUShort<true>() / 0x5556) != 0) // Successful cute charm
            {
                // Get nature
                state.setNature(go.nextUShort<true>() / 0xa3e);

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
                state.setNature(go.nextUShort<true>() / 0xa3e);

                if (!filter.compareNature(state))
                {
                    continue;
                }

                // Begin search for valid pid
                do
                {
                    u16 low = go.nextUShort<true>();
                    u16 high = go.nextUShort<true>();
                    pid = static_cast<u32>((high << 16) | low);
                } while (pid % 25 != state.getNature());
            }

            break;
        }

        state.setPID(pid);
        state.setAbility(pid & 1);
        state.setGender(pid & 255, genderRatio);
        state.setShiny<8>(tsv, (pid >> 16) ^ (pid & 0xffff));

        u16 iv1 = go.nextUShort<true>();
        u16 iv2 = go.nextUShort<true>();

        go.nextUShort<true>();
        if (encounterArea == 0) {
            state.setLetterIndex4(go.nextUShort<true>()%20);
        }
        else
        {
            state.setLetterIndex4b(go.nextUShort<true>()%2);
        }

        state.setIVs(iv1, iv2);
        state.calculateHiddenPower();

        if (filter.compareState(state) && filter.compareLetter(state))
        {
            state.setOccidentary(occidentary);
            state.setSeed(first);
            states.emplace_back(state);
        }
    }

    return states;
}

std::vector<UnownState4> UnownGenerator4::generateMethodK(u32 seed) const
{
    return std::vector<UnownState4>();
}

std::vector<UnownState4> UnownGenerator4::generateChainedShiny(u32 seed) const
{
    return std::vector<UnownState4>();
}
