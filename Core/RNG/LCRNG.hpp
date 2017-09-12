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

#ifndef LCRNG_HPP
#define LCRNG_HPP
#include <cstdint>

enum rng_type 
{
    A_RNG,
    POKE_RNG,
    XD_RNG,
};

class LCRNG
{

private:
    const uint32_t A_ADD = 0x01;
    const uint32_t A_ADDR = 0x69c77f93;
    const uint32_t A_MULT = 0x6c078965;
    const uint32_t A_MULTR = 0x9638806d;
    const uint32_t POKEADD = 0x6073;
    const uint32_t POKEADDR = 0xa3561a1;
    const uint32_t POKEMULT = 0x41c64e6d;
    const uint32_t POKEMULTR = 0xeeb9eb65;
    const uint32_t XDADD = 0x269EC3;
    const uint32_t XDADDR = 0xA170F641;
    const uint32_t XDMULT = 0x343FD;
    const uint32_t XDMULTR = 0xB9B33155;
    uint32_t add;
    uint32_t addR;
    uint32_t mult;
    uint32_t multR;
    uint32_t seed;

    void setConst(uint32_t mult, uint32_t add, uint32_t multR, uint32_t addR);

public:

    LCRNG();

    LCRNG(uint32_t seed);

    LCRNG(uint32_t seed, rng_type type);

    void AdvanceFrames(int frames);

    uint32_t GetSeed();

    uint32_t Next16Bit();

    uint32_t Next32Bit();

    uint32_t Prev16Bit();

    uint32_t Prev32Bit();

    void ReverseFrames(int frames);

    void SetARNG();

    void SetPokeRNG();

    void SetSeed(uint32_t newSeed);

    void SetXDRNG();

};

class LCRNG64
{
    
private:
    const uint64_t ADD = 0x269ec3;
    const uint64_t ADDR = 0x9b1ae6e9a384e6f9;
    const uint64_t MULT = 0x5d588b656c078965;
    const uint64_t MULTR = 0xdedcedae9638806d;
    uint64_t seed;
    
public:

    LCRNG64();
    
    LCRNG64(uint64_t seed);

    void AdvanceFrames(int frames);
    
    uint64_t GetSeed();

    uint32_t Next32Bit();
    
    uint64_t Next64Bit();
    
    uint32_t Prev32Bit();

    uint64_t Prev64Bit();
    
    void ReverseFrames(int frames);
    
};

#endif //LCRNG_HPP
