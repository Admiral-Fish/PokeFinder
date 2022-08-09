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

/**
 * @brief Provides random numbers via the Tiny Mersenne Twister algorithm.
 */
class TinyMT
{
public:
    /**
     * @brief Creates a new TinyMT
     * @param seed Starting PRNG value
     */
    TinyMT(u32 seed);

    /**
     * @brief Creates a new TinyMT
     * @param state Starting PRNG state
     */
    explicit TinyMT(const u32 *state);

    /**
     * @brief Advances the RNG by provided amount
     * @param advances Number of advances
     */
    void advance(u32 advances);

    /**
     * @brief Gets the next 32bit PRNG state
     * @return PRNG value
     */
    u32 next();

    /**
     * @brief Gets the next 16bit PRNG state
     * @return PRNG value
     */
    u16 nextUShort();

private:
    u32 state[4];

    /**
     * @brief Advances the RNG to the next PRNG state
     */
    void nextState();

    /**
     * @brief Generates the PRNG value from the current PRNG state
     * @return PRNG value
     */
    u32 temper();
};

#endif // TINYMT_HPP
