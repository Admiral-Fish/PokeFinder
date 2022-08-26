/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2022 by Admiral_Fish, bumba, and EzPzStreamz
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
#include <Core/Enum/Game.hpp>
#include <Core/Enum/Lead.hpp>
#include <Core/Enum/Method.hpp>
#include <Core/Gen3/EncounterArea3.hpp>
#include <Core/Gen3/States/WildState3.hpp>
#include <Core/Parents/PersonalInfo.hpp>
#include <Core/Parents/Slot.hpp>
#include <Core/Parents/States/WildState.hpp>
#include <Core/RNG/LCRNG.hpp>
#include <Core/Util/EncounterSlot.hpp>

WildGenerator3::WildGenerator3(u32 initialAdvances, u32 maxAdvances, u32 offset, u16 tid, u16 sid, Game version, Method method,
                               Encounter encounter, Lead lead, const WildStateFilter3 &filter) :
    WildGenerator<WildStateFilter3>(initialAdvances, maxAdvances, offset, tid, sid, version, method, encounter, lead, filter)
{
}

std::vector<WildGeneratorState3> WildGenerator3::generate(u32 seed, const EncounterArea3 &encounterArea) const
{
    std::vector<WildGeneratorState3> states;

    PokeRNG rng(seed);
    rng.advance(initialAdvances + offset);

    u16 rate = encounterArea.getRate() * 16;
    bool rse = (version & Game::RSE) != Game::None;
    bool safari = encounterArea.safariZone(version);

    bool cuteCharmFlag = false;
    bool (*cuteCharm)(const PersonalInfo *, u32);
    if (lead == Lead::CuteCharmF)
    {
        cuteCharm = [](const PersonalInfo *info, u32 pid) { return (pid & 0xff) < info->getGender(); };
    }
    else if (lead == Lead::CuteCharmM)
    {
        cuteCharm = [](const PersonalInfo *info, u32 pid) { return (pid & 0xff) >= info->getGender(); };
    }

    for (u32 cnt = 0; cnt <= maxAdvances; cnt++, rng.next())
    {
        PokeRNG go(rng.getSeed());

        // RSE uses the main rng to check for rock smash encounters
        if (rse && encounter == Encounter::RockSmash && (go.nextUShort() % 2880) >= rate)
        {
            continue;
        }

        u8 encounterSlot = EncounterSlot::hSlot(go.nextUShort(), encounter);
        if (!filter.compareEncounterSlot(encounterSlot))
        {
            continue;
        }
        u8 level = encounterArea.calcLevel(encounterSlot, go.nextUShort());

        const Slot &slot = encounterArea.getPokemon(encounterSlot);
        const PersonalInfo *info = slot.getInfo();
        if (lead == Lead::CuteCharmM || lead == Lead::CuteCharmF)
        {
            switch (info->getGender())
            {
            case 0:
            case 254:
            case 255:
                cuteCharmFlag = false;
                break;
            default:
                cuteCharmFlag = (go.nextUShort() % 3) > 0;
                break;
            }
        }

        // Safari zone in RSE takes an extra RNG call to determine shuffling of pokeblocks
        if (safari)
        {
            go.next();
        }

        u8 nature;
        if (lead == Lead::Synchronize)
        {
            nature = (go.nextUShort() & 1) == 0 ? synchNature : go.nextUShort() % 25;
        }
        else
        {
            nature = go.nextUShort() % 25;
        }

        if (!filter.compareNature(nature))
        {
            continue;
        }

        u32 pid;
        do
        {
            u16 low = go.nextUShort();
            u16 high = go.nextUShort();
            pid = (high << 16) | low;
        } while (pid % 25 != nature || (cuteCharmFlag && !cuteCharm(info, pid)));

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

        WildGeneratorState3 state(initialAdvances + cnt, pid, iv1, iv2, tsv, level, encounterSlot, slot.getSpecie(), info);
        if (filter.compareState(state))
        {
            states.emplace_back(state);
        }
    }

    return states;
}
