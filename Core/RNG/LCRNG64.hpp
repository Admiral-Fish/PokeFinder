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

#include <Core/RNG/IRNG.hpp>

namespace PokeFinderCore
{
    template<u64 add, u64 mult>
    class LCRNG64 : public IRNG<u64>
    {
    public:
        LCRNG64(u64 seed = 0, u32 frames = 0)
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

        u32 nextUInt(u32 max, u32 frames = 0)
        {
            return ((nextULong(frames) >> 32) * max) >> 32;
        }

        u64 nextULong(u32 frames = 0)
        {
            advanceFrames(frames + 1);
            return seed;
        }

        u32 nextUInt(u32 frames = 0)
        {
            return nextULong(frames) >> 32;
        }

        u64 next(u32 frames = 0) override
        {
            return nextULong(frames);
        }

        void setSeed(u64 seed, u32 frames = 0) override
        {
            this->seed = seed;
            advanceFrames(frames);
        }

        u64 getSeed() const
        {
            return seed;
        }

    private:
        u64 seed{};
    };

    class BWRNG : public LCRNG64<0x269ec3, 0x5d588b656c078965>
    {
    public:
        BWRNG(u64 seed = 0, u32 frames = 0) : LCRNG64(seed, frames)
        {
        }
    };

    class BWRNGR : public LCRNG64<0x9b1ae6e9a384e6f9, 0xdedcedae9638806d>
    {
    public:
        BWRNGR(u64 seed = 0, u32 frames = 0) : LCRNG64(seed, frames)
        {
        }
    };
}

#endif // LCRNG64_HPP
