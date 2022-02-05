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

#ifndef XORSHIFT_HPP
#define XORSHIFT_HPP

#include <Core/Util/Global.hpp>

class Xorshift
{
public:
    Xorshift(u64 seed0, u64 seed1);
    Xorshift(const Xorshift &rng);
    void advance(u32 advances);

    template <int min = -0x7fffffff - 1, int max = 0x7fffffff>
    u32 next()
    {
        u32 t = nextState();
        u32 diff = max - min;

        return (t % diff) + min;
    }

public:
    u32 state[4];

    u32 nextState();
};

#endif // XORSHIFT_HPP
