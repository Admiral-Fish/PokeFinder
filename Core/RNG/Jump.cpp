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

template <int period>
static void f2x_mulx(vuint128 &a, const vuint128 &characteristic)
{
    u64 carry = 0;
    for (int i = 0; i < 2; i++)
    {
        const u64 next_carry = a.uint64[i] >> 63;
        a.uint64[i] = (a.uint64[i] << 1) | carry;
        carry = next_carry;
    }

    // Coefficient of x^POLY_DEG after the shift.
    int top;
    if constexpr ((period % 64) == 0)
    {
        top = (int)carry;
    }
    else
    {
        top = a.uint64[1] >> 63;
        a.uint64[1] &= 0x7fffffffffffffff;
    }

    if (top)
    {
        a = a ^ characteristic;
    }
}

template <int period>
static void f2x_mulmod(vuint128 &a, const vuint128 &b, const vuint128 &characteristic)
{
    vuint128 r(0, 0);

    for (int k = period - 1; k >= 0; k--)
    {
        f2x_mulx<period>(r, characteristic);
        if ((a.uint64[k >> 6] >> (k & 63)) & 1)
        {
            r = r ^ b;
        }
    }

    a = r;
}

namespace Jump
{
    template <int period>
    vuint128 computeJumpPolynomial(const vuint128 &characteristic, u32 advances)
    {
        vuint128 jump(1, 0);

        for (int k = 31; k >= 0; k--)
        {
            f2x_mulmod<period>(jump, jump, characteristic);
            if ((advances >> k) & 1)
            {
                f2x_mulx<period>(jump, characteristic);
            }
        }

        return jump;
    }

    template vuint128 computeJumpPolynomial<127>(const vuint128 &, u32);
    template vuint128 computeJumpPolynomial<128>(const vuint128 &, u32);
}
