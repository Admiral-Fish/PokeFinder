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

#include "RNGEuclidean.hpp"
#include <Core/RNG/LCRNG.hpp>

namespace RNGEuclidean
{
    int recoverChannelIV(u32 hp, u32 atk, u32 def, u32 spa, u32 spd, u32 spe, u32 *seeds)
    {
        int size = 0;

        // Mult(j) = Mult^j
        // Add(j) = Add * (Mult^0 + Mult^1 + ... + Mult^(j-1))
        // Using j = 3 and XDRNG gives Mult = 0x45c82be5 and Add = 0xd2f65b55
        constexpr u32 mult = 0x45c82be5; // Modified mult
        constexpr u32 sub = 0xcaf65b56; // Modified add - 0x7ffffff
        constexpr u64 base = 0x22e415eea37d41a; // (Modified mult + 1) * 0x7ffffff

        constexpr u32 prime = 3;
        constexpr u64 add = 0x300000000; // prime * 0x100000000
        constexpr u32 rmax = 0x18000000; // prime * 0x8000000
        constexpr u64 skip = 0x661D29; // prime * 2^32 % mult

        u32 first = hp << 27;

        u64 t = (((spe << 27) - mult * first) - sub) & 0xFFFFFFFF;
        u64 x = (t * prime) % mult;
        u32 kmax = (base - t) >> 32;

        for (u32 k = 0; k <= kmax;)
        {
            u64 r = (x + skip * k) % mult;

            u8 m = r % prime;
            if (m != 0)
            {
                m = m == 1 ? 2 : 1;
                r += m * skip;
                k += m;
            }

            u64 tmp = t + 0x100000000 * k;
            while (r < rmax && k <= kmax)
            {
                u32 seed = first | (tmp / mult);
                XDRNG rng(seed);
                if ((rng.next() >> 27) == atk)
                {
                    if ((rng.next() >> 27) == def)
                    {
                        if ((rng.advance<2>() >> 27) == spa)
                        {
                            if ((rng.next() >> 27) == spd)
                            {
                                seeds[size++] = seed;
                            }
                        }
                    }
                }

                r += prime * skip;
                k += prime;
                tmp += add;
            }

            // Rounding up without using floats
            k += ((mult - r) + skip - 1) / skip;
        }

        return size;
    }

    int recoverXDRNGIV(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe, u32 *seeds)
    {
        int size = 0;

        constexpr u32 mult = 0x343fd; // XDRNG mult
        constexpr u32 sub = 0x259ec4; // XDRNG add - 0xffff
        constexpr u64 base = 0x343fabc02; // (XDRNG mult + 1) * 0xffff

        u32 first = static_cast<u32>((hp | (atk << 5) | (def << 10)) << 16);
        u32 second = static_cast<u32>((spe | (spa << 5) | (spd << 10)) << 16);

        u64 t = ((second - mult * first) - sub) & 0x7FFFFFFF;
        u32 kmax = static_cast<u32>((base - t) >> 31);

        for (u32 k = 0; k <= kmax; k++, t += 0x80000000)
        {
            if ((t % mult) < 0x10000)
            {
                u32 seed = first | static_cast<u32>(t / mult);
                seeds[size++] = seed;
                seeds[size++] = seed ^ 0x80000000;
            }
        }

        return size;
    }

    int recoverXDRNGPID(u32 pid, u32 *seeds)
    {
        int size = 0;

        constexpr u32 mult = 0x343fd; // XDRNG mult
        constexpr u32 sub = 0x259ec4; // XDRNG add - 0xffff
        constexpr u64 base = 0x343fabc02; // (XDRNG mult + 1) * 0xffff

        u32 first = pid & 0xffff0000;
        u32 second = pid << 16;

        u64 t = ((second - mult * first) - sub) & 0xFFFFFFFF;
        u32 kmax = (base - t) >> 32;

        for (u32 k = 0; k <= kmax; k++, t += 0x100000000)
        {
            if ((t % mult) < 0x10000)
            {
                u32 seed = first | static_cast<u32>(t / mult);
                seeds[size++] = seed;
            }
        }

        return size;
    }
}
