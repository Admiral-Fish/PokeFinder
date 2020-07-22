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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301,
 * USA.
 */

#ifndef RNGLIST_HPP
#define RNGLIST_HPP

#include <Core/Util/Global.hpp>

template <typename IntegerType, typename RNGType, u32 size, u8 shift>
class RNGList
{
public:
    explicit RNGList(RNGType &rng) : rng(rng), head(0), tail(size - 1), pointer(0)
    {
        for (u32 i = 0; i < size; i++)
        {
            list[i] = this->rng.next() >> shift;
        }
    }

    RNGList(const RNGList &) = delete;

    void operator=(const RNGList &) = delete;

    void advanceStates(u32 advances)
    {
        for (u32 advance = 0; advance < advances; advance++)
        {
            advanceState();
        }
    }

    void advanceState()
    {
        if (++tail == size)
        {
            tail = 0;
        }

        list[head++] = rng.next() >> shift;
        if (head == size)
        {
            head = 0;
        }

        pointer = head;
    }

    void advance(u32 advances)
    {
        pointer += advances;
        if (pointer >= size)
        {
            pointer -= size;
        }
    }

    IntegerType getValue()
    {
        if (pointer == size)
        {
            pointer = 0;
        }

        return list[pointer++];
    }

    void resetState()
    {
        pointer = head;
    }

private:
    RNGType rng;
    IntegerType list[size];
    u32 head, tail, pointer;
};

#endif // RNGLIST_HPP
