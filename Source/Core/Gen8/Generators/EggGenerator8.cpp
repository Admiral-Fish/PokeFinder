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

#include "EggGenerator8.hpp"
#include <Core/Enum/Game.hpp>
#include <Core/Enum/Method.hpp>
#include <Core/Parents/States/EggState.hpp>
#include <Core/RNG/RNGList.hpp>
#include <Core/RNG/Xoroshiro.hpp>
#include <Core/RNG/Xorshift.hpp>

EggGenerator8::EggGenerator8(u32 initialAdvances, u32 maxAdvances, u16 tid, u16 sid, u8 genderRatio, const StateFilter &filter,
                             const Daycare &daycare, bool shinyCharm, u8 compatability) :
    EggGenerator(initialAdvances, maxAdvances, tid, sid, genderRatio, Method::None, filter, daycare),
    shinyCharm(shinyCharm),
    compatability(compatability)
{
}

std::vector<EggState> EggGenerator8::generate(u64 seed0, u64 seed1) const
{
    Xorshift rng(seed0, seed1);
    rng.advance(initialAdvances + offset);

    RNGList<u32, Xorshift, 2, 0> rngList(rng);

    u8 pidRolls = 0;
    if (daycare.getMasuda())
    {
        pidRolls += 6;
    }
    if (shinyCharm)
    {
        pidRolls += 2;
    }

    std::vector<EggState> states;
    for (u32 cnt = 0; cnt <= maxAdvances; cnt++, rngList.advanceState())
    {
        if ((rngList.getValue() % 100) < compatability)
        {
            EggState state(initialAdvances + cnt);
            state.setSeed(rngList.getValue());

            // Sign extend seed to signed 64bit
            u64 seed = state.getSeed();
            if (seed & 0x80000000)
            {
                seed |= 0xffffffff00000000;
            }

            XoroshiroBDSP gen(seed);

            // Nidoran, Illumise/Volbeat, Indeedee
            if (daycare.getNidoranVolbeat())
            {
                // gen.next(2);
                // Handle display result later
                gen.next();
            }

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
                u8 gender = gen.next(252) + 1 < genderRatio;
                state.setGender(gender);
            }

            u8 nature = gen.next(25);
            if (daycare.getEverstoneCount(Game::BDSP) == 2)
            {
                nature = daycare.getParentNature(gen.next(2));
            }
            else if (daycare.getParentItem(0) == 1)
            {
                nature = daycare.getParentNature(0);
            }
            else if (daycare.getParentItem(1) == 1)
            {
                nature = daycare.getParentNature(1);
            }
            state.setNature(nature);

            // If we have a ditto acting as the female, get the ability from the other parent (this will be slot 0)
            u8 parentAbility = daycare.getParentAbility(daycare.getParentGender(1) == 3 ? 0 : 1);
            u8 ability = gen.next(100);
            if (parentAbility == 2)
            {
                ability = ability < 20 ? 0 : ability < 40 ? 1 : 2;
            }
            else if (parentAbility == 1)
            {
                ability = ability < 20 ? 0 : 1;
            }
            else
            {
                ability = ability < 80 ? 0 : 1;
            }
            state.setAbility(ability);

            // Intentionally ignoring power items
            u8 inheritance = 3;
            if (daycare.getParentItem(0) == 8 || daycare.getParentItem(1) == 8)
            {
                inheritance = 5;
            }

            // Determine inheritance
            for (u8 i = 0; i < inheritance;)
            {
                u8 index = gen.next(6);
                if (state.getInheritance(index) == 0)
                {
                    state.setInheritance(index, gen.next(2) + 1);
                    i++;
                }
            }

            // Assign IVs and inheritance
            for (u8 i = 0; i < 6; i++)
            {
                u8 iv = gen.next(32);
                if (state.getInheritance(i) == 1)
                {
                    iv = daycare.getParentIV(0, i);
                }
                else if (state.getInheritance(i) == 2)
                {
                    iv = daycare.getParentIV(1, i);
                }
                state.setIV(i, iv);
            }

            // Encryption constant
            gen.next();

            // Assign PID if
            u32 pid = 0;
            u16 psv = 0;
            for (u8 roll = 0; roll < pidRolls; roll++)
            {
                pid = gen.next(0xffffffff);
                psv = (pid >> 16) ^ (pid & 0xffff);
                if ((psv ^ tsv) < 16)
                {
                    break;
                }
            }
            state.setPID(pid);
            state.setShiny<16>(tsv, psv);

            // Ball handling check
            // Uses a rand call, maybe add later

            if (filter.comparePID(state) && filter.compareIV(state))
            {
                states.emplace_back(state);
            }
        }
    }

    return states;
}
