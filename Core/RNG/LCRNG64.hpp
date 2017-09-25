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

#ifndef LCRNG64_HPP
#define LCRNG64_HPP
#include <cstdint>

class LCRNG64
{

private:
    uint64_t add;
    uint64_t addr;
    uint64_t mult;
    uint64_t multr;

public:
    uint64_t seed;

    LCRNG64(uint64_t add, uint64_t addr, uint64_t mult, uint64_t multr, uint64_t seed);

    void AdvanceFrames(int frames);

    uint32_t Next32Bit();

    uint32_t Next32Bit(uint32_t max);

    uint64_t Next64Bit();

    uint32_t Prev32Bit();

    uint32_t Prev32Bit(uint32_t max);

    uint64_t Prev64Bit();

    void ReverseFrames(int frames);

};

class BWRNG : public LCRNG64
{

public:

    BWRNG(uint64_t seed);

};

#endif // LCRNG64_HPP
