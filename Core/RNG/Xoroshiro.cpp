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

#include "Xoroshiro.hpp"
#include <Core/RNG/Jump.hpp>

static const vuint128 polynomial = vuint128(0x095b8f76579aa001, 0x0008828e513b43d5);

static inline u64 splitmix(u64 seed)
{
    seed = 0xBF58476D1CE4E5B9 * (seed ^ (seed >> 30));
    seed = 0x94D049BB133111EB * (seed ^ (seed >> 27));
    return seed ^ (seed >> 31);
}

Xoroshiro::Xoroshiro(u64 seed) : Xoroshiro(seed, 0x82A2B175229D6A5B)
{
}

Xoroshiro::Xoroshiro(u64 seed0, u64 seed1)
{
    u64 *ptr = &state.uint64[0];
    ptr[0] = seed0;
    ptr[1] = seed1;
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
    auto jump = Jump::computeJumpPolynomial<128>(polynomial, advances);

    vuint128 temp(0);
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 64; j++)
        {
            if (jump.uint64[i] & (1ULL << j))
            {
                temp = temp ^ state;
            }
            next();
        }
    }

    state = temp;
}

u64 Xoroshiro::next()
{
    u64 *ptr = &state.uint64[0];
    u64 s0 = ptr[0];
    u64 s1 = ptr[1];
    u64 result = s0 + s1;

    s1 ^= s0;
    ptr[0] = std::rotl(s0, 24) ^ s1 ^ (s1 << 16);
    ptr[1] = std::rotl(s1, 37);

    return result;
}

XoroshiroBDSP::XoroshiroBDSP(u64 seed) : Xoroshiro(splitmix(seed + 0x9E3779B97F4A7C15), splitmix(seed + 0x3C6EF372FE94F82A))
{
    // Non-zero state check, doesn't seem possible with 32bit input seed
    // state[0] = (state[0] | state[1]) == 0 ? 1 : state[0];
}
