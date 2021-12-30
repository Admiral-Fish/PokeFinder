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

#include "StaticGenerator8.hpp"
#include <Core/Enum/Game.hpp>
#include <Core/Enum/Method.hpp>
#include <Core/Parents/PersonalInfo.hpp>
#include <Core/Parents/PersonalLoader.hpp>
#include <Core/Parents/StaticTemplate.hpp>
#include <Core/RNG/RNGList.hpp>
#include <Core/RNG/Xoroshiro.hpp>
#include <Core/RNG/Xorshift.hpp>

StaticGenerator8::StaticGenerator8(u32 initialAdvances, u32 maxAdvances, u16 tid, u16 sid, const StateFilter &filter) :
    StaticGenerator(initialAdvances, maxAdvances, tid, sid, 0, Method::None, filter)
{
}

std::vector<StaticState> StaticGenerator8::generate(u64 seed0, u64 seed1, const StaticTemplate &parameters) const
{
    PersonalInfo info = PersonalLoader::getPersonal(Game::BDSP, parameters.getSpecies());

    Xorshift rng(seed0, seed1);
    rng.advance(initialAdvances + offset);

    RNGList<u32, Xorshift, 64, 0> rngList(rng);

    std::vector<StaticState> states;
    for (u32 cnt = 0; cnt <= maxAdvances; cnt++, rngList.advanceState())
    {
        StaticState state(initialAdvances + cnt);

        rngList.advance(1); // EC call
        u32 sidtid = rngList.getValue();
        u32 pid = rngList.getValue();

        u16 psv = (pid >> 16) ^ (pid & 0xffff);
        if (parameters.getShiny() == Shiny::Never)
        {
            state.setShiny(0);
            if ((psv ^ tsv) < 16)
            {
                pid ^= 0x10000000;
            }
        }
        else
        {
            u16 fakeXOR = (sidtid >> 16) ^ (sidtid & 0xffff) ^ psv;
            if (fakeXOR < 16) // Force shiny
            {
                u8 fakeShinyType = fakeXOR == 0 ? 2 : 1;

                u16 realXOR = psv ^ tsv;
                u8 realShinyType = realXOR == 0 ? 2 : realXOR < 16 ? 1 : 0;

                state.setShiny(fakeShinyType);
                if (realShinyType != fakeShinyType)
                {
                    u16 high = (pid & 0xFFFF) ^ tsv ^ (2 - fakeShinyType);
                    pid = (high << 16) | (pid & 0xFFFF);
                }
            }
            else // Force non shiny
            {
                state.setShiny(0);
                if (psv ^ tsv < 16)
                {
                    pid ^= 0x10000000;
                }
            }
        }
        state.setPID(pid);

        for (int i = 0; i < 6; i++)
        {
            state.setIV(i, 255);
        }

        // Assign IVs set by template
        for (int i = 0; i < parameters.getIVCount();)
        {
            u8 index = rngList.getValue() % 6;
            if (state.getIV(index) == 255)
            {
                state.setIV(index, 31);
                i++;
            }
        }

        for (int i = 0; i < 6; i++)
        {
            if (state.getIV(i) == 255)
            {
                state.setIV(i, rngList.getValue() % 32);
            }
        }

        if (parameters.getAbility() != 255)
        {
            state.setAbility(parameters.getAbility());
        }
        else
        {
            state.setAbility(rngList.getValue() % 2);
        }

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
        else if ((lead == Lead::CuteCharm || lead == Lead::CuteCharmFemale) && (rngList.getValue() % 3) > 0)
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
            u8 gender = (rngList.getValue() % 253) + 1 < info.getGender();
            state.setGender(gender);
        }

        if (lead == Lead::Synchronize)
        {
            state.setNature(synchNature);
        }
        else
        {
            state.setNature(rngList.getValue() % 25);
        }

        if (filter.comparePID(state) && filter.compareIV(state))
        {
            states.emplace_back(state);
        }
    }

    return states;
}

std::vector<StaticState> StaticGenerator8::generateRoamer(u64 seed0, u64 seed1, const StaticTemplate &parameters) const
{
    // Going to ignore most of the parameters
    // Only roamers are Cresselia/Mesprit which have identical parameters
    u8 gender = parameters.getSpecies() == 488 ? 1 : 2;

    Xorshift rng(seed0, seed1);
    rng.advance(initialAdvances + offset);

    std::vector<StaticState> states;
    for (u32 cnt = 0; cnt <= maxAdvances; cnt++)
    {
        StaticState state(initialAdvances + cnt);
        XoroshiroBDSP gen(rng.next());

        u32 sidtid = gen.next(0xffffffff);
        u32 pid = gen.next(0xffffffff);

        u16 psv = (pid >> 16) ^ (pid & 0xffff);
        u16 fakeXOR = (sidtid >> 16) ^ (sidtid & 0xffff) ^ psv;
        if (fakeXOR < 16) // Force shiny
        {
            u8 fakeShinyType = fakeXOR == 0 ? 2 : 1;

            u16 realXOR = psv ^ tsv;
            u8 realShinyType = realXOR == 0 ? 2 : realXOR < 16 ? 1 : 0;

            state.setShiny(fakeShinyType);
            if (realShinyType != fakeShinyType)
            {
                u16 high = (pid & 0xFFFF) ^ tsv ^ (2 - fakeShinyType);
                pid = (high << 16) | (pid & 0xFFFF);
            }
        }
        else // Force non shiny
        {
            state.setShiny(0);
            if (psv ^ tsv < 16)
            {
                pid ^= 0x10000000;
            }
        }
        state.setPID(pid);

        for (int i = 0; i < 6; i++)
        {
            state.setIV(i, 255);
        }

        // Assign 3 31 IVs
        for (int i = 0; i < 3;)
        {
            u8 index = gen.next(6);
            if (state.getIV(index) == 255)
            {
                state.setIV(index, 31);
                i++;
            }
        }

        for (int i = 0; i < 6; i++)
        {
            if (state.getIV(i) == 255)
            {
                state.setIV(i, gen.next(32));
            }
        }

        // No HA possible for roamers
        state.setAbility(gen.next(2));

        // Each roamer has a fixed gender, set that now
        state.setGender(gender);

        if (lead == Lead::Synchronize)
        {
            state.setNature(synchNature);
        }
        else
        {
            state.setNature(gen.next(25));
        }

        if (filter.comparePID(state) && filter.compareIV(state))
        {
            states.emplace_back(state);
        }
    }

    return states;
}
