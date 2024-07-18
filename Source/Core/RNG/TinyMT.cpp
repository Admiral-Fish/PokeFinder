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
    = { { 0xb0a48045db1bfe95, 0x1b98a18f31f57486 }, { 0xe29d1503ee564039, 0x342d0c6dc777e228 }, { 0xfd7a37b1acaa7823, 0x9951a06456708b7e },
        { 0x5ab81fcd13ccd9fa, 0xce6673b3d158340e }, { 0xe7d0c5907aee0eea, 0x90d98e45a895878 },  { 0x2e1bd6473d093826, 0x61def4964ec4ab34 },
        { 0x33ae14e5d2005a71, 0x334a0fe77ab182de }, { 0xd654b5930b12fe3e, 0x3794cc23a5de8a5e }, { 0x586e1d6b2670a75, 0x86bf0979d37c9a1e },
        { 0x8d859b2a345b1a3f, 0xe2d08ec75db83196 }, { 0x9d2132eac57edc3a, 0xd8731c41bcf9f318 }, { 0xa5c8c0d51e112335, 0x2ebb41367c1e3386 },
        { 0x7c5c99ea483c815a, 0x9f1173b680f6752e }, { 0x658cd2f421d18c04, 0x41fbd20233bcb628 }, { 0x694898799783db46, 0xc8fc1f0f485cc220 },
        { 0x4cf6c5ecc4826e0b, 0x8e695f0109724eb6 }, { 0xf20cef18f4cd9a96, 0x7478b18cfd3ccb36 }, { 0x9f0de9fe452bc110, 0x7feb70c475efda16 },
        { 0xabf913e20fcbe635, 0x1ad541a07a6c610a }, { 0x20999170716ca869, 0x203777ca7d356342 }, { 0x5dcb2d78b3e9ca0f, 0x7222f0529a9dd99c },
        { 0x197365ac9569a8b4, 0x6dd7a644730f081a }, { 0xf2156d44b37e61be, 0x80bfd2b6153ed5cc }, { 0xac7a0ab2f43b15a9, 0x227df3de640734f4 },
        { 0x40afea91e9ad4b2c, 0x58440d15ded1d336 } };

TinyMT::TinyMT(u32 seed) : state { seed, 0x8f7011ee, 0xfc78ff1f, 0x3793fdff }
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
            vuint32x4 jump = v32x4_set(0);

            for (int j = 1; j >= 0; j--)
            {
                u64 val = jumpTable[i][j];
                for (int k = 0; k < 64; k++, val >>= 1)
                {
                    if (val & 1)
                    {
                        jump = v32x4_xor(jump, state.uint128);
                    }
                    nextState();
                }
            }

            state.uint128 = jump;
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
