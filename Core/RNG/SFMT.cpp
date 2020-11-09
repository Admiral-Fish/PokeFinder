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

#include "SFMT.hpp"
#include <emmintrin.h>

SFMT::SFMT(u32 seed)
{
    u32 inner = seed & 1;
    sfmt[0] = seed;

    for (index = 1; index < 624; index++)
    {
        seed = 0x6C078965 * (seed ^ (seed >> 30)) + index;
        sfmt[index] = seed;
    }

    inner ^= sfmt[3] & 0x13c9e684;
    inner ^= inner >> 16;
    inner ^= inner >> 8;
    inner ^= inner >> 4;
    inner ^= inner >> 2;
    inner ^= inner >> 1;

    sfmt[0] ^= ~inner & 1;
}

void SFMT::advance(u32 advances)
{
    index += (advances * 2);
    while (index >= 624)
    {
        shuffle();
    }
}

u64 SFMT::next()
{
    if (index >= 624)
    {
        shuffle();
    }

    u32 high = sfmt[index++];
    u32 low = sfmt[index++];
    return high | (static_cast<u64>(low) << 32);
}

u32 SFMT::nextUInt()
{
    if (index >= 624)
    {
        shuffle();
    }

    return sfmt[index++];
}

void SFMT::shuffle()
{
    __m128i c = _mm_load_si128((const __m128i *)&sfmt[616]);
    __m128i d = _mm_load_si128((const __m128i *)&sfmt[620]);
    __m128i mask = _mm_set_epi32(0xbffffff6, 0xbffaffff, 0xddfecb7f, 0xdfffffef);

    auto mm_recursion = [&mask](__m128i &a, const __m128i &b, const __m128i &c, const __m128i &d) {
        __m128i x = _mm_slli_si128(a, 1);
        __m128i y = _mm_srli_si128(c, 1);

        __m128i b1 = _mm_and_si128(_mm_srli_epi32(b, 11), mask);
        __m128i d1 = _mm_slli_epi32(d, 18);

        a = _mm_xor_si128(_mm_xor_si128(_mm_xor_si128(_mm_xor_si128(a, x), b1), y), d1);
    };

    for (int i = 0; i < 136; i += 4)
    {
        __m128i a = _mm_load_si128((const __m128i *)&sfmt[i]);
        __m128i b = _mm_load_si128((const __m128i *)&sfmt[i + 488]);

        mm_recursion(a, b, c, d);
        _mm_store_si128((__m128i *)&sfmt[i], a);

        c = d;
        d = a;
    }

    for (int i = 136; i < 624; i += 4)
    {
        __m128i a = _mm_load_si128((const __m128i *)&sfmt[i]);
        __m128i b = _mm_load_si128((const __m128i *)&sfmt[i - 136]);

        mm_recursion(a, b, c, d);
        _mm_store_si128((__m128i *)&sfmt[i], a);

        c = d;
        d = a;
    }

    index -= 624;
}
