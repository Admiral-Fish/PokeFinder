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

#include "TinyMT.hpp"

constexpr u64 jumpTable[25][2]
    = { { 0x68f6c067369601df, 0x9654f148a90fcec5 }, { 0x3acf552103dbbf73, 0xb9e15caa5f8d586b }, { 0x2528779341278769, 0x149df0a3ce8a313d },
        { 0x5ab81fcd13ccd9fa, 0xce6673b3d158340e }, { 0x3f8285b29763f1a0, 0x84c1c823c273e23b }, { 0x2e1bd6473d093826, 0x61def4964ec4ab34 },
        { 0x33ae14e5d2005a71, 0x334a0fe77ab182de }, { 0x0e06f5b1e69f0174, 0xba589ce43d24301d }, { 0x0586e1d6b2670a75, 0x86bf0979d37c9a1e },
        { 0x55d7db08d9d6e575, 0x6f1cde00c5428bd5 }, { 0x457372c828f32370, 0x55bf4c862403495b }, { 0x7d9a80f7f39cdc7f, 0xa37711f1e4e489c5 },
        { 0x7c5c99ea483c815a, 0x9f1173b680f6752e }, { 0x658cd2f421d18c04, 0x41fbd20233bcb628 }, { 0x694898799783db46, 0xc8fc1f0f485cc220 },
        { 0x4cf6c5ecc4826e0b, 0x8e695f0109724eb6 }, { 0x2a5eaf3a194065dc, 0xf9b4e14b65c67175 }, { 0x475fa9dca8a63e5a, 0xf2272003ed156055 },
        { 0x73ab53c0e246197f, 0x97191167e296db49 }, { 0x20999170716ca869, 0x203777ca7d356342 }, { 0x5dcb2d78b3e9ca0f, 0x7222f0529a9dd99c },
        { 0x197365ac9569a8b4, 0x6dd7a644730f081a }, { 0x2a472d665ef39ef4, 0x0d7382718dc46f8f }, { 0x74284a9019b6eae3, 0xafb1a319fcfd8eb7 },
        { 0x40afea91e9ad4b2c, 0x58440d15ded1d336 } };

TinyMT::TinyMT(u32 seed) : state(seed, 0x8f7011ee, 0xfc78ff1f, 0x3793fdff)
{
    u32 *ptr = &state.uint32[0];
    for (u32 i = 1; i < 8; i++)
    {
        ptr[i & 3] ^= 0x6c078965 * (ptr[(i - 1) & 3] ^ (ptr[(i - 1) & 3] >> 30)) + i;
    }

    if ((ptr[0] & 0x7FFFFFFF) == 0 && ptr[1] == 0 && ptr[2] == 0 && ptr[3] == 0)
    {
        ptr[0] = 'T';
        ptr[1] = 'I';
        ptr[2] = 'N';
        ptr[3] = 'Y';
    }

    advance(8);
}

TinyMT::TinyMT(u32 seed0, u32 seed1, u32 seed2, u32 seed3) : state { seed0, seed1, seed2, seed3 }
{
}

void TinyMT::advance(u32 advances)
{
    for (u64 advance = 0; advance < advances; advance++)
    {
        nextState();
    }
}

void TinyMT::jump(u32 advances)
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
                    nextState();
                }
            }

            state = jump;
        }
    }
}

u32 TinyMT::next()
{
    nextState();
    return temper();
}

u16 TinyMT::nextUShort()
{
    return next() >> 16;
}

void TinyMT::nextState()
{
    u32 *ptr = &state.uint32[0];
    u32 y = ptr[3];
    u32 x = (ptr[0] & 0x7FFFFFFF) ^ ptr[1] ^ ptr[2];

    x ^= (x << 1);
    y ^= (y >> 1) ^ x;

    ptr[0] = ptr[1];
    ptr[1] = ptr[2] ^ ((y & 1) * 0x8f7011ee);
    ptr[2] = x ^ (y << 10) ^ ((y & 1) * 0xfc78ff1f);
    ptr[3] = y;
}

u32 TinyMT::temper()
{
    u32 *ptr = &state.uint32[0];
    u32 t0 = ptr[3];
    u32 t1 = ptr[0] + (ptr[2] >> 8);

    t0 ^= t1;
    if (t1 & 1)
    {
        t0 ^= 0x3793fdff;
    }

    return t0;
}
