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

#include "HGSSRoamer.hpp"
#include <Core/RNG/LCRNG.hpp>

/**
 * @brief Calculates roamer location for Entei/Raikou
 *
 * @param prng PRNG state
 *
 * @return Roamer location
 */
static u8 getRouteJ(u16 prng)
{
    u8 val = prng & 15;
    return val < 11 ? val + 29 : val + 31;
}

/**
 * @brief Calculates roamer location for Latios/Latias
 *
 * @param prng PRNG state
 *
 * @return Roamer location
 */
static u8 getRouteK(u16 prng)
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

HGSSRoamer::HGSSRoamer(u32 seed, const std::array<bool, 3> &roamers, const std::array<u8, 3> &routes) :
    enteiRoute(0), latiRoute(0), raikouRoute(0), skips(0)
{
    PokeRNG rng(seed);

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

HGSSRoamer::HGSSRoamer(const HGSSRoamer *other) :
    enteiRoute(other->enteiRoute), latiRoute(other->latiRoute), raikouRoute(other->raikouRoute), skips(other->skips)
{
}

std::string HGSSRoamer::getRouteString() const
{
    std::string route;

    if (raikouRoute != 0)
    {
        route += "R: " + std::to_string(raikouRoute) + " ";
    }
    if (enteiRoute != 0)
    {
        route += "E: " + std::to_string(enteiRoute) + " ";
    }
    if (latiRoute != 0)
    {
        route += "L: " + std::to_string(latiRoute);
    }

    return route;
}

u8 HGSSRoamer::getSkips() const
{
    return skips;
}
