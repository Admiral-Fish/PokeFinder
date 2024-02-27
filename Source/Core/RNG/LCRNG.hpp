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

#ifndef LCRNG_HPP
#define LCRNG_HPP

#include <Core/Global.hpp>

struct Jump
{
    u32 mult;
    u32 add;
};

struct JumpTable
{
    Jump jump[32];
};

extern const JumpTable ARNGTable;
extern const JumpTable ARNGRTable;
extern const JumpTable PokeRNGTable;
extern const JumpTable PokeRNGRTable;
extern const JumpTable XDRNGTable;
extern const JumpTable XDRNGRTable;

/**
 * @brief Provides random numbers via the LCRNG algorithm. Most commonly used ones are defined at the bottom of the file.
 *
 * @tparam add LCRNG addition value
 * @tparam mult LCRNG multiplication value
 */
template <u32 add, u32 mult>
class LCRNG
{
public:
    /**
     * @brief Construct a new LCRNG object
     *
     * @param seed Starting PRNG value
     */
    LCRNG(u32 seed) : seed(seed)
    {
    }

    /**
     * @brief Construct a new LCRNG object
     *
     * @tparam add1 LCRNG addition value
     * @tparam mult1 LCRNG multiplication value
     * @param rng LCRNG object to copy
     */
    template <u32 add1, u32 mult1>
    LCRNG(const LCRNG<add1, mult1> &rng) : seed(rng.getSeed())
    {
    }

    /**
     * @brief Construct a new LCRNG object
     *
     * @param seed Starting PRNG value
     * @param advances Number of initial advances
     */
    LCRNG(u32 seed, u32 advances) : seed(seed)
    {
        jump(advances);
    }

    /**
     * @brief Construct a new LCRNG object
     *
     * @param rng LCRNG object to copy
     * @param j Multipler and adder to advance by
     */
    LCRNG(const LCRNG &rng, const Jump &j) : seed(rng.seed)
    {
        jump(j);
    }

    /**
     * @brief Advances the RNG by \p advances amount
     *
     * @param advances Number of advances
     * @param count Pointer to keep track of advance count
     *
     * @return PRNG value after the advances
     */
    u32 advance(u32 advances, u32 *count = nullptr)
    {
        for (u64 advance = 0; advance < advances; advance++)
        {
            next(count);
        }
        return seed;
    }

    /**
     * @brief Computes the number of advances between \p start and \p end PRNG states
     *
     * @param start Starting PRNG state
     * @param end Ending PRNG state
     *
     * @return Number of advances between \p start and \p end
     */
    static u32 distance(u32 start, u32 end)
    {
        const JumpTable *table = getJumpTable();

        u32 count = 0;
        u32 p = 1;

        for (int i = 0; i < 32 && start != end; i++, p <<= 1)
        {
            if ((start ^ end) & p)
            {
                const Jump *jump = &table->jump[i];
                start = jump->mult * start + jump->add;
                count += p;
            }
        }

        return count;
    }

    /**
     * @brief Returns the adder of the LCRNG
     *
     * @return LCRNG adder value
     */
    constexpr static u32 getAdd()
    {
        return add;
    }

    /**
     * @brief Returns the multipler of the LCRNG
     *
     * @return LCRNG multipler value
     */
    constexpr static u32 getMult()
    {
        return mult;
    }

    /**
     * @brief Computes the multipler and adder to jump the RNG by \p advances amount
     *
     * @param advances Number of advances
     *
     * @return Multipler and adder to jump the RNG
     */
    const Jump getJump(u32 advances)
    {
        const JumpTable *table = getJumpTable();
        Jump jump;

        jump.add = 0;
        jump.mult = 1;
        for (int i = 0; advances > 0; advances >>= 1, i++)
        {
            if (advances & 1)
            {
                jump.add = jump.add * table->jump[i].mult + table->jump[i].add;
                jump.mult *= table->jump[i].mult;
            }
        }

        return jump;
    }

    /**
     * @brief Returns the current PRNG state
     *
     * @return PRNG value
     */
    u32 getSeed() const
    {
        return seed;
    }

    /**
     * @brief Jumps the RNG by \p advances amount
     * This function uses a jump ahead table to advance any amount in just O(32)
     *
     * @param advances Number of advances
     *
     * @return PRNG value after the advances
     */
    u32 jump(u32 advances)
    {
        const JumpTable *table = getJumpTable();

        for (int i = 0; advances; advances >>= 1, i++)
        {
            if (advances & 1)
            {
                seed = seed * table->jump[i].mult + table->jump[i].add;
            }
        }

        return seed;
    }

    /**
     * @brief Jumps the RNG by multipler and adder in \p jump
     *
     * @param jump Multipler and adder to jump by
     *
     * @return PRNG value after the advances
     */
    u32 jump(const Jump &jump)
    {
        return seed = seed * jump.mult + jump.add;
    }

    /**
     * @brief Gets the next 32bit PRNG state
     *
     * @param count Pointer to keep track of advance count
     *
     * @return PRNG value
     */
    u32 next(u32 *count = nullptr)
    {
        if (count)
        {
            (*count)++;
        }
        return seed = seed * mult + add;
    }

    /**
     * @brief Gets the next 16bit PRNG state
     *
     * @param count Pointer to keep track of advance count
     *
     * @return PRNG value
     */
    u16 nextUShort(u32 *count = nullptr)
    {
        return next(count) >> 16;
    }

    /**
     * @brief Gets the next 16bit PRNG state
     *
     * @tparam mod Whether the max calculation is done without mod
     * @param max Max bounding value
     * @param count Pointer to keep track of advance count
     *
     * @return PRNG value
     */
    template <bool mod = true>
    u16 nextUShort(u16 max, u32 *count = nullptr)
    {
        u16 rand = next(count) >> 16;
        if constexpr (mod)
        {
            return rand % max;
        }
        else
        {
            return rand / ((0xffff / max) + 1);
        }
    }

    /**
     * @brief Sets the PRNG state
     *
     * @param seed PRNG state
     */
    void setSeed(u32 seed)
    {
        this->seed = seed;
    }

private:
    u32 seed;

    static const JumpTable *getJumpTable()
    {
        if constexpr (add == 0x01) // ARNG
        {
            return &ARNGTable;
        }
        else if constexpr (add == 0x69C77F93) // ARNG(R)
        {
            return &ARNGRTable;
        }
        else if constexpr (add == 0x6073) // PokeRNG
        {
            return &PokeRNGTable;
        }
        else if constexpr (add == 0xA3561A1) // PokeRNG(R)
        {
            return &PokeRNGRTable;
        }
        else if constexpr (add == 0x269EC3) // XDRNG
        {
            return &XDRNGTable;
        }
        else // XDRNG(R)
        {
            static_assert(add == 0xA170F641, "Unsupported LCRNG");
            return &XDRNGRTable;
        }
    }
};

using ARNG = LCRNG<0x01, 0x6C078965>;
using ARNGR = LCRNG<0x69C77F93, 0x9638806D>;
using PokeRNG = LCRNG<0x6073, 0x41C64E6D>;
using PokeRNGR = LCRNG<0xA3561A1, 0xEEB9EB65>;
using XDRNG = LCRNG<0x269EC3, 0x343FD>;
using XDRNGR = LCRNG<0xA170F641, 0xB9B33155>;

#endif // LCRNG_HPP
