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

#include "EventGenerator5.hpp"
#include <Core/Enum/Method.hpp>
#include <Core/Gen5/States/State5.hpp>
#include <Core/Parents/PersonalInfo.hpp>
#include <Core/Parents/PersonalLoader.hpp>
#include <Core/RNG/LCRNG64.hpp>
#include <Core/Util/Utilities.hpp>

static u8 getGender(u32 pid, const PersonalInfo *info)
{
    switch (info->getGender())
    {
    case 255: // Genderless
        return 2;
        break;
    case 254: // Female
        return 1;
        break;
    case 0: // Male
        return 0;
        break;
    default: // Random gender
        return (pid & 255) < info->getGender();
        break;
    }
}

static u8 getShiny(u32 pid, u16 tsv)
{
    u16 psv = (pid >> 16) ^ (pid & 0xffff);
    if (tsv == psv)
    {
        return 2; // Square
    }
    else if ((tsv ^ psv) < 8)
    {
        return 1; // Star
    }
    else
    {
        return 0;
    }
}

EventGenerator5::EventGenerator5(u32 initialAdvances, u32 maxAdvances, u32 delay, const PGF &pgf, const Profile5 &profile,
                                 const StateFilter5 &filter) :
    Generator(initialAdvances, maxAdvances, delay, Method::None, profile, filter), pgf(pgf)
{
    if (!pgf.getEgg())
    {
        tsv = pgf.getTID() ^ pgf.getSID();
    }

    wondercardAdvances = pgf.getAdvances();
}

std::vector<State5> EventGenerator5::generate(u64 seed) const
{
    const PersonalInfo *info = PersonalLoader::getPersonal(profile.getVersion(), pgf.getSpecies());

    u32 advances = Utilities5::initialAdvances(seed, profile);
    BWRNG rng(seed, advances + initialAdvances + delay);

    std::vector<State5> states;
    for (u32 cnt = 0; cnt <= maxAdvances; cnt++)
    {
        BWRNG go(rng, wondercardAdvances);

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
            pid = Utilities5::forceGender(pid, go, pgf.getGender(), info->getGender());
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

        State5 state(rng.nextUInt(0x1fff), advances + initialAdvances + cnt, pid, ivs, ability, getGender(pid, info), pgf.getLevel(),
                     nature, getShiny(pid, tsv), info);
        if (filter.compareState(state))
        {
            states.emplace_back(state);
        }
    }

    return states;
}
