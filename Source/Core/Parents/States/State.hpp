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

#ifndef STATE_HPP
#define STATE_HPP

#include <Core/Global.hpp>

class State
{
public:
    State() = default;

    explicit State(u64 advances) : advances(advances)
    {
    }

    u64 getAdvances() const
    {
        return advances;
    }

    u32 getPID() const
    {
        return pid;
    }

    u8 getIV(u8 index) const
    {
        return ivs[index];
    }

    u8 getAbility() const
    {
        return ability;
    }

    u8 getGender() const
    {
        return gender;
    }

    u8 getNature() const
    {
        return nature;
    }

    u8 getLevel() const
    {
        return level;
    }

    u8 getShiny() const
    {
        return shiny;
    }

protected:
    u64 advances;
    u32 pid;
    u8 ivs[6];
    u8 ability;
    u8 gender;
    u8 nature;
    u8 level;
    u8 shiny;
};

#endif // STATE_HPP
