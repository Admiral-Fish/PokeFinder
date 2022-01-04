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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301,
 * USA.
 */

#ifndef RNGLIST_HPP
#define RNGLIST_HPP

#include <Core/Util/Global.hpp>

template <typename Integer, class RNG, u16 size, u8 shift>
class RNGList
{
public:
    explicit RNGList(RNG &rng) : rng(rng), head(0), pointer(0)
    {
        static_assert(size && ((size & (size - 1)) == 0), "Number is not a perfect multiple of two");

        for (Integer &x : list)
        {
            x = this->rng.next() >> shift;
        }
    }

    RNGList(const RNGList &) = delete;

    void operator=(const RNGList &) = delete;

    void advanceStates(u32 advances)
    {
        for (u32 i = 0; i < advances; i++)
        {
            advanceState();
        }
    }

    void advanceState()
    {
        list[head++] = rng.next() >> shift;
        head &= size - 1;

        pointer = head;
    }

    void advance(u32 advances)
    {
        pointer = (pointer + advances) & (size - 1);
    }

    Integer getValue()
    {
        Integer result = list[pointer++];
        pointer &= size - 1;
        return result;
    }

    void resetState()
    {
        pointer = head;
    }

private:
    RNG rng;
    Integer list[size];
    u16 head, pointer;
};

#endif // RNGLIST_HPP
