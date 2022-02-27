/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2022 by Admiral_Fish, bumba, and EzPzStreamz
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

#ifndef RESEARCHERSTATE_HPP
#define RESEARCHERSTATE_HPP

#include <Core/Util/Global.hpp>

class ResearcherState
{
public:
    ResearcherState(bool rng64Bit, u32 advances) : custom {}, rng64Bit(rng64Bit), advances(advances)
    {
    }

    u64 getState() const
    {
        return state;
    }

    void setState(u64 seed)
    {
        state = seed;
    }

    u32 getAdvances() const
    {
        return advances;
    }

    u64 getCustom(u8 index) const
    {
        return custom[index];
    }

    void setCustom(u8 index, u64 val)
    {
        custom[index] = val;
    }

    u32 getHigh32() const
    {
        return state >> 32;
    }

    u32 getLow32() const
    {
        return state & 0xffffffff;
    }

    u32 getHigh16() const
    {
        return rng64Bit ? getHigh32() >> 16 : state >> 16;
    }

    u32 getLow16() const
    {
        return rng64Bit ? getHigh32() & 0xFFFF : state & 0xFFFF;
    }

    u32 getMod25() const
    {
        return rng64Bit ? getHigh32() % 25 : getHigh16() % 25;
    }

    u32 getMod100() const
    {
        return rng64Bit ? getHigh32() % 100 : getHigh16() % 100;
    }

    u32 getMod3() const
    {
        return rng64Bit ? getHigh32() % 3 : getHigh16() % 3;
    }

    u32 getDiv656() const
    {
        return getHigh16() / 656;
    }

    u32 getHighBit() const
    {
        return rng64Bit ? getHigh32() >> 31 : getHigh16() >> 15;
    }

    u32 getLowBit() const
    {
        return rng64Bit ? getHigh32() & 1 : getHigh16() & 1;
    }

private:
    u64 custom[10];
    bool rng64Bit;
    u64 state;
    u32 advances;
};

#endif // RESEARCHERSTATE_HPP
