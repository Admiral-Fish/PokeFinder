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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301,
 * USA.
 */

#ifndef RNGLIST_HPP
#define RNGLIST_HPP

#include <Core/Global.hpp>
#include <cassert>
#include <type_traits>

/**
 * @brief Provides a storage container to reuse RNG calculations and cycle out old states with new states
 *
 * @tparam Integer Integer type that is being stored
 * @tparam RNG RNG class used to generate states
 * @tparam size Size of the storage container (must be a perfect multiple of two)
 * @tparam generate Function pointer to modify generated PRNG states
 */
template <typename Integer, class RNG, u16 size, Integer (*generate)(RNG &) = nullptr>
class RNGList
{
    static_assert(size && ((size & (size - 1)) == 0), "Number is not a perfect multiple of two");

public:
    /**
     * @brief Construct a new RNGList object
     *
     * @param seed Starting PRNG state
     * @param advances Initial advances
     */
    RNGList(u32 seed, u32 advances) : rng(seed, advances), head(0), pointer(0)
    {
        init();
    }

    /**
     * @brief Construct a new RNGList object
     *
     * @param seed0 Starting PRNG state0
     * @param seed1 Starting PRNG state1
     * @param advances Initial advances
     */
    RNGList(u64 seed0, u64 seed1, u32 advances) : rng(seed0, seed1, advances), head(0), pointer(0)
    {
        init();
    }

    /**
     * @brief Deleted copy constructor
     */
    RNGList(const RNGList &) = delete;

    /**
     * @brief Deleted move constructor
     */
    RNGList(RNGList &&) = delete;

    /**
     * @brief Deleted copy assignment
     */
    void operator=(const RNGList &) = delete;

    /**
     * @brief Deleted move assignment
     */
    RNGList &operator=(RNGList &&) = delete;

    /**
     * @brief Advances the RNG by \p advances amount
     *
     * @param advances Number of advances
     */
    void advanceStates(u32 advances)
    {
        for (u32 i = 0; i < advances; i++)
        {
            advanceState();
        }
    }

    /**
     * @brief Advances the RNG by 1
     */
    void advanceState()
    {
        if constexpr (generate)
        {
            list[head++] = generate(rng);
        }
        else
        {
            list[head++] = rng.next();
        }

        if constexpr (size != 256)
        {
            head %= size;
        }

        pointer = head;
    }

    /**
     * @brief Advances the internal state by \p advances amount
     *
     * @param advances Number of advances
     */
    void advance(u32 advances)
    {
        pointer += advances;
        if constexpr (size != 256)
        {
            pointer %= size;
        }
    }

    /**
     * @brief Gets the next PRNG state
     *
     * @return PRNG state
     */
    Integer next()
    {
        Integer result = list[pointer++];

        if constexpr (size != 256)
        {
            pointer %= size;
        }

        // Debug assert to help discover if the array is too small
        // Only check on bigger sizes. Smaller sizes are prone to false positives if we use size number of prng calls
        if constexpr (size > 8)
        {
            assert(pointer != head);
        }

        return result;
    }

    /**
     * @brief Gets the next PRNG state
     *
     * @tparam Ret Return value type
     * @param value Function pointer to modify returned PRNG state
     *
     * @return PRNG state
     */
    template <typename Ret>
    Ret next(Ret (*value)(Integer))
    {
        return value(next());
    }

    /**
     * @brief Resets the current internal state
     */
    void resetState()
    {
        pointer = head;
    }

private:
    using SizeType = std::conditional_t<size <= 256, u8, u16>;

    RNG rng;
    Integer list[size];
    SizeType head, pointer;

    /**
     * @brief Populates the list with PRNG states
     */
    void init()
    {
        for (Integer &x : list)
        {
            if constexpr (generate != nullptr)
            {
                x = generate(rng);
            }
            else
            {
                x = rng.next();
            }
        }
    }
};

#endif // RNGLIST_HPP
