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

#include "Profile4.hpp"

Profile4::Profile4(const std::string &profileName, Game version, u16 tid, u16 sid, Game dual, int radio, bool radar, bool swarm, bool dex) :
    Profile(profileName, version, tid, sid), dex(dex), radar(radar), swarm(swarm), dual(dual), radio(radio)
{
}

Game Profile4::getDualSlot() const
{
    return dual;
}

bool Profile4::getNationalDex() const
{
    return dex;
}

bool Profile4::getRadar() const
{
    return radar;
}

int Profile4::getRadio() const
{
    return radio;
}

std::string Profile4::getRadioString() const
{
    switch (radio)
    {
    case 1:
        return "Hoenn Sound";
    case 2:
        return "Sinnoh Sound";
    default:
        return "None";
    }
}

bool Profile4::getSwarm() const
{
    return swarm;
}

bool Profile4::operator==(const Profile4 &other) const
{
    return Profile::operator==(other) && dual == other.dual && radio == other.radio && radar == other.radar && swarm == other.swarm && dex == other.dex;
}

bool Profile4::operator!=(const Profile4 &other) const
{
    return !(operator==(other));
}
