/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2021 by Admiral_Fish, bumba, and EzPzStreamz
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

#include "LiveIDState3.hpp"

LiveIDState3::LiveIDState3(u32 advance, u16 tid, u16 sid, u16 initial, std::string shiny, std::string date) : IDState(advance, tid, sid)
{
    this->initial = initial;
    this->shiny = shiny;
    this->date = date;
    tsv = (tid ^ sid) >> 3;
}

u16 LiveIDState3::getInitial() const
{
    return initial;
}

std::string LiveIDState3::getShiny() const
{
    return shiny;
}

std::string LiveIDState3::getDate() const
{
    return date;
}
