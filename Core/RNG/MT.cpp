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

#include "MT.hpp"
#include <cstring>

constexpr u8 jumpTable[9][2493] = {
#include "MTJump.txt"
};

MT::MT() : index(0)
{
    std::memset(state, 0, sizeof(state));
}

MT::MT(u32 seed) : index(624)
{
    u32 *ptr = &state[0].uint32[0];
    ptr[0] = seed;

    for (u32 i = 1; i < 624; i++)
    {
        seed = 0x6c078965 * (seed ^ (seed >> 30)) + i;
        ptr[i] = seed;
    }
}

MT::MT(u32 seed, u32 advances) : MT(seed)
{
    jump(advances);
}

void MT::advance(u32 advances)
{
    u64 advance = static_cast<u64>(advances) + index;
    while (advance >= 624)
    {
        shuffle();
        advance -= 624;
    }
    index = advance;
}

u32 MT::next()
{
    if (index == 624)
    {
        shuffle();
        index = 0;
    }

    u32 *ptr = &state[0].uint32[0];
    u32 y = ptr[index++];
    y ^= (y >> 11);
    y ^= (y << 7) & 0x9d2c5680;
    y ^= (y << 15) & 0xefc60000;
    y ^= (y >> 18);

    return y;
}

u16 MT::nextUShort()
{
    return next() >> 16;
}

void MT::addState(const MT *other)
{
    u32 *ptr = &state[0].uint32[0];
    const u32 *ptr1 = &other->state[0].uint32[0];

    int split = 624 - other->index;

    int i = 0;
    for (; i < split - (split % 4); i += 4)
    {
        state[i / 4] = state[i / 4] ^ v32x4_load(&ptr1[other->index + i]);
    }

    while (i < split || (i % 4) != 0)
    {
        ptr[i] ^= ptr1[(other->index + i) % 624];
        i++;
    }

    for (; i < 624; i += 4)
    {
        state[i / 4] = state[i / 4] ^ v32x4_load(&ptr1[other->index + i - 624]);
    }
}

void MT::jump(u32 advances)
{
    // Since this is only called by the constructor we need to reset index to 0 so we can shuffle 1 at a time
    index = 0;

    u32 high = advances >> 23;
    u32 low = advances & 0x7fffff;

    // Advance by amount unsupported by the jump tables
    // First shuffle by an amount of times divisible by 624 to utilize SIMD logic
    if (low)
    {
        u32 num = low % 624;
        advance(low - num);

        // Advance 1 by 1 for remaining amount. This will be less than 624
        for (int i = 0; i < num; i++)
        {
            nextState();
        }
    }

    for (int i = 0; high; i++, high >>= 1)
    {
        if (high & 1)
        {
            MT temp;

            for (int j = 2492; j >= 0; j--)
            {
                u8 val = jumpTable[i][j];
                for (int bit = 0; bit < 8; bit++)
                {
                    if (val & (1 << bit))
                    {
                        temp.addState(this);
                    }
                    nextState();
                }
            }

            *this = temp;
        }
    }

    shuffle();
}

void MT::nextState()
{
    u32 *ptr = &state[0].uint32[0];

    u32 y = (ptr[index] & 0x80000000) | (ptr[(index + 1) % 624] & 0x7fffffff);
    u32 y1 = y >> 1;
    if (y & 1)
    {
        y1 ^= 0x9908b0df;
    }
    ptr[index] = ptr[(index + 397) % 624] ^ y1;

    index = (index + 1) % 624;
}

void MT::shuffle()
{
    u32 *ptr = &state[0].uint32[0];
    vuint128 upperMask(0x80000000);
    vuint128 lowerMask(0x7fffffff);
    vuint128 matrix(0x9908b0df);
    vuint128 one(1);

    auto mm_recursion = [&upperMask, &lowerMask, &matrix, &one](vuint128 m0, vuint128 m1, vuint128 m2) {
        vuint128 y = (m0 & upperMask) | (m1 & lowerMask);
        vuint128 y1 = y >> 1;
        vuint128 mag01 = ((y & one) == one) & matrix;
        return y1 ^ mag01 ^ m2;
    };

    for (int i = 0; i < 224; i += 4)
    {
        vuint128 m0 = state[i / 4];
        vuint128 m1 = v32x4_load(ptr + i + 1);
        vuint128 m2 = v32x4_load(ptr + i + 397);
        state[i / 4] = mm_recursion(m0, m1, m2);
    }

    // This technically reads out of bounds of the array
    // This is okay however since it will read our MT index plus the additional padding
    // The out of bounds read is immediately replaced with valid data from mt[0]
    vuint128 last = v32x4_load(ptr + 621);
    last[3] = ptr[0];
    {
        vuint128 m0 = state[56];
        vuint128 m1 = v32x4_load(ptr + 225);
        state[56] = mm_recursion(m0, m1, last);
    }

    for (int i = 228; i < 620; i += 4)
    {
        vuint128 m0 = state[i / 4];
        vuint128 m1 = v32x4_load(ptr + i + 1);
        vuint128 m2 = v32x4_load(ptr + i - 227);
        state[i / 4] = mm_recursion(m0, m1, m2);
    }

    {
        vuint128 m0 = state[155];
        vuint128 m2 = v32x4_load(ptr + 393);
        state[155] = mm_recursion(m0, last, m2);
    }
}
