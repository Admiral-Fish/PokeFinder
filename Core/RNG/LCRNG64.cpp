/*
 * This file is part of PokéFinder
 * Copyright (C) 2017 by Admiral_Fish and bumba
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
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

// LCRNG64 constructor that sets a given seed
LCRNG64::LCRNG64(uint64_t add, uint64_t addr, uint64_t mult, uint64_t multr, uint64_t seed)
{
    this->add = add;
    this->addr = addr;
    this->mult = mult;
    this->multr = multr;
    this->seed = seed;
}

// Method for advancing seed by a given number of frames
void LCRNG64::AdvanceFrames(int frames)
{
    for (int i = 0; i < frames; i++)
        seed = seed * mult + add;
}

// Method for finding next 32 bit seed
uint32_t LCRNG64::Next32Bit()
{
    return (uint32_t) (Next64Bit() >> 32);
}

// Method for finding next modified 32 bit seed
uint32_t LCRNG64::Next32Bit(uint32_t max)
{
    return (uint32_t) (((Next64Bit() >> 32) * max) >> 32);
}

// Method for finding next 64 bit seed
uint64_t LCRNG64::Next64Bit()
{
    seed = seed * mult + add;
    return seed;
}

// Method for finding previous 32 bit seed
uint32_t LCRNG64::Prev32Bit()
{
    return (uint32_t) (Prev64Bit() >> 32);
}

// Method for finding previous modified 32 bit seed
uint32_t LCRNG64::Prev32Bit(uint32_t max)
{
    return (uint32_t) (((Prev64Bit() >> 32) * max) >> 32);
}

// Method for finding previous 64 bit seed
uint64_t LCRNG64::Prev64Bit()
{
    seed = seed * multr + addr;
    return seed;
}

// Method for reversing seed by a given number of frames
void LCRNG64::ReverseFrames(int frames)
{
    for (int i = 0; i < frames; i++)
        seed = seed * multr + addr;
}


// Sub classes

BWRNG::BWRNG(uint64_t seed)
    : LCRNG64(0x269ec3, 0x9b1ae6e9a384e6f9, 0x5d588b656c078965, 0xdedcedae9638806d, seed)
{
}
