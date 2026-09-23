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
#include <Core/RNG/Jump.hpp>
#include <cstring>

constexpr u8 POLY[] = {
#include "SFMTPoly.txt"
};

static void mm_recursion(vuint128 &a, const vuint128 &b, const vuint128 &c, const vuint128 &d)
{
    vuint128 mask(0xdfffffef, 0xddfecb7f, 0xbffaffff, 0xbffffff6);

    vuint128 x = v128_shl<1>(a);
    vuint128 y = v128_shr<1>(c);

    vuint128 b1 = (b >> 11) & mask;
    vuint128 d1 = d << 18;

    a = a ^ x ^ b1 ^ y ^ d1;
}

SFMT::SFMT() : index(0)
{
    std::memset(state, 0, sizeof(state));
}

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

SFMT::SFMT(u32 seed, u32 advances) : SFMT(seed)
{
    jump(advances);
}

void SFMT::advance(u32 advances)
{
    u64 advance = (static_cast<u64>(advances) * 2) + index;
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

void SFMT::addState(const SFMT *other)
{
    u16 idx = other->index / 4;
    int split = 156 - idx;

    int i = 0;
    for (; i < split; i++)
    {
        state[i] = state[i] ^ other->state[idx + i];
    }

    for (; i < 156; i++)
    {
        state[i] = state[i] ^ other->state[idx + i - 156];
    }
}

void SFMT::jump(u32 advances)
{
    // Advances refers to advancing 64bit states
    // Normal advancing is done by 32bit states so it needs to be doubled
    // Jumping is done by 128bit states so it needs to be halfed
    if (advances < (2 * 32768))
    {
        advance(advances);
    }
    else
    {
        // Since this is only called by the constructor we need to reset index to 0 so we can shuffle 1 at a time
        index = 0;

        auto jump = Jump::computeJumpPolynomial(POLY, sizeof(POLY), advances >> 1);

        int byteCount = (jump.degree() + 8) / sizeof(u64);
        const u8 *bytes = reinterpret_cast<const u8 *>(jump.coefficients().store());

        SFMT temp;
        for (int i = 0; i < byteCount; i++)
        {
            u8 val = bytes[i];
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
        shuffle();

        // Handle the case where advances is odd and we can't jump by a final partial 64bit advance
        if ((advances & 1) == 1)
        {
            next();
        }
    }
}

void SFMT::nextState()
{
    u16 idx = index / 4;
    vuint128 a = state[idx];
    vuint128 b = state[(idx + 122) % 156];
    vuint128 c = state[(idx + 154) % 156];
    vuint128 d = state[(idx + 155) % 156];

    mm_recursion(a, b, c, d);
    state[idx] = a;

    index = (index + 4) % 624;
}

void SFMT::shuffle()
{
    vuint128 c = state[154];
    vuint128 d = state[155];

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
