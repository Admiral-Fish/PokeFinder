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

#include "LCRNG.hpp"

// LCRNG is used for Gen 3 and 4

// Default constructor for LCRNG
LCRNG::LCRNG(uint32_t add, uint32_t addr, uint32_t mult, uint32_t multr, uint32_t seed)
{
    this->add = add;
    this->addr = addr;
    this->mult = mult;
    this->multr = multr;
    this->seed = seed;
}

// Method for advancing seed by a given number of frames
void LCRNG::AdvanceFrames(int frames)
{
    for(int i = 0; i < frames; i++)
        seed = seed * mult + add;
}

// Method for finding next 16 bit seed
uint32_t LCRNG::Next16Bit()
{
    return (Next32Bit() >> 16);
}

// Method for finding next 32 bit seed
uint32_t LCRNG::Next32Bit()
{
    seed = seed * mult + add;
    return seed;
}

// Method for finding previous 16 bit seed
uint32_t LCRNG::Prev16Bit()
{
    return (Prev32Bit() >> 16);
}

// Method for finding previous 32 bit seed
uint32_t LCRNG::Prev32Bit()
{
    seed = seed * multr + addr;
    return seed;
}

// Method for reversing seed by a given number of frames
void LCRNG::ReverseFrames(int frames)
{
    for(int i = 0; i < frames; i++)
        seed = seed * multr + addr;
}


// Sub classes

ARNG::ARNG(uint32_t seed)
    : LCRNG(0x01, 0x69c77f93, 0x6c078965, 0x9638806d, seed)
{
}

PokeRNG::PokeRNG(uint32_t seed)
    : LCRNG(0x6073, 0xa3561a1, 0x41c64e6d, 0xeeb9eb65, seed)
{
}

XDRNG::XDRNG(uint32_t seed)
    : LCRNG(0x269EC3, 0xA170F641, 0x343FD, 0xB9B33155, seed)
{
}
