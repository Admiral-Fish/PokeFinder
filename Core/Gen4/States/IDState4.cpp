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

#include "IDState4.hpp"

IDState4::IDState4(u32 seed, u16 tid, u16 sid) : IDState(tid, sid), seed(seed)
{
    tsv = (tid ^ sid) >> 3;
}

u32 IDState4::getSeed() const
{
    return seed;
}

void IDState4::setSeed(u32 seed)
{
    this->seed = seed;
}

u32 IDState4::getDelay() const
{
    return delay;
}

void IDState4::setDelay(u32 delay)
{
    this->delay = delay;
}

u8 IDState4::getSeconds() const
{
    return seconds;
}

void IDState4::setSeconds(u8 seconds)
{
    this->seconds = seconds;
}
