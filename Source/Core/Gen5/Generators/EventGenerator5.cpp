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

#include "EventGenerator5.hpp"
#include <Core/Enum/Method.hpp>
#include <Core/Gen5/States/State5.hpp>
#include <Core/Parents/PersonalInfo.hpp>
#include <Core/Parents/PersonalLoader.hpp>
#include <Core/RNG/LCRNG64.hpp>
#include <Core/Util/Utilities.hpp>

EventGenerator5::EventGenerator5(u32 initialAdvances, u32 maxAdvances, u32 offset, u16 tid, u16 sid, Game version, const PGF &pgf,
                                 const StateFilter5 &filter) :
    Generator<StateFilter5>(initialAdvances, maxAdvances, offset, tid, sid, version, Method::None, filter), pgf(pgf)
{
    if (!pgf.getEgg())
    {
        tsv = pgf.getTID() ^ pgf.getSID();
    }

    wondercardAdvances = pgf.getAdvances();
}

std::vector<State5> EventGenerator5::generate(u64 seed) const
{
    std::vector<State5> states;
    const PersonalInfo *info = PersonalLoader::getPersonal(version, pgf.getSpecies());

    BWRNG rng(seed, initialAdvances + offset);
    for (u32 cnt = 0; cnt <= maxAdvances; cnt++)
    {
        BWRNG go(rng.getSeed(), wondercardAdvances);

        std::array<u8, 6> ivs;
        for (u8 i = 0; i < 6; i++)
        {
            u8 parameterIV = pgf.getIV(i);
            if (parameterIV == 255)
            {
                ivs[i] = go.nextUInt(32);
            }
            else
            {
                ivs[i] = parameterIV;
            }
        }

        // 2 blanks
        go.advance(2);

        // Gender locked handling
        u32 pid = go.nextUInt();
        if (pgf.getGender() == 0 || pgf.getGender() == 1)
        {
            pid = Utilities5::forceGender(pid, go, pgf.getGender(), info);
        }

        if (pgf.getShiny() == 0) // No shiny
        {
            if (((pid >> 16) ^ (pid & 0xffff) ^ tsv) < 8)
            {
                pid ^= 0x10000000;
            }
        }
        else if (pgf.getShiny() == 2) // Force shiny
        {
            u32 low = pid & 0xff;
            pid = ((low ^ tsv) << 16) | low;
        }

        // Handle ability
        u8 ability;
        if (pgf.getAbility() < 3)
        {
            ability = pgf.getAbility();
            if (pgf.getAbility() == 1)
            {
                pid |= 0x10000U;
            }
            else
            {
                pid &= ~0x10000U;
            }
        }
        else // Ability flip
        {
            pid ^= 0x10000;
            ability = (pid >> 16) & 1;
        }

        u8 nature;
        if (pgf.getNature() != 0xff)
        {
            nature = pgf.getNature();
        }
        else
        {
            go.advance(1);
            nature = go.nextUInt(25);
        }

        State5 state(rng.nextUInt(0x1fff), initialAdvances + cnt, ivs, pid, ability, nature, tsv, pgf.getLevel(), info);
        if (filter.compareState(state))
        {
            states.emplace_back(state);
        }
    }

    return states;
}

void EventGenerator5::setInitialAdvances(u32 initialAdvances)
{
    this->initialAdvances = initialAdvances;
}
