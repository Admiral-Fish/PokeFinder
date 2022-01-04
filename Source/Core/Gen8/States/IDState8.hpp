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

#ifndef IDSTATE8_HPP
#define IDSTATE8_HPP

#include <Core/Parents/States/IDState.hpp>

class IDState8 : public IDState
{
public:
    IDState8() = default;

    IDState8(u32 advance, u16 tid, u16 sid, u32 g8tid) : IDState(advance, tid, sid), g8tid(g8tid)
    {
        tsv = (tid ^ sid) >> 4;
    }

    u32 getG8TID() const
    {
        return g8tid;
    }

private:
    u32 g8tid;
};

#endif // IDSTATE8_HPP
