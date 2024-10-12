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

#include "SFMT.hpp"

SFMT::SFMT(u32 seed) : index(624)
{
    u32 inner = seed & 1;
    u32 *ptr = &state[0].uint32[0];
    ptr[0] = seed;

    for (u32 i = 1; i < 624; i++)
    {
        seed = 0x6C078965 * (seed ^ (seed >> 30)) + i;
        ptr[i] = seed;
    }

    inner ^= ptr[3] & 0x13c9e684;
    inner ^= inner >> 16;
    inner ^= inner >> 8;
    inner ^= inner >> 4;
    inner ^= inner >> 2;
    inner ^= inner >> 1;

    ptr[0] ^= ~inner & 1;
}

void SFMT::advance(u32 advances)
{
    u64 advance = (advances * 2) + index;
    while (advance >= 624)
    {
        shuffle();
        advance -= 624;
    }
    index = advance;
}

u64 SFMT::next()
{
    if (index == 624)
    {
        shuffle();
        index = 0;
    }

    u32 *ptr = &state[0].uint32[0];
    u64 val = *reinterpret_cast<u64 *>(ptr + index);
    index += 2;
    return val;
}

u32 SFMT::nextUInt()
{
    if (index == 624)
    {
        shuffle();
        index = 0;
    }

    u32 *ptr = &state[0].uint32[0];
    return ptr[index++];
}

void SFMT::shuffle()
{
    vuint128 c = state[154];
    vuint128 d = state[155];
    vuint128 mask(0xdfffffef, 0xddfecb7f, 0xbffaffff, 0xbffffff6);

    auto mm_recursion = [&mask](vuint128 &a, const vuint128 &b, const vuint128 &c, const vuint128 &d) {
        vuint128 x = v128_shl<1>(a);
        vuint128 y = v128_shr<1>(c);

        vuint128 b1 = (b >> 11) & mask;
        vuint128 d1 = d << 18;

        a = a ^ x ^ b1 ^ y ^ d1;
    };

    for (int i = 0; i < 34; i++)
    {
        vuint128 a = state[i];
        vuint128 b = state[i + 122];

        mm_recursion(a, b, c, d);
        state[i] = a;

        c = d;
        d = a;
    }

    for (int i = 34; i < 156; i++)
    {
        vuint128 a = state[i];
        vuint128 b = state[i - 34];

        mm_recursion(a, b, c, d);
        state[i] = a;

        c = d;
        d = a;
    }
}
