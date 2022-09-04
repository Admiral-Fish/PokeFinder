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

#ifndef LCRNG64_HPP
#define LCRNG64_HPP

#include <Core/Global.hpp>

constexpr u64 BWRNGAddTable[64]
    = { 0x269ec3,           0x7188d00c55ae9cb2, 0xa8b4e34c910a194,  0x229675654eac71e8, 0x9d8474851566aed0, 0xe7f4341592709a0,
        0xaf8278456068c340, 0x6545aeb598dc4680, 0xed1f0ea72ee38d00, 0x536510bd3a731a00, 0x6a3422cd27963400, 0x5c11e19f19ec6800,
        0xb85689215ed8d000, 0xa5d4d84f69b1a000, 0x42ce3cd183634000, 0x705a4a6dc6c68000, 0x9d08d8068d8d0000, 0xb64abcb91b1a0000,
        0xb4b9ac2236340000, 0x440423046c680000, 0xc24b7108d8d00000, 0xeda38e11b1a00000, 0x7f79cc2363400000, 0x8fbe5846c6800000,
        0x62a7b08d8d000000, 0xd1fb611b1a000000, 0xd6a6c23634000000, 0x780d846c68000000, 0x1b1b08d8d0000000, 0xe23611b1a0000000,
        0x746c236340000000, 0xa8d846c680000000, 0x51b08d8d00000000, 0xa3611b1a00000000, 0x46c2363400000000, 0x8d846c6800000000,
        0x1b08d8d000000000, 0x3611b1a000000000, 0x6c23634000000000, 0xd846c68000000000, 0xb08d8d0000000000, 0x611b1a0000000000,
        0xc236340000000000, 0x846c680000000000, 0x8d8d00000000000,  0x11b1a00000000000, 0x2363400000000000, 0x46c6800000000000,
        0x8d8d000000000000, 0x1b1a000000000000, 0x3634000000000000, 0x6c68000000000000, 0xd8d0000000000000, 0xb1a0000000000000,
        0x6340000000000000, 0xc680000000000000, 0x8d00000000000000, 0x1a00000000000000, 0x3400000000000000, 0x6800000000000000,
        0xd000000000000000, 0xa000000000000000, 0x4000000000000000, 0x800000000000000 };

constexpr u64 BWRNGMultTable[64]
    = { 0x5d588b656c078965, 0x722c73d8054341d9, 0x355bc66e0285e9f1, 0x6c5234d0ae3294e1, 0x4d5a22005a78edc1, 0x84d4844b75beeb81,
        0x2c8b173c56221701, 0xf02de276ca552e01, 0x9804b5dd28ee5c01, 0xd0379e6982ecb801, 0x8cb37296ca197001, 0xd6824c74a532e001,
        0x82ba37c58e65c001, 0x35c2f8fc2ccb8001, 0xec0087bc99970001, 0xbeee68a332e0001,  0xf7b12958665c0001, 0xef8fc3c0ccb80001,
        0xe6d54bc199700001, 0x2481a88332e00001, 0x645f950665c00001, 0x36303a0ccb800001, 0x2224b41997000001, 0x1b5a68332e000001,
        0x92f8d0665c000001, 0x9701a0ccb8000001, 0xf243419970000001, 0xf5868332e0000001, 0x2f0d0665c0000001, 0x6e1a0ccb80000001,
        0x1c34199700000001, 0x3868332e00000001, 0x70d0665c00000001, 0xe1a0ccb800000001, 0xc341997000000001, 0x868332e000000001,
        0xd0665c000000001,  0x1a0ccb8000000001, 0x3419970000000001, 0x68332e0000000001, 0xd0665c0000000001, 0xa0ccb80000000001,
        0x4199700000000001, 0x8332e00000000001, 0x665c00000000001,  0xccb800000000001,  0x1997000000000001, 0x332e000000000001,
        0x665c000000000001, 0xccb8000000000001, 0x9970000000000001, 0x32e0000000000001, 0x65c0000000000001, 0xcb80000000000001,
        0x9700000000000001, 0x2e00000000000001, 0x5c00000000000001, 0xb800000000000001, 0x7000000000000001, 0xe000000000000001,
        0xc000000000000001, 0x8000000000000001, 0x0000000000000001, 0x0000000000000001 };

constexpr u64 BWRNGRAddTable[64]
    = { 0x9b1ae6e9a384e6f9, 0x1c1530cd230fbefe, 0x11ef4891a39cb92c, 0x1412364cffdfb918, 0x2042ea353835fd30, 0xf93379be940ba660,
        0xd45e8e85c061fcc0, 0x3d4bf12daa4eb980, 0x6915bc51ddc87300, 0x89d2d1e6f83ce600, 0xb0f6d3c3a329cc00, 0x7550b8a411139800,
        0xd89fdfeb4d273000, 0x54c4cbe3464e6000, 0x139853f93c9cc000, 0xa93ff8bd39398000, 0xc96034a572730000, 0xe35975f6e4e60000,
        0xb1d71e9dc9cc0000, 0x563f07fb93980000, 0xa6c13af727300000, 0xb68f21ee4e600000, 0x1150f3dc9cc00000, 0xb36ca7b939800000,
        0xaa044f7273000000, 0x60b49ee4e6000000, 0xf4193dc9cc000000, 0xb2f27b9398000000, 0x90e4f72730000000, 0xcdc9ee4e60000000,
        0x4b93dc9cc0000000, 0x5727b93980000000, 0xae4f727300000000, 0x5c9ee4e600000000, 0xb93dc9cc00000000, 0x727b939800000000,
        0xe4f7273000000000, 0xc9ee4e6000000000, 0x93dc9cc000000000, 0x27b9398000000000, 0x4f72730000000000, 0x9ee4e60000000000,
        0x3dc9cc0000000000, 0x7b93980000000000, 0xf727300000000000, 0xee4e600000000000, 0xdc9cc00000000000, 0xb939800000000000,
        0x7273000000000000, 0xe4e6000000000000, 0xc9cc000000000000, 0x9398000000000000, 0x2730000000000000, 0x4e60000000000000,
        0x9cc0000000000000, 0x3980000000000000, 0x7300000000000000, 0xe600000000000000, 0xcc00000000000000, 0x9800000000000000,
        0x3000000000000000, 0x6000000000000000, 0xc000000000000000, 0x8000000000000000 };

constexpr u64 BWRNGRMultTable[64]
    = { 0xdedcedae9638806d, 0xf216a9242c1d2e69, 0x7521bacfa433e711, 0x7d68ec11a955af21, 0x6d18584955b82241, 0x5d7404426c055481,
        0x59e019ed40eee901, 0x6815f15091eed201, 0x74038b504821a401, 0x7178874c41534801, 0x12ff344046e69001, 0x37694ee79ecd2001,
        0xa70435ab819a4001, 0x9236bcc813348001, 0x43e64f5466690001, 0xb3ac75b9ccd20001, 0x6bc47b799a40001,  0xa26007f33480001,
        0x1c53e66900001,    0x76da9b7ccd200001, 0x9117af99a400001,  0x7f9405f334800001, 0xb4ec4be669000001, 0x40e997ccd2000001,
        0xde172f99a4000001, 0x2d3e5f3348000001, 0x1ebcbe6690000001, 0x4e797ccd20000001, 0xe0f2f99a40000001, 0xd1e5f33480000001,
        0xe3cbe66900000001, 0xc797ccd200000001, 0x8f2f99a400000001, 0x1e5f334800000001, 0x3cbe669000000001, 0x797ccd2000000001,
        0xf2f99a4000000001, 0xe5f3348000000001, 0xcbe6690000000001, 0x97ccd20000000001, 0x2f99a40000000001, 0x5f33480000000001,
        0xbe66900000000001, 0x7ccd200000000001, 0xf99a400000000001, 0xf334800000000001, 0xe669000000000001, 0xccd2000000000001,
        0x99a4000000000001, 0x3348000000000001, 0x6690000000000001, 0xcd20000000000001, 0x9a40000000000001, 0x3480000000000001,
        0x6900000000000001, 0xd200000000000001, 0xa400000000000001, 0x4800000000000001, 0x9000000000000001, 0x2000000000000001,
        0x4000000000000001, 0x8000000000000001, 0x0000000000000001, 0x0000000000000001 };

/**
 * @brief Provides random numbers via the LCRNG algorithm. Most commonly used ones are defined at the bottom of the file.
 *
 * @tparam add LCRNG64 addition value
 * @tparam mult LCRNG64 multiplication value
 */
template <u64 add, u64 mult>
class LCRNG64
{
public:
    /**
     * @brief Construct a new LCRNG64 object
     *
     * @param seed Starting PRNG state
     * @param advances Initial number of advances
     */
    LCRNG64(u64 seed) : seed(seed)
    {
    }

    /**
     * @brief Advances the RNG by \p advances amount
     * This function uses a jump ahead table to advance any amount in just O(32). The table is computed as follows
     * addTable[0] = add;
     * multTable[0] = mult;
     * for (int i = 1; i < 64; i++)
     * {
     *   addTable[i] = addTable[i - 1] * (multTable[i - 1] + 1);
     *   multTable[i] = multTable[i - 1] * multTable[i - 1];
     * }
     *
     * @param advances Number of advances
     *
     * @return PRNG value after the advances
     */
    u64 advance(u64 advances)
    {
        const u64 *addTable;
        const u64 *multTable;

        if constexpr (add == 0x269ec3) // BWRNG
        {
            addTable = BWRNGAddTable;
            multTable = BWRNGMultTable;
        }
        else // BWRNG(R)
        {
            static_assert(add == 0x9b1ae6e9a384e6f9, "Unsupported LCRNG64");
            addTable = BWRNGRAddTable;
            multTable = BWRNGRMultTable;
        }

        for (int i = 0; advances && i < 64; advances >>= 1, i++)
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
     *
     * @return PRNG value after the advances
     */
    template <u64 advances>
    u64 advance()
    {
        for (u64 advance = 0; advance < advances; advance++)
        {
            next();
        }
        return seed;
    }

    /**
     * @brief Returns the current PRNG state
     *
     * @return PRNG value
     */
    u64 getSeed() const
    {
        return seed;
    }

    /**
     * @brief Gets the next 64bit PRNG state
     *
     * @return PRNG value
     */
    u64 next()
    {
        return seed = seed * mult + add;
    }

    /**
     * @brief Gets the next 32bit PRNG state
     *
     * @return PRNG value
     */
    u32 nextUInt()
    {
        return next() >> 32;
    }

    /**
     * @brief Gets the next 32bit PRNG state bounded by the \p max value
     *
     * @param max Max value
     *
     * @return PRNG value
     */
    u32 nextUInt(u32 max)
    {
        return ((next() >> 32) * max) >> 32;
    }

private:
    u64 seed;
};

using BWRNG = LCRNG64<0x269ec3, 0x5d588b656c078965>;
using BWRNGR = LCRNG64<0x9b1ae6e9a384e6f9, 0xdedcedae9638806d>;

#endif // LCRNG64_HPP
