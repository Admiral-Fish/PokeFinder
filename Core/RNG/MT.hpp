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
#include <Core/RNG/SIMD.hpp>

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
// 2. storing less of the internal MT array
// 3. skipping the shuffle check when generating numbers for use
// 4. if the fast parameter is true skip the last bit shift operation (only in gen 5)
template <u16 size, bool fast = false>
class MTFast
{
public:
    MTFast(u32 seed, u32 advances = 0) : index(1)
    {
        static_assert(size < 227, "Size exceeds range of MTFast");

        mt[0] = seed;
        do
        {
            seed = 0x6c078965 * (seed ^ (seed >> 30)) + index;
            mt[index++] = seed;
        } while (index < (size + 1));

        do
        {
            seed = 0x6c078965 * (seed ^ (seed >> 30)) + index++;
        } while (index < 397);

        u32 *ptr = &temper[0];
        do
        {
            seed = 0x6c078965 * (seed ^ (seed >> 30)) + index++;
            *ptr++ = seed;
        } while (index < (size + 397));

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
    alignas(16) u32 mt[size + 1];
    alignas(16) u32 temper[size];
    u16 index;

    void shuffle()
    {
        int i = 0;

#ifdef SIMD_256BIT
        // Even with 256-bit SIMD only use it if the number of iterations remaining is less than 4
        // Otherwise using 128-bit SIMD is preferred so we don't mix-match SIMD extensions
        if constexpr (size >= 8 && (size % 8) < 4)
        {
            vuint32x8 upperMask256 = v32x8_splat(0x80000000);
            vuint32x8 lowerMask256 = v32x8_splat(0x7fffffff);
            vuint32x8 matrix256 = v32x8_splat(0x9908b0df);
            vuint32x8 one256 = v32x8_splat(1);

            for (; i < size - (size % 8); i += 8)
            {
                vuint32x8 m0 = v32x8_load(&mt[i]);
                vuint32x8 m1 = v32x8_load(&mt[i + 1]);
                vuint32x8 m2 = v32x8_load(&temper[i]);

                vuint32x8 y = v32x8_or(v32x8_and(m0, upperMask256), v32x8_and(m1, lowerMask256));
                vuint32x8 y1 = v32x8_shr(y, 1);
                vuint32x8 mag01 = v32x8_and(v32x8_cmpeq(v32x8_and(y, one256), one256), matrix256);

                v32x8_store(&mt[i], v32x8_xor(v32x8_xor(y1, mag01), m2));
            }
        }
        else if constexpr (size >= 4)
        {
            vuint32x4 upperMask = v32x4_splat(0x80000000);
            vuint32x4 lowerMask = v32x4_splat(0x7fffffff);
            vuint32x4 matrix = v32x4_splat(0x9908b0df);
            vuint32x4 one = v32x4_splat(1);

            for (; i < size - (size % 4); i += 4)
            {
                vuint32x4 m0 = v32x4_load((const vuint32x4 *)&mt[i]);
                vuint32x4 m1 = v32x4_load((const vuint32x4 *)&mt[i + 1]);
                vuint32x4 m2 = v32x4_load((const vuint32x4 *)&temper[i]);

                vuint32x4 y = v32x4_or(v32x4_and(m0, upperMask), v32x4_and(m1, lowerMask));
                vuint32x4 y1 = v32x4_shr(y, 1);
                vuint32x4 mag01 = v32x4_and(v32x4_cmpeq(v32x4_and(y, one), one), matrix);

                _mm_storeu_si128((vuint32x4 *)&mt[i], v32x4_xor(v32x4_xor(y1, mag01), m2));
            }
        }
#else
        if constexpr (size >= 4)
        {
            vuint32x4 upperMask = v32x4_splat(0x80000000);
            vuint32x4 lowerMask = v32x4_splat(0x7fffffff);
            vuint32x4 matrix = v32x4_splat(0x9908b0df);
            vuint32x4 one = v32x4_splat(1);

            for (; i < size - (size % 4); i += 4)
            {
                vuint32x4 m0 = v32x4_load(&mt[i]);
                vuint32x4 m1 = v32x4_load(&mt[i + 1]);
                vuint32x4 m2 = v32x4_load(&temper[i]);

                vuint32x4 y = v32x4_or(v32x4_and(m0, upperMask), v32x4_and(m1, lowerMask));
                vuint32x4 y1 = v32x4_shr(y, 1);
                vuint32x4 mag01 = v32x4_and(v32x4_cmpeq(v32x4_and(y, one), one), matrix);

                v32x4_store(&mt[i], v32x4_xor(v32x4_xor(y1, mag01), m2));
            }
        }
#endif

        for (; i < size; i++)
        {
            u32 m0 = mt[i];
            u32 m1 = mt[i + 1];
            u32 m2 = temper[i];

            u32 y = (m0 & 0x80000000) | (m1 & 0x7fffffff);

            u32 y1 = y >> 1;
            if (y & 1)
            {
                y1 ^= 0x9908b0df;
            }

            mt[i] = y1 ^ m2;
        }
    }
};

#endif // MT_HPP
