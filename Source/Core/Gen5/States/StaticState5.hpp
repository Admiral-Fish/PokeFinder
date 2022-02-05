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

#ifndef STATICSTATE5_HPP
#define STATICSTATE5_HPP

#include <Core/Gen5/States/State5.hpp>
#include <Core/Parents/States/StaticState.hpp>

class StaticState5 : public State5, public StaticState
{
public:
    StaticState5() = default;

    explicit StaticState5(u32 advance) : StaticState(advance)
    {
    }

    u32 getIVState() const
    {
        return ivState;
    }

    void setIVState(u32 ivState)
    {
        this->ivState = ivState;
    }

private:
    u32 ivState;
};

#endif // STATICSTATE5_HPP
