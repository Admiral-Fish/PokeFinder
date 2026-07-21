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
    constexpr u32 LAG0 = 0x6134;
    constexpr u32 LAG1 = 0xC907;
    constexpr u32 LOWER = 0x64833CB0;
    constexpr u32 UPPER = 0x6483CBBC;

    u32 first = static_cast<u32>((hp | (atk << 5) | (def << 10)) << 16);
    u32 second = static_cast<u32>((spe | (spa << 5) | (spd << 10)) << 16);

    u64 tmp = ((PokeRNG::getMult() * first - second) >> 16) * LAG1;
    u32 lo = ((tmp + LOWER) >> 15) * LAG0;
    u32 mi = lo + LAG0;
    u32 up = ((tmp + UPPER) >> 15) * LAG0;

    RecoverySeeds<6> seeds;
    for (u32 lbits = lo % LAG1; lbits < 0x10000; lbits += LAG1)
    {
        u32 seed = (first | lbits);

        PokeRNG rng(seed);
        if ((rng.next() & 0x7fff0000) == second)
        {
            seeds[seeds.count++] = seed;
            seeds[seeds.count++] = seed ^ 0x80000000;
        }
    }

    for (u32 lbits = mi % LAG1; lbits < 0x10000; lbits += LAG1)
    {
        u32 seed = (first | lbits);

        PokeRNG rng(seed);
        if ((rng.next() & 0x7fff0000) == second)
        {
            seeds[seeds.count++] = seed;
            seeds[seeds.count++] = seed ^ 0x80000000;
        }
    }

    if (mi != up)
    {
        for (u32 lbits = up % LAG1; lbits < 0x10000; lbits += LAG1)
        {
            u32 seed = (first | lbits);

            PokeRNG rng(seed);
            if ((rng.next() & 0x7fff0000) == second)
            {
                seeds[seeds.count++] = seed;
                seeds[seeds.count++] = seed ^ 0x80000000;
            }
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
    constexpr u32 LAG0 = 0x6C31;
    constexpr u32 LAG1 = 0x2E90;
    constexpr u32 LOWER = 0x4B8CE21D;
    constexpr u32 UPPER = 0x4B8D08D7;
    constexpr u32 MULT = PokeRNGR::getMult() * PokeRNGR::getMult();

    u32 first = static_cast<u32>((hp | (atk << 5) | (def << 10)) << 16);
    u32 second = static_cast<u32>((spe | (spa << 5) | (spd << 10)) << 16);

    u32 tmp = ((first - second * MULT) >> 16) * LAG0;
    u32 lo = (tmp + LOWER) >> 15;
    u32 up = (tmp + UPPER) >> 15;

    RecoverySeeds<6> seeds;
    for (u32 lbits = (lo * LAG1) % LAG0; lbits < 0x10000; lbits += LAG0)
    {
        u32 seed = second | lbits;

        PokeRNGR rng(seed, 2);
        if ((rng.getSeed() & 0x7fff0000) == first)
        {
            seeds[seeds.count++] = rng.getSeed();
            seeds[seeds.count++] = rng.getSeed() ^ 0x80000000;
        }
    }

    if (lo != up)
    {
        for (u32 lbits = (up * LAG1) % LAG0; lbits < 0x10000; lbits += LAG0)
        {
            u32 seed = second | lbits;

            PokeRNGR rng(seed, 2);
            if ((rng.getSeed() & 0x7fff0000) == first)
            {
                seeds[seeds.count++] = rng.getSeed();
                seeds[seeds.count++] = rng.getSeed() ^ 0x80000000;
            }
        }
    }

    return seeds;
}

namespace LCRNGReverse
{
    RecoverySeeds<12> recoverChannelIV(u32 hp, u32 atk, u32 def, u32 spa, u32 spd, u32 spe)
    {
        // First row of the BKZ-reduced matrix
        constexpr s64 R[] = { -2528644, -24142902, 52961366, 7565619, 24945956, -99942057 };
        constexpr s64 LOWER[] = { 0x2AB966D1C2, 0x2169A3AA47, -0x5049D5FDC, -0x2AACDA387, 0xFE7FFFFFF, -0x898000001 };
        constexpr s64 UPPER[] = { 0x2E8966D1C3, 0x23D9A3AA48, -0x3549D5FDB, -0xDACDA386, 0x1098000000, -0x7E8000000 };

        // clang-format off
        const s64 f[] = { 
            (-10 * (s64)hp + 23 * (s64)atk - (s64)def - 15 * (s64)spe + 52 * (s64)spa - 53 * (s64)spd) << 27,
            (-14 * (s64)hp + 7 * (s64)atk - 18 * (s64)def - 21 * (s64)spe - 26 * (s64)spa - 24 * (s64)spd) << 27,
            (24 * (s64)hp - 5 * (s64)atk + 22 * (s64)def + 15 * (s64)spe - 5 * (s64)spa - 15 * (s64)spd) << 27,
            (-5 * (s64)hp - 24 * (s64)atk + 26 * (s64)def - 12 * (s64)spe + 9 * (s64)spa + 14 * (s64)spd) << 27,
            (27 * (s64)atk - 18 * (s64)spe - 8 * (s64)spa - (s64)spd) << 27,
            (-27 * (s64)hp + 18 * (s64)def + 8 * (s64)spe + (s64)spa) << 27
        };

        const s64 min[] = {
            ((f[0] + UPPER[0]) >> 32) * R[0],
            ((f[1] + UPPER[1]) >> 32) * R[1],
            ((f[2] + LOWER[2]) >> 32) * R[2],
            ((f[3] + LOWER[3]) >> 32) * R[3],
            ((f[4] + LOWER[4]) >> 32) * R[4],
            ((f[5] + UPPER[5]) >> 32) * R[5]
        };

        const s64 max[] = {
            ((f[0] + LOWER[0]) >> 32) * R[0],
            ((f[1] + LOWER[1]) >> 32) * R[1],
            ((f[2] + UPPER[2]) >> 32) * R[2],
            ((f[3] + UPPER[3]) >> 32) * R[3],
            ((f[4] + UPPER[4]) >> 32) * R[4],
            ((f[5] + LOWER[5]) >> 32) * R[5]
        };
        // clang-format on

        RecoverySeeds<12> seeds;
        for (s64 x5 = min[5]; x5 <= max[5]; x5 += -R[5])
        {
            for (s64 x4 = min[4]; x4 <= max[4]; x4 += R[4])
            {
                s64 l4 = x5 + x4;
                for (s64 x2 = min[2]; x2 <= max[2]; x2 += R[2])
                {
                    s64 l2 = l4 + x2;
                    for (s64 x3 = min[3]; x3 <= max[3]; x3 += R[3])
                    {
                        s64 l3 = l2 + x3;
                        for (s64 x1 = min[1]; x1 <= max[1]; x1 += -R[1])
                        {
                            s64 l1 = l3 + x1;
                            for (s64 x0 = min[0]; x0 <= max[0]; x0 += -R[0])
                            {
                                u32 seed = static_cast<u32>(l1 + x0);
                                if ((seed >> 27) != hp)
                                {
                                    continue;
                                }

                                XDRNG rng(seed);
                                if ((rng.next() >> 27) != atk)
                                {
                                    continue;
                                }
                                if ((rng.next() >> 27) != def)
                                {
                                    continue;
                                }
                                if ((rng.next() >> 27) != spe)
                                {
                                    continue;
                                }
                                if ((rng.next() >> 27) != spa)
                                {
                                    continue;
                                }
                                if ((rng.next() >> 27) != spd)
                                {
                                    continue;
                                }

                                seeds[seeds.count++] = seed;
                            }
                        }
                    }
                }
            }
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
        constexpr u32 LAG0 = 0x7ED7;
        constexpr u32 LAG1 = 0x71A4;
        constexpr u32 LOWER = 0x79C8BF4A;
        constexpr u32 UPPER = 0x79C8A5F4;

        u32 first = pid << 16;
        u32 second = pid & 0xffff0000;

        u32 tmp = ((first - second * PokeRNGR::getMult()) >> 16) * LAG0;
        u32 lo = (tmp + LOWER) >> 16;
        u32 up = (tmp + UPPER) >> 16;

        RecoverySeeds<3> seeds;
        if (lo == up)
        {
            for (u32 lbits = (lo * LAG1) % LAG0; lbits < 0x10000; lbits += LAG0)
            {
                u32 seed = (second | lbits);

                PokeRNGR rng(seed);
                if ((rng.next() & 0xffff0000) == first)
                {
                    seeds[seeds.count++] = rng.getSeed();
                }
            }
        }

        return seeds;
    }

    RecoverySeeds<6> recoverXDRNGIV(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe)
    {
        constexpr u32 LAG0 = 0x44C5;
        constexpr u32 LAG1 = 0xE8D1;
        constexpr u32 LOWER = 0x1E694392;
        constexpr u32 UPPER = 0x1E69FAC8;

        u32 first = static_cast<u32>((hp | (atk << 5) | (def << 10)) << 16);
        u32 second = static_cast<u32>((spe | (spa << 5) | (spd << 10)) << 16);

        u64 tmp = ((XDRNGR::getMult() * second - first) >> 16) * LAG1;
        u32 lo = ((tmp + LOWER) >> 15) * LAG0;
        u32 mi = lo + LAG0;
        u32 up = ((tmp + UPPER) >> 15) * LAG0;

        RecoverySeeds<6> seeds;
        for (u32 lbits = lo % LAG1; lbits < 0x10000; lbits += LAG1)
        {
            u32 seed = (second | lbits);

            XDRNGR rng(seed);
            if ((rng.next() & 0x7fff0000) == first)
            {
                seeds[seeds.count++] = rng.getSeed();
                seeds[seeds.count++] = rng.getSeed() ^ 0x80000000;
            }
        }

        for (u32 lbits = mi % LAG1; lbits < 0x10000; lbits += LAG1)
        {
            u32 seed = (second | lbits);

            XDRNGR rng(seed);
            if ((rng.next() & 0x7fff0000) == first)
            {
                seeds[seeds.count++] = rng.getSeed();
                seeds[seeds.count++] = rng.getSeed() ^ 0x80000000;
            }
        }

        if (mi != up)
        {
            for (u32 lbits = up % LAG1; lbits < 0x10000; lbits += LAG1)
            {
                u32 seed = (second | lbits);

                XDRNGR rng(seed);
                if ((rng.next() & 0x7fff0000) == first)
                {
                    seeds[seeds.count++] = rng.getSeed();
                    seeds[seeds.count++] = rng.getSeed() ^ 0x80000000;
                }
            }
        }

        return seeds;
    }

    RecoverySeeds<2> recoverXDRNGPID(u32 pid)
    {
        constexpr u32 LAG0 = 0xE8D1;
        constexpr u32 LAG1 = 0x5F47;
        constexpr u32 LOWER = 0x55FF8537;
        constexpr u32 UPPER = 0x55FFBC6D;

        u32 first = pid & 0xffff0000;
        u32 second = pid << 16;

        u64 tmp = ((first - second * XDRNGR::getMult()) >> 16) * LAG0;
        u32 lo = (tmp + LOWER) >> 16;
        u32 up = (tmp + UPPER) >> 16;

        RecoverySeeds<2> seeds;
        for (u32 lbits = (lo * LAG1) % LAG0; lbits < 0x10000; lbits += LAG0)
        {
            u32 seed = second | lbits;

            XDRNGR rng(seed);
            if ((rng.next() & 0xffff0000) == first)
            {
                seeds[seeds.count++] = rng.getSeed();
            }
        }

        if (lo != up)
        {
            for (u32 lbits = (up * LAG1) % LAG0; lbits < 0x10000; lbits += LAG0)
            {
                u32 seed = second | lbits;

                XDRNGR rng(seed);
                if ((rng.next() & 0xffff0000) == first)
                {
                    seeds[seeds.count++] = rng.getSeed();
                }
            }
        }

        return seeds;
    }
}
