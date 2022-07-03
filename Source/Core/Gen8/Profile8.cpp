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

#include "Profile8.hpp"

Profile8::Profile8() : shinyCharm(false), ovalCharm(false), radar(false), swarm(false)
{
    version = Game::Sword;
}

Profile8::Profile8(const std::string &name, Game version, u16 tid, u16 sid, bool shinyCharm, bool ovalCharm, bool radar, bool swarm) :
    Profile(name, version, tid, sid), shinyCharm(shinyCharm), ovalCharm(ovalCharm), radar(radar), swarm(swarm)
{
}

bool Profile8::getShinyCharm() const
{
    return shinyCharm;
}

bool Profile8::getOvalCharm() const
{
    return ovalCharm;
}

bool Profile8::getRadar() const
{
    return radar;
}

bool Profile8::getSwarm() const
{
    return swarm;
}

bool Profile8::operator==(const Profile8 &other) const
{
    return Profile::operator==(other) && shinyCharm == other.shinyCharm && ovalCharm == other.ovalCharm && radar == other.radar
        && swarm == other.swarm;
}

bool Profile8::operator!=(const Profile8 &other) const
{
    return !(operator==(other));
}
