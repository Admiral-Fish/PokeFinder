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

#include "IDState.hpp"

IDState::IDState(u32 advances, u16 tid, u16 sid) : advances(advances), tid(tid), sid(sid)
{
}

IDState::IDState(u16 tid, u16 sid) : tid(tid), sid(sid)
{
}

u32 IDState::getAdvances() const
{
    return advances;
}

u16 IDState::getTID() const
{
    return tid;
}

u16 IDState::getSID() const
{
    return sid;
}

u16 IDState::getTSV() const
{
    return tsv;
}
