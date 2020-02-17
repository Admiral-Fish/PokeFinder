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

#include "Profile3.hpp"
#include <Core/Enum/Game.hpp>

Profile3::Profile3() : deadBattery(false)
{
    version = Game::Emerald;
}

Profile3::Profile3(const QString &name, Game version, u16 tid, u16 sid, bool deadBattery) : Profile(name, version, tid, sid)
{
    this->deadBattery = deadBattery;
}

bool Profile3::getDeadBattery() const
{
    return deadBattery;
}

bool operator==(const Profile3 &left, const Profile3 &right)
{
    return left.getName() == right.getName() && left.getVersion() == right.getVersion() && left.getTID() == right.getTID()
        && left.getSID() == right.getSID() && left.getDeadBattery() == right.getDeadBattery();
}

bool operator!=(const Profile3 &left, const Profile3 &right)
{
    return !(left == right);
}
