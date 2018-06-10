/*
 * This file is part of PokéFinder
 * Copyright (C) 2017 by Admiral_Fish, bumba, and EzPzStreamz
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

DateTime::DateTime(QDateTime dateTime, u32 delay, Game version, vector<bool> roamers, vector<u32> routes)
{
    this->dateTime = dateTime;
    this->delay = delay;
    seed = Utilities::calcGen4Seed(dateTime, delay - (2000 - dateTime.date().year()));
    this->version = version;
    info = HGSSRoamer(seed, roamers, routes);
}

DateTime::DateTime(QDateTime dateTime, u32 delay, Game version, HGSSRoamer info)
{
    this->dateTime = dateTime;
    this->delay = delay;
    seed = Utilities::calcGen4Seed(dateTime, delay - (2000 - dateTime.date().year()));
    this->version = version;
    this->info = info;
}

QString DateTime::sequence()
{
    if (version == HeartGold || version == SoulSilver)
        return Utilities::getCalls(seed, 15, info);
    else // DPPt
        return Utilities::coinFlips(seed, 15);
}

QString DateTime::getDate()
{
    return dateTime.date().toString(Qt::SystemLocaleShortDate);
}

QString DateTime::getTime()
{
    return dateTime.time().toString();
}

u32 DateTime::getSeed()
{
    return seed;
}

int DateTime::getDelay()
{
    return delay;
}

Game DateTime::getVersion()
{
    return version;
}

QDateTime DateTime::getDateTime()
{
    return dateTime;
}

HGSSRoamer DateTime::getInfo()
{
    return info;
}
