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

#include "TinyMT.hpp"
#include <Core/RNG/Jump.hpp>
#include <bit>

static const vuint128 polynomial = vuint128(0x8dcc50c798faba43, 0x58524022ed8dff4a);

TinyMT::TinyMT(u32 seed) : state(seed, 0x8f7011ee, 0xfc78ff1f, 0x3793fdff)
{
    u32 *ptr = &state.uint32[0];
    for (u32 i = 1; i < 8; i++)
    {
        ptr[i & 3] ^= 0x6c078965 * (ptr[(i - 1) & 3] ^ (ptr[(i - 1) & 3] >> 30)) + i;
    }

    if ((ptr[0] & 0x7FFFFFFF) == 0 && ptr[1] == 0 && ptr[2] == 0 && ptr[3] == 0)
    {
        ptr[0] = 'T';
        ptr[1] = 'I';
        ptr[2] = 'N';
        ptr[3] = 'Y';
    }

    advance(8);
}

TinyMT::TinyMT(u32 seed0, u32 seed1, u32 seed2, u32 seed3) : state { seed0, seed1, seed2, seed3 }
{
}

void TinyMT::advance(u32 advances)
{
    for (u64 advance = 0; advance < advances; advance++)
    {
        nextState();
    }
}

void TinyMT::jump(u32 advances)
{
    auto jump = Jump::computeJumpPolynomial<127>(polynomial, advances);

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

u32 TinyMT::next()
{
    nextState();
    return temper();
}

u16 TinyMT::nextUShort()
{
    return next() >> 16;
}

void TinyMT::nextState()
{
    u32 *ptr = &state.uint32[0];
    u32 y = ptr[3];
    u32 x = (ptr[0] & 0x7FFFFFFF) ^ ptr[1] ^ ptr[2];

    x ^= (x << 1);
    y ^= (y >> 1) ^ x;

    ptr[0] = ptr[1];
    ptr[1] = ptr[2] ^ ((y & 1) * 0x8f7011ee);
    ptr[2] = x ^ (y << 10) ^ ((y & 1) * 0xfc78ff1f);
    ptr[3] = y;
}

u32 TinyMT::temper()
{
    u32 *ptr = &state.uint32[0];
    u32 t0 = ptr[3];
    u32 t1 = ptr[0] + (ptr[2] >> 8);

    t0 ^= t1;
    if (t1 & 1)
    {
        t0 ^= 0x3793fdff;
    }

    return t0;
}
