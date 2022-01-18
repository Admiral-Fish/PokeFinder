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

#ifndef WILDSTATE8_HPP
#define WILDSTATE8_HPP

#include <Core/Parents/States/WildState.hpp>

class WildState8 : public WildState
{
public:
    WildState8() = default;

    explicit WildState8(u32 advance) : WildState(advance)
    {
    }

    u8 getHook() const
    {
        return hook;
    }

    void setHook(u8 hook)
    {
        this->hook = hook;
    }

private:
    u8 hook;
};

#endif // WILDSTATE8_HPP
