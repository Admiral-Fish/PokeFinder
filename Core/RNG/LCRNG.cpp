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
LCRNG::LCRNG()
{
    seed = 0;
    SetPokeRNG();
}

// LCRNG constructor with seed
LCRNG::LCRNG(uint32_t seed)
{
    this->seed = seed;
    SetPokeRNG();
}

// LCRNG constructor with seed and RNG type
LCRNG::LCRNG(uint32_t seed, rng_type type): LCRNG(seed)
{
    if (type == POKE_RNG)
      SetPokeRNG();
    else if (type == XD_RNG)
        SetXDRNG();
    else
      SetARNG();
}

// Method for setting other constants
void LCRNG::setConst(uint32_t mult, uint32_t add, uint32_t multR, uint32_t addR)
{
    this->mult = mult;
    this->add = add;
    this->multR = multR;
    this->addR = addR;
}

// Method for advancing seed by a given number of frames
void LCRNG::AdvanceFrames(int frames)
{
    for(int i = 0; i < frames; i++)
        seed = seed * mult + add;
}

// Get method for seed
uint32_t LCRNG::GetSeed()
{
    return seed;
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
    seed = seed * multR + addR;
    return seed;
}

// Method for reversing seed by a given number of frames
void LCRNG::ReverseFrames(int frames)
{
    for(int i = 0; i < frames; i++)
        seed = seed * multR + addR;
}

// Method for setting aRNG constants
void LCRNG::SetARNG()
{
    setConst(A_MULT, A_ADD, A_MULTR, A_ADDR);
}

// Method for setting pokeRNG constants
void LCRNG::SetPokeRNG()
{
    setConst(POKEMULT, POKEADD, POKEMULTR, POKEADDR);
}

// Set method for seed
void LCRNG::SetSeed(uint32_t newSeed)
{
    seed = newSeed;
}

// Method for setting xdRNG constants
void LCRNG::SetXDRNG()
{
    setConst(XDMULT, XDADD, XDMULTR, XDADDR);
}


// LCRNG64 is used for Gen 5

// Default constructor for LCRNG64
LCRNG64::LCRNG64()
{
    seed = 0;
}

// LCRNG64 constructor that sets a given seed
LCRNG64::LCRNG64(uint64_t seed)
{
    this->seed = seed;
}

// Method for advancing seed by a given number of frames
void LCRNG64::AdvanceFrames(int frames)
{
    for (int i = 0; i < frames; i++)
        seed = seed * MULT + ADD;
}

// Get method for seed
uint64_t LCRNG64::GetSeed()
{
    return seed;
}

// Method for finding next 32 bit seed
uint32_t LCRNG64::Next32Bit()
{
    return (uint32_t) (Next64Bit() >> 32);
}

// Method for finding next 64 bit seed
uint64_t LCRNG64::Next64Bit()
{
    seed = seed * MULT + ADD;
    return seed;
}

// Method for finding previous 32 bit seed
uint32_t LCRNG64::Prev32Bit()
{
    return (uint32_t) (Prev64Bit() >> 32);
}

// Method for finding previous 64 bit seed
uint64_t LCRNG64::Prev64Bit()
{
    seed = seed * MULTR + ADDR;
    return seed;
}

// Method for reversing seed by a given number of frames
void LCRNG64::ReverseFrames(int frames)
{
    for (int i = 0; i < frames; i++)
        seed = seed * MULTR + ADDR;
}
