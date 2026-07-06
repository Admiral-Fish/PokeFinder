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

#include "Jump.hpp"
#include <bit>

static const vuint128 MASK(0xffffffffffffffff, 0x7fffffffffffffff);

/**
 * @brief Generates mask that determines if we need to reduce the polynomial by the characteristic
 * This takes the carry bit at 127 and fills all bits with that value to use as a mask
 * 
 * @param x Input polynomial
 * 
 * @return Computed reduction mask
 */
static vuint128 generateReductionMask(const vuint128 &x)
{
    vuint128 top;
#if defined(SIMD_X86)
    top.uint128 = _mm_shuffle_epi32(x.uint128, 0xff);
    top.uint128 = _mm_srai_epi32(top.uint128, 31);
#elif defined(SIMD_ARM)
    top.uint128 = vdupq_laneq_u32(x.uint128, 3);
    top.uint128 = vreinterpretq_u32_s32(vshrq_n_s32(vreinterpretq_s32_u32(top.uint128), 31));
#else
    u64 bit = x.uint64[1] >> 63;
    u64 mask = (static_cast<s64>(bit << 63) >> 63);
    top.uint64 = {mask, mask};
#endif
    return top;
}

/**
 * @brief Shifts 254bit polynomial right by 127 bits
 *
 * @param high Upper 128bit of input
 * @param low Lower 128bit of input
 *
 * @return 128bit polynomial containing remaining 127bits
 */
static vuint128 gf2Shr127(const vuint128 &high, const vuint128 &low)
{
    // Shift high left by 1
    vuint128 high_shifted = v64x2_shl<1>(high);
    vuint128 high_carry = v128_shl<8>(v64x2_shr<63>(high));
    vuint128 high_part = high_shifted | high_carry;

    // Extract bit 127 from low
    vuint128 low_bit = v64x2_shr<63>(v128_shr<8>(low));

    return high_part | low_bit;
}

/**
 * @brief Multiples two 128bit polynomials into a 256bit output
 *
 * @param a First polynomial
 * @param b Second polynomial
 * @param prod_high Upper 128bit of output
 * @param prod_low Lower 128bit of output
 */
static void gf2Clmul128(const vuint128 &a, const vuint128 &b, vuint128 &prod_high, vuint128 &prod_low)
{
    // Compute the baseline low and high terms (2 multiplications)
    vuint128 p00 = v128_clmul64<0x00>(a, b); // low(a) * low(b)
    vuint128 p11 = v128_clmul64<0x11>(a, b); // high(a) * high(b)

    // Extract the high halves down into the low position for Karatsuba
    vuint128 a_high = v128_shr<8>(a);
    vuint128 b_high = v128_shr<8>(b);

    // XOR the low and high halves together
    vuint128 a_xor = a ^ a_high; // low(a) ^ high(a)
    vuint128 b_xor = b ^ b_high; // low(b) ^ high(b)

    // Compute the third multiplication
    vuint128 p_mid = v128_clmul64<0x00>(a_xor, b_xor);

    // Combine into the true middle term via Karatsuba identity
    vuint128 mid = p_mid ^ p00 ^ p11;

    // Align middle terms using 8-byte shifts
    vuint128 mid_low = v128_shl<8>(mid);
    vuint128 mid_high = v128_shr<8>(mid);

    // Accumulate into the final 256-bit product split registers
    prod_low = p00 ^ mid_low;
    prod_high = p11 ^ mid_high;
}

/**
 * @brief Multiples polynomial by 2 followed with reduction by the \p characteristic
 *
 * @tparam period RNG period size in bits
 * @param a Polynomial
 * @param characteristic Polynomial that describes linear transformation
 */
template <int period>
static void gf2MulMod(vuint128 &a, const vuint128 &characteristic)
{
    vuint128 carry = v64x2_shr<63>(a);
    carry = v128_shl<8>(carry);

    vuint128 shifted = v64x2_shl<1>(a);
    vuint128 a_next = carry | shifted;

    vuint128 mask;
    if constexpr (period == 128)
    {
        mask = generateReductionMask(a);
    }
    else
    {
        mask = generateReductionMask(a_next);
    }

    a = a_next ^ (characteristic & mask);
    if constexpr (period == 127)
    {
        a = a & MASK;
    }
}

/**
 * @brief Squares polynomial followed with reduction by the \p characteristic
 *
 * @tparam period RNG period size in bits
 * @param a Input polynomial
 * @param characteristic Polynomial that describes linear transformation
 * @param inverse Precomputed Barrett Reduction polynomial to approximate division
 */
template <int period>
static void gf2SqrMod(vuint128 &a, const vuint128 &characteristic, const vuint128 &mu)
{
    vuint128 prod_high, prod_low;
    gf2Clmul128(a, a, prod_high, prod_low);

    if constexpr (period == 128)
    {
        vuint128 q_high, q_low;
        gf2Clmul128(prod_high, mu, q_high, q_low);

        // Add prod_high back in to account for not having the implicit 128th bit
        q_high = q_high ^ prod_high;

        vuint128 r_high, r_low;
        gf2Clmul128(q_high, characteristic, r_high, r_low);

        a = prod_low ^ r_low;
    }
    else
    {
        // prod >> 127
        vuint128 val = gf2Shr127(prod_high, prod_low);

        vuint128 q_high, q_low;
        gf2Clmul128(val, mu, q_high, q_low);

        // q >> 127
        val = gf2Shr127(q_high, q_low);

        vuint128 r_high, r_low;
        gf2Clmul128(val, characteristic, r_high, r_low);

        a = prod_low ^ r_low;
        a = a & MASK;
    }
}

namespace Jump
{
    template <int period>
    vuint128 computeJumpPolynomial(const vuint128 &characteristic, const vuint128 &inverse, u32 advances)
    {
        static_assert(period == 127 || period == 128, "This function only supports 127/128bit polynomial period sizes");

        // We can ignore all leading zeros
        // We can also skip the first 1bit as it will only compute jump to 2
        int start = 31 - std::countl_zero(advances);
        vuint128 jump(2, 0);

        for (int k = start - 1; k >= 0; k--)
        {
            gf2SqrMod<period>(jump, characteristic, inverse);
            if ((advances >> k) & 1)
            {
                gf2MulMod<period>(jump, characteristic);
            }
        }

        return jump;
    }

    template vuint128 computeJumpPolynomial<127>(const vuint128 &, const vuint128 &, u32);
    template vuint128 computeJumpPolynomial<128>(const vuint128 &, const vuint128 &, u32);
}
