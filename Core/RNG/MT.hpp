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

#ifdef __SSE2__
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
    u32 mt[624];
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
            seed = 0x6C078965 * (seed ^ (seed >> 30)) + index;
            mt[index] = seed;
        }

        index = advances;
        shuffle();
    }

    u32 next()
    {
        u32 y = mt[index++];
        y ^= (y >> 11);
        y ^= (y << 7) & 0x9D2C5680;
        y ^= (y << 15) & 0xEFC60000;
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
#ifdef __SSE2__
        if constexpr (size > 4)
        {
            __m128i upperMask = _mm_set1_epi32(0x80000000);
            __m128i lowerMask = _mm_set1_epi32(0x7fffffff);
            __m128i matrix = _mm_set1_epi32(0x9908B0DF);
            __m128i one = _mm_set1_epi32(1);

            u16 i = 0;
            for (; i < size - (size % 4); i += 4)
            {
                __m128i m0 = *(__m128i *)&mt[i];
                __m128i m1 = *(__m128i *)&mt[i + 1];
                __m128i m2 = *(__m128i *)&mt[i + 397];

                __m128i y = _mm_or_si128(_mm_and_si128(m0, upperMask), _mm_and_si128(m1, lowerMask));
                __m128i y1 = _mm_srli_epi32(y, 1);

                __m128i mag01 = _mm_and_si128(y, one);
                mag01 = _mm_cmpeq_epi32(mag01, one);
                mag01 = _mm_and_si128(mag01, matrix);

                y1 = _mm_xor_si128(y1, mag01);
                y1 = _mm_xor_si128(y1, m2);

                *(__m128i *)&mt[i] = y1;
            }

            for (; i < size; i++)
            {
                u32 y = (mt[i] & 0x80000000) | (mt[i + 1] & 0x7fffffff);

                u32 y1 = y >> 1;
                if (y & 1)
                {
                    y1 ^= 0x9908B0DF;
                }

                mt[i] = y1 ^ mt[i + 397];
            }
        }
        else
        {
            u32 mt1 = mt[0], mt2;

            for (u16 i = 0; i < size; i++)
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
        }
#else
        u32 mt1 = mt[0], mt2;

        for (u16 i = 0; i < size; i++)
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
#endif
    }
};

#endif // MT_HPP
