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

constexpr u16 JOHTO_MAX = 16;
constexpr u16 KANTO_MAX = 25;

/**
 * @brief Calculates roamer location for Entei/Raikou
 *
 * @param rng RNG state
 *
 * @return Roamer location
 */
static u8 getRouteJ(PokeRNG &rng)
{
    constexpr u8 JOHTO_MAPS[] = { 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 42, 43, 44, 45, 46 };
    return JOHTO_MAPS[rng.nextUShort(sizeof(JOHTO_MAPS))];
}

/**
 * @brief Calculates roamer location for Latios/Latias
 *
 * @param rng RNG state
 *
 * @return Roamer location
 */
static u8 getRouteK(PokeRNG &rng)
{
    constexpr u8 KANTO_MAPS[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 24, 26, 28 };
    return KANTO_MAPS[rng.nextUShort(sizeof(KANTO_MAPS))];
}

HGSSRoamer::HGSSRoamer(u32 seed, u8 playerRoute, const std::array<bool, 3> &roamers, const std::array<u8, 3> &routes) :
    enteiRoute(0), latiRoute(0), raikouRoute(0), skips(0)
{
    PokeRNG rng(seed);

    if (roamers[0])
    {
        do
        {
            skips++;
            raikouRoute = getRouteJ(rng);
        } while (routes[0] == raikouRoute || playerRoute == raikouRoute);
    }

    if (roamers[1])
    {
        do
        {
            skips++;
            enteiRoute = getRouteJ(rng);
        } while (routes[1] == enteiRoute || playerRoute == enteiRoute);
    }

    if (roamers[2])
    {
        do
        {
            skips++;
            latiRoute = getRouteK(rng);
        } while (routes[2] == latiRoute || playerRoute == latiRoute);
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
