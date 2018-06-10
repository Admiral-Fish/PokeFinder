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

#include <cstdint>
#include <QDateTime>
#include <QDate>
#include <QTime>
#include <QString>
#include <PokeFinderCore/Objects/Game.hpp>
#include <PokeFinderCore/Objects/Utilities.hpp>
#include <PokeFinderCore/Gen4/HGSSRoamer.hpp>

typedef uint32_t u32;

class DateTime
{

private:
    u32 seed;
    int delay;
    QDateTime dateTime;
    Game version;
    HGSSRoamer info;

public:
    DateTime(QDateTime dateTime, u32 delay, Game version, vector<bool> roamers, vector<u32> routes);
    DateTime(QDateTime dateTime, u32 delay, Game version, HGSSRoamer info);
    QString sequence();
    QString getDate();
    QString getTime();
    u32 getSeed();
    int getDelay();
    Game getVersion();
    QDateTime getDateTime();
    HGSSRoamer getInfo();

};

#endif // DATETIME_HPP
