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

#ifndef HGSSROAMER_HPP
#define HGSSROAMER_HPP

#include <Core/Global.hpp>
#include <array>
#include <string>

/**
 * @brief Generates routes HGSS roamers will appear on
 */
class HGSSRoamer
{
public:
    /**
     * @brief Construct a new HGSSRoamer object
     *
     * @param seed PRNG state
     * @param roamers Array of active roamers
     * @param routes Array of roamer locations
     */
    HGSSRoamer(u32 seed, const std::array<bool, 3> &roamers, const std::array<u8, 3> &routes);

    /**
     * @brief Construct a new HGSSRoamer object
     *
     * @param other HGSSRoamer to copy
     */
    HGSSRoamer(const HGSSRoamer *other);

    /**
     * @brief Returns string of roamer locations
     *
     * @return Roamer locations
     */
    std::string getRouteString() const;

    /**
     * @brief Returns number of PRNG calls used to determine roamer locations
     *
     * @return PRNG call count
     */
    u8 getSkips() const;

private:
    u8 enteiRoute;
    u8 latiRoute;
    u8 raikouRoute;
    u8 skips;

    /**
     * @brief Calculates roamer location for Entei/Raikou
     *
     * @param prng PRNG state
     *
     * @return Roamer location
     */
    u8 getRouteJ(u16 prng) const;

    /**
     * @brief Calculates roamer location for Latios/Latias
     *
     * @param prng PRNG state
     *
     * @return Roamer location
     */
    u8 getRouteK(u16 prng) const;
};

#endif // HGSSROAMER_HPP
