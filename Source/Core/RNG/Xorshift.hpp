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

#ifndef XORSHIFT_HPP
#define XORSHIFT_HPP

#include <Core/Util/Global.hpp>

class Xorshift
{
public:
    Xorshift(u64 seed0, u64 seed1) :
        state { static_cast<u32>(seed0 >> 32), static_cast<u32>(seed0 & 0xffffffff), static_cast<u32>(seed1 >> 32),
                static_cast<u32>(seed1 & 0xffffffff) }
    {
    }

    Xorshift(const Xorshift &other) : state { other.state[0], other.state[1], other.state[2], other.state[3] }
    {
    }

    u32 next()
    {
        u32 t = state[1];
        u32 s = state[2];

        t ^= t << 11;
        t ^= t >> 8;
        t ^= s ^ (s >> 19);

        state[1] = state[0];
        state[0] = state[3];
        state[3] = state[2];
        state[2] = t;

        return (t % 0xffffffff) + 0x80000000;
    }

    u32 next(u32 max)
    {
        return next() % max;
    }

    void advance(u32 advances)
    {
        for (u32 advance = 0; advance < advances; advance++)
        {
            next();
        }
    }

public:
    u32 state[4];
};

#endif // XORSHIFT_HPP
