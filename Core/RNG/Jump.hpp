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

#ifndef JUMP_HPP
#define JUMP_HPP

#include <Core/Global.hpp>
#include <Core/RNG/SIMD.hpp>
#include <gf2/BitPolynomial.h>

/**
 * @brief Computes jump polynomials for various RNG classes
 */
namespace Jump
{
    /**
     * @brief Computes jump polynomial using GF(2)
     * Highly specialized for 127/128bit period RNGs
     * This should only be called with \p advances greater than 0
     *
     * @tparam period RNG period size in bits
     * @param characteristic Polynomial that describes linear transformation
     * @param inverse Precomputed Barrett Reduction polynomial to approximate division
     * @param advances Number of advances to jump
     *
     * @return Computed jump polynomial
     */
    template <int period>
    vuint128 computeJumpPolynomial(const vuint128 &characteristic, const vuint128 &inverse, u32 advances);

    /**
     * @brief Computes jump polynomial using GF(2)
     * This should only be called with \p advances greater than 0
     *
     * @param characteristic Polynomial that describes linear transformation
     * @param count Size of polynomial in bytes
     * @param advances Number of advances to jump
     *
     * @return Computed jump polynomial
     */
    gf2::BitPolynomial<u64> computeJumpPolynomial(const u8 *characteristic, int count, u32 advances);
}

#endif // JUMP_HPP
