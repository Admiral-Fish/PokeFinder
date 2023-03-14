/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2023 by Admiral_Fish, bumba, and EzPzStreamz
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

#ifndef XOROSHIRO_HPP
#define XOROSHIRO_HPP

#include <Core/Global.hpp>
#include <Core/RNG/SIMD.hpp>

/**
 * @brief Provides random numbers via the Xoroshiro algorithm.
 */
class Xoroshiro
{
public:
    /**
     * @brief Construct a new Xoroshiro object
     *
     * @param seed Starting PRNG state
     */
    Xoroshiro(u64 seed);

    /**
     * @brief Construct a new Xoroshiro object
     *
     * @param seed0 Starting PRNG state0
     * @param seed1 Starting PRNG state1
     */
    Xoroshiro(u64 seed0, u64 seed1);

    /**
     * @brief Advances the RNG by \p advances amount
     *
     * @param advances Number of advances
     */
    void advance(u32 advances);

    /**
     * @brief Jumps the RNG by \p advances amount
     * Uses a precomputed jump table to complete in O(4096)
     *
     * @param advances Number of advances
     */
    void jump(u32 advances);

    /**
     * @brief Gets the next 64bit PRNG state
     *
     * @return PRNG value
     */
    u64 next();

    /**
     * @brief Gets the next 32bit PRNG state bounded by the \p max value
     *
     * @tparam max Max value
     *
     * @return PRNG value
     */
    template <u32 max>
    u32 nextUInt()
    {
        auto bitMask = [](u32 x) constexpr
        {
            x--;
            x |= x >> 1;
            x |= x >> 2;
            x |= x >> 4;
            x |= x >> 8;
            x |= x >> 16;
            return x;
        };

        constexpr u32 mask = bitMask(max);
        if constexpr ((max - 1) == mask)
        {
            return next() & mask;
        }
        else
        {
            u32 result;
            do
            {
                result = next() & mask;
            } while (result >= max);
            return result;
        }
    }

private:
    alignas(16) vuint128 state;
};

/**
 * @brief Provides random numbers via the Xoroshiro algorithm with modified construction.
 */
class XoroshiroBDSP : public Xoroshiro
{
public:
    /**
     * @brief Construct a new XoroshiroBDSP object
     * Uses splitmix to initalize the PRNG state
     *
     * @param seed Starting PRNG state
     */
    XoroshiroBDSP(u64 seed);

    /**
     * @brief Gets the next 32bit PRNG state bounded by the \p max value
     *
     * @param max Max value
     *
     * @return PRNG value
     */
    u32 nextUInt(u32 max)
    {
        return (next() >> 32) % max;
    }
};

#endif // XOROSHIRO_HPP
