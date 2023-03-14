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

#ifndef RESEARCHERSTATE_HPP
#define RESEARCHERSTATE_HPP

#include <Core/Global.hpp>

/**
 * @brief Contains the information to display custom calculations for PRNG states
 */
class ResearcherState
{
public:
    /**
     * @brief Construct a new ResearcherState object
     *
     * @param advances State advances
     * @param prng State PRNG value
     * @param rng64Bit Whether value was generated from a 64bit RNG
     */
    ResearcherState(u32 advances, u64 prng, bool rng64Bit) : prng(prng), advances(advances), rng64Bit(rng64Bit)
    {
    }

    /**
     * @brief Returns the advances of the state
     *
     * @return State advances
     */
    u32 getAdvances() const
    {
        return advances;
    }

    /**
     * @brief Returns the custom value at \p index
     *
     * @param index Custom index
     *
     * @return Custom value
     */
    u64 getCustom(u8 index) const
    {
        return custom[index];
    }

    /**
     * @brief Computes the high 32bits of the PRNG value
     *
     * @return Computed value
     */
    u32 getHigh32() const
    {
        return prng >> 32;
    }

    /**
     * @brief Computes the high 16bits of the PRNG value
     *
     * @return Computed value
     */
    u32 getHigh16() const
    {
        return rng64Bit ? getHigh32() >> 16 : prng >> 16;
    }

    /**
     * @brief Computes the low 32bits of the PRNG value
     *
     * @return Computed value
     */
    u32 getLow32() const
    {
        return prng & 0xffffffff;
    }

    /**
     * @brief Computes the low 16bits of the PRNG value
     *
     * @return Computed value
     */
    u32 getLow16() const
    {
        return rng64Bit ? getHigh32() & 0xFFFF : prng & 0xFFFF;
    }

    /**
     * @brief Returns the PRNG value of the state
     *
     * @return PRNG value
     */
    u64 getPRNG() const
    {
        return prng;
    }

    /**
     * @brief Sets the custom value at \p index
     *
     * @param index Custom index
     * @param custom Custom value
     */
    void setCustom(u8 index, u64 custom)
    {
        this->custom[index] = custom;
    }

private:
    u64 custom[10];
    u64 prng;
    u32 advances;
    bool rng64Bit;
};

#endif // RESEARCHERSTATE_HPP
