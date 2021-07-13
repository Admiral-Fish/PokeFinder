/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2021 by Admiral_Fish, bumba, and EzPzStreamz
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

#include <Core/Util/Global.hpp>

template <u32 add, u32 mult>
class LCRNG
{
public:
    LCRNG(u32 seed = 0) : seed(seed)
    {
    }

    u32 modpow32(u32 base, u32 exp)
    {
        u32 result = 1;
        while (exp > 0)
        {
            if (exp & 1)
            result = result * base;
            base = base * base;
            exp >>= 1;
        }
        return result;
    }

    void advance(u32 advances, bool lcgn = false)
    {
        if (lcgn)
        {
            u32 ex = advances - 1;
            u32 q = mult;
            u32 factor = 1;
            u32 sum = 0;
            while (ex > 0)
            {
                if (!(ex & 1))
                {
                    sum = sum + (factor * modpow32(q, ex));
                    ex--;
                }
                factor *= (1 + q);
                q *= q;
                ex /= 2;
            }
            seed = (seed * modpow32(mult, advances)) + (sum + factor) * add;
        }
        else
        {
            for (u32 advance = 0; advance < advances; advance++)
            {
                next();
            }
        }
    }

    u32 next()
    {
        return seed = seed * mult + add;
    }

    u32 next(u32 &count)
    {
        count++;
        return seed = seed * mult + add;
    }

    u16 nextUShort()
    {
        return next() >> 16;
    }

    u16 nextHalfUShort()
    {
        return (next() >> 16) & 0x7FFF;
    }

    u16 nextUShort(u32 &count)
    {
        count++;
        return next() >> 16;
    }

    void setSeed(u32 seed)
    {
        this->seed = seed;
    }

    u32 getSeed() const
    {
        return seed;
    }

private:
    u32 seed;
};

using ARNG = LCRNG<0x01, 0x6C078965>;
using ARNGR = LCRNG<0x69C77F93, 0x9638806D>;
using PokeRNG = LCRNG<0x6073, 0x41C64E6D>;
using PokeRNGR = LCRNG<0xA3561A1, 0xEEB9EB65>;
using XDRNG = LCRNG<0x269EC3, 0x343FD>;
using XDRNGR = LCRNG<0xA170F641, 0xB9B33155>;
using RBRNG = LCRNG<0x3039, 0x41C64E6D>;
using RBRNGR = LCRNG<0xFC77A683, 0xEEB9EB65>;
#endif // LCRNG_HPP
