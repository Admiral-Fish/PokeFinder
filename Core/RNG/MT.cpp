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

#include "MT.hpp"

MT::MT(u32 seed)
{
    mt[0] = seed;

    for (index = 1; index < 624; index++)
    {
        seed = 0x6C078965 * (seed ^ (seed >> 30)) + index;
        mt[index] = seed;
    }
}

void MT::advance(u32 advances)
{
    index += advances;
    while (index >= 624)
    {
        shuffle();
    }
}

u32 MT::next()
{
    if (index >= 624)
    {
        shuffle();
    }

    u32 y = mt[index++];
    y ^= (y >> 11);
    y ^= (y << 7) & 0x9D2C5680;
    y ^= (y << 15) & 0xEFC60000;
    y ^= (y >> 18);

    return y;
}

u16 MT::nextUShort()
{
    return next() >> 16;
}

void MT::shuffle()
{
#ifdef __SSE2__
    __m128i upperMask = _mm_set1_epi32(0x80000000);
    __m128i lowerMask = _mm_set1_epi32(0x7fffffff);
    __m128i matrix = _mm_set1_epi32(0x9908B0DF);
    __m128i one = _mm_set1_epi32(1);

    for (int i = 0; i < 224; i += 4)
    {
        __m128i m0 = *(__m128i *)&mt[i];
        __m128i m1 = *(__m128i_u *)&mt[i + 1];
        __m128i m2 = *(__m128i_u *)&mt[i + 397];

        __m128i y = _mm_or_si128(_mm_and_si128(m0, upperMask), _mm_and_si128(m1, lowerMask));
        __m128i y1 = _mm_srli_epi32(y, 1);

        __m128i mag01 = _mm_and_si128(y, one);
        mag01 = _mm_cmpeq_epi32(mag01, one);
        mag01 = _mm_and_si128(mag01, matrix);

        y1 = _mm_xor_si128(y1, mag01);
        y1 = _mm_xor_si128(y1, m2);

        *(__m128i *)&mt[i] = y1;
    }

    {
        u32 y = (mt[224] & 0x80000000) | (mt[225] & 0x7fffffff);
        mt[224] = (y >> 1) ^ ((y & 1) ? 0x9908B0DF : 0) ^ mt[621];

        y = (mt[225] & 0x80000000) | (mt[226] & 0x7fffffff);
        mt[225] = (y >> 1) ^ ((y & 1) ? 0x9908B0DF : 0) ^ mt[622];

        y = (mt[226] & 0x80000000) | (mt[227] & 0x7fffffff);
        mt[226] = (y >> 1) ^ ((y & 1) ? 0x9908B0DF : 0) ^ mt[623];

        y = (mt[227] & 0x80000000) | (mt[228] & 0x7fffffff);
        mt[227] = (y >> 1) ^ ((y & 1) ? 0x9908B0DF : 0) ^ mt[0];
    }

    for (int i = 228; i < 620; i += 4)
    {
        __m128i m0 = *(__m128i *)&mt[i];
        __m128i m1 = *(__m128i_u *)&mt[i + 1];
        __m128i m2 = *(__m128i_u *)&mt[i - 227];

        __m128i y = _mm_or_si128(_mm_and_si128(m0, upperMask), _mm_and_si128(m1, lowerMask));
        __m128i y1 = _mm_srli_epi32(y, 1);

        __m128i mag01 = _mm_and_si128(y, one);
        mag01 = _mm_cmpeq_epi32(mag01, one);
        mag01 = _mm_and_si128(mag01, matrix);

        y1 = _mm_xor_si128(y1, mag01);
        y1 = _mm_xor_si128(y1, m2);

        *(__m128i *)&mt[i] = y1;
    }

    {
        u32 y = (mt[620] & 0x80000000) | (mt[621] & 0x7fffffff);
        mt[620] = (y >> 1) ^ ((y & 1) ? 0x9908B0DF : 0) ^ mt[393];

        y = (mt[621] & 0x80000000) | (mt[622] & 0x7fffffff);
        mt[621] = (y >> 1) ^ ((y & 1) ? 0x9908B0DF : 0) ^ mt[394];

        y = (mt[622] & 0x80000000) | (mt[623] & 0x7fffffff);
        mt[622] = (y >> 1) ^ ((y & 1) ? 0x9908B0DF : 0) ^ mt[395];

        y = (mt[623] & 0x80000000) | (mt[0] & 0x7fffffff);
        mt[623] = (y >> 1) ^ ((y & 1) ? 0x9908B0DF : 0) ^ mt[396];
    }
#else
    u32 mt1 = mt[0], mt2;

    for (u16 i = 0; i < 227; i++)
    {
        mt2 = mt[i + 1];

        u32 y = (mt1 & 0x80000000) | (mt2 & 0x7fffffff);

        u32 y1 = y >> 1;
        if (y & 1)
        {
            y1 ^= 0x9908B0DF;
        }

        mt[i] = y1 ^ mt[i + 397];
        mt1 = mt2;
    }

    for (u16 i = 227; i < 623; i++)
    {
        mt2 = mt[i + 1];

        u32 y = (mt1 & 0x80000000) | (mt2 & 0x7fffffff);

        u32 y1 = y >> 1;
        if (y & 1)
        {
            y1 ^= 0x9908B0DF;
        }

        mt[i] = y1 ^ mt[i - 227];
        mt1 = mt2;
    }

    u32 y = (mt1 & 0x80000000) | (mt[0] & 0x7fffffff);

    u32 y1 = y >> 1;
    if (y & 1)
    {
        y1 ^= 0x9908B0DF;
    }

    mt[623] = y1 ^ mt[396];
#endif

    index -= 624;
}
