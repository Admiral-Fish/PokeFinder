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

#include "Xorshift.hpp"
#include <bit>

constexpr u64 jumpTable[25][2]
    = { { 0x10046d8b3, 0xf985d65ffd3c8001 },        { 0x956c89fbfa6b67e9, 0xa42ca9aeb1e10da6 }, { 0xff7aa97c47ec17c7, 0x1a0988e988f8a56e },
        { 0x9dff33679bd01948, 0xfb6668ff443b16f0 }, { 0xbd36a1d3e3b212da, 0x46a4759b1dc83ce2 }, { 0x6d2f354b8b0e3c0b, 0x9640bc4ca0cbaa6c },
        { 0xecf6383dca4f108f, 0x947096c72b4d52fb }, { 0xe1054e817177890a, 0xdaf32f04ddca12e },  { 0x2ae1912115107c6, 0xb9fa05aab78641a5 },
        { 0x59981d3df81649be, 0x382fa5aa95f950e3 }, { 0x6644b35f0f8cee00, 0xdba31d29fc044fdb }, { 0xecff213c169fd455, 0x3ca16b953c338c19 },
        { 0xa9dfd9fb0a094939, 0x3ffdcb096a60ecbe }, { 0x79d7462b16c479f, 0xfd6aef50f8c0b5fa },  { 0x3896736d707b6b6, 0x9148889b8269b55d },
        { 0xdea22e8899dbbeaa, 0x4c6ac659b91ef36a }, { 0xc1150ddd5ae7d320, 0x67ccf586cddb0649 }, { 0x5f0be91ac7e9c381, 0x33c8177d6b2cc0f0 },
        { 0xcd15d2ba212e573, 0x4a5f78fc104e47b9 },  { 0xab586674147dec3e, 0xd69063e6e8a0b936 }, { 0x4bfd9d67ed372866, 0x7071114af22d34f5 },
        { 0xdaf387cab4ef5c18, 0x686287302b5cd38c }, { 0xffaf82745790af3e, 0xbb7d371f547cca1e }, { 0x7b932849fe573afa, 0xeb96acd6c88829f9 },
        { 0x8cedf8dfe2d6e821, 0xb4fd2c6573bf7047 } };

Xorshift::Xorshift(u64 seed0, u64 seed1)
{
    u64 *ptr = &state.uint64[0];
    ptr[0] = std::rotl(seed0, 32);
    ptr[1] = std::rotl(seed1, 32);
}

Xorshift::Xorshift(u64 seed0, u64 seed1, u32 advances) : Xorshift(seed0, seed1)
{
    jump(advances);
}

void Xorshift::advance(u32 advances)
{
    for (u64 advance = 0; advance < advances; advance++)
    {
        next();
    }
}

void Xorshift::jump(u32 advances)
{
    advance(advances & 0x7f);
    advances >>= 7;

    for (int i = 0; advances; advances >>= 1, i++)
    {
        if (advances & 1)
        {
            vuint128 jump(0);

            for (int j = 1; j >= 0; j--)
            {
                u64 val = jumpTable[i][j];
                for (int k = 0; k < 64; k++, val >>= 1)
                {
                    if (val & 1)
                    {
                        jump = jump ^ state;
                    }
                    next();
                }
            }

            state = jump;
        }
    }
}

u32 Xorshift::next()
{
    u32 *ptr = &state.uint32[0];
    u32 t = ptr[0];
    u32 s = ptr[3];

    t ^= t << 11;
    t ^= t >> 8;
    t ^= s ^ (s >> 19);

    ptr[0] = ptr[1];
    ptr[1] = ptr[2];
    ptr[2] = ptr[3];
    ptr[3] = t;

    return t;
}
