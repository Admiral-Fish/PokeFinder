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

#ifndef UNDERGROUNDSTATE_HPP
#define UNDERGROUNDSTATE_HPP

#include <Core/Parents/States/State.hpp>

class PersonalInfo;

class UndergroundState : public GeneratorState
{
public:
    UndergroundState(u32 advances, u16 specie, u32 pid, u8 shiny, const std::array<u8, 6> &ivs, u8 ability, u8 gender, u8 nature, u16 item,
                     u16 eggMove, const PersonalInfo *info);

    u16 getEggMove() const
    {
        return eggMove;
    }

    u16 getItem() const
    {
        return item;
    }

    u16 getSpecie() const
    {
        return specie;
    }

protected:
    u16 eggMove;
    u16 item;
    u16 specie;
};

#endif // UNDERGROUNDSTATE_HPP
