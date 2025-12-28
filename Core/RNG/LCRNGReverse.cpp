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

#include "LCRNGReverse.hpp"
#include <Core/Enum/Method.hpp>
#include <Core/RNG/LCRNG.hpp>

/**
 * @brief Recovers origin seeds for two 16 bit calls(15 bits known) without gap
 *
 * @param hp HP iv
 * @param atk Atk iv
 * @param def Def iv
 * @param spa SpA iv
 * @param spd SpD iv
 * @param spe Spe iv
 *
 * @return Array of origin seeds (Won't be higher than 6)
 */
static RecoverySeeds<6> recoverPokeRNGIVMethod12(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe)
{
    constexpr u32 add = 0x6073;
    constexpr u32 mult = 0x41c64e6d;
    constexpr u32 mod = 0x67d3;
    constexpr u32 pat = 0xd3e;
    constexpr u32 inc = 0x4034;

    RecoverySeeds<6> seeds;

    u32 first = static_cast<u32>((hp | (atk << 5) | (def << 10)) << 16);
    u32 second = static_cast<u32>((spe | (spa << 5) | (spd << 10)) << 16);

    u16 diff = (second - first * mult) >> 16;
    u16 start1 = (((diff * mod + inc) >> 16) * pat) % mod;
    u16 start2 = ((((diff ^ 0x8000) * mod + inc) >> 16) * pat) % mod;

    for (u32 low = start1; low < 0x10000; low += mod)
    {
        u32 seed = first | low;
        if (((seed * mult + add) & 0x7fff0000) == second)
        {
            seeds[seeds.count++] = seed;
            seeds[seeds.count++] = seed ^ 0x80000000;
        }
    }

    for (u32 low = start2; low < 0x10000; low += mod)
    {
        u32 seed = first | low;
        if (((seed * mult + add) & 0x7fff0000) == second)
        {
            seeds[seeds.count++] = seed;
            seeds[seeds.count++] = seed ^ 0x80000000;
        }
    }

    return seeds;
}

/**
 * @brief Recovers origin seeds for two 16 bit calls(15 bits known) with gap
 *
 * @param hp HP iv
 * @param atk Atk iv
 * @param def Def iv
 * @param spa SpA iv
 * @param spd SpD iv
 * @param spe Spe iv
 * @param seeds Array to write results
 *
 * @return Array of origin seeds (Won't be higher than 6)
 */
static RecoverySeeds<6> recoverPokeRNGIVMethod4(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe)
{
    constexpr u32 add = 0xe97e7b6a;
    constexpr u32 mult = 0xc2a29a69;
    constexpr u32 mod = 0x3a89;
    constexpr u32 pat = 0x2e4c;
    constexpr u32 inc = 0x5831;

    RecoverySeeds<6> seeds;

    u32 first = static_cast<u32>((hp | (atk << 5) | (def << 10)) << 16);
    u32 second = static_cast<u32>((spe | (spa << 5) | (spd << 10)) << 16);

    u16 diff = (second - (first * mult + add)) >> 16;
    u16 start1 = (((diff * mod + inc) >> 16) * pat) % mod;
    u16 start2 = ((((diff ^ 0x8000) * mod + inc) >> 16) * pat) % mod;

    for (u32 low = start1; low < 0x10000; low += mod)
    {
        u32 seed = first | low;
        if (((seed * mult + add) & 0x7fff0000) == second)
        {
            seeds[seeds.count++] = seed;
            seeds[seeds.count++] = seed ^ 0x80000000;
        }
    }

    for (u32 low = start2; low < 0x10000; low += mod)
    {
        u32 seed = first | low;
        if (((seed * mult + add) & 0x7fff0000) == second)
        {
            seeds[seeds.count++] = seed;
            seeds[seeds.count++] = seed ^ 0x80000000;
        }
    }

    return seeds;
}

namespace LCRNGReverse
{
    RecoverySeeds<12> recoverChannelIV(u32 hp, u32 atk, u32 def, u32 spa, u32 spd, u32 spe)
    {
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

        RecoverySeeds<12> seeds;

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
                        if ((rng.advance(2) >> 27) == spa)
                        {
                            if ((rng.next() >> 27) == spd)
                            {
                                seeds[seeds.count++] = seed;
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

        return seeds;
    }

    RecoverySeeds<6> recoverPokeRNGIV(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe, Method method)
    {
        if (method == Method::Method4)
        {
            return recoverPokeRNGIVMethod4(hp, atk, def, spa, spd, spe);
        }
        return recoverPokeRNGIVMethod12(hp, atk, def, spa, spd, spe);
    }

    RecoverySeeds<3> recoverPokeRNGPID(u32 pid)
    {
        constexpr u32 add = 0x6073;
        constexpr u32 mult = 0x41c64e6d;
        constexpr u32 mod = 0x67d3;
        constexpr u32 pat = 0xd3e;
        constexpr u32 inc = 0x4034;

        RecoverySeeds<3> seeds;
        
        u32 first = pid << 16;
        u32 second = pid & 0xffff0000;

        u16 diff = (second - first * mult) >> 16;
        u16 start = (((diff * mod + inc) >> 16) * pat) % mod;

        for (u32 low = start; low < 0x10000; low += mod)
        {
            u32 seed = first | low;
            if (((seed * mult + add) & 0xffff0000) == second)
            {
                seeds[seeds.count++] = seed;
            }
        }

        return seeds;
    }

    RecoverySeeds<6> recoverXDRNGIV(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe)
    {
        constexpr u32 mult = 0x343fd; // XDRNG mult
        constexpr u32 sub = 0x259ec4; // XDRNG add - 0xffff
        constexpr u64 base = 0x343fabc02; // (XDRNG mult + 1) * 0xffff

        RecoverySeeds<6> seeds;

        u32 first = static_cast<u32>((hp | (atk << 5) | (def << 10)) << 16);
        u32 second = static_cast<u32>((spe | (spa << 5) | (spd << 10)) << 16);

        u64 t = ((second - mult * first) - sub) & 0x7FFFFFFF;
        u32 kmax = (base - t) >> 31;

        for (u32 k = 0; k <= kmax; k++, t += 0x80000000)
        {
            if ((t % mult) < 0x10000)
            {
                u32 seed = first | static_cast<u32>(t / mult);
                seeds[seeds.count++] = seed;
                seeds[seeds.count++] = seed ^ 0x80000000;
            }
        }

        return seeds;
    }

    RecoverySeeds<2> recoverXDRNGPID(u32 pid)
    {
        constexpr u32 mult = 0x343fd; // XDRNG mult
        constexpr u32 sub = 0x259ec4; // XDRNG add - 0xffff
        constexpr u64 base = 0x343fabc02; // (XDRNG mult + 1) * 0xffff

        RecoverySeeds<2> seeds;

        u32 first = pid & 0xffff0000;
        u32 second = pid << 16;

        u64 t = ((second - mult * first) - sub) & 0xFFFFFFFF;
        u32 kmax = (base - t) >> 32;

        for (u32 k = 0; k <= kmax; k++, t += 0x100000000)
        {
            if ((t % mult) < 0x10000)
            {
                u32 seed = first | static_cast<u32>(t / mult);
                seeds[seeds.count++] = seed;
            }
        }

        return seeds;
    }
}
