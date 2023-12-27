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

#include "EventGenerator4.hpp"
#include <Core/Enum/Method.hpp>
#include <Core/Gen4/States/State4.hpp>
#include <Core/Parents/PersonalInfo.hpp>
#include <Core/Parents/PersonalLoader.hpp>
#include <Core/RNG/LCRNG.hpp>

EventGenerator4::EventGenerator4(u32 initialAdvances, u32 maxAdvances, u32 delay, u16 species, u8 nature, u8 level, const Profile4 &profile,
                                 const StateFilter &filter) :
    Generator(initialAdvances, maxAdvances, delay, Method::None, profile, filter), species(species), nature(nature), level(level)
{
}

std::vector<GeneratorState4> EventGenerator4::generate(u32 seed) const
{
    std::vector<GeneratorState4> states;
    const PersonalInfo *info = PersonalLoader::getPersonal(profile.getVersion(), species);

    PokeRNG rng(seed, initialAdvances + delay);
    auto jump = rng.getJump(delay);

    for (u32 cnt = 0; cnt <= maxAdvances; cnt++)
    {
        PokeRNG go(rng, jump);

        u16 iv1 = go.nextUShort();
        u16 iv2 = go.nextUShort();

        std::array<u8, 6> ivs;
        ivs[0] = iv1 & 31;
        ivs[1] = (iv1 >> 5) & 31;
        ivs[2] = (iv1 >> 10) & 31;
        ivs[3] = (iv2 >> 5) & 31;
        ivs[4] = (iv2 >> 10) & 31;
        ivs[5] = iv2 & 31;

        u16 prng = rng.nextUShort();
        if (filter.compareIV(ivs))
        {
            GeneratorState4 state(prng, initialAdvances + cnt, 0, ivs, 0, 0, level, nature, 0, info);
            states.emplace_back(state);
        }
    }

    return states;
}
