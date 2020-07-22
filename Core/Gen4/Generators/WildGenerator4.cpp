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
#include <Core/Parents/Filters/StateFilter.hpp>
#include <Core/RNG/LCRNG.hpp>
#include <Core/Util/EncounterSlot.hpp>

WildGenerator4::WildGenerator4(u32 initialAdvances, u32 maxResults, u16 tid, u16 sid, u8 genderRatio, Method method,
                               const StateFilter &filter) :
    WildGenerator(initialAdvances, maxResults, tid, sid, genderRatio, method, filter)
{
}

QVector<WildState> WildGenerator4::generate(u32 seed) const
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
        return QVector<WildState>();
    }
}

void WildGenerator4::setEncounterArea(const EncounterArea4 &encounterArea)
{
    this->encounterArea = encounterArea;
}

QVector<WildState> WildGenerator4::generateMethodJ(u32 seed) const
{
    QVector<WildState> states;

    PokeRNG rng(seed);
    rng.advance(initialAdvances + offset);

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

    for (u32 cnt = 0; cnt < maxResults; cnt++, rng.next())
    {
        WildState state(initialAdvances + cnt);

        PokeRNG go(rng.getSeed());

        u16 first = go.nextUShort(); // Encounter slot call, nibble call for fishing
        state.setSeed(first);

        switch (encounter)
        {
        case Encounter::Grass:
            state.setEncounterSlot(EncounterSlot::jSlot(first, encounter));
            if (!filter.compareEncounterSlot(state))
            {
                continue;
            }

            state.setLevel(encounterArea.calcLevel(state.getEncounterSlot()));
            break;
        case Encounter::Surfing:
            state.setEncounterSlot(EncounterSlot::jSlot(first, encounter));
            if (!filter.compareEncounterSlot(state))
            {
                continue;
            }

            state.setLevel(encounterArea.calcLevel(state.getEncounterSlot(), go.nextUShort()));
            break;
        case Encounter::OldRod:
        case Encounter::GoodRod:
        case Encounter::SuperRod:
            if ((first / 656) >= thresh)
            {
                continue;
            }

            state.setEncounterSlot(EncounterSlot::jSlot(go.nextUShort(), encounter));
            if (!filter.compareEncounterSlot(state))
            {
                continue;
            }

            state.setLevel(encounterArea.calcLevel(state.getEncounterSlot(), go.nextUShort()));
            break;
        default:
            break;
        }

        u32 pid = 0;
        switch (lead)
        {
        case Lead::None:
            // Get hunt nature
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
                pid = static_cast<u32>((high << 16) | low);
            } while (pid % 25 != state.getNature());

            break;
        case Lead::Synchronize:
            if ((go.nextUShort() >> 15) == 0) // Successful synch
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
                pid = static_cast<u32>((high << 16) | low);
            } while (pid % 25 != state.getNature());

            break;
        default: // Default to cover all cute charm cases
            if ((go.nextUShort() / 0x5556) != 0) // Successful cute charm
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
                    pid = static_cast<u32>((high << 16) | low);
                } while (pid % 25 != state.getNature());
            }

            break;
        }

        state.setPID(pid);
        state.setAbility(pid & 1);
        state.setGender(pid & 255, genderRatio);
        state.setShiny(tsv, (pid >> 16) ^ (pid & 0xffff), 8);

        u16 iv1 = go.nextUShort();
        u16 iv2 = go.nextUShort();

        state.setIVs(iv1, iv2);
        state.calculateHiddenPower();

        if (filter.compareState(state))
        {
            states.append(state);
        }
    }

    return states;
}

QVector<WildState> WildGenerator4::generateMethodK(u32 seed) const
{
    QVector<WildState> states;

    PokeRNG rng(seed);
    rng.advance(initialAdvances + offset);

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

    for (u32 cnt = 0; cnt < maxResults; cnt++, rng.next())
    {
        WildState state(initialAdvances + cnt);

        PokeRNG go(rng.getSeed());

        u16 first = go.nextUShort(); // Encounter slot, nibble for fishing, blank or item for rock smash
        state.setSeed(first);

        switch (encounter)
        {
        case Encounter::Grass:
            state.setEncounterSlot(EncounterSlot::kSlot(first, encounter));
            if (!filter.compareEncounterSlot(state))
            {
                continue;
            }

            state.setLevel(encounterArea.calcLevel(state.getEncounterSlot()));
            break;
        case Encounter::Surfing:
            state.setEncounterSlot(EncounterSlot::kSlot(first, encounter));
            if (!filter.compareEncounterSlot(state))
            {
                continue;
            }

            state.setLevel(encounterArea.calcLevel(state.getEncounterSlot(), go.nextUShort()));
            break;
        case Encounter::OldRod:
        case Encounter::GoodRod:
        case Encounter::SuperRod:
            if ((first % 100) >= thresh)
            {
                continue;
            }

            state.setEncounterSlot(EncounterSlot::kSlot(go.nextUShort(), encounter));
            if (!filter.compareEncounterSlot(state))
            {
                continue;
            }

            state.setLevel(encounterArea.calcLevel(state.getEncounterSlot()));
            go.advance(1);
            break;
        case Encounter::RockSmash:
            if (((go.nextUShort()) % 100) >= rate)
            {
                continue;
            }

            state.setEncounterSlot(EncounterSlot::kSlot(go.nextUShort(), encounter));
            if (!filter.compareEncounterSlot(state))
            {
                continue;
            }

            state.setLevel(encounterArea.calcLevel(state.getEncounterSlot(), go.nextUShort()));
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
                pid = static_cast<u32>((high << 16) | low);
            } while (pid % 25 != state.getNature());

            break;
        case Lead::Synchronize:
            if ((go.nextUShort() & 1) == 0) // Successful synch
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
                pid = static_cast<u32>((high << 16) | low);
            } while (pid % 25 != state.getNature());

            break;
        default: // Default to cover all cute charm cases
            if ((go.nextUShort() % 3) != 0) // Successfull cute charm
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
                    pid = static_cast<u32>((high << 16) | low);
                } while (pid % 25 != state.getNature());
            }

            break;
        }

        state.setPID(pid);
        state.setAbility(pid & 1);
        state.setGender(pid & 255, genderRatio);
        state.setShiny(tsv, (pid >> 16) ^ (pid & 0xffff), 8);

        u16 iv1 = go.nextUShort();
        u16 iv2 = go.nextUShort();

        state.setIVs(iv1, iv2);
        state.calculateHiddenPower();

        if (filter.compareState(state))
        {
            states.append(state);
        }
    }

    return states;
}

QVector<WildState> WildGenerator4::generateChainedShiny(u32 seed) const
{
    QVector<WildState> states;

    PokeRNG rng(seed);
    rng.advance(initialAdvances + offset);

    for (u32 cnt = 0; cnt < maxResults; cnt++, rng.next())
    {
        WildState state(initialAdvances + cnt);

        PokeRNG go(rng.getSeed());
        u32 originSeed = go.next(); // TODO: is this necessary

        u16 low = go.nextUShort() & 7;
        u16 high = go.nextUShort() & 7;

        for (int i = 3; i < 16; i++)
        {
            low |= (go.nextUShort() & 1) << i;
        }
        high |= (low ^ tid ^ sid) & 0xfff8;

        u16 iv1 = go.nextUShort();
        u16 iv2 = go.nextUShort();

        state.setPID(high, low);
        state.setAbility(low & 1);
        state.setGender(low & 255, genderRatio);
        state.setNature(state.getPID() % 25);
        state.setShiny(true);

        state.setIVs(iv1, iv2);
        state.calculateHiddenPower();

        if (filter.compareState(state))
        {
            state.setSeed(originSeed);
            states.append(state);
        }
    }

    return states;
}
