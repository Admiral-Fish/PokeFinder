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

#ifndef IDSTATE_HPP
#define IDSTATE_HPP

#include <Core/Util/Global.hpp>

class IDState
{
public:
    IDState(u32 advances, u16 tid, u16 sid) : advances(advances), tid(tid), sid(sid)
    {
    }

    IDState(u16 tid, u16 sid) : tid(tid), sid(sid)
    {
    }

    u32 getAdvances() const
    {
        return advances;
    }

    u16 getTID() const
    {
        return tid;
    }

    u16 getSID() const
    {
        return sid;
    }

    u16 getTSV() const
    {
        return tsv;
    }

protected:
    u32 advances;
    u16 tid;
    u16 sid;
    u16 tsv;
};

#endif // IDSTATE_HPP
