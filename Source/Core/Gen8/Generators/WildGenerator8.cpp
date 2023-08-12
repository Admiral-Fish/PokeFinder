/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2023 by Admiral_Fish, bumba, and EzPzStreamz
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
#include <Core/Enum/Lead.hpp>
#include <Core/Enum/Method.hpp>
#include <Core/Gen8/EncounterArea8.hpp>
#include <Core/Parents/PersonalInfo.hpp>
#include <Core/Parents/States/WildState.hpp>
#include <Core/RNG/RNGList.hpp>
#include <Core/RNG/Xorshift.hpp>
#include <Core/Util/EncounterSlot.hpp>
#include <algorithm>

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

static u32 rand(u32 prng)
{
    return (prng % 0xffffffff) + 0x80000000;
}

WildGenerator8::WildGenerator8(u32 initialAdvances, u32 maxAdvances, u32 delay, Encounter encounter, Lead lead, const Profile8 &profile,
                               const WildStateFilter &filter) :
    WildGenerator(initialAdvances, maxAdvances, delay, Method::None, encounter, lead, profile, filter)
{
    tsv = (profile.getTID() & 0xFFF0) ^ profile.getSID();
}

std::vector<WildGeneratorState> WildGenerator8::generate(u64 seed0, u64 seed1, const EncounterArea8 &encounterArea) const
{
    RNGList<u32, Xorshift, 128> rngList(seed0, seed1, initialAdvances + delay);

    bool encounterForce
        = lead == Lead::MagnetPull || lead == Lead::Static || lead == Lead::Harvest || lead == Lead::FlashFire || lead == Lead::StormDrain;
    std::vector<u8> modifiedSlots = encounterArea.getSlots(lead);

    std::vector<WildGeneratorState> states;
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

        const Slot &slot = encounterArea.getPokemon(encounterSlot);
        u8 form = 0;
        if (slot.getSpecie() == 201)
        {
            form = encounterArea.unownForm(rngList.next());
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

        u32 ec = rngList.next(rand);
        u32 sidtid = rngList.next(rand);
        u32 pid = rngList.next(rand);

        u16 psv = (pid >> 16) ^ (pid & 0xfff0);
        u16 fakeXor = (sidtid >> 16) ^ (sidtid & 0xfff0) ^ psv;
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
        std::generate(ivs.begin(), ivs.end(), [&rngList] { return rngList.next(rand) % 32; });

        u8 ability = rngList.next(rand) % 2;

        const PersonalInfo *info = slot.getInfo();

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

        WildGeneratorState state(initialAdvances + cnt, ec, pid, ivs, ability, gender, level, nature, shiny, encounterSlot, item,
                                 slot.getSpecie(), form, info);
        if (filter.compareState(static_cast<const WildState &>(state)))
        {
            states.emplace_back(state);
        }
    }

    return states;
}
