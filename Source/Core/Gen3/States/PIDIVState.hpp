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

#ifndef PIDIVSTATE_HPP
#define PIDIVSTATE_HPP

#include <Core/Util/Global.hpp>

enum Method : u8;

class PIDIVState
{
public:
    PIDIVState() = default;

    PIDIVState(u32 seed, Method method) : seed(seed), method(method)
    {
    }

    u32 getSeed() const
    {
        return seed;
    }

    Method getMethod() const
    {
        return method;
    }

    u8 getIV(u8 index) const
    {
        return ivs[index];
    }

    void setIV(u8 index, u8 iv)
    {
        ivs[index] = iv;
    }

private:
    u32 seed;
    Method method;
    u8 ivs[6];
};

#endif // PIDIVSTATE_HPP
