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

#ifndef DATETIME_HPP
#define DATETIME_HPP

#include <PokeFinderCore/Gen4/HGSSRoamer.hpp>
#include <PokeFinderCore/Objects/Game.hpp>
#include <PokeFinderCore/Objects/Utilities.hpp>

class DateTime
{

private:
    u32 seed;
    u32 delay;
    QDateTime dateTime;
    Game version;
    HGSSRoamer info;

public:
    DateTime() = default;
    DateTime(const QDateTime &dateTime, u32 delay, Game version, const QVector<bool> &roamers, const QVector<u16> &routes);
    DateTime(const QDateTime &dateTime, u32 delay, Game version, const HGSSRoamer &info);
    QString sequence();
    QString getDate();
    QString getTime();
    u32 getSeed();
    u32 getDelay();
    Game getVersion();
    QDateTime getDateTime();
    HGSSRoamer getInfo();

};

#endif // DATETIME_HPP
