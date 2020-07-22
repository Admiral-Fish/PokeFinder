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

#include "WildGenerator3.hpp"
#include <Core/Enum/Encounter.hpp>
#include <Core/Enum/Lead.hpp>
#include <Core/Enum/Method.hpp>
#include <Core/Parents/Filters/StateFilter.hpp>
#include <Core/Parents/States/WildState.hpp>
#include <Core/RNG/LCRNG.hpp>
#include <Core/Util/EncounterSlot.hpp>
#include <functional>

WildGenerator3::WildGenerator3(u32 initialAdvances, u32 maxResults, u16 tid, u16 sid, u8 genderRatio, Method method,
                               const StateFilter &filter) :
    WildGenerator(initialAdvances, maxResults, tid, sid, genderRatio, method, filter)
{
}

QVector<WildState> WildGenerator3::generate(u32 seed) const
{
    QVector<WildState> states;

    PokeRNG rng(seed);
    rng.advance(initialAdvances + offset);

    u16 rate = encounterArea.getEncounterRate() * 16;
    bool rock = rate == 2880;

    bool cuteCharmFlag = false;
    std::function<bool(u32)> cuteCharm;
    switch (lead)
    {
    case Lead::CuteCharm125F:
        cuteCharm = [](u32 pid) { return (pid & 0xff) < 31; };
        break;
    case Lead::CuteCharm875M:
        cuteCharm = [](u32 pid) { return (pid & 0xff) >= 31; };
        break;
    case Lead::CuteCharm25F:
        cuteCharm = [](u32 pid) { return (pid & 0xff) < 63; };
        break;
    case Lead::CuteCharm75M:
        cuteCharm = [](u32 pid) { return (pid & 0xff) >= 63; };
        break;
    case Lead::CuteCharm50F:
        cuteCharm = [](u32 pid) { return (pid & 0xff) < 127; };
        break;
    case Lead::CuteCharm50M:
        cuteCharm = [](u32 pid) { return (pid & 0xff) >= 127; };
        break;
    case Lead::CuteCharm75F:
        cuteCharm = [](u32 pid) { return (pid & 0xff) < 191; };
        break;
    case Lead::CuteCharm25M:
        cuteCharm = [](u32 pid) { return (pid & 0xff) >= 191; };
        break;
    default:
        cuteCharm = [](u32 pid) {
            (void)pid;
            return true;
        };
    }

    for (u32 cnt = 0; cnt < maxResults; cnt++, rng.next())
    {
        WildState currentState(initialAdvances + cnt);
        PokeRNG go(rng.getSeed());

        switch (encounter)
        {
        case Encounter::RockSmash:
            if (!rock)
            {
                go.next();
            }
            if (((go.nextUShort()) % 2880) >= rate)
            {
                continue;
            }

            currentState.setEncounterSlot(EncounterSlot::hSlot(go.nextUShort(), encounter));
            if (!filter.compareEncounterSlot(currentState))
            {
                continue;
            }

            currentState.setLevel(encounterArea.calcLevel(currentState.getEncounterSlot(), go.nextUShort()));
            break;
        case Encounter::SafariZone:
            currentState.setEncounterSlot(EncounterSlot::hSlot(go.nextUShort(), encounter));
            if (!filter.compareEncounterSlot(currentState))
            {
                continue;
            }

            currentState.setLevel(encounterArea.calcLevel(currentState.getEncounterSlot()));
            go.advance(2);
            break;
        case Encounter::Grass:
            go.next();
            currentState.setEncounterSlot(EncounterSlot::hSlot(go.nextUShort(), encounter));
            if (!filter.compareEncounterSlot(currentState))
            {
                continue;
            }

            currentState.setLevel(encounterArea.calcLevel(currentState.getEncounterSlot()));
            go.advance(1);
            break;
        case Encounter::Surfing:
        case Encounter::OldRod:
        case Encounter::GoodRod:
        case Encounter::SuperRod:
            go.next();
            currentState.setEncounterSlot(EncounterSlot::hSlot(go.nextUShort(), encounter));
            if (!filter.compareEncounterSlot(currentState))
            {
                continue;
            }

            currentState.setLevel(encounterArea.calcLevel(currentState.getEncounterSlot(), go.nextUShort()));
            break;
        default:
            break;
        }

        if (lead == Lead::None)
        {
            currentState.setNature(go.nextUShort() % 25);
        }
        else if (lead == Lead::Synchronize)
        {
            if ((go.nextUShort() & 1) == 0) // state is synchable so set nature to synch nature
            {
                currentState.setNature(synchNature);
            }
            else // Synch failed so grab hunt nature from next RNG call
            {
                currentState.setNature(go.nextUShort() % 25);
            }
        }
        else // Covers cutecharm
        {
            cuteCharmFlag = go.nextUShort() % 3 > 0;
            currentState.setNature(go.nextUShort() % 25);
        }

        if (!filter.compareNature(currentState))
        {
            continue;
        }

        // Now search for a Method 124 PID that matches our hunt nature
        u32 pid;
        do
        {
            u16 low = go.nextUShort();
            u16 high = go.nextUShort();
            pid = (high << 16) | low;
        } while (pid % 25 != currentState.getNature() || (cuteCharmFlag && !cuteCharm(pid)));

        currentState.setPID(pid);
        currentState.setAbility(pid & 1);
        currentState.setGender(pid & 255, genderRatio);
        currentState.setShiny(tsv, (pid & 0xffff) ^ (pid >> 16), 8);

        // Valid PID is found now time to generate IVs
        u16 iv1;
        u16 iv2;
        if (method == Method::MethodH1)
        {
            iv1 = go.nextUShort();
            iv2 = go.nextUShort();
        }
        else if (method == Method::MethodH2)
        {
            go.next();
            iv1 = go.nextUShort();
            iv2 = go.nextUShort();
        }
        else
        {
            iv1 = go.nextUShort();
            go.next();
            iv2 = go.nextUShort();
        }
        currentState.setIVs(iv1, iv2);
        currentState.calculateHiddenPower();

        if (filter.compareState(currentState))
        {
            states.append(currentState);
        }
    }

    return states;
}

void WildGenerator3::setEncounterArea(const EncounterArea3 &encounterArea)
{
    this->encounterArea = encounterArea;
}
