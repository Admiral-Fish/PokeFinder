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

#ifndef MTRNG_HPP
#define MTRNG_HPP

#include <Core/RNG/IRNG.hpp>

class MT : public IRNG<u32>
{
public:
    void advanceFrames(u32 frames) override;
    void setSeed(u32 seed, u32 frames) override;
    u16 nextUShort();
    u32 next() override;
    virtual u32 nextUInt() = 0;

protected:
    u32 mt[624];
    u32 index;
    virtual void shuffle();
    virtual void initialize(u32 seed);
};

class MersenneTwister : public MT
{
public:
    MersenneTwister(u32 seed = 0);
    u32 nextUInt() override;
};

class MersenneTwisterUntempered : public MT
{
public:
    MersenneTwisterUntempered(u32 seed = 0);
    u32 nextUInt() override;
};

class MersenneTwisterFast : public MT
{
public:
    explicit MersenneTwisterFast(u32 calls, u32 seed = 0);
    u32 nextUInt() override;

private:
    u32 calls;

    void initialize(u32 seed) override;
    void shuffle() override;
};

#endif // MTRNG_HPP
