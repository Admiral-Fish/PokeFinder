/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2021 by Admiral_Fish, bumba, and EzPzStreamz
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

#ifndef LOCKINFO_HPP
#define LOCKINFO_HPP

#include <Core/Util/Global.hpp>

class LockInfo
{
public:
    LockInfo() = default;

    constexpr LockInfo(u8 nature, u8 genderLower, u8 genderUpper) :
        nature(nature), genderLower(genderLower), genderUpper(genderUpper), free(nature == 255 && genderLower == 255 && genderUpper == 255)
    {
    }

    bool compare(u32 pid) const
    {
        u8 gender = pid & 255;
        return gender >= genderLower && gender <= genderUpper && nature == (pid % 25);
    }

    bool getFree() const
    {
        return free;
    }

public:
    u8 nature;
    u8 genderLower;
    u8 genderUpper;
    bool free;
};

#endif // LOCKINFO_HPP
