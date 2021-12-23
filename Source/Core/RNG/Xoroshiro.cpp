/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2021 by Admiral_Fish, bumba, and EzPzStreamz
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

inline u64 rotl(u64 x, int k)
{
    return (x << k) | (x >> (64 - k));
}

Xoroshiro::Xoroshiro(u64 seed) : state { seed, 0x82A2B175229D6A5B }
{
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

XoroshiroBDSP::XoroshiroBDSP(u64 seed)
{
    auto splitmix = [](u64 seed, u64 state) {
        seed += state;
        seed = 0xBF58476D1CE4E5B9 * (seed ^ (seed >> 30));
        seed = 0x94D049BB133111EB * (seed ^ (seed >> 27));
        return seed ^ (seed >> 31);
    };

    state[0] = splitmix(seed, 0x9E3779B97F4A7C15);
    state[1] = splitmix(seed, 0x3C6EF372FE94F82A);
    // Non-zero state check, doesn't seem possible with 32bit input seed
    // state[0] = (state[0] | state[1]) == 0 ? 1 : state[0];
}

void XoroshiroBDSP::advance(u32 advances)
{
    for (u32 advance = 0; advance < advances; advance++)
    {
        next();
    }
}

u32 XoroshiroBDSP::next()
{
    const u64 s0 = state[0];
    u64 s1 = state[1];
    const u64 result = s0 + s1;

    s1 ^= s0;
    state[0] = rotl(s0, 24) ^ s1 ^ (s1 << 16);
    state[1] = rotl(s1, 37);

    return result >> 32;
}