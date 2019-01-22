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

#ifndef LCRNG64_HPP
#define LCRNG64_HPP

#include <Core/RNG/IRNG64.hpp>

class LCRNG64 : public IRNG64
{

public:
    LCRNG64(u64 add, u64 mult, u64 seed, u32 frames = 0);
    void advanceFrames(u32 frames) override;
    u32 nextUInt(u32 max);
    u64 nextULong() override;
    u32 nextUInt() override;
    void setSeed(u64 seed) override;
    void setSeed(u64 seed, u32 frames) override;
    u64 getSeed() override;

protected:
    u64 add;
    u64 mult;
    u64 seed;

};

class BWRNG : public LCRNG64
{

public:
    BWRNG(u64 seed, u32 frames = 0) : LCRNG64(0x269ec3, 0x5d588b656c078965, seed, frames)
    {
    }

};

class BWRNGR : public LCRNG64
{

public:
    BWRNGR(u64 seed, u32 frames = 0) : LCRNG64(0x9b1ae6e9a384e6f9, 0xdedcedae9638806d, seed, frames)
    {
    }

};

#endif // LCRNG64_HPP
