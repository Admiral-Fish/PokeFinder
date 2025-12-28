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

#ifndef XORSHIFT_HPP
#define XORSHIFT_HPP

#include <Core/Global.hpp>
#include <Core/RNG/SIMD.hpp>

/**
 * @brief Provides random numbers via the Xoroshift algorithm.
 */
class Xorshift
{
public:
    /**
     * @brief Construct a new Xorshift object
     *
     * @param seed0 Starting PRNG state0
     * @param seed1 Starting PRNG state1
     */
    Xorshift(u64 seed0, u64 seed1);

    /**
     * @brief Construct a new Xorshift object
     *
     * @param seed0 Starting PRNG state0
     * @param seed1 Starting PRNG state1
     * @param advances Number of initial advances
     */
    Xorshift(u64 seed0, u64 seed1, u32 advances);

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
     * @brief Gets the next 32bit PRNG state
     *
     * @return PRNG value
     */
    u32 next();

    /**
     * @brief Gets the next 32bit PRNG state bounded by the \p min and \p max values
     *
     * @param min Minimum value
     * @param max Maximum value
     *
     * @return PRNG value
     */
    u32 next(u32 min, u32 max)
    {
        u32 diff = max - min;
        return (next() % diff) + min;
    }

private:
    vuint128 state;
};

#endif // XORSHIFT_HPP
