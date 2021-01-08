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
// 5. Temper the results in the initial shuffle to take advantage of SIMD
template <u16 size, bool fast = false>
class MTFast
{
public:
    MTFast(u32 seed, u32 advances = 0) : index(advances)
    {
        static_assert(size < 227, "Size exceeds range of MTFast");

        mt[0] = seed;
        u32 i = 1;
        do
        {
            seed = 0x6c078965 * (seed ^ (seed >> 30)) + i;
            mt[i++] = seed;
        } while (i < (size + 1));

        do
        {
            seed = 0x6c078965 * (seed ^ (seed >> 30)) + i++;
        } while (i < 397);

        for (u32 &x : temper)
        {
            seed = 0x6c078965 * (seed ^ (seed >> 30)) + i++;
            x = seed;
        }

        shuffle();
    }

    u32 next()
    {
        return mt[index++];
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

        if constexpr (size >= 4)
        {
            vuint32x4 upperMask = v32x4_set(0x80000000);
            vuint32x4 lowerMask = v32x4_set(0x7fffffff);
            vuint32x4 matrix = v32x4_set(0x9908b0df);
            vuint32x4 one = v32x4_set(1);
            vuint32x4 mask1 = v32x4_set(0x9d2c5680);
            vuint32x4 mask2 = v32x4_set(0xefc60000);

            for (; i < size - (size % 4); i += 4)
            {
                vuint32x4 m0 = v32x4_load(&mt[i]);
                vuint32x4 m1 = v32x4_load(&mt[i + 1]);
                vuint32x4 m2 = v32x4_load(&temper[i]);

                vuint32x4 y = v32x4_or(v32x4_and(m0, upperMask), v32x4_and(m1, lowerMask));
                vuint32x4 y1 = v32x4_shr(y, 1);
                vuint32x4 mag01 = v32x4_and(v32x4_cmpeq(v32x4_and(y, one), one), matrix);

                y = v32x4_xor(v32x4_xor(y1, mag01), m2);
                y = v32x4_xor(y, v32x4_shr(y, 11));
                y = v32x4_xor(y, v32x4_and(v32x4_shl(y, 7), mask1));
                y = v32x4_xor(y, v32x4_and(v32x4_shl(y, 15), mask2));
                if constexpr (!fast)
                {
                    y = v32x4_xor(y, v32x4_shr(y, 18));
                }

                v32x4_store(&mt[i], y);
            }
        }

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

            y = y1 ^ m2;
            y ^= (y >> 11);
            y ^= (y << 7) & 0x9d2c5680;
            y ^= (y << 15) & 0xefc60000;
            if constexpr (!fast)
            {
                y ^= (y >> 18);
            }

            mt[i] = y;
        }
    }
};

#endif // MT_HPP
