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

#ifndef DREAMRADARSLOT_HPP
#define DREAMRADARSLOT_HPP

#include <Core/Util/Global.hpp>

class DreamRadarSlot
{
public:
    DreamRadarSlot() = default;

    DreamRadarSlot(u8 type, u8 gender, u8 genderRatio) : type(type), gender(gender), genderRatio(genderRatio)
    {
    }

    u8 getType() const
    {
        return type;
    }

    u8 getGender() const
    {
        return gender;
    }

    u8 getGenderRatio() const
    {
        return genderRatio;
    }

private:
    u8 type; // 0: genie, 1: gen 4 legends, 2: non-legend
    u8 gender;
    u8 genderRatio;
};

#endif // DREAMRADARSLOT_HPP
