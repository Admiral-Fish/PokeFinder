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

#include "StaticGenerator3.hpp"
#include <Core/Enum/Lead.hpp>
#include <Core/Enum/Method.hpp>
#include <Core/Parents/PersonalInfo.hpp>
#include <Core/Parents/States/State.hpp>
#include <Core/RNG/LCRNG.hpp>
#include <Core/Util/Utilities.hpp>

StaticGenerator3::StaticGenerator3(u32 initialAdvances, u32 maxAdvances, u32 delay, Method method, const StaticTemplate &staticTemplate,
                                   const Profile3 &profile, const StateFilter &filter) :
    StaticGenerator(initialAdvances, maxAdvances, delay, method, Lead::None, staticTemplate, profile, filter)
{
}

std::vector<GeneratorState> StaticGenerator3::generate(u32 seed) const
{
    std::vector<GeneratorState> states;
    const PersonalInfo *info = staticTemplate.getInfo();

    PokeRNG rng(seed, initialAdvances + delay);
    for (u32 cnt = 0; cnt <= maxAdvances; cnt++, rng.next())
    {
        PokeRNG go(rng);

        u32 pid = go.nextUShort();
        pid |= go.nextUShort() << 16;

        u16 iv1 = go.nextUShort();
        if (method == Method::Method4)
        {
            go.next();
        }
        u16 iv2 = go.nextUShort();

        std::array<u8, 6> ivs;
        ivs[0] = iv1 & 31;
        ivs[1] = (iv1 >> 5) & 31;
        ivs[2] = (iv1 >> 10) & 31;
        ivs[3] = (iv2 >> 5) & 31;
        ivs[4] = (iv2 >> 10) & 31;
        ivs[5] = iv2 & 31;

        GeneratorState state(initialAdvances + cnt, pid, ivs, pid & 1, Utilities::getGender(pid, info), staticTemplate.getLevel(), pid % 25,
                             Utilities::getShiny(pid, tsv), info);
        if (filter.compareState(static_cast<const State &>(state)))
        {
            states.emplace_back(state);
        }
    }

    return states;
}
