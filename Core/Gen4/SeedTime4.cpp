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

#include "SeedTime4.hpp"
#include <Core/Enum/Game.hpp>
#include <Core/Gen4/HGSSRoamer.hpp>
#include <Core/Util/Utilities.hpp>

SeedTime4::SeedTime4(const DateTime &dateTime, u32 delay) : dateTime(dateTime), delay(delay)
{
}

DateTime SeedTime4::getDateTime() const
{
    return dateTime;
}

u32 SeedTime4::getDelay() const
{
    return delay;
}

SeedTimeCalibrate4::SeedTimeCalibrate4(const DateTime &dateTime, u32 delay) :
    SeedTime4(dateTime, delay), roamer(nullptr), seed(Utilities4::calcSeed(dateTime, delay))
{
}

SeedTimeCalibrate4::SeedTimeCalibrate4(const DateTime &dateTime, u32 delay, const std::array<bool, 3> &roamers,
                                       const std::array<u8, 3> &routes) :
    SeedTime4(dateTime, delay), seed(Utilities4::calcSeed(dateTime, delay))
{
    roamer = new HGSSRoamer(seed, roamers, routes);
}

SeedTimeCalibrate4::SeedTimeCalibrate4(const SeedTimeCalibrate4 &other) :
    SeedTime4(other.dateTime, other.delay), roamer(nullptr), seed(other.seed)
{
    if (other.roamer)
    {
        roamer = new HGSSRoamer(other.roamer);
    }
}

SeedTimeCalibrate4::~SeedTimeCalibrate4()
{
    delete roamer;
}

HGSSRoamer *SeedTimeCalibrate4::getRoamer() const
{
    return roamer;
}

u32 SeedTimeCalibrate4::getSeed() const
{
    return seed;
}

std::string SeedTimeCalibrate4::getSequence() const
{
    return roamer ? Utilities4::getCalls(seed, roamer->getSkips()) : Utilities4::coinFlips(seed);
}
