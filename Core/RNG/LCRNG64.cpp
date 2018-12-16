/*
 * This file is part of PokéFinder
 * Copyright (C) 2017 by Admiral_Fish, bumba, and EzPzStreamz
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

#include "LCRNG64.hpp"

LCRNG64::LCRNG64(u64 add, u64 mult, u64 seed, u32 frames)
{
    this->add = add;
    this->mult = mult;
    this->seed = seed;
    advanceFrames(frames);
}

void LCRNG64::advanceFrames(u32 frames)
{
    for (u32 i = 0; i < frames; i++)
    {
        seed = seed * mult + add;
    }
}

u32 LCRNG64::nextUInt(u32 max)
{
    return static_cast<u32>(((nextULong() >> 32) * max) >> 32);
}

u64 LCRNG64::nextULong()
{
    seed = seed * mult + add;
    return seed;
}

u32 LCRNG64::nextUInt()
{
    return static_cast<u32>(nextULong() >> 32);
}

void LCRNG64::setSeed(u64 seed)
{
    this->seed = seed;
}

void LCRNG64::setSeed(u64 seed, u32 frames)
{
    this->seed = seed;
    advanceFrames(frames);
}

u64 LCRNG64::getSeed()
{
    return seed;
}
