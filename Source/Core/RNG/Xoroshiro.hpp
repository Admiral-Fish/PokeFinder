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

#ifndef XOROSHIRO_HPP
#define XOROSHIRO_HPP

#include <Core/Util/Global.hpp>

class Xoroshiro
{
public:
    Xoroshiro(u64 seed0, u64 seed1 = 0x82A2B175229D6A5B) : state0(seed0), state1(seed1)
    {
    }

    template <u32 max>
    u32 nextInt()
    {
        auto bitMask = [](u32 x) constexpr
        {
            x--;
            x |= x >> 1;
            x |= x >> 2;
            x |= x >> 4;
            x |= x >> 8;
            x |= x >> 16;
            return x;
        };

        constexpr u32 mask = bitMask(max);
        if constexpr ((max - 1) == mask)
        {
            return next() & mask;
        }
        else
        {
            u32 result;
            do
            {
                result = next() & mask;
            } while (result >= max);
            return result;
        }
    }

private:
    u64 state0;
    u64 state1;

    u64 next()
    {
        const u64 s0 = state0;
        u64 s1 = state1;
        const u64 result = s0 + s1;

        auto rotl = [](u64 x, int k) { return (x << k) | (x >> (64 - k)); };

        s1 ^= s0;
        state0 = rotl(s0, 24) ^ s1 ^ (s1 << 16);
        state1 = rotl(s1, 37);

        return result;
    }
};

class XoroshiroBDSP
{
public:
    XoroshiroBDSP(u64 seed)
    {
        auto splitmix = [](u64 seed, u64 state) {
            seed += state;
            seed = 0xBF58476D1CE4E5B9 * (seed ^ (seed >> 30));
            seed = 0x94D049BB133111EB * (seed * (seed >> 27));
            return seed ^ (seed >> 31);
        };

        state0 = splitmix(seed, 0x9E3779B97F4A7C15);
        state1 = splitmix(seed, 0x3C6EF372FE94F82A);
        // Non-zero state check, doesn't seem possible with 32bit input seed
        // state0 = (state0 | state1) == 0 ? 1 : state0;
    }

    void advance(u32 advances)
    {
        for (u32 advance = 0; advance < advances; advance++)
        {
            next();
        }
    }

    u64 next()
    {
        const u64 s0 = state0;
        u64 s1 = state1;
        const u64 result = s0 + s1;

        auto rotl = [](u64 x, int k) { return (x << k) | (x >> (64 - k)); };

        s1 ^= s0;
        state0 = rotl(s0, 24) ^ s1 ^ (s1 << 16);
        state1 = rotl(s1, 37);

        return result;
    }

    u32 nextUInt()
    {
        return next() >> 32;
    }

    u32 nextUInt(u32 max)
    {
        return nextUInt() % max;
    }

private:
    u64 state0;
    u64 state1;
};

#endif // XOROSHIRO_HPP
