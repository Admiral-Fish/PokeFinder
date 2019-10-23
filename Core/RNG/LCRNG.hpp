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

namespace PokeFinderCore
{
    template<u32 add, u32 mult>
    class LCRNG : public IRNG<u32>
    {
    public:
        LCRNG(u32 seed = 0, u32 frames = 0)
        {
            this->seed = seed;
            advanceFrames(frames);
        }

        void advanceFrames(u32 frames) override
        {
            for (u32 frame = 0; frame < frames; frame++)
            {
                seed = seed * mult + add;
            }
        }

        u16 nextUShort(u32 frames = 0)
        {
            return nextUInt(frames) >> 16;
        }

        u32 nextUInt(u32 frames = 0)
        {
            advanceFrames(frames + 1);
            return seed;
        }

        u32 next(u32 frames = 0) override
        {
            return nextUInt(frames);
        }

        void setSeed(u32 seed, u32 frames = 0) override
        {
            this->seed = seed;
            advanceFrames(frames);
        }

        u32 getSeed() const
        {
            return seed;
        }

    private:
        u32 seed{};
    };

    class ARNG : public LCRNG<0x01, 0x6c078965>
    {
    public:
        ARNG(u32 seed = 0, u32 frames = 0) : LCRNG(seed, frames)
        {
        }
    };

    class ARNGR : public LCRNG<0x69c77f93, 0x9638806d>
    {
    public:
        ARNGR(u32 seed = 0, u32 frames = 0) : LCRNG(seed, frames)
        {
        }
    };

    class PokeRNG : public LCRNG<0x6073, 0x41c64e6d>
    {
    public:
        PokeRNG(u32 seed = 0, u32 frames = 0) : LCRNG(seed, frames)
        {
        }
    };

    class PokeRNGR : public LCRNG<0xa3561a1, 0xeeb9eb65>
    {
    public:
        PokeRNGR(u32 seed = 0, u32 frames = 0) : LCRNG(seed, frames)
        {
        }
    };

    class XDRNG : public LCRNG<0x269EC3, 0x343FD>
    {
    public:
        XDRNG(u32 seed = 0, u32 frames = 0) : LCRNG(seed, frames)
        {
        }
    };

    class XDRNGR: public LCRNG<0xA170F641, 0xB9B33155>
    {
    public:
        XDRNGR(u32 seed = 0, u32 frames = 0) : LCRNG(seed, frames)
        {
        }
    };
}

#endif //LCRNG_HPP
