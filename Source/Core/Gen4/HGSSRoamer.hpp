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

#ifndef HGSSROAMER_HPP
#define HGSSROAMER_HPP

#include <Core/Util/Global.hpp>
#include <array>
#include <string>

class HGSSRoamer
{
public:
    HGSSRoamer() = default;
    HGSSRoamer(u32 seed, const std::array<bool, 3> &roamers, const std::array<u8, 3> &routes);
    u8 getSkips() const;
    std::string getRouteString() const;
    void recalculateRoamers(u32 seed);

private:
    void calculateRoamers();
    u8 getRouteJ(u16 prng) const;
    u8 getRouteK(u16 prng) const;

    u8 skips;
    u8 raikouRoute;
    u8 enteiRoute;
    u8 latiRoute;
    u32 seed;
    std::array<bool, 3> roamers;
    std::array<u8, 3> routes;
};

#endif // HGSSROAMER_HPP
