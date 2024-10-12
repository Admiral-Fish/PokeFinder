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

#ifndef MTFAST_HPP
#define MTFAST_HPP

#include <Core/Global.hpp>
#include <Core/RNG/SIMD.hpp>

/**
 * @brief Provides random numbers via the Mersenne Twister algorithm.
 * The assumptions of MTFast allow some simplifications to be made from normal MT
 * 1. Computing less of the internal MT array
 * 2. Storing less of the internal MT array
 * 3. Skipping the shuffle check when generating numbers for use
 * 4. If the fast parameter is true skip the last bit shift operation and shift by 27 during shuffle (only in gen 5)
 * 5. Temper the results in the initial shuffle to take advantage of SIMD
 *
 * @tparam size Number of values to have generated
 * @tparam fast Whether to skip last bit shift operations
 */
template <u16 size, bool fast = false>
class MTFast
{
public:
    /**
     * @brief Construct a new MTFast object
     *
     * @param seed Starting PRNG state
     * @param advances Number of initial advances
     */
    MTFast(u32 seed, u32 advances = 0) : index(advances)
    {
        static_assert(size < 227, "Size exceeds range of MTFast");

        u32 i = 1;
        u32 *ptr = &state[0].uint32[0];
        for (; i < size + 2; i++)
        {
            ptr[i - 1] = seed;
            seed = 0x6c078965 * (seed ^ (seed >> 30)) + i;
        }

        do
        {
            seed = 0x6c078965 * (seed ^ (seed >> 30)) + i++;
        } while (i < 397);

        // Shuffle with SIMD if size is big enough
        if constexpr (size >= 4)
        {
            vuint128 upperMask(0x80000000);
            vuint128 lowerMask(0x7fffffff);
            vuint128 matrix(0x9908b0df);
            vuint128 one(1);
            vuint128 mask1(0x9d2c5680);
            vuint128 mask2(fast ? 0xe8000000 : 0xefc60000);

            for (u32 j = 0; j < size - (size % 4); j += 4)
            {
                vuint128 m0 = state[j / 4];
                vuint128 m1 = v32x4_load(ptr + j + 1);

                u32 x0 = 0x6c078965 * (seed ^ (seed >> 30)) + (j + 397);
                u32 x1 = 0x6c078965 * (x0 ^ (x0 >> 30)) + (j + 398);
                u32 x2 = 0x6c078965 * (x1 ^ (x1 >> 30)) + (j + 399);
                seed = 0x6c078965 * (x2 ^ (x2 >> 30)) + (j + 400);

                vuint128 m2(x0, x1, x2, seed);

                vuint128 y = (m0 & upperMask) | (m1 & lowerMask);
                vuint128 y1 = y >> 1;
                vuint128 mag01 = ((y & one) == one) & matrix;

                // Temper results while shuffling
                y = y1 ^ mag01 ^ m2;
                y = y ^ (y >> 11);
                y = y ^ ((y << 7) & mask1);
                y = y ^ ((y << 15) & mask2);
                if constexpr (fast)
                {
                    y = y >> 27;
                }
                else
                {
                    y = y ^ (y >> 18);
                }

                state[j / 4] = y;
            }
        }

        // Shuffle without SIMD if neccessary (SIMD usage not possible or didn't cover everything)
        if constexpr ((size % 4) != 0)
        {
            for (u32 j = size - (size % 4); j < size; j++)
            {
                u32 m0 = ptr[j];
                u32 m1 = ptr[j + 1];
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
                    y >>= 27;
                }
                else
                {
                    y ^= (y << 15) & 0xefc60000;
                    y ^= (y >> 18);
                }

                ptr[j] = y;
            }
        }
    }

    /**
     * @brief Gets the next 32bit PRNG state
     *
     * @return PRNG value
     */
    u32 next()
    {
        u32 *ptr = &state[0].uint32[0];
        return ptr[index++];
    }

private:
    vuint128 state[(size + 4) / 4];
    u16 index;
};

#endif // MTFAST_HPP
