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

#include "HGSSRoamer.hpp"
#include <Core/RNG/LCRNG.hpp>

HGSSRoamer::HGSSRoamer(u32 seed, const std::vector<bool> &roamers, const std::vector<u8> &routes) :
    seed(seed), roamers(roamers), routes(routes)
{
    calculateRoamers();
}

u8 HGSSRoamer::getSkips() const
{
    return skips;
}

std::string HGSSRoamer::getRouteString() const
{
    std::string route;

    if (roamers[0])
    {
        route = "R: " + std::to_string(raikouRoute);
    }
    if (roamers[1])
    {
        route = "E: " + std::to_string(enteiRoute);
    }
    if (roamers[2])
    {
        route = "L: " + std::to_string(latiRoute);
    }

    return route;
}

void HGSSRoamer::recalculateRoamers(u32 seed)
{
    this->seed = seed;
    calculateRoamers();
}

void HGSSRoamer::calculateRoamers()
{
    PokeRNG rng(seed);
    skips = 0;

    if (roamers[0])
    {
        do
        {
            skips++;
            raikouRoute = getRouteJ(rng.nextUShort());
        } while (routes[0] == raikouRoute);
    }

    if (roamers[1])
    {
        do
        {
            skips++;
            enteiRoute = getRouteJ(rng.nextUShort());
        } while (routes[1] == enteiRoute);
    }

    if (roamers[2])
    {
        do
        {
            skips++;
            latiRoute = getRouteK(rng.nextUShort());
        } while (routes[2] == latiRoute);
    }
}

u8 HGSSRoamer::getRouteJ(u16 prng) const
{
    u8 val = prng & 15;
    return val < 11 ? val + 29 : val + 31;
}

u8 HGSSRoamer::getRouteK(u16 prng) const
{
    u8 val = prng % 25;
    if (val > 21)
    {
        switch (val)
        {
        case 22:
            return 24;
        case 23:
            return 26;
        case 24:
            return 28;
        }
    }

    return val + 1;
}
