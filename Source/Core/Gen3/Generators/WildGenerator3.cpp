/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2024 by Admiral_Fish, bumba, and EzPzStreamz
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
#include <Core/Parents/PersonalInfo.hpp>
#include <Core/Parents/Slot.hpp>
#include <Core/Parents/States/WildState.hpp>
#include <Core/RNG/LCRNG.hpp>
#include <Core/Util/EncounterSlot.hpp>
#include <Core/Util/Utilities.hpp>

static u8 unownLetter(u32 pid)
{
    return (((pid & 0x3000000) >> 18) | ((pid & 0x30000) >> 12) | ((pid & 0x300) >> 6) | (pid & 0x3)) % 0x1c;
}

WildGenerator3::WildGenerator3(u32 initialAdvances, u32 maxAdvances, u32 offset, Method method, Lead lead, bool feebasTile,
                               const EncounterArea3 &area, const Profile3 &profile, const WildStateFilter &filter) :
    WildGenerator(initialAdvances, maxAdvances, offset, method, lead, area, profile, filter), feebasTile(feebasTile)
{
}

std::vector<WildGeneratorState> WildGenerator3::generate(u32 seed) const
{
    std::vector<WildGeneratorState> states;

    auto modifiedSlots = area.getSlots(lead);
    u16 rate = area.getRate() * 16;
    bool rock = (profile.getVersion() & Game::RSE) != Game::None && area.getEncounter() == Encounter::RockSmash;
    bool feebas = area.feebasLocation(profile.getVersion());
    bool safari = area.safariZone(profile.getVersion());
    bool tanoby = area.tanobyChamber(profile.getVersion());

    bool cuteCharm = false;
    auto cuteCharmCheck = [this](const PersonalInfo *info, u32 pid) {
        if (lead == Lead::CuteCharmF)
        {
            return (pid & 0xff) >= info->getGender();
        }
        return (pid & 0xff) < info->getGender();
    };

    PokeRNG rng(seed, initialAdvances + offset);
    for (u32 cnt = 0; cnt <= maxAdvances; cnt++, rng.next())
    {
        PokeRNG go(rng);

        // RSE uses the main rng to check for rock smash encounters
        if (rock && go.nextUShort(2880) >= rate)
        {
            continue;
        }

        u8 encounterSlot;
        if (feebas && go.nextUShort(100) < 50 && feebasTile)
        {
            if (area.getEncounter() == Encounter::OldRod)
            {
                encounterSlot = 2;
            }
            else if (area.getEncounter() == Encounter::GoodRod)
            {
                encounterSlot = 3;
            }
            else
            {
                encounterSlot = 5;
            }
        }
        else
        {
            if ((lead == Lead::MagnetPull || lead == Lead::Static) && go.nextUShort(2) == 0 && !modifiedSlots.empty())
            {
                encounterSlot = modifiedSlots[go.nextUShort(modifiedSlots.count)];
            }
            else
            {
                encounterSlot = EncounterSlot::hSlot(go.nextUShort(100), area.getEncounter());
            }
        }

        if (!filter.compareEncounterSlot(encounterSlot))
        {
            continue;
        }

        // Modify level based on pressure if necessary
        u8 level = area.calculateLevel(encounterSlot, go, lead == Lead::Pressure);

        const Slot &slot = area.getPokemon(encounterSlot);
        const PersonalInfo *info = slot.getInfo();
        if (lead == Lead::CuteCharmM || lead == Lead::CuteCharmF)
        {
            switch (info->getGender())
            {
            case 0:
            case 254:
            case 255:
                cuteCharm = false;
                break;
            default:
                cuteCharm = go.nextUShort(3) != 0;
                break;
            }
        }

        // Safari zone in RSE takes an extra RNG call to determine shuffling of pokeblocks
        if (safari)
        {
            go.next();
        }

        u8 nature;
        u32 pid;
        if (tanoby)
        {
            do
            {
                u16 low = go.nextUShort();
                u16 high = go.nextUShort();
                pid = (low << 16) | high;
            } while (unownLetter(pid) != slot.getForm());

            nature = pid % 25;
            if (!filter.compareNature(nature))
            {
                continue;
            }
        }
        else
        {
            if (lead <= Lead::SynchronizeEnd)
            {
                nature = go.nextUShort(2) == 0 ? toInt(lead) : go.nextUShort(25);
            }
            else
            {
                nature = go.nextUShort(25);
            }

            if (!filter.compareNature(nature))
            {
                continue;
            }

            do
            {
                u16 low = go.nextUShort();
                u16 high = go.nextUShort();
                pid = (high << 16) | low;

            } while (pid % 25 != nature || (cuteCharm && !cuteCharmCheck(info, pid)));
        }

        if (method == Method::Method2)
        {
            go.next();
        }
        u16 iv1 = go.nextUShort();
        if (method == Method::Method4)
        {
            go.next();
        }
        u16 iv2 = go.nextUShort();

        std::array<u8, 6> ivs;
        ivs[0] = iv1 & 31;
        ivs[1] = (iv1 >> 5) & 31;
        ivs[2] = (iv1 >> 10) & 31;
        ivs[3] = (iv2 >> 5) & 31;
        ivs[4] = (iv2 >> 10) & 31;
        ivs[5] = iv2 & 31;

        WildGeneratorState state(initialAdvances + cnt, pid, ivs, pid & 1, Utilities::getGender(pid, info), level, nature,
                                 Utilities::getShiny<true>(pid, tsv), encounterSlot, 0, slot.getSpecie(), slot.getForm(), info);
        if (filter.compareState(state))
        {
            states.emplace_back(state);
        }
    }

    return states;
}
