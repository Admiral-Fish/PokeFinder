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

#ifndef IDSTATE4_HPP
#define IDSTATE4_HPP

#include <Core/Parents/States/IDState.hpp>

class IDState4 : public IDState
{
public:
    IDState4(u32 seed, u16 tid, u16 sid) : IDState(tid, sid), seed(seed)
    {
        tsv = (tid ^ sid) >> 3;
    }

    u32 getSeed() const
    {
        return seed;
    }

    void setSeed(u32 seed)
    {
        this->seed = seed;
    }

    u32 getDelay() const
    {
        return delay;
    }

    void setDelay(u32 delay)
    {
        this->delay = delay;
    }

    u8 getSeconds() const
    {
        return seconds;
    }

    void setSeconds(u8 seconds)
    {
        this->seconds = seconds;
    }

private:
    u32 seed;
    u32 delay;
    u8 seconds;
};

#endif // IDSTATE4_HPP
