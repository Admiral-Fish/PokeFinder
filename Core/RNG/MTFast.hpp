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

#ifndef MTFAST_HPP
#define MTFAST_HPP

#include <Core/RNG/SIMD.hpp>
#include <Core/Util/Global.hpp>

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

        u32 i = 1;
        for (u32 &x : mt)
        {
            x = seed;
            seed = 0x6c078965 * (seed ^ (seed >> 30)) + i++;
        }

        do
        {
            seed = 0x6c078965 * (seed ^ (seed >> 30)) + i++;
        } while (i < 397);

        // Shuffle with SIMD if size is big enough
        if constexpr (size >= 4)
        {
            vuint32x4 upperMask = v32x4_set(0x80000000);
            vuint32x4 lowerMask = v32x4_set(0x7fffffff);
            vuint32x4 matrix = v32x4_set(0x9908b0df);
            vuint32x4 one = v32x4_set(1);
            vuint32x4 mask1 = v32x4_set(0x9d2c5680);
            vuint32x4 mask2 = v32x4_set(fast ? 0xe8000000 : 0xefc60000);

            for (int j = 0; j < size - (size % 4); j += 4)
            {
                vuint32x4 m0 = v32x4_load(&mt[j]);
                vuint32x4 m1 = v32x4_load(&mt[j + 1]);

                u32 x0 = 0x6c078965 * (seed ^ (seed >> 30)) + (j + 397);
                u32 x1 = 0x6c078965 * (x0 ^ (x0 >> 30)) + (j + 398);
                u32 x2 = 0x6c078965 * (x1 ^ (x1 >> 30)) + (j + 399);
                seed = 0x6c078965 * (x2 ^ (x2 >> 30)) + (j + 400);

                vuint32x4 m2 = v32x4_set(x0, x1, x2, seed);

                vuint32x4 y = v32x4_or(v32x4_and(m0, upperMask), v32x4_and(m1, lowerMask));
                vuint32x4 y1 = v32x4_shr(y, 1);
                vuint32x4 mag01 = v32x4_and(v32x4_cmpeq(v32x4_and(y, one), one), matrix);

                // Temper results while shuffling
                y = v32x4_xor(v32x4_xor(y1, mag01), m2);
                y = v32x4_xor(y, v32x4_shr(y, 11));
                y = v32x4_xor(y, v32x4_and(v32x4_shl(y, 7), mask1));
                y = v32x4_xor(y, v32x4_and(v32x4_shl(y, 15), mask2));
                if constexpr (!fast)
                {
                    y = v32x4_xor(y, v32x4_shr(y, 18));
                }

                v32x4_store(&mt[j], y);
            }
        }

        // Shuffle without SIMD if neccessary (SIMD usage not possible or didn't cover everything)
        if constexpr ((size % 4) != 0)
        {
            for (int j = size - (size % 4); j < size; j++)
            {
                u32 m0 = mt[j];
                u32 m1 = mt[j + 1];
                seed = 0x6c078965 * (seed ^ (seed >> 30)) + (j + 397);

                u32 y = (m0 & 0x80000000) | (m1 & 0x7fffffff);

                u32 y1 = y >> 1;
                if (y & 1)
                {
                    y1 ^= 0x9908b0df;
                }

                // Temper results while shuffling
                y = y1 ^ seed;
                y ^= (y >> 11);
                y ^= (y << 7) & 0x9d2c5680;
                if constexpr (fast)
                {
                    y ^= (y << 15) & 0xe8000000;
                }
                else
                {
                    y ^= (y << 15) & 0xefc60000;
                    y ^= (y >> 18);
                }

                mt[j] = y;
            }
        }
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
    u16 index;
};

#endif // MTFAST_HPP
