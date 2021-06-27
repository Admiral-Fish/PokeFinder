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

#include "UnownGenerator3.hpp"
#include <Core/Enum/Encounter.hpp>
#include <Core/Enum/Lead.hpp>
#include <Core/Enum/Method.hpp>
#include <Core/Gen3/EncounterArea3.hpp>
#include <Core/Parents/Filters/StateFilter.hpp>
#include <Core/Parents/States/UnownState.hpp>
#include <Core/RNG/LCRNG.hpp>
#include <Core/Util/EncounterSlot.hpp>
#include <functional>
#include <string>

UnownGenerator3::UnownGenerator3(u32 initialAdvances, u32 maxAdvances, u16 tid, u16 sid, u8 genderRatio, Method method,
                               const StateFilter &filter) :
    UnownGenerator(initialAdvances, maxAdvances, tid, sid, genderRatio, method, filter)
{
}

std::string UnownGenerator3::getLetter(u32 pid) const
{
    u32 val1, val2, val3, val4, val;
    val1 = (pid >> 24) & 3;
    val2 = (pid >> 16) & 3;
    val3 = (pid >> 8) & 3;
    val4 = pid & 3;
    val = (val1 << 6) | (val2 << 4) | (val3 << 2) | val4;
    return letters[val % 28];
}

u8 UnownGenerator3::getLetterIndex(u32 pid) const
{
    u32 val1, val2, val3, val4, val;
    val1 = (pid >> 24) & 3;
    val2 = (pid >> 16) & 3;
    val3 = (pid >> 8) & 3;
    val4 = pid & 3;
    val = (val1 << 6) | (val2 << 4) | (val3 << 2) | val4;
    return val % 28;
}

std::string UnownGenerator3::getTargetLetter(u8 location, u8 slot) const
{
    switch (location)
    {
    case 0:
    {
        return slot < 99 ? "A" : "?";
    }
    case 1:
    {
        return slot < 50 ? "C" : slot < 80 ? "D" : slot < 94 ? "H" : slot < 99 ? "U" : "O";
    }
    case 2:
    {
        return slot < 60 ? "N" : slot < 90 ? "S" : slot < 98 ? "H" : "E";
    }
    case 3:
    {
        return slot < 40 ? "P" : slot < 60 ? "L" : slot < 80 ? "J" : slot < 94 ? "R" : "Q";
    }
    case 4:
    {
        return slot < 40 ? "Y" : slot < 60 ? "T" : slot < 85 ? "G" : slot < 98 ? "F" : "K";
    }
    case 5:
    {
        return slot < 50 ? "V" : slot < 80 ? "W" : slot < 90 ? "X" : slot < 98 ? "M" : "B";
    }
    case 6:
    {
        return slot < 99 ? "Z" : "!";
    }
    default:
    {
        return "0";
    }
    }
}

std::vector<UnownState> UnownGenerator3::generate(u32 seed, u8 location) const
{
    std::vector<UnownState> states;

    PokeRNG rng(seed);
    rng.advance(initialAdvances + offset);

    for (u32 cnt = 0; cnt <= maxAdvances; cnt++, rng.next())
    {
        UnownState state(initialAdvances + cnt);
        PokeRNG go(rng.getSeed());
        u8 slot = go.nextUShort() % 100;
        std::string targetLetter = getTargetLetter(location,slot);
        go.next();

        // Now search for a Method 124 PID that matches our hunt letter
        std::string letter;
        u32 pid;
        do
        {
            u16 high = go.nextUShort();
            u16 low = go.nextUShort();
            pid = (high << 16) | low;
            letter = getLetter(pid);
        } while (letter != targetLetter);

        state.setLetter(letter);
        state.setLetterIndex(getLetterIndex(pid));
        state.setNature(pid%25);
        state.setPID(pid);
        state.setAbility(pid & 1);
        state.setGender(pid & 255, genderRatio);
        state.setShiny<8>(tsv, (pid & 0xffff) ^ (pid >> 16));

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
        state.setIVs(iv1, iv2);
        state.calculateHiddenPower();

        if (filter.compareState(state) && filter.compareLetter(state))
        {
            states.emplace_back(state);
        }
    }

    return states;
}
