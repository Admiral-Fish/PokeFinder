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
        seed = 0x6c078965 * (seed ^ (seed >> 30)) + index;
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
    y ^= (y << 7) & 0x9d2c5680;
    y ^= (y << 15) & 0xefc60000;
    y ^= (y >> 18);

    return y;
}

u16 MT::nextUShort()
{
    return next() >> 16;
}

void MT::shuffle()
{
    /*  AVX2 implementation, eventually determine if doing runtime dispatching is beneifical
    __m128i upperMask = _mm_set1_epi32(0x80000000);
    __m128i lowerMask = _mm_set1_epi32(0x7fffffff);
    __m128i matrix = _mm_set1_epi32(0x9908b0df);
    __m128i one = _mm_set1_epi32(1);

    __m256i upperMask256 = _mm256_set1_epi32(0x80000000);
    __m256i lowerMask256 = _mm256_set1_epi32(0x7fffffff);
    __m256i matrix256 = _mm256_set1_epi32(0x9908b0df);
    __m256i one256 = _mm256_set1_epi32(1);

    for (int i = 0; i < 224; i += 8)
    {
        __m256i m0 = _mm256_loadu_si256((const __m256i *)&mt[i]);
        __m256i m1 = _mm256_loadu_si256((const __m256i *)&mt[i + 1]);
        __m256i m2 = _mm256_loadu_si256((const __m256i *)&mt[i + 397]);

        __m256i y = _mm256_or_si256(_mm256_and_si256(m0, upperMask256), _mm256_and_si256(m1, lowerMask256));
        __m256i y1 = _mm256_srli_epi32(y, 1);
        __m256i mag01 = _mm256_and_si256(_mm256_cmpeq_epi32(_mm256_and_si256(y, one256), one256), matrix256);

        _mm256_storeu_si256((__m256i *)&mt[i], _mm256_xor_si256(_mm256_xor_si256(y1, mag01), m2));
    }

    __m128i last = _mm_insert_epi32(_mm_loadu_si128((const __m128i *)&mt[621]), mt[0], 3);
    {
        __m128i m0 = _mm_loadu_si128((const __m128i *)&mt[224]);
        __m128i m1 = _mm_loadu_si128((const __m128i *)&mt[225]);

        __m128i y = _mm_or_si128(_mm_and_si128(m0, upperMask), _mm_and_si128(m1, lowerMask));
        __m128i y1 = _mm_srli_epi32(y, 1);
        __m128i mag01 = _mm_and_si128(_mm_cmpeq_epi32(_mm_and_si128(y, one), one), matrix);

        _mm_storeu_si128((__m128i *)&mt[224], _mm_xor_si128(_mm_xor_si128(y1, mag01), last));
    }

    for (int i = 228; i < 620; i += 8)
    {
        __m256i m0 = _mm256_loadu_si256((const __m256i *)&mt[i]);
        __m256i m1 = _mm256_loadu_si256((const __m256i *)&mt[i + 1]);
        __m256i m2 = _mm256_loadu_si256((const __m256i *)&mt[i - 227]);

        __m256i y = _mm256_or_si256(_mm256_and_si256(m0, upperMask256), _mm256_and_si256(m1, lowerMask256));
        __m256i y1 = _mm256_srli_epi32(y, 1);
        __m256i mag01 = _mm256_and_si256(_mm256_cmpeq_epi32(_mm256_and_si256(y, one256), one256), matrix256);

        _mm256_storeu_si256((__m256i *)&mt[i], _mm256_xor_si256(_mm256_xor_si256(y1, mag01), m2));
    }

    {
        __m128i m0 = _mm_loadu_si128((const __m128i *)&mt[620]);
        __m128i m2 = _mm_loadu_si128((const __m128i *)&mt[393]);

        __m128i y = _mm_or_si128(_mm_and_si128(m0, upperMask), _mm_and_si128(last, lowerMask));
        __m128i y1 = _mm_srli_epi32(y, 1);
        __m128i mag01 = _mm_and_si128(_mm_cmpeq_epi32(_mm_and_si128(y, one), one), matrix);

        _mm_storeu_si128((__m128i *)&mt[620], _mm_xor_si128(_mm_xor_si128(y1, mag01), m2));
    }*/

    __m128i upperMask = _mm_set1_epi32(0x80000000);
    __m128i lowerMask = _mm_set1_epi32(0x7fffffff);
    __m128i matrix = _mm_set1_epi32(0x9908b0df);
    __m128i one = _mm_set1_epi32(1);

    for (int i = 0; i < 224; i += 4)
    {
        __m128i m0 = _mm_loadu_si128((const __m128i *)&mt[i]);
        __m128i m1 = _mm_loadu_si128((const __m128i *)&mt[i + 1]);
        __m128i m2 = _mm_loadu_si128((const __m128i *)&mt[i + 397]);

        __m128i y = _mm_or_si128(_mm_and_si128(m0, upperMask), _mm_and_si128(m1, lowerMask));
        __m128i y1 = _mm_srli_epi32(y, 1);
        __m128i mag01 = _mm_and_si128(_mm_cmpeq_epi32(_mm_and_si128(y, one), one), matrix);

        _mm_storeu_si128((__m128i *)&mt[i], _mm_xor_si128(_mm_xor_si128(y1, mag01), m2));
    }

    __m128i last = _mm_insert_epi32(_mm_loadu_si128((const __m128i *)&mt[621]), mt[0], 3);
    {
        __m128i m0 = _mm_loadu_si128((const __m128i *)&mt[224]);
        __m128i m1 = _mm_loadu_si128((const __m128i *)&mt[225]);

        __m128i y = _mm_or_si128(_mm_and_si128(m0, upperMask), _mm_and_si128(m1, lowerMask));
        __m128i y1 = _mm_srli_epi32(y, 1);
        __m128i mag01 = _mm_and_si128(_mm_cmpeq_epi32(_mm_and_si128(y, one), one), matrix);

        _mm_storeu_si128((__m128i *)&mt[224], _mm_xor_si128(_mm_xor_si128(y1, mag01), last));
    }

    for (int i = 228; i < 620; i += 4)
    {
        __m128i m0 = _mm_loadu_si128((const __m128i *)&mt[i]);
        __m128i m1 = _mm_loadu_si128((const __m128i *)&mt[i + 1]);
        __m128i m2 = _mm_loadu_si128((const __m128i *)&mt[i - 227]);

        __m128i y = _mm_or_si128(_mm_and_si128(m0, upperMask), _mm_and_si128(m1, lowerMask));
        __m128i y1 = _mm_srli_epi32(y, 1);
        __m128i mag01 = _mm_and_si128(_mm_cmpeq_epi32(_mm_and_si128(y, one), one), matrix);

        _mm_storeu_si128((__m128i *)&mt[i], _mm_xor_si128(_mm_xor_si128(y1, mag01), m2));
    }

    {
        __m128i m0 = _mm_loadu_si128((const __m128i *)&mt[620]);
        __m128i m2 = _mm_loadu_si128((const __m128i *)&mt[393]);

        __m128i y = _mm_or_si128(_mm_and_si128(m0, upperMask), _mm_and_si128(last, lowerMask));
        __m128i y1 = _mm_srli_epi32(y, 1);
        __m128i mag01 = _mm_and_si128(_mm_cmpeq_epi32(_mm_and_si128(y, one), one), matrix);

        _mm_storeu_si128((__m128i *)&mt[620], _mm_xor_si128(_mm_xor_si128(y1, mag01), m2));
    }

    index -= 624;
}
