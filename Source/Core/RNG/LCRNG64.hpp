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

#ifndef LCRNG64_HPP
#define LCRNG64_HPP

#include <Core/Global.hpp>

/**
 * @brief Provides random numbers via the LCRNG algorithm. Most commonly used ones are defined at the bottom of the file.
 *
 * @tparam add LCRNG64 addition value
 * @tparam mult LCRNG64 multiplication value
 */
template <u64 add, u64 mult>
class LCRNG64
{
public:
    /**
     * @brief Construct a new LCRNG64 object
     *
     * @param seed Starting PRNG state
     * @param advances Initial number of advances
     */
    LCRNG64(u64 seed, u64 advances = 0) : seed(seed)
    {
        advance(advances);
    }

    /**
     * @brief Advances the RNG by \p advances amount
     *
     * @param advances Number of advances
     *
     * @return PRNG value after the advances
     */
    u64 advance(u64 advances)
    {
        for (u64 advance = 0; advance < advances; advance++)
        {
            next();
        }
        return seed;
    }

    /**
     * @brief Returns the current PRNG state
     *
     * @return PRNG value
     */
    u64 getSeed() const
    {
        return seed;
    }

    /**
     * @brief Gets the next 64bit PRNG state
     *
     * @return PRNG value
     */
    u64 next()
    {
        return seed = seed * mult + add;
    }

    /**
     * @brief Gets the next 32bit PRNG state
     *
     * @return PRNG value
     */
    u32 nextUInt()
    {
        return next() >> 32;
    }

    /**
     * @brief Gets the next 32bit PRNG state bounded by the \p max value
     *
     * @param max Max value
     *
     * @return PRNG value
     */
    u32 nextUInt(u32 max)
    {
        return ((next() >> 32) * max) >> 32;
    }

private:
    u64 seed;
};

using BWRNG = LCRNG64<0x269ec3, 0x5d588b656c078965>;
using BWRNGR = LCRNG64<0x9b1ae6e9a384e6f9, 0xdedcedae9638806d>;

#endif // LCRNG64_HPP
