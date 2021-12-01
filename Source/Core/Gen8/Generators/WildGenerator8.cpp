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
#include <Core/Enum/Game.hpp>
#include <Core/Enum/Method.hpp>
#include <Core/Parents/PersonalLoader.hpp>
#include <Core/RNG/RNGList.hpp>
#include <Core/RNG/Xorshift.hpp>
#include <Core/Util/EncounterSlot.hpp>

WildGenerator8::WildGenerator8(u32 initialAdvances, u32 maxAdvances, u16 tid, u16 sid, const StateFilter &filter) :
    WildGenerator(initialAdvances, maxAdvances, tid, sid, genderRatio, Method::Null, filter)
{
}

void WildGenerator8::setEncounterArea(const EncounterArea8 &encounterArea)
{
    this->encounterArea = encounterArea;
}

std::vector<WildState> WildGenerator8::generate(u64 seed0, u64 seed1) const
{
    Xorshift rng(seed0, seed1);
    rng.advance(initialAdvances + offset);

    std::vector<WildState> states;
    for (u32 cnt = 0; cnt <= maxAdvances; cnt++, rng.next())
    {
        WildState state(initialAdvances + cnt);
        Xorshift gen(rng);
        u8 slotPercent = gen.next<0, 100>();
        gen.advance(84);
        switch (encounter)
        {
        case Encounter::Grass:
            state.setEncounterSlot(EncounterSlot::bdspSlot(slotPercent, encounter));
            if (!filter.compareEncounterSlot(state))
            {
                continue;
            }

            state.setLevel(encounterArea.calcLevel(state.getEncounterSlot()));
            break;
        case Encounter::Surfing:
            state.setEncounterSlot(EncounterSlot::bdspSlot(slotPercent, encounter));
            if (!filter.compareEncounterSlot(state))
            {
                continue;
            }

            state.setLevel(encounterArea.calcLevel(state.getEncounterSlot(), gen.next<0, 10000>()));
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

        PersonalInfo info = PersonalLoader::getPersonal(Game::BDSP, encounterArea.getPokemon()[state.getEncounterSlot()].getSpecie(), 0);

        gen.next(); // EC call
        u32 sidtid = gen.next();
        u32 pid = gen.next();

        u16 fakeXor = (sidtid >> 16) ^ (sidtid & 0xffff) ^ (pid >> 16) ^ (pid & 0xffff);
        u16 psv = ((pid >> 16) ^ (pid & 0xffff)) >> 4;
        u16 realXor = (pid >> 16) ^ (pid & 0xffff) ^ tid ^ sid;

        if (fakeXor < 16) // Force shiny
        {
            u8 shinyType = fakeXor == 0 ? 2 : 1;
            state.setShiny(shinyType);
            if (fakeXor != realXor)
            {
                u16 high = (pid & 0xFFFF) ^ tid ^ sid ^ (2 - shinyType);
                pid = (high << 16) | (pid & 0xFFFF);
            }
        }
        else // Force non shiny
        {
            state.setShiny(0);
            if (psv == tsv)
            {
                pid ^= 0x10000000;
            }
        }

        state.setPID(pid);

        for (u8 i = 0; i < 6; i++)
        {
            state.setIV(i, gen.next() % 32);
        }

        state.setAbility(gen.next() % 2);

        if (encounterArea.getLocation() > 222 && encounterArea.getLocation() < 244)
        { // TODO: add unown check
            u32 unownForm = gen.next() % 28; // Form call
        }

        if (lead == Lead::CuteCharm)
        {
            if (info.getGender() + 2 > 2)
            {
                u8 charmRand = gen.next() % 3;
                if (charmRand > 0)
                {
                    if (lead == Lead::CuteCharmFemale)
                    {
                        state.setGender(0);
                    }
                    else
                    {
                        state.setGender(1);
                    }
                }
                else
                {
                    u8 gender = (gen.next() % 253) + 1 < genderRatio;
                    state.setGender(gender);
                }
            }
            else
            {
                if (info.getGender() == 255)
                {
                    state.setGender(2);
                }
                else if (info.getGender() == 254)
                {
                    state.setGender(1);
                }
                else if (info.getGender() == 0)
                {
                    state.setGender(0);
                }
            }
        }
        else
        {
            if (info.getGender() == 255)
            {
                state.setGender(2);
            }
            else if (info.getGender() == 254)
            {
                state.setGender(1);
            }
            else if (info.getGender() == 0)
            {
                state.setGender(0);
            }
            else
            {
                u8 gender = (gen.next() % 253) + 1 < genderRatio;
                state.setGender(gender);
            }
        }

        if (lead == Lead::Synchronize)
        {
            state.setNature(synchNature);
        }
        else
        {
            state.setNature(gen.next() % 25);
        }

        gen.advance(4); // 2 calls height, 2 calls weight

        state.setItem(gen.next<0, 100>());

        if (filter.comparePID(state) && filter.compareIV(state))
        {
            states.emplace_back(state);
        }
    }

    return states;
}
