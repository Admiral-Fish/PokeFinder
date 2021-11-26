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

#include "WildGenerator8.hpp"
#include <Core/Enum/Method.hpp>
#include <Core/RNG/RNGList.hpp>
#include <Core/RNG/Xorshift.hpp>
#include <Core/Util/EncounterSlot.hpp>

WildGenerator8::WildGenerator8(u32 initialAdvances, u32 maxAdvances, u16 tid, u16 sid, u8 genderRatio, const StateFilter &filter) :
    WildGenerator(initialAdvances, maxAdvances, tid, sid, genderRatio, Method::Null, filter)
{
}

// void WildGenerator8::setEncounterArea(const EncounterArea8 &encounterArea)
//{
//    this->encounterArea = encounterArea;
//}

std::vector<WildState> WildGenerator8::generate(u64 seed0, u64 seed1) const
{
    Xorshift rng(seed0, seed1);
    rng.advance(initialAdvances + offset);

    RNGList<u32, Xorshift, 128, 0> rngList(rng);

    std::vector<WildState> states;
    for (u32 cnt = 0; cnt <= maxAdvances; cnt++, rngList.advanceState())
    {
        WildState state(initialAdvances + cnt);
        Xorshift gen(rng);
        u32 slotPercent = rng.next(100);
        rngList.advance(85);
        switch (encounter)
        {
        case Encounter::Grass:
            state.setEncounterSlot(EncounterSlot::jSlot(slotPercent * 656, encounter));
            if (!filter.compareEncounterSlot(state))
            {
                continue;
            }

            // state.setLevel(encounterArea.calcLevel(state.getEncounterSlot()));
            break;
        case Encounter::Surfing:
            state.setEncounterSlot(EncounterSlot::jSlot(slotPercent * 656, encounter));
            if (!filter.compareEncounterSlot(state))
            {
                continue;
            }
            rngList.advance(3);

            // state.setLevel(encounterArea.calcLevel(state.getEncounterSlot(), go.nextUShort<true>()));
            break;
        case Encounter::OldRod:
        case Encounter::GoodRod:
        case Encounter::SuperRod: // TODO fishing
                                  // if ((first / 656) >= thresh)
                                  //{
                                  //    continue;
                                  //}

            // state.setEncounterSlot(EncounterSlot::jSlot(go.nextUShort<true>(), encounter));
            // if (!filter.compareEncounterSlot(state))
            //{
            //    continue;
            //}

            // state.setLevel(encounterArea.calcLevel(state.getEncounterSlot(), go.nextUShort<true>()));
            // occidentary += platinum ? 2 : 6; // Compensate for the game's advances after the battle ends
            // break;
        default:
            break;
        }

        rngList.getValue(); // EC call
        u32 sidtid = rngList.getValue();
        u32 pid = rngList.getValue();
        state.setPID(pid);

        u16 fakeXor = (sidtid >> 16) ^ (sidtid & 0xffff) ^ (pid >> 16) ^ (pid & 0xffff);

        if (fakeXor < 16) // Force shiny
        {
            u8 shinyType = fakeXor == 0 ? 2 : 1;
            state.setShiny(shinyType);
        }
        else // Force non shiny
        {
            state.setShiny(0);
        }

        for (u8 i = 0; i < 6; i++)
        {
            u32 ivRand = rngList.getValue();
            state.setIV(i, ivRand - (ivRand / 32) * 32);
        }

        u32 abilityRand = rngList.getValue();
        state.setAbility(abilityRand - (abilityRand / 2) * 2);

        // if (false)
        //{ // TODO: add unown check
        // gen.next(); // Form call (unown?)
        //}

        if (genderRatio == 255)
        {
            state.setGender(2);
        }
        else if (genderRatio == 254)
        {
            state.setGender(1);
        }
        else if (genderRatio == 0)
        {
            state.setGender(0);
        }
        else
        {
            u32 genderRand = rngList.getValue();
            u8 gender = (genderRand - (genderRand / 253) * 253) + 1 < genderRatio;
            state.setGender(gender);
        }

        u32 natureRand = rngList.getValue();
        state.setNature(natureRand - (natureRand / 25) * 25);

        rngList.getValue(); // friendship
        rngList.getValue(); // height
        rngList.getValue(); // weight

        u32 itemRand = rngList.getValue();
        state.setItem(itemRand - (itemRand / 100) * 100);

        if (filter.comparePID(state) && filter.compareIV(state))
        {
            states.emplace_back(state);
        }
    }

    return states;
}
