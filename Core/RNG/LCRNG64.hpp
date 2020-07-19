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

#ifndef LCRNG64_HPP
#define LCRNG64_HPP

#include <Core/RNG/IRNG.hpp>

template <u64 add, u64 mult>
class LCRNG64 : public IRNG<u64>
{
public:
    LCRNG64(u64 seed = 0) : seed(seed)
    {
    }

    void advanceFrames(u32 frames) final
    {
        for (u32 frame = 0; frame < frames; frame++)
        {
            nextULong();
        }
    }

    u32 nextUInt(u32 max)
    {
        return ((nextULong() >> 32) * max) >> 32;
    }

    u64 nextULong()
    {
        return seed = seed * mult + add;
    }

    u32 nextUInt()
    {
        return nextULong() >> 32;
    }

    u64 next() final
    {
        return nextULong();
    }

    void setSeed(u64 seed, u32 frames = 0) final
    {
        this->seed = seed;
        advanceFrames(frames);
    }

    u64 getSeed() const
    {
        return seed;
    }

    u64 getSeed(u32 max) const
    {
        return ((seed >> 32) * max) >> 32;
    }

private:
    u64 seed;
};

using BWRNG = LCRNG64<0x269ec3, 0x5d588b656c078965>;
using BWRNGR = LCRNG64<0x9b1ae6e9a384e6f9, 0xdedcedae9638806d>;

#endif // LCRNG64_HPP
