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
#include <functional>

template <typename IntegerType>
class RNGList
{
public:
    explicit RNGList(std::function<IntegerType()> function, u32 size = 5000) :
        function(function), list(new IntegerType[size]), size(size), head(0), tail(size - 1), pointer(0)
    {
        for (u32 i = 0; i < size; i++)
        {
            list[i] = function();
        }
    }

    RNGList(const RNGList &) = delete;

    void operator=(const RNGList &) = delete;

    ~RNGList()
    {
        delete[] list;
    }

    void advanceState()
    {
        if (++tail == size)
        {
            tail = 0;
        }

        list[head++] = function();
        if (head == size)
        {
            head = 0;
        }

        pointer = head;
    }

    void advanceFrames(u32 frames)
    {
        pointer += frames;
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
    std::function<IntegerType()> function;
    IntegerType *list;
    u32 size, head, tail, pointer;
};

#endif // RNGLIST_HPP
