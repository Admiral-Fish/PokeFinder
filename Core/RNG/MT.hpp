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

#ifndef MT_HPP
#define MT_HPP

#include <Core/Util/Global.hpp>

#ifdef USE_AVX2
#include <immintrin.h>
#else
#include <smmintrin.h>
#endif

class MT
{
public:
    explicit MT(u32 seed = 0);
    void advance(u32 advances);
    u32 next();
    u16 nextUShort();

private:
    alignas(16) u32 mt[624];
    u16 index;

    void shuffle();
};

// The assumptions of MTFast allow some simplifications to be made from normal MT
// 1. computing less of the internal MT array
// 2. skipping the shuffle check when generating numbers for use
// 3. if the fast parameter is true skip the last bit shift operation (only in IV generation)
template <u16 size, bool fast = false>
class MTFast
{
public:
    MTFast(u32 seed, u32 advances=0)
    {
        static_assert(size < 227, "Size exceeds range of MTFast");

        mt[0] = seed;

        for (index = 1; index < (size + 397); index++)
        {
            seed = 0x6c078965 * (seed ^ (seed >> 30)) + index;
            mt[index] = seed;
        }

        index = advances;
        shuffle();
    }

    u32 next()
    {
        u32 y = mt[index++];
        y ^= (y >> 11);
        y ^= (y << 7) & 0x9d2c5680;
        y ^= (y << 15) & 0xefc60000;
        if constexpr (!fast)
        {
            y ^= (y >> 18);
        }

        return y;
    }

    u16 nextUShort()
    {
        return next() >> 16;
    }

private:
    u32 mt[size + 397];
    u16 index;

    void shuffle()
    {
        int i = 0;

#ifdef USE_AVX2
        // Even with AVX2 only use it if the number of iterations remaining is less then 4
        // Otherwise using SSE2 is preferred so we don't mix-match AVX2 and SSE2
        if constexpr (size > 8 && (size % 8) < 4)
        {
            __m256i upperMask256 = _mm256_set1_epi32(0x80000000);
            __m256i lowerMask256 = _mm256_set1_epi32(0x7fffffff);
            __m256i matrix256 = _mm256_set1_epi32(0x9908b0df);
            __m256i one256 = _mm256_set1_epi32(1);

            for (; i < size - (size % 8); i += 8)
            {
                __m256i m0 = _mm256_loadu_si256((const __m256i *)&mt[i]);
                __m256i m1 = _mm256_loadu_si256((const __m256i *)&mt[i + 1]);
                __m256i m2 = _mm256_loadu_si256((const __m256i *)&mt[i + 397]);

                __m256i y = _mm256_or_si256(_mm256_and_si256(m0, upperMask256), _mm256_and_si256(m1, lowerMask256));
                __m256i y1 = _mm256_srli_epi32(y, 1);
                __m256i mag01 = _mm256_and_si256(_mm256_cmpeq_epi32(_mm256_and_si256(y, one256), one256), matrix256);

                _mm256_storeu_si256((__m256i *)&mt[i], _mm256_xor_si256(_mm256_xor_si256(y1, mag01), m2));
            }
        }
        else if constexpr (size > 4)
        {
            __m128i upperMask = _mm_set1_epi32(0x80000000);
            __m128i lowerMask = _mm_set1_epi32(0x7fffffff);
            __m128i matrix = _mm_set1_epi32(0x9908b0df);
            __m128i one = _mm_set1_epi32(1);

            for (; i < size - (size % 4); i += 4)
            {
                __m128i m0 = _mm_loadu_si128((const __m128i *)&mt[i]);
                __m128i m1 = _mm_loadu_si128((const __m128i *)&mt[i + 1]);
                __m128i m2 = _mm_loadu_si128((const __m128i *)&mt[i + 397]);

                __m128i y = _mm_or_si128(_mm_and_si128(m0, upperMask), _mm_and_si128(m1, lowerMask));
                __m128i y1 = _mm_srli_epi32(y, 1);
                __m128i mag01 = _mm_and_si128(_mm_cmpeq_epi32(_mm_and_si128(y, one), one), matrix);

                _mm_storeu_si128((__m128i *)&mt[i], _mm_xor_si128(_mm_xor_si128(y1, mag01), m2));
            }
        }
#else
        if constexpr (size > 4)
        {
            __m128i upperMask = _mm_set1_epi32(0x80000000);
            __m128i lowerMask = _mm_set1_epi32(0x7fffffff);
            __m128i matrix = _mm_set1_epi32(0x9908b0df);
            __m128i one = _mm_set1_epi32(1);

            for (; i < size - (size % 4); i += 4)
            {
                __m128i m0 = _mm_loadu_si128((const __m128i *)&mt[i]);
                __m128i m1 = _mm_loadu_si128((const __m128i *)&mt[i + 1]);
                __m128i m2 = _mm_loadu_si128((const __m128i *)&mt[i + 397]);

                __m128i y = _mm_or_si128(_mm_and_si128(m0, upperMask), _mm_and_si128(m1, lowerMask));
                __m128i y1 = _mm_srli_epi32(y, 1);
                __m128i mag01 = _mm_and_si128(_mm_cmpeq_epi32(_mm_and_si128(y, one), one), matrix);

                _mm_storeu_si128((__m128i *)&mt[i], _mm_xor_si128(_mm_xor_si128(y1, mag01), m2));
            }
        }
#endif

        for (; i < size; i++)
        {
            u32 m0 = mt[i];
            u32 m1 = mt[i + 1];

            u32 y = (m0 & 0x80000000) | (m1 & 0x7fffffff);

            u32 y1 = y >> 1;
            if (y & 1)
            {
                y1 ^= 0x9908b0df;
            }

            mt[i] = y1 ^ mt[i + 397];
        }
    }
};

#endif // MT_HPP
