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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301,
 * USA.
 */

#ifndef RNGLIST_HPP
#define RNGLIST_HPP

#include <Core/Global.hpp>
#include <cassert>

/**
 * @brief Provides a storage container to reuse RNG calculations and cycle out old states with new states
 *
 * @tparam Integer Integer type that is being stored
 * @tparam RNG RNG class used to generate states
 * @tparam size Size of the storage container (must be a perfect multiple of two)
 */
template <typename Integer, class RNG, u16 size>
class RNGList
{
public:
    /**
     * @brief Construct a new RNGList object
     *
     * @param rng RNG object to generate states
     * @param generate Function pointer to modify generated PRNG states
     */
    RNGList(RNG &rng, Integer (*generate)(RNG &) = nullptr) : generate(generate), rng(rng), head(0), pointer(0)
    {
        static_assert(size && ((size & (size - 1)) == 0), "Number is not a perfect multiple of two");

        for (Integer &x : list)
        {
            x = generate ? generate(this->rng) : this->rng.next();
        }
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
        list[head++] = generate ? generate(rng) : this->rng.next();
        head &= size - 1;

        pointer = head;
    }

    /**
     * @brief Advances the internal state by \p advances amount
     *
     * @param advances Number of advances
     */
    void advance(u32 advances)
    {
        pointer = (pointer + advances) & (size - 1);
    }

    /**
     * @brief Gets the next PRNG state
     *
     * @param value Function pointer to modify returned PRNG state
     *
     * @return PRNG state
     */
    Integer next(Integer (*value)(Integer) = nullptr)
    {
        Integer result = list[pointer++];
        pointer &= size - 1;

        // Debug assert to help discover if the array is too small
        assert(pointer != head);

        return value ? value(result) : result;
    }

    /**
     * @brief Resets the current internal state
     */
    void resetState()
    {
        pointer = head;
    }

private:
    Integer (*generate)(RNG &);
    RNG rng;
    Integer list[size];
    u16 head, pointer;
};

#endif // RNGLIST_HPP
