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

#include "EventGenerator8.hpp"
#include <Core/Enum/Method.hpp>
#include <Core/Parents/States/State.hpp>
#include <Core/RNG/RNGList.hpp>
#include <Core/RNG/Xorshift.hpp>

static u32 gen(Xorshift &rng)
{
    return rng.next(0x80000000, 0x7fffffff);
}

EventGenerator8::EventGenerator8(u32 initialAdvances, u32 maxAdvances, u32 delay, const WB8 &wb8, const Profile8 &profile,
                                 const StateFilter8 &filter) :
    Generator(initialAdvances, maxAdvances, delay, Method::None, profile, filter), wb8(wb8)
{
    if (!wb8.getEgg())
    {
        tsv = wb8.getTID() ^ wb8.getSID();
    }
}

std::vector<GeneratorState> EventGenerator8::generate(u64 seed0, u64 seed1) const
{
    const PersonalInfo *info = wb8.getInfo(profile.getVersion());
    RNGList<u32, Xorshift, 32, gen> rngList(seed0, seed1, initialAdvances + delay);

    std::vector<GeneratorState> states;
    for (u32 cnt = 0; cnt <= maxAdvances; cnt++, rngList.advanceState())
    {
        // Check for rand EC
        if (wb8.getEC() == 0)
        {
            rngList.advance(1);
        }

        u32 pid;
        u8 shiny;
        switch (wb8.getShiny())
        {
        case 0:
        {
            pid = rngList.next();
            u16 psv = (pid >> 16) & (pid & 0xffff);

            if ((psv ^ tsv) < 16)
            {
                pid ^= 0x10000000;
            }
            shiny = 0;
            break;
        }
        case 1:
        case 2:
        {
            pid = rngList.next();
            u16 psv = (pid >> 16) & (pid & 0xffff);

            u16 realXOR = psv ^ tsv;
            u8 shinyType = realXOR == 0 ? 2 : realXOR < 16 ? 1 : 0;

            if (shinyType != wb8.getShiny())
            {
                u16 high = (pid & 0xFFFF) ^ tsv ^ (2 - wb8.getShiny());
                pid = (high << 16) | (pid & 0xFFFF);
            }

            shiny = wb8.getShiny();
            break;
        }
        case 4:
        {
            pid = wb8.getPID();
            u16 realXor = (pid >> 16) & (pid & 0xffff) ^ tsv;
            shiny = realXor == 0 ? 2 : realXor < 16 ? 1 : 0;
            break;
        }
        }

        std::array<u8, 6> ivs = { 255, 255, 255, 255, 255, 255 };
        for (u8 i = 0; i < wb8.getIVCount();)
        {
            u8 index = rngList.next() % 6;
            if (ivs[index] == 255)
            {
                ivs[index] = 31;
                i++;
            }
        }

        for (u8 &iv : ivs)
        {
            if (iv == 255)
            {
                iv = rngList.next() % 32;
            }
        }

        u8 ability;
        switch (wb8.getAbility())
        {
        case 0:
        case 1:
        case 2:
            ability = wb8.getAbility();
            break;
        case 3:
            ability = rngList.next() % 2;
            break;
        case 4:
            ability = rngList.next() % 3;
            break;
        }

        u8 gender;
        switch (wb8.getGender())
        {
        // Force gender
        case 0:
        case 1:
        case 2:
            gender = wb8.getGender();
            break;
        default:
            gender = (rngList.next() % 252) + 1 < wb8.getGender();
            break;
        }

        u8 nature = wb8.getNature() != 255 ? wb8.getNature() : rngList.next() % 25;

        GeneratorState state(initialAdvances + cnt, pid, ivs, ability, gender, wb8.getLevel(), nature, shiny, info);
        if (filter.compareState(state))
        {
            states.emplace_back(state);
        }
    }

    return states;
}
