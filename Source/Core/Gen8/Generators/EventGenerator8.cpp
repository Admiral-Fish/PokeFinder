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

#include "EventGenerator8.hpp"
#include <Core/Enum/Method.hpp>
#include <Core/Parents/PersonalLoader.hpp>
#include <Core/RNG/RNGList.hpp>
#include <Core/RNG/Xorshift.hpp>

void fixPID(u32 &pid, u16 tsv, u8 shinyType)
{
    u16 psv = (pid >> 16) ^ (pid & 0xffff);
    u8 currentShinyType;
    if ((psv ^ tsv) == 0)
    {
        currentShinyType = 2;
    }
    else if ((psv ^ tsv) < 16)
    {
        currentShinyType = 1;
    }
    else
    {
        currentShinyType = 0;
    }

    // Force non-shiny
    if (shinyType == 0 && currentShinyType != 0)
    {
        pid ^= 0x10000000;
    }
    // Force to the appropriate shiny type
    else if (shinyType != 0 && currentShinyType != shinyType)
    {
        u16 high = (pid & 0xFFFF) ^ tsv ^ (2 - shinyType);
        pid = (high << 16) | (pid & 0xFFFF);
    }
}

EventGenerator8::EventGenerator8(u32 initialAdvances, u32 maxAdvances, u16 tid, u16 sid, const StateFilter &filter, const WB8 &parameters) :
    Generator(initialAdvances, maxAdvances, tid, sid, 0, Method::Null, filter), parameters(parameters), ivCount(0)
{
    if (!parameters.isEgg())
    {
        tsv = parameters.getTID() ^ parameters.getSID();
    }

    ivCount = parameters.getIVCount();
}

std::vector<State> EventGenerator8::generate(u64 seed0, u64 seed1) const
{
    std::vector<State> states;

    Xorshift rng(seed0, seed1);
    rng.advance(initialAdvances + offset);

    RNGList<u32, Xorshift, 32, 0> rngList(rng);

    for (u32 cnt = 0; cnt <= maxAdvances; cnt++, rngList.advanceState())
    {
        State state(initialAdvances + cnt);

        Xorshift gen(rng);

        // Check for rand EC
        if (parameters.getEC() == 0)
        {
            rngList.advance(1);
        }

        u32 pid;
        switch (parameters.getPIDType())
        {
        case 0:
        case 1:
        case 2:
            pid = rngList.getValue();
            fixPID(pid, tsv, parameters.getPIDType());
            state.setShiny(parameters.getPIDType());
            break;
        case 4:
            pid = parameters.getPID();
            state.setShiny<16>(tsv, (pid >> 16) ^ (pid & 0xffff));
            break;
        }
        state.setPID(pid);

        for (u8 i = 0; i < 6; i++)
        {
            state.setIV(i, 255);
        }

        for (u8 i = 0; i < ivCount;)
        {
            u8 index = rngList.getValue() % 6;
            if (state.getIV(index) == 255)
            {
                state.setIV(index, 31);
                i++;
            }
        }

        for (u8 i = 0; i < 6; i++)
        {
            if (state.getIV(i) == 255)
            {
                state.setIV(i, rngList.getValue() % 32);
            }
        }

        switch (parameters.getAbilityType())
        {
        case 0:
        case 1:
        case 2:
            state.setAbility(parameters.getAbilityType());
            break;
        case 3:
            state.setAbility(rngList.getValue() % 2);
            break;
        case 4:
            state.setAbility(rngList.getValue() % 3);
            break;
        }

        switch (parameters.getGender())
        {
        // Force gender
        case 0:
        case 1:
        case 2:
            state.setGender(parameters.getGender());
            break;
        default:
            state.setGender((rngList.getValue() % 252) + 1 < parameters.getGender());
            break;
        }

        state.setNature(parameters.getNature() != 255 ? parameters.getNature() : gen.next() % 25);

        if (filter.comparePID(state) && filter.compareIV(state))
        {
            // states.emplace_back(state);
        }
    }

    return states;
}
