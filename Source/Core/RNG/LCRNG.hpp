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

#ifndef LCRNG_HPP
#define LCRNG_HPP

#include <Core/Global.hpp>

constexpr u32 ARNGAddTable[32]
    = { 0x1,        0x6c078966, 0xdbffe6dc, 0x895277f8, 0xe69948f0, 0x392f75e0, 0x778e7bc0, 0xabbb3780, 0x68ef6f00, 0xfc2de00,  0xd715bc00,
        0x8c6b7800, 0x91d6f000, 0x7ade000,  0x9f5bc000, 0x7eb78000, 0xfd6f0000, 0xfade0000, 0xf5bc0000, 0xeb780000, 0xd6f00000, 0xade00000,
        0x5bc00000, 0xb7800000, 0x6f000000, 0xde000000, 0xbc000000, 0x78000000, 0xf0000000, 0xe0000000, 0xc0000000, 0x80000000 };

constexpr u32 ARNGMultTable[32] = {
    0x6c078965, 0x54341d9,  0x285e9f1,  0xae3294e1, 0x5a78edc1, 0x75beeb81, 0x56221701, 0xca552e01, 0x28ee5c01, 0x82ecb801, 0xca197001,
    0xa532e001, 0x8e65c001, 0x2ccb8001, 0x99970001, 0x332e0001, 0x665c0001, 0xccb80001, 0x99700001, 0x32e00001, 0x65c00001, 0xcb800001,
    0x97000001, 0x2e000001, 0x5c000001, 0xb8000001, 0x70000001, 0xe0000001, 0xc0000001, 0x80000001, 0x1,        0x1,
};

constexpr u32 ARNGRAddTable[32]
    = { 0x69c77f93, 0x3daa512a, 0x8cdd2764, 0x5f040108, 0xfae49b10, 0xcf081a20, 0xcd4fc440, 0xa7bdc880, 0xe4f49100, 0x27cd2200, 0x72a4400,
        0xec948800, 0x52291000, 0x88522000, 0xa0a44000, 0x81488000, 0x2910000,  0x5220000,  0xa440000,  0x14880000, 0x29100000, 0x52200000,
        0xa4400000, 0x48800000, 0x91000000, 0x22000000, 0x44000000, 0x88000000, 0x10000000, 0x20000000, 0x40000000, 0x80000000 };

constexpr u32 ARNGRMultTable[32]
    = { 0x9638806d, 0x2c1d2e69, 0xa433e711, 0xa955af21, 0x55b82241, 0x6c055481, 0x40eee901, 0x91eed201, 0x4821a401, 0x41534801, 0x46e69001,
        0x9ecd2001, 0x819a4001, 0x13348001, 0x66690001, 0xccd20001, 0x99a40001, 0x33480001, 0x66900001, 0xcd200001, 0x9a400001, 0x34800001,
        0x69000001, 0xd2000001, 0xa4000001, 0x48000001, 0x90000001, 0x20000001, 0x40000001, 0x80000001, 0x1,        0x1 };

constexpr u32 PokeRNGAddTable[32]
    = { 0x6073,     0xe97e7b6a, 0x31b0dde4, 0x67dbb608, 0xcba72510, 0x1d29ae20, 0xba84ec40, 0x79f01880, 0x8793100,  0x6b566200, 0x803cc400,
        0xa6b98800, 0xe6731000, 0x30e62000, 0xf1cc4000, 0x23988000, 0x47310000, 0x8e620000, 0x1cc40000, 0x39880000, 0x73100000, 0xe6200000,
        0xcc400000, 0x98800000, 0x31000000, 0x62000000, 0xc4000000, 0x88000000, 0x10000000, 0x20000000, 0x40000000, 0x80000000 };

constexpr u32 PokeRNGMultTable[32] = {
    0x41c64e6d, 0xc2a29a69, 0xee067f11, 0xcfdddf21, 0x5f748241, 0x8b2e1481, 0x76006901, 0x1711d201, 0xbe67a401, 0xdddf4801, 0x3ffe9001,
    0x90fd2001, 0x65fa4001, 0xdbf48001, 0xf7e90001, 0xefd20001, 0xdfa40001, 0xbf480001, 0x7e900001, 0xfd200001, 0xfa400001, 0xf4800001,
    0xe9000001, 0xd2000001, 0xa4000001, 0x48000001, 0x90000001, 0x20000001, 0x40000001, 0x80000001, 0x1,        0x1,
};

constexpr u32 PokeRNGRAddTable[32]
    = { 0xa3561a1,  0x4d3cb126, 0x7cd1f85c, 0x9019e2f8, 0x24d33ef0, 0x2effe1e0, 0x1a2153c0, 0x18a8e780, 0x41eacf00, 0xbe399e00, 0x26033c00,
        0xf2467800, 0x7d8cf000, 0x5f19e000, 0x4e33c000, 0xdc678000, 0xb8cf0000, 0x719e0000, 0xe33c0000, 0xc6780000, 0x8cf00000, 0x19e00000,
        0x33c00000, 0x67800000, 0xcf000000, 0x9e000000, 0x3c000000, 0x78000000, 0xf0000000, 0xe0000000, 0xc0000000, 0x80000000 };

constexpr u32 PokeRNGRMultTable[32]
    = { 0xeeb9eb65, 0xdc6c95d9, 0xbece51f1, 0xb61664e1, 0x6a6c8dc1, 0xbd562b81, 0xbc109701, 0xb1322e01, 0x62a85c01, 0xa660b801, 0xd1017001,
        0xb302e001, 0xaa05c001, 0x640b8001, 0x8170001,  0x102e0001, 0x205c0001, 0x40b80001, 0x81700001, 0x2e00001,  0x5c00001,  0xb800001,
        0x17000001, 0x2e000001, 0x5c000001, 0xb8000001, 0x70000001, 0xe0000001, 0xc0000001, 0x80000001, 0x1,        0x1 };

constexpr u32 XDRNGAddTable[32]
    = { 0x269ec3,   0x1e278e7a, 0x98520c4,  0x7e1dbec8, 0x3e314290, 0x824e1920, 0x844e8240, 0xfd864480, 0xdfb18900, 0xd9f71200, 0x5e3e2400,
        0x65bc4800, 0x70789000, 0x74f12000, 0x39e24000, 0xb3c48000, 0x67890000, 0xcf120000, 0x9e240000, 0x3c480000, 0x78900000, 0xf1200000,
        0xe2400000, 0xc4800000, 0x89000000, 0x12000000, 0x24000000, 0x48000000, 0x90000000, 0x20000000, 0x40000000, 0x8000000 };

constexpr u32 XDRNGMultTable[32] = {
    0x343fd,    0xa9fc6809, 0xddff5051, 0xf490b9a1, 0x43ba1741, 0xd290be81, 0x82e3bd01, 0xbf507a01, 0xf8c4f401, 0x7a19e801, 0x1673d001,
    0xb5e7a001, 0x8fcf4001, 0xaf9e8001, 0x9f3d0001, 0x3e7a0001, 0x7cf40001, 0xf9e80001, 0xf3d00001, 0xe7a00001, 0xcf400001, 0x9e800001,
    0x3d000001, 0x7a000001, 0xf4000001, 0xe8000001, 0xd0000001, 0xa0000001, 0x40000001, 0x80000001, 0x1,        0x1,
};

constexpr u32 XDRNGRAddTable[32]
    = { 0xa170f641, 0x3882ad6,  0x3e0a787c, 0xe493e638, 0xbdc95170, 0xdc36e0,   0xbc5abdc0, 0x451ebb80, 0x2ae27700, 0x5058ee00, 0x4b01dc00,
        0x3f43b800, 0x23877000, 0xdb0ee000, 0x61dc000,  0x4c3b8000, 0x98770000, 0x30ee0000, 0x61dc0000, 0xc3b80000, 0x87700000, 0xee00000,
        0x1dc00000, 0x3b800000, 0x77000000, 0xee000000, 0xdc000000, 0xb8000000, 0x70000000, 0xe0000000, 0xc0000000, 0x8000000 };

constexpr u32 XDRNGRMultTable[32]
    = { 0xb9b33155, 0xe05fa639, 0x8a3bf8b1, 0x672d6a61, 0xa04e78c1, 0xe918181,  0x11a54301, 0x92d38601, 0x4fcb0c01, 0xa8261801, 0x728c3001,
        0x6e186001, 0x30c001,   0x90618001, 0x60c30001, 0xc1860001, 0x830c0001, 0x6180001,  0xc300001,  0x18600001, 0x30c00001, 0x61800001,
        0xc3000001, 0x86000001, 0xc000001,  0x18000001, 0x30000001, 0x60000001, 0xc0000001, 0x80000001, 0x1,        0x1 };

/**
 * @brief Provides random numbers via the LCRNG algorithm. Most commonly used ones are defined at the bottom of the file.
 *
 * @tparam add LCRNG addition value
 * @tparam mult LCRNG multiplication value
 */
template <u32 add, u32 mult>
class LCRNG
{
public:
    /**
     * @brief Construct a new LCRNG object
     *
     * @param seed Starting PRNG value
     * @param advances Number of initial advances
     * @param count Pointer to keep track of advance count
     */
    LCRNG(u32 seed, u64 advances = 0, u32 *count = nullptr) : seed(seed), count(count)
    {
    }

    /**
     * @brief Advances the RNG by \p advances amount
     * This function uses a jump ahead table to advance any amount in just O(32). The table is computed as follows
     * addTable[0] = add;
     * multTable[0] = mult;
     * for (int i = 1; i < 32; i++)
     * {
     *   addTable[i] = addTable[i - 1] * (multTable[i - 1] + 1);
     *   multTable[i] = multTable[i - 1] * multTable[i - 1];
     * }
     *
     * @tparam flag Whether count should be incremented or not
     * @param advances Number of advances
     *
     * @return PRNG value after the advances
     */
    template <bool flag = false>
    u32 advance(u32 advances)
    {
        if constexpr (flag)
        {
            *count += advances;
        }

        const u32 *addTable;
        const u32 *multTable;

        if constexpr (add == 0x01) // ARNG
        {
            addTable = ARNGAddTable;
            multTable = ARNGMultTable;
        }
        else if constexpr (add == 0x69C77F93) // ARNG(R)
        {
            addTable = ARNGRAddTable;
            multTable = ARNGRMultTable;
        }
        else if constexpr (add == 0x6073) // PokeRNG
        {
            addTable = PokeRNGAddTable;
            multTable = PokeRNGMultTable;
        }
        else if constexpr (add == 0xA3561A1) // PokeRNG(R)
        {
            addTable = PokeRNGRAddTable;
            multTable = PokeRNGRMultTable;
        }
        else if constexpr (add == 0x269EC3) // XDRNG
        {
            addTable = XDRNGAddTable;
            multTable = XDRNGMultTable;
        }
        else // XDRNG(R)
        {
            static_assert(add == 0xA170F641, "Unsupported LCRNG");
            addTable = XDRNGRAddTable;
            multTable = XDRNGRMultTable;
        }

        for (int i = 0; advances; advances >>= 1, i++)
        {
            if (advances & 1)
            {
                seed = seed * multTable[i] + addTable[i];
            }
        }

        return seed;
    }

    /**
     * @brief Advances the RNG by \p advances amount
     * This function is called when the number of advances is predetermined and allows the compiler to optimize to the final mult/add
     *
     * @tparam advances Number of advances
     * @tparam flag Whether count should be incremented or not
     *
     * @return PRNG value after the advances
     */
    template <u32 advances, bool flag = false>
    u32 advance()
    {
        for (u64 advance = 0; advance < advances; advance++)
        {
            next<flag>();
        }
        return seed;
    }

    /**
     * @brief Returns the current PRNG state
     *
     * @return PRNG value
     */
    u32 getSeed() const
    {
        return seed;
    }

    /**
     * @brief Gets the next 32bit PRNG state
     *
     * @tparam flag Whether count should be incremented or not
     *
     * @return PRNG value
     */
    template <bool flag = false>
    u32 next()
    {
        if constexpr (flag)
        {
            (*count)++;
        }
        return seed = seed * mult + add;
    }

    /**
     * @brief Gets the next 16bit PRNG state
     *
     * @tparam flag Whether count should be incremented or not
     *
     * @return PRNG value
     */
    template <bool flag = false>
    u16 nextUShort()
    {
        return next<flag>() >> 16;
    }

    /**
     * @brief Sets the PRNG state
     *
     * @param seed PRNG state
     */
    void setSeed(u32 seed)
    {
        this->seed = seed;
    }

private:
    u32 *count;
    u32 seed;
};

using ARNG = LCRNG<0x01, 0x6C078965>;
using ARNGR = LCRNG<0x69C77F93, 0x9638806D>;
using PokeRNG = LCRNG<0x6073, 0x41C64E6D>;
using PokeRNGR = LCRNG<0xA3561A1, 0xEEB9EB65>;
using XDRNG = LCRNG<0x269EC3, 0x343FD>;
using XDRNGR = LCRNG<0xA170F641, 0xB9B33155>;

#endif // LCRNG_HPP
