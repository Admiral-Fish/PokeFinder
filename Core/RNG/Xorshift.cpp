/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2024 by Admiral_Fish, bumba, and EzPzStreamz
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

#include "Xorshift.hpp"
#include <Core/RNG/Jump.hpp>
#include <bit>

static const vuint128 polynomial = vuint128(0xf985d65ffd3c8001, 0x000000010046d8b3);

Xorshift::Xorshift(u64 seed0, u64 seed1)
{
    u64 *ptr = &state.uint64[0];
    ptr[0] = std::rotl(seed0, 32);
    ptr[1] = std::rotl(seed1, 32);
}

Xorshift::Xorshift(u64 seed0, u64 seed1, u32 advances) : Xorshift(seed0, seed1)
{
    jump(advances);
}

void Xorshift::advance(u32 advances)
{
    for (u64 advance = 0; advance < advances; advance++)
    {
        next();
    }
}

void Xorshift::jump(u32 advances)
{
    auto jump = Jump::computeJumpPolynomial<128>(polynomial, advances);

    vuint128 temp(0);
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 64; j++)
        {
            if (jump.uint64[i] & (1ULL << j))
            {
                temp = temp ^ state;
            }
            next();
        }
    }

    state = temp;
}

u32 Xorshift::next()
{
    u32 *ptr = &state.uint32[0];
    u32 t = ptr[0];
    u32 s = ptr[3];

    t ^= t << 11;
    t ^= t >> 8;
    t ^= s ^ (s >> 19);

    ptr[0] = ptr[1];
    ptr[1] = ptr[2];
    ptr[2] = ptr[3];
    ptr[3] = t;

    return t;
}
