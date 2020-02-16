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

#include "HGSSRoamer.hpp"
#include <Core/RNG/LCRNG.hpp>

HGSSRoamer::HGSSRoamer(u32 seed, const QVector<bool> &roamers, const QVector<u8> &routes)
{
    this->seed = seed;
    this->roamers = roamers;
    this->routes = routes;

    calculateRoamers();
}

u8 HGSSRoamer::getSkips() const
{
    return skips;
}

u8 HGSSRoamer::getRaikouRoute() const
{
    return raikouRoute;
}

u8 HGSSRoamer::getEnteiRoute() const
{
    return enteiRoute;
}

u8 HGSSRoamer::getLatiRoute() const
{
    return latiRoute;
}

QString HGSSRoamer::getRouteString() const
{
    QString routes;

    if (roamers.at(0))
    {
        routes += QString("R: %1 ").arg(raikouRoute);
    }
    if (roamers.at(1))
    {
        routes += QString("E: %1 ").arg(enteiRoute);
    }
    if (roamers.at(2))
    {
        routes += QString("L: %1 ").arg(latiRoute);
    }

    return routes;
}

QVector<bool> HGSSRoamer::getRoamers() const
{
    return roamers;
}

QVector<u8> HGSSRoamer::getRoutes() const
{
    return routes;
}

void HGSSRoamer::recalculateRoamers(u32 seed)
{
    this->seed = seed;
    calculateRoamers();
}

void HGSSRoamer::calculateRoamers()
{
    PokeRNG rng(seed);
    skips = 0;

    if (roamers.at(0))
    {
        do
        {
            skips++;
            raikouRoute = getRouteJ(rng.nextUShort());
        } while (routes.at(0) == raikouRoute);
    }

    if (roamers.at(1))
    {
        do
        {
            skips++;
            enteiRoute = getRouteJ(rng.nextUShort());
        } while (routes.at(1) == enteiRoute);
    }

    if (roamers.at(2))
    {
        do
        {
            skips++;
            latiRoute = getRouteK(rng.nextUShort());
        } while (routes.at(2) == latiRoute);
    }
}

u8 HGSSRoamer::getRouteJ(u16 prng) const
{
    u8 val = prng & 15;
    return val < 11 ? val + 29 : val + 31;
}

u8 HGSSRoamer::getRouteK(u16 prng) const
{
    u8 val = prng % 25;
    if (val > 21)
    {
        switch (val)
        {
        case 22:
            return 24;
        case 23:
            return 26;
        case 24:
            return 28;
        }
    }

    return val + 1;
}
