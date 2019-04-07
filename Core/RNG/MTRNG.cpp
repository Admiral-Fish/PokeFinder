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

void MT::shuffle()
{
    u32 y;
    u16 i = 0;

    for (; i < 227; i++)
    {
        y = (mt.at(i) & UPPERMASK) | (mt.at(i + 1) & LOWERMASK);
        mt[i] = mt.at(i + M) ^ (y >> 1) ^ mag01.at(y & 0x1);
    }

    for (; i < 623; ++i)
    {
        y = (mt.at(i) & UPPERMASK) | (mt.at(i + 1) & LOWERMASK);
        mt[i] = mt.at(i - 227) ^ (y >> 1) ^ mag01.at(y & 0x1);
    }

    y = (mt.at(623) & UPPERMASK) | (mt.at(0) & LOWERMASK);
    mt[623] = mt.at(396) ^ (y >> 1) ^ mag01.at(y & 0x1);
}

void MT::initialize(u32 seed)
{
    this->seed = seed;
    mt = QVector<u32>(624);
    mt[0] = seed;

    for (index = 1; index < N; index++)
    {
        mt[index] = (0x6C078965 * (mt.at(index - 1) ^ (mt.at(index - 1) >> 30)) + index);
    }
}

void MT::setSeed(u32 seed)
{
    initialize(seed);
}

void MT::setSeed(u32 seed, u32 frames)
{
    initialize(seed);
    advanceFrames(frames);
}

u16 MT::nextUShort()
{
    return nextUInt() >> 16;
}

u32 MT::getSeed()
{
    return seed;
}


MersenneTwister::MersenneTwister()
{
    initialize(0);
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

    u32 y = mt.at(index++);
    y ^= (y >> 11);
    y ^= (y << 7) & TEMPERINGMASKB;
    y ^= (y << 15) & TEMPERINGMASKC;
    y ^= (y >> 18);

    return y;
}


MersenneTwisterUntempered::MersenneTwisterUntempered()
{
    initialize(0);
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

    return mt.at(index++);
}


MersenneTwisterFast::MersenneTwisterFast()
{
    maxCalls = 227;
    max = M + 227;
    initialize(0);
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

void MersenneTwisterFast::initialize(u32 seed)
{
    this->seed = seed;
    mt = QVector<u32>(max);
    mt[0] = seed;

    for (index = 1; index < max; ++index)
    {
        mt[index] = (0x6C078965 * (mt.at(index - 1) ^ (mt.at(index - 1) >> 30)) + index);
    }
}

void MersenneTwisterFast::shuffle()
{
    u32 y;

    for (u16 i = 0; i < maxCalls; ++i)
    {
        y = (mt.at(i) & UPPERMASK) | (mt.at(i + 1) & LOWERMASK);
        mt[i] = mt.at(i + M) ^ (y >> 1) ^ mag01.at(y & 0x1);
    }
}
