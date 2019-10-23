/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2019 by Admiral_Fish, bumba, and EzPzStreamz
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

#include "MTRNG.hpp"

constexpr u32 LOWERMASK = 0x7FFFFFFF;
constexpr u16 M = 397;
constexpr u16 N = 624;
constexpr u32 UPPERMASK = 0x80000000;
constexpr u32 TEMPERINGMASKB = 0x9D2C5680;
constexpr u32 TEMPERINGMASKC = 0xEFC60000;
constexpr u32 TEMPERINGMASKC2 = 0xEF000000;
constexpr u32 mag01[2] = { 0x0, 0x9908B0DF };


namespace PokeFinderCore
{
    void MT::advanceFrames(u32 frames)
    {
        index += frames;
        while (index >= N)
        {
            index -= N;
            shuffle();
        }
    }

    void MT::shuffle()
    {
        u32 y;
        u16 i = 0;

        for (; i < N - M; i++)
        {
            y = (mt[i] & UPPERMASK) | (mt[i + 1] & LOWERMASK);
            mt[i] = mt[i + M] ^ (y >> 1) ^ mag01[y & 0x1];
        }

        for (; i < N - 1; ++i)
        {
            y = (mt[i] & UPPERMASK) | (mt[i + 1] & LOWERMASK);
            mt[i] = mt[i - 227] ^ (y >> 1) ^ mag01[y & 0x1];
        }

        y = (mt[N - 1] & UPPERMASK) | (mt[0] & LOWERMASK);
        mt[N - 1] = mt[396] ^ (y >> 1) ^ mag01[y & 0x1];
    }

    void MT::initialize(u32 seed)
    {
        mt[0] = seed;

        for (index = 1; index < N; index++)
        {
            mt[index] = (0x6C078965 * (mt[index - 1] ^ (mt[index - 1] >> 30)) + index);
        }
    }

    void MT::setSeed(u32 seed, u32 frames)
    {
        initialize(seed);
        advanceFrames(frames);
    }

    u16 MT::nextUShort(u32 frames)
    {
        return nextUInt(frames) >> 16;
    }

    u32 MT::next(u32 frames)
    {
        return nextUInt(frames);
    }


    MersenneTwister::MersenneTwister(u32 seed, u32 frames)
    {
        initialize(seed);
        advanceFrames(frames);
    }

    u32 MersenneTwister::nextUInt(u32 frames)
    {
        advanceFrames(frames);

        u32 y = mt[index++];
        y ^= (y >> 11);
        y ^= (y << 7) & TEMPERINGMASKB;
        y ^= (y << 15) & TEMPERINGMASKC;
        y ^= (y >> 18);

        return y;
    }


    MersenneTwisterUntempered::MersenneTwisterUntempered(u32 seed, u32 frames)
    {
        initialize(seed);
        advanceFrames(frames);
    }

    u32 MersenneTwisterUntempered::nextUInt(u32 frames)
    {
        advanceFrames(frames);
        return mt[index++];
    }


    MersenneTwisterFast::MersenneTwisterFast(u32 calls, u32 seed, u32 frames)
    {
        maxCalls = calls;

        if (maxCalls > 227)
        {
            return;
        }
        max = M + maxCalls;
        initialize(seed);
        advanceFrames(frames);
    }

    u32 MersenneTwisterFast::nextUInt(u32 frames)
    {
        advanceFrames(frames);

        u32 y = mt[index++];
        y ^= (y >> 11);
        y ^= (y << 7) & TEMPERINGMASKB;
        y ^= (y << 15) & TEMPERINGMASKC2;

        return y;
    }

    void MersenneTwisterFast::initialize(u32 seed)
    {
        mt[0] = seed;

        for (index = 1; index < max; ++index)
        {
            mt[index] = (0x6C078965 * (mt[index - 1] ^ (mt[index - 1] >> 30)) + index);
        }
    }

    void MersenneTwisterFast::shuffle()
    {
        u32 y;

        for (u16 i = 0; i < maxCalls; ++i)
        {
            y = (mt[i] & UPPERMASK) | (mt[i + 1] & LOWERMASK);
            mt[i] = mt[i + M] ^ (y >> 1) ^ mag01[y & 0x1];
        }
    }
}
