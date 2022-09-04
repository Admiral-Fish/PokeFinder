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

struct JumpTable64
{
    u64 add[64];
    u64 mult[64];
};

consteval JumpTable64 computeJumpTable64(u64 add, u64 mult)
{
    JumpTable64 table;
    table.add[0] = add;
    table.mult[0] = mult;

    for (int i = 1; i < 64; i++)
    {
        table.add[i] = table.add[i - 1] * (table.mult[i - 1] + 1);
        table.mult[i] = table.mult[i - 1] * table.mult[i - 1];
    }

    return table;
}

constexpr JumpTable64 BWRNGTable = computeJumpTable64(0x269ec3, 0x5d588b656c078965);
constexpr JumpTable64 BWRNGRTable = computeJumpTable64(0x9b1ae6e9a384e6f9, 0xdedcedae9638806d);

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
    LCRNG64(u64 seed) : seed(seed)
    {
    }

    /**
     * @brief Advances the RNG by \p advances amount
     * This function uses a jump ahead table to advance any amount in just O(64)
     *
     * @param advances Number of advances
     *
     * @return PRNG value after the advances
     */
    u64 advance(u64 advances)
    {
        const JumpTable64 *table;

        if constexpr (add == 0x269ec3) // BWRNG
        {
            table = &BWRNGTable;
        }
        else // BWRNG(R)
        {
            static_assert(add == 0x9b1ae6e9a384e6f9, "Unsupported LCRNG64");
            table = &BWRNGRTable;
        }

        for (int i = 0; advances; advances >>= 1, i++)
        {
            if (advances & 1)
            {
                seed = seed * table->mult[i] + table->add[i];
            }
        }

        return seed;
    }

    /**
     * @brief Advances the RNG by \p advances amount
     * This function is called when the number of advances is predetermined and allows the compiler to optimize to the final mult/add
     *
     * @tparam advances Number of advances
     *
     * @return PRNG value after the advances
     */
    template <u64 advances>
    u64 advance()
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
