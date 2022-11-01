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

#include "WildGenerator8.hpp"
#include <Core/Enum/Encounter.hpp>
#include <Core/Enum/Game.hpp>
#include <Core/Enum/Method.hpp>
#include <Core/Gen8/EncounterArea8.hpp>
#include <Core/Gen8/States/WildState8.hpp>
#include <Core/Parents/Slot.hpp>
#include <Core/RNG/RNGList.hpp>
#include <Core/RNG/Xorshift.hpp>
#include <Core/Util/EncounterSlot.hpp>

static u16 getItem(u8 rand, Lead lead, const PersonalInfo *info)
{
    constexpr u8 ItemTableRange[2][2] = { { 50, 5 }, { 60, 20 } };

    u8 thresh1 = ItemTableRange[lead == Lead::CompoundEyes ? 1 : 0][0];
    u8 thresh2 = ItemTableRange[lead == Lead::CompoundEyes ? 1 : 0][1];

    if (rand >= thresh1)
    {
        if (rand >= (thresh1 + thresh2))
        {
            return info->getItem(2);
        }
        else
        {
            return info->getItem(1);
        }
    }
    else
    {
        return info->getItem(0);
    }
}

WildGenerator8::WildGenerator8(u32 initialAdvances, u32 maxAdvances, u32 offset, u16 tid, u16 sid, Game version, Encounter encounter,
                               Lead lead, const WildStateFilter8 &filter) :
    WildGenerator(initialAdvances, maxAdvances, offset, tid, sid, version, Method::None, encounter, lead, filter)
{
}

std::vector<WildGeneratorState8> WildGenerator8::generate(u64 seed0, u64 seed1, const EncounterArea8 &encounterArea) const
{
    RNGList<u32, Xorshift, 128> rngList(seed0, seed1, initialAdvances + offset);
    auto rand = [](u32 prng) { return (prng % 0xffffffff) + 0x80000000; };

    bool encounterForce
        = lead == Lead::MagnetPull || lead == Lead::Static || lead == Lead::Harvest || lead == Lead::FlashFire || lead == Lead::StormDrain;
    std::vector<u8> modifiedSlots = encounterArea.getSlots(version, lead);

    std::vector<WildGeneratorState8> states;
    for (u32 cnt = 0; cnt <= maxAdvances; cnt++, rngList.advanceState())
    {
        u8 encounterSlot;
        if (encounterForce && (rngList.next() % 2) == 0 && !modifiedSlots.empty())
        {
            encounterSlot = modifiedSlots[rngList.next() % modifiedSlots.size()];
        }
        else
        {
            encounterSlot = EncounterSlot::bdspSlot(rngList.next() % 100, encounter);
        }

        if (!filter.compareEncounterSlot(encounterSlot))
        {
            continue;
        }

        if (encounterArea.getLocation() > 222 && encounterArea.getLocation() < 244)
        {
            // Unown form call
            rngList.advance(1); // rngList.getValue() % 1 for F/R/I/E/N/D, rngList.getValue() % 2 for !/?, rngList.getValue() % 20 otherwise
        }

        rngList.advance(84);

        u8 level;
        if (encounter == Encounter::Grass)
        {
            level = encounterArea.calculateLevel<false>(encounterSlot, rngList, lead == Lead::Pressure);
        }
        else
        {
            level = encounterArea.calculateLevel<true>(encounterSlot, rngList, lead == Lead::Pressure);
        }

        const Slot &slot = encounterArea.getPokemon(encounterSlot);
        const PersonalInfo *info = slot.getInfo();

        rngList.next(rand); // EC call
        u32 sidtid = rngList.next(rand);
        u32 pid = rngList.next(rand);

        u16 psv = (pid >> 16) ^ (pid & 0xffff);
        u16 fakeXor = (sidtid >> 16) ^ (sidtid & 0xffff) ^ psv;
        u8 shiny;
        if (fakeXor < 16) // Force shiny
        {
            shiny = fakeXor == 0 ? 2 : 1;

            u16 realXor = psv ^ tsv;
            u8 realShiny = realXor == 0 ? 2 : realXor < 16 ? 1 : 0;

            if (realShiny != shiny)
            {
                u16 high = (pid & 0xFFFF) ^ tsv ^ (2 - shiny);
                pid = (high << 16) | (pid & 0xFFFF);
            }
        }
        else // Force non shiny
        {
            shiny = 0;
            if ((psv ^ tsv) < 16)
            {
                pid ^= 0x10000000;
            }
        }

        std::array<u8, 6> ivs;
        for (u8 &iv : ivs)
        {
            iv = rngList.next(rand) % 32;
        }

        u8 ability = rngList.next(rand) % 2;

        u8 gender;
        switch (info->getGender())
        {
        case 255:
            gender = 2;
            break;
        case 254:
            gender = 1;
            break;
        case 0:
            gender = 0;
            break;
        default:
            if ((lead == Lead::CuteCharmF || lead == Lead::CuteCharmM) && (rngList.next(rand) % 3) != 0)
            {
                gender = lead == Lead::CuteCharmF ? 0 : 1;
            }
            else
            {
                gender = (rngList.next(rand) % 253) + 1 < info->getGender();
            }
            break;
        }

        u8 nature;
        if (lead <= Lead::SynchronizeEnd)
        {
            nature = toInt(lead);
        }
        else
        {
            nature = rngList.next(rand) % 25;
        }

        rngList.advance(4); // 2 calls height, 2 calls weight

        u16 item = getItem(rngList.next() % 100, lead, info);

        WildGeneratorState8 state(initialAdvances + cnt, encounterSlot, slot.getSpecie(), level, pid, shiny, ivs, ability, gender, nature,
                                  item, info);
        if (filter.compareState(state))
        {
            states.emplace_back(state);
        }
    }

    return states;
}
