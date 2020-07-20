/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2020 by Admiral_Fish, bumba, and EzPzStreamz
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

#include "MT.hpp"

MT::MT(u32 seed)
{
    mt[0] = seed;

    for (index = 1; index < 624; index++)
    {
        mt[index] = 0x6C078965 * (mt[index - 1] ^ (mt[index - 1] >> 30)) + index;
    }
}

void MT::advanceFrames(u32 frames)
{
    index += frames;
    while (index >= 624)
    {
        shuffle();
    }
}

u32 MT::next()
{
    if (index >= 624)
    {
        shuffle();
    }

    u32 y = mt[index++];
    y ^= (y >> 11);
    y ^= (y << 7) & 0x9D2C5680;
    y ^= (y << 15) & 0xEFC60000;
    y ^= (y >> 18);

    return y;
}

u16 MT::nextUShort()
{
    return next() >> 16;
}

void MT::shuffle()
{
    for (u16 i = 0; i < 624; i++)
    {
        u32 y = (mt[i] & 0x80000000) | (mt[(i + 1) % 624] & 0x7FFFFFFF);
        u32 next = y >> 1;

        if (y & 1)
        {
            next ^= 0x9908B0DF;
        }

        mt[i] = next ^ mt[(i + 397) % 624];
    }

    index -= 624;
}

// Assume everything that uses MT Fast checks the call limit manually
// Avoids needing to have exceptions
MTFast::MTFast(u32 seed, u8 size) : size(size)
{
    mt[0] = seed;

    for (index = 1; index < size + 397; index++)
    {
        mt[index] = 0x6C078965 * (mt[index - 1] ^ (mt[index - 1] >> 30)) + index;
    }
}

u32 MTFast::next()
{
    if (index >= size + 397)
    {
        shuffle();
    }

    u32 y = mt[index++];
    y ^= (y >> 11);
    y ^= (y << 7) & 0x9D2C5680;
    y ^= (y << 15) & 0xEFC60000;

    return y;
}

u16 MTFast::nextUShort()
{
    return next() >> 16;
}

void MTFast::advanceFrames(u32 frames)
{
    index += frames;
    while (index >= size + 397)
    {
        shuffle();
    }
}

void MTFast::shuffle()
{
    for (u16 i = 0; i < size; i++)
    {
        u32 y = (mt[i] & 0x80000000) | (mt[i + 1] & 0x7FFFFFFF);
        u32 next = y >> 1;

        if (y & 1)
        {
            next ^= 0x9908B0DF;
        }

        mt[i] = next ^ mt[i + 397];
    }

    index -= size + 397;
}
