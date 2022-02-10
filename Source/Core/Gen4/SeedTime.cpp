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

#include "SeedTime.hpp"
#include <Core/Enum/Game.hpp>
#include <Core/Util/Utilities.hpp>

SeedTime::SeedTime(const DateTime &dateTime, u32 delay, Game version, const std::vector<bool> &roamers, const std::vector<u8> &routes) :
    seed(Utilities4::calcSeed(dateTime, delay)), delay(delay), dateTime(dateTime), version(version)
{
    info = HGSSRoamer(seed, roamers, routes);
}

SeedTime::SeedTime(const DateTime &dateTime, u32 delay, Game version, const HGSSRoamer &info) :
    seed(Utilities4::calcSeed(dateTime, delay)), delay(delay), dateTime(dateTime), version(version), info(info)
{
    this->info.recalculateRoamers(seed);
}

std::string SeedTime::getSequence() const
{
    return (version & Game::HGSS) != Game::None ? Utilities4::getCalls(seed, info) : Utilities4::coinFlips(seed);
}

u32 SeedTime::getSeed() const
{
    return seed;
}

u32 SeedTime::getDelay() const
{
    return delay;
}

Game SeedTime::getVersion() const
{
    return version;
}

DateTime SeedTime::getDateTime() const
{
    return dateTime;
}

HGSSRoamer SeedTime::getInfo() const
{
    return info;
}
