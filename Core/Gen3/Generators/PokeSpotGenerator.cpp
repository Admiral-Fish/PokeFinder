/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2020 by Admiral_Fish, bumba, and EzPzStreamz
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

#include "PokeSpotGenerator.hpp"
#include <Core/Enum/Method.hpp>
#include <Core/Parents/Filters/StateFilter.hpp>
#include <Core/RNG/LCRNG.hpp>

PokeSpotGenerator::PokeSpotGenerator(u32 initialAdvances, u32 maxAdvances, u16 tid, u16 sid, u8 genderRatio, const StateFilter &filter) :
    Generator(initialAdvances, maxAdvances, tid, sid, genderRatio, Method::XDColo, filter)
{
}

void PokeSpotGenerator::setSpots(const std::vector<bool> &spots)
{
    this->spots = spots;
}

std::vector<GameCubeState> PokeSpotGenerator::generate(u32 seed) const
{
    std::vector<GameCubeState> states;

    XDRNG rng(seed);
    rng.advance(initialAdvances);

    for (u32 cnt = 0; cnt <= maxAdvances; cnt++, rng.next())
    {
        XDRNG go(rng.getSeed());

        // Check if state is a valid pokespot call
        if ((go.nextUShort() % 3) == 0)
        {
            // Munchlax provides a skip
            if ((go.nextUShort() % 100) >= 10)
            {
                GameCubeState state(initialAdvances + cnt);

                // Check what type the pokespot is
                u8 call = go.nextUShort() % 100;
                if (call < 50)
                {
                    if (!spots.at(0))
                    {
                        continue;
                    }
                    state.setInfo(0);
                }
                else if (call > 49 && call < 85)
                {
                    if (!spots.at(1))
                    {
                        continue;
                    }
                    state.setInfo(1);
                }
                else
                {
                    if (!spots.at(2))
                    {
                        continue;
                    }
                    state.setInfo(2);
                }

                u16 high = go.nextUShort();
                u16 low = go.nextUShort();

                state.setPID(high, low);
                state.setAbility(low & 1);
                state.setGender(low & 255, genderRatio);
                state.setNature(state.getPID() % 25);
                state.setShiny(tsv, high ^ low, 8);

                if (filter.comparePID(state))
                {
                    states.emplace_back(state);
                }
            }
        }
    }

    return states;
}
