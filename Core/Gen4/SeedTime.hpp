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

#ifndef SEEDTIME_HPP
#define SEEDTIME_HPP

#include <Core/Gen4/HGSSRoamer.hpp>
#include <Core/Util/DateTime.hpp>

enum Game : u16;

class SeedTime
{
public:
    SeedTime() = default;
    SeedTime(const DateTime &dateTime, u32 delay, Game version, const std::vector<bool> &roamers, const std::vector<u8> &routes);
    SeedTime(const DateTime &dateTime, u32 delay, Game version, const HGSSRoamer &info);
    std::string sequence() const;
    u32 getSeed() const;
    u32 getDelay() const;
    Game getVersion() const;
    DateTime getDateTime() const;
    HGSSRoamer getInfo() const;

private:
    u32 seed;
    u32 delay;
    DateTime dateTime;
    Game version;
    HGSSRoamer info;
};

#endif // SEEDTIME_HPP
