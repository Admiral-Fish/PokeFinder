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

#ifndef MT_HPP
#define MT_HPP

#include <Core/Global.hpp>
#include <Core/RNG/SIMD.hpp>

/**
 * @brief Provides random numbers via the Mersenne Twister algorithm.
 */
class MT
{
public:
    /**
     * @brief Construct a new MT object
     *
     * @param seed Starting PRNG state
     */
    MT(u32 seed);

    /**
     * @brief Construct a new MT object
     *
     * @param seed Starting PRNG state
     * @param advances Number of initial advances
     */
    MT(u32 seed, u32 advances);

    /**
     * @brief Advances the RNG by \p advances amount
     *
     * @param advances Number of advances
     */
    void advance(u32 advances);

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
    vuint128 state[156];
    u16 index;

    /**
     * @brief Generates the next MT state after all 624 states have been consumed
     */
    void shuffle();
};

#endif // MT_HPP
