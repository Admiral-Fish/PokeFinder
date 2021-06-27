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

#include "TinyMT.hpp"
#include <cstring>

TinyMT::TinyMT(u32 seed) : state { seed, 0x8f7011ee, 0xfc78ff1f, 0x3793fdff }
{
    for (u32 i = 1; i < 8; i++)
    {
        state[i & 3] ^= 0x6c078965 * (state[(i - 1) & 3] ^ (state[(i - 1) & 3] >> 30)) + i;
    }

    if ((state[0] & 0x7FFFFFFF) == 0 && state[1] == 0 && state[2] == 0 && state[3] == 0)
    {
        state[0] = 'T';
        state[1] = 'I';
        state[2] = 'N';
        state[3] = 'Y';
    }

    for (u8 i = 0; i < 8; i++)
    {
        nextState();
    }
}

TinyMT::TinyMT(const u32 state[])
{
    std::memcpy(this->state, state, sizeof(this->state));
}

void TinyMT::advance(u32 advances)
{
    for (u32 advance = 0; advance < advances; advance++)
    {
        nextState();
    }
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
    u32 y = state[3];
    u32 x = (state[0] & 0x7FFFFFFF) ^ state[1] ^ state[2];

    x ^= (x << 1);
    y ^= (y >> 1) ^ x;

    state[0] = state[1];
    state[1] = state[2] ^ ((y & 1) * 0x8f7011ee);
    state[2] = x ^ (y << 10) ^ ((y & 1) * 0xfc78ff1f);
    state[3] = y;
}

u32 TinyMT::temper()
{
    u32 t0 = state[3];
    u32 t1 = state[0] + (state[2] >> 8);

    t0 ^= t1;
    if (t1 & 1)
    {
        t0 ^= 0x3793fdff;
    }

    return t0;
}
