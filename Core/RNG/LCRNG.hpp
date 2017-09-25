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

class LCRNG
{

private:
    uint32_t add;
    uint32_t addr;
    uint32_t mult;
    uint32_t multr;

public:
    uint32_t seed;

    LCRNG(uint32_t add, uint32_t addr, uint32_t mult, uint32_t multr, uint32_t seed);

    void AdvanceFrames(int frames);

    uint32_t Next16Bit();

    uint32_t Next32Bit();

    uint32_t Prev16Bit();

    uint32_t Prev32Bit();

    void ReverseFrames(int frames);

};

class ARNG : public LCRNG
{

public:

    ARNG(uint32_t seed);

};

class PokeRNG : public LCRNG
{

public:

    PokeRNG(uint32_t seed);

};

class XDRNG : public LCRNG
{

public:

    XDRNG(uint32_t seed);

};

#endif //LCRNG_HPP
