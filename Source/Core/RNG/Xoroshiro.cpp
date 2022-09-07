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

#include "Xoroshiro.hpp"
#include <Core/RNG/SIMD.hpp>

constexpr u64 jumpTable[32][2]
    = { /*{ 0x0, 0x2 },
        { 0x0, 0x4 },
        { 0x0, 0x10 },
        { 0x0, 0x100 },
        { 0x0, 0x10000 },
        { 0x0, 0x100000000 },
        { 0x1, 0x0 },*/
        { 0x8828e513b43d5, 0x95b8f76579aa001 },     { 0x7a8ff5b1c465a931, 0x162ad6ec01b26eae }, { 0xb18b0d36cd81a8f5, 0xb4fbaa5c54ee8b8f },
        { 0x23ac5e0ba1cecb29, 0x1207a1706bebb202 }, { 0xbb18e9c8d463bb1b, 0x2c88ef71166bc53d }, { 0xe3fbe606ef4e8e09, 0xc3865bb154e9be10 },
        { 0x28faaaebb31ee2db, 0x1a9fc99fa7818274 }, { 0x30a7c4eef203c7eb, 0x588abd4c2ce2ba80 }, { 0xa425003f3220a91d, 0x9c90debc053e8cef },
        { 0x81e1dd96586cf985, 0xb82ca99a09a4e71e }, { 0x4f7fd3dfbb820bfb, 0x35d69e118698a31d }, { 0xfee2760ef3a900b3, 0x49613606c466efd3 },
        { 0xf0df0531f434c57d, 0xbd031d011900a9e5 }, { 0x442576715266740c, 0x235e761b3b378590 }, { 0x1e8bae8f680d2b35, 0x3710a7ae7945df77 },
        { 0xfd7027fe6d2f6764, 0x75d8e7dbceda609c }, { 0x28eff231ad438124, 0xde2cba60cd3332b5 }, { 0x1808760d0a0909a1, 0x377e64c4e80a06fa },
        { 0xb9a362fafedfe9d2, 0xcf0a2225da7fb95 },  { 0xf57881ab117349fd, 0x2bab58a3cadfc0a3 }, { 0x849272241425c996, 0x8d51ecdb9ed82455 },
        { 0xf1ccb8898cbc07cd, 0x521b29d0a57326c1 }, { 0x61179e44214caafa, 0xfbe65017abec72dd }, { 0xd9aa6b1e93fbb6e4, 0x6c446b9bc95c267b },
        { 0x86e3772194563f6d, 0x64f80248d23655c6 }
      };

inline u64 rotl(u64 x, int k)
{
    return (x << k) | (x >> (64 - k));
}

inline u64 splitmix(u64 seed, u64 state)
{
    seed += state;
    seed = 0xBF58476D1CE4E5B9 * (seed ^ (seed >> 30));
    seed = 0x94D049BB133111EB * (seed ^ (seed >> 27));
    return seed ^ (seed >> 31);
}

Xoroshiro::Xoroshiro(u64 seed) : state { seed, 0x82A2B175229D6A5B }
{
}

Xoroshiro::Xoroshiro(u64 seed0, u64 seed1) : state { seed0, seed1 }
{
}

void Xoroshiro::advance(u32 advances)
{
    for (u64 advance = 0; advance < advances; advance++)
    {
        next();
    }
}

void Xoroshiro::jump(u32 advances)
{
    advance(advances & 0x7f);
    advances >>= 7;

    for (int i = 0; advances; advances >>= 1, i++)
    {
        if (advances & 1)
        {
            vuint32x4 jump = v32x4_set(0);

            u64 val = jumpTable[i][1];
            for (int j = 0; j < 64; j++, val >>= 1)
            {
                if (val & 1)
                {
                    jump = v32x4_xor(jump, v32x4_load(reinterpret_cast<u32 *>(&state[0])));
                }
                next();
            }

            val = jumpTable[i][0];
            for (; val; val >>= 1)
            {
                if (val & 1)
                {
                    jump = v32x4_xor(jump, v32x4_load(reinterpret_cast<u32 *>(&state[0])));
                }
                next();
            }

            v32x4_store(reinterpret_cast<u32 *>(&state[0]), jump);
        }
    }
}

u64 Xoroshiro::next()
{
    const u64 s0 = state[0];
    u64 s1 = state[1];
    const u64 result = s0 + s1;

    s1 ^= s0;
    state[0] = rotl(s0, 24) ^ s1 ^ (s1 << 16);
    state[1] = rotl(s1, 37);

    return result;
}

XoroshiroBDSP::XoroshiroBDSP(u64 seed) : Xoroshiro(splitmix(seed, 0x9E3779B97F4A7C15), splitmix(seed, 0x3C6EF372FE94F82A))
{
    // Non-zero state check, doesn't seem possible with 32bit input seed
    // state[0] = (state[0] | state[1]) == 0 ? 1 : state[0];
}
