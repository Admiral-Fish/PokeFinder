/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2024 by Admiral_Fish, bumba, and EzPzStreamz
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
#include <Core/Gen5/States/EventState5.hpp>
#include <Core/Parents/PersonalInfo.hpp>
#include <Core/Parents/PersonalLoader.hpp>
#include <Core/RNG/LCRNG64.hpp>
#include <Core/Util/Utilities.hpp>

EventGenerator5::EventGenerator5(u32 initialAdvances, u32 maxAdvances, u32 offset, const PGF &pgf, const Profile5 &profile,
                                 const StateFilter &filter) :
    Generator(initialAdvances, maxAdvances, offset, Method::None, profile, filter), pgf(pgf)
{
    if (!pgf.getEgg())
    {
        tsv = pgf.getTID() ^ pgf.getSID();
    }
}

std::vector<EventState5> EventGenerator5::generate(u64 seed) const
{
    const PersonalInfo *info = PersonalLoader::getPersonal(profile.getVersion(), pgf.getSpecies());

    u32 advances = Utilities5::initialAdvances(seed, profile);
    BWRNG rng(seed, advances + initialAdvances);
    auto jump = rng.getJump(pgf.getAdvances() + offset);
    u8 abilitySpec = pgf.getAbility() == 2 ? 0 : pgf.getAbility();

    std::vector<EventState5> states;
    for (u32 cnt = 0; cnt <= maxAdvances; cnt++)
    {
        BWRNG go(rng, jump);

        // Level can be random, but no existing wondercard utilizies this

        std::array<u8, 6> ivs;
        for (u8 i = 0; i < 6; i++)
        {
            u8 iv = pgf.getIV(i);
            if (iv == 255)
            {
                ivs[i] = go.nextUInt(32);
            }
            else
            {
                ivs[i] = iv;
            }
        }

        // Temp PID, temp nature
        go.advance(2);

        // Ability can be random between 1/2/H, but no existing wondercard utilizies this

        // PID can be static, but no existing wondercard utilizies this
        u32 pid = Utilities5::createPID(tsv, abilitySpec, pgf.getGender(), pgf.getShiny(), false, info->getGender(), go);

        u8 nature;
        if (pgf.getNature() != 0xff)
        {
            nature = pgf.getNature();
        }
        else
        {
            go.advance(1); // Temp nature, this call happens regardless but we can hide it in our else branch for speed
            nature = go.nextUInt(25);
        }

        u8 ability;
        if (pgf.getAbility() <= 2)
        {
            ability = pgf.getAbility();
        }
        else
        {
            ability = (pid >> 16) & 1;
        }

        EventState5 state(rng.nextUInt(0x1fff), advances + initialAdvances + cnt, pid, ivs, ability, Utilities::getGender(pid, info),
                          pgf.getLevel(), nature, Utilities::getShiny<true>(pid, tsv), info);
        if (filter.compareState(static_cast<const State &>(state)))
        {
            states.emplace_back(state);
        }
    }

    return states;
}
