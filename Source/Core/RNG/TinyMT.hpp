/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2022 by Admiral_Fish, bumba, and EzPzStreamz
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

#ifndef TINYMT_HPP
#define TINYMT_HPP

#include <Core/Global.hpp>
#include <Core/RNG/SIMD.hpp>

/**
 * @brief Provides random numbers via the Tiny Mersenne Twister algorithm.
 */
class TinyMT
{
public:
    /**
     * @brief Construct a new TinyMT object
     *
     * @param seed Starting PRNG value
     */
    TinyMT(u32 seed);

    /**
     * @brief Construct a new TinyMT object
     *
     * @param seed0 Starting PRNG value 0
     * @param seed1 Starting PRNG value 1
     * @param seed2 Starting PRNG value 2
     * @param seed3 Starting PRNG value 3
     */
    TinyMT(u32 seed0, u32 seed1, u32 seed2, u32 seed3);

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
     * @brief Gets the next 16bit PRNG state
     *
     * @return PRNG value
     */
    u16 nextUShort();

private:
    alignas(16) vuint128 state;

    /**
     * @brief Advances the RNG to the next PRNG state
     */
    void nextState();

    /**
     * @brief Generates the PRNG value from the current PRNG state
     *
     * @return PRNG value
     */
    u32 temper();
};

#endif // TINYMT_HPP
