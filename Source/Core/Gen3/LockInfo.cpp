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

#include "LockInfo.hpp"
#include <Core/Enum/Method.hpp>
#include <Core/Enum/ShadowType.hpp>
#include <Core/Resources/Resources.hpp>

constexpr LockInfo::LockInfo(u8 nature, u8 genderLower, u8 genderUpper) :
    nature(nature), genderLower(genderLower), genderUpper(genderUpper), free(nature == 255 && genderLower == 255 && genderUpper == 255)
{
}

bool LockInfo::compare(u32 pid) const
{
    if (free)
    {
        return true;
    }

    u8 gender = pid & 255;
    return gender >= genderLower && gender <= genderUpper && nature == (pid % 25);
}

bool LockInfo::getFree() const
{
    return free;
}

ShadowTeam::ShadowTeam(const std::vector<LockInfo> &locks, ShadowType type) : locks(locks), type(type)
{
}

std::vector<LockInfo> ShadowTeam::getLocks() const
{
    return locks;
}

ShadowType ShadowTeam::getType() const
{
    return type;
}

std::vector<ShadowTeam> ShadowTeam::loadShadowTeams(Method version)
{
    const u8 *data;
    size_t size;

    if (version == Method::XD)
    {
        data = gales.data();
        size = gales.size();
    }
    else
    {
        data = colo.data();
        size = colo.size();
    }

    std::vector<ShadowTeam> teams;

    for (size_t i = 0; i < size;)
    {
        u8 count = data[i];
        auto type = static_cast<ShadowType>(data[i + 1]);

        std::vector<LockInfo> locks;
        for (u8 j = 0; j < count; j++)
        {
            u8 nature = data[i + 2 + j * 3];
            u8 genderLower = data[i + 3 + j * 3];
            u8 genderUpper = data[i + 4 + j * 3];
            locks.emplace_back(nature, genderLower, genderUpper);
        }

        teams.emplace_back(locks, type);
        i += count * 3 + 2;
    }

    return teams;
}
