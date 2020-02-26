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

#include "DateTime.hpp"
#include <Core/Enum/Game.hpp>
#include <Core/Util/Utilities.hpp>

DateTime::DateTime(const QDateTime &dateTime, u32 delay, Game version, const QVector<bool> &roamers, const QVector<u8> &routes) :
    seed(Utilities::calcGen4Seed(dateTime, delay + dateTime.date().year() - 2000)),
    delay(delay),
    dateTime(dateTime),
    version(version)
{
    info = HGSSRoamer(seed, roamers, routes);
}

DateTime::DateTime(const QDateTime &dateTime, u32 delay, Game version, const HGSSRoamer &info) :
    seed(Utilities::calcGen4Seed(dateTime, delay + dateTime.date().year() - 2000)),
    delay(delay),
    dateTime(dateTime),
    version(version),
    info(info)
{
    this->info.recalculateRoamers(seed);
}

QString DateTime::sequence() const
{
    return (version & Game::HGSS) ? Utilities::getCalls(seed, 15, info) : Utilities::coinFlips(seed, 15);
}

QString DateTime::getDate() const
{
    return dateTime.date().toString(Qt::SystemLocaleShortDate);
}

QString DateTime::getTime() const
{
    return dateTime.time().toString();
}

u32 DateTime::getSeed() const
{
    return seed;
}

u32 DateTime::getDelay() const
{
    return delay;
}

Game DateTime::getVersion() const
{
    return version;
}

QDateTime DateTime::getDateTime() const
{
    return dateTime;
}

HGSSRoamer DateTime::getInfo() const
{
    return info;
}
