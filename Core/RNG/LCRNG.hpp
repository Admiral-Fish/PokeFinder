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

#ifndef LCRNG_HPP
#define LCRNG_HPP

#include <Core/RNG/IRNG.hpp>

class LCRNG : public IRNG
{

public:
    LCRNG() = default;
    LCRNG(u32 add, u32 mult, u32 seed, u32 frames = 0);
    void advanceFrames(u32 frames) override;
    u16 nextUShort() override;
    u32 nextUInt() override;
    void setSeed(u32 seed) override;
    void setSeed(u32 seed, u32 frames) override;
    u32 getSeed() override;

protected:
    u32 add{};
    u32 mult{};
    u32 seed{};

};

class ARNG : public LCRNG
{

public:
    ARNG() : LCRNG(0x01, 0x6c078965, 0, 0)
    {
    }

    ARNG(u32 seed, u32 frames = 0) : LCRNG(0x01, 0x6c078965, seed, frames)
    {
    }

};

class ARNGR : public LCRNG
{

public:
    ARNGR() : LCRNG(0x69c77f93, 0x9638806d, 0, 0)
    {
    }

    ARNGR(u32 seed, u32 frames = 0) : LCRNG(0x69c77f93, 0x9638806d, seed, frames)
    {
    }

};

class PokeRNG : public LCRNG
{

public:
    PokeRNG() : LCRNG(0x6073, 0x41c64e6d, 0, 0)
    {
    }

    PokeRNG(u32 seed, u32 frames = 0) : LCRNG(0x6073, 0x41c64e6d, seed, frames)
    {
    }

};

class PokeRNGR : public LCRNG
{

public:
    PokeRNGR() : LCRNG(0xa3561a1, 0xeeb9eb65, 0, 0)
    {
    }

    PokeRNGR(u32 seed, u32 frames = 0) : LCRNG(0xa3561a1, 0xeeb9eb65, seed, frames)
    {
    }

};

class XDRNG : public LCRNG
{

public:
    XDRNG() : LCRNG(0x269EC3, 0x343FD, 0, 0)
    {
    }

    XDRNG(u32 seed, u32 frames = 0) : LCRNG(0x269EC3, 0x343FD, seed, frames)
    {
    }

};

class XDRNGR: public LCRNG
{

public:
    XDRNGR() : LCRNG(0xA170F641, 0xB9B33155, 0, 0)
    {
    }

    XDRNGR(u32 seed, u32 frames = 0) : LCRNG(0xA170F641, 0xB9B33155, seed, frames)
    {
    }

};

#endif //LCRNG_HPP
