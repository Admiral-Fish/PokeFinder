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

#define LOWERMASK           0x7FFFFFFF
#define M                   397
#define N                   624
#define UPPERMASK           0x80000000
#define TEMPERINGMASKB      0x9D2C5680
#define TEMPERINGMASKC      0xEFC60000
#define TEMPERINGMASKC2     0xEF000000

void MT::advanceFrames(u32 frames)
{
    index += frames;
    while (index >= N)
    {
        index -= N;
        shuffle();
    }
}


MersenneTwister::MersenneTwister(u32 seed, u32 frames)
{
    initialize(seed);
    advanceFrames(frames);
}

u32 MersenneTwister::nextUInt()
{
    if (index >= N)
    {
        shuffle();
        index = 0;
    }

    u32 y = mt[index++];
    y ^= (y >> 11);
    y ^= (y << 7) & TEMPERINGMASKB;
    y ^= (y << 15) & TEMPERINGMASKC;
    y ^= (y >> 18);

    return y;
}

u16 MersenneTwister::nextUShort()
{
    return nextUInt() >> 16;
}

void MersenneTwister::setSeed(u32 seed)
{
    initialize(seed);
}

void MersenneTwister::setSeed(u32 seed, u32 frames)
{
    initialize(seed);
    advanceFrames(frames);
}

u32 MersenneTwister::getSeed()
{
    return seed;
}

void MersenneTwister::initialize(u32 seed)
{
    this->seed = seed;
    mt[0] = seed;

    for (index = 1; index < N; index++)
    {
        mt[index] = (0x6C078965 * (mt[index - 1] ^ (mt[index - 1] >> 30)) + index);
    }
}

void MersenneTwister::shuffle()
{
    u32 y;
    u16 kk = 0;

    for (; kk < 227; ++kk)
    {
        y = (mt[kk] & UPPERMASK) | (mt[kk + 1] & LOWERMASK);
        mt[kk] = mt[kk + M] ^ (y >> 1) ^ mag01[y & 0x1];
    }

    for (; kk < 623; ++kk)
    {
        y = (mt[kk] & UPPERMASK) | (mt[kk + 1] & LOWERMASK);
        mt[kk] = mt[kk - 227] ^ (y >> 1) ^ mag01[y & 0x1];
    }

    y = (mt[623] & UPPERMASK) | (mt[0] & LOWERMASK);
    mt[623] = mt[396] ^ (y >> 1) ^ mag01[y & 0x1];
}


MersenneTwisterUntempered::MersenneTwisterUntempered(u32 seed, u32 frames)
{
    initialize(seed);
    advanceFrames(frames);
}

u32 MersenneTwisterUntempered::nextUInt()
{
    if (index >= N)
    {
        shuffle();
        index = 0;
    }

    return mt[index++];
}

u16 MersenneTwisterUntempered::nextUShort()
{
    return nextUInt() >> 16;
}

void MersenneTwisterUntempered::setSeed(u32 seed)
{
    initialize(seed);
}

void MersenneTwisterUntempered::setSeed(u32 seed, u32 frames)
{
    initialize(seed);
    advanceFrames(frames);
}

u32 MersenneTwisterUntempered::getSeed()
{
    return seed;
}

void MersenneTwisterUntempered::initialize(u32 seed)
{
    this->seed = seed;
    mt[0] = seed;

    for (index = 1; index < N; index++)
    {
        mt[index] = (0x6C078965 * (mt[index - 1] ^ (mt[index - 1] >> 30)) + index);
    }
}

void MersenneTwisterUntempered::shuffle()
{
    u16 kk = 0;
    u32 y;

    for (; kk < 227; ++kk)
    {
        y = (mt[kk] & UPPERMASK) | (mt[kk + 1] & LOWERMASK);
        mt[kk] = mt[kk + M] ^ (y >> 1) ^ mag01[y & 0x1];
    }

    for (; kk < 623; ++kk)
    {
        y = (mt[kk] & UPPERMASK) | (mt[kk + 1] & LOWERMASK);
        mt[kk] = mt[kk - 227] ^ (y >> 1) ^ mag01[y & 0x1];
    }

    y = (mt[623] & UPPERMASK) | (mt[0] & LOWERMASK);
    mt[623] = mt[396] ^ (y >> 1) ^ mag01[y & 0x1];
}


MersenneTwisterFast::MersenneTwisterFast(u32 seed, u32 calls, u32 frames)
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

u32 MersenneTwisterFast::nextUInt()
{
    if (index >= max)
    {
        shuffle();
        index = 0;
    }

    u32 y = mt[index++];
    y ^= (y >> 11);
    y ^= (y << 7) & TEMPERINGMASKB;
    y ^= (y << 15) & TEMPERINGMASKC2;

    return y;
}

u16 MersenneTwisterFast::nextUShort()
{
    return nextUInt() >> 16;
}

void MersenneTwisterFast::setSeed(u32 seed)
{
    initialize(seed);
}

void MersenneTwisterFast::setSeed(u32 seed, u32 frames)
{
    initialize(seed);
    advanceFrames(frames);
}

u32 MersenneTwisterFast::getSeed()
{
    return seed;
}

void MersenneTwisterFast::initialize(u32 seed)
{
    this->seed = seed;
    mt[0] = seed;

    for (index = 1; index < max; ++index)
    {
        mt[index] = (0x6C078965 * (mt[index - 1] ^ (mt[index - 1] >> 30)) + index);
    }
}

void MersenneTwisterFast::shuffle()
{
    u32 y;

    for (u16 kk = 0; kk < maxCalls; ++kk)
    {
        y = (mt[kk] & UPPERMASK) | (mt[kk + 1] & LOWERMASK);
        mt[kk] = mt[kk + M] ^ (y >> 1) ^ mag01[y & 0x1];
    }
}
