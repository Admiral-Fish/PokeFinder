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

#include "IDFrame5.hpp"

IDFrame5::IDFrame5(u32 frame, u16 tid, u16 sid) : IDFrame(frame, tid, sid)
{
    tsv = (tid ^ sid) >> 3;
}

void IDFrame5::setDateTime(const QDateTime &dt)
{
    this->dt = dt;
}

QDateTime IDFrame5::getDateTime() const
{
    return dt;
}

void IDFrame5::setInitialFrame(u32 initialFrame)
{
    this->initialFrame = initialFrame;
}

u32 IDFrame5::getInitialFrame() const
{
    return initialFrame;
}

void IDFrame5::setKeypress(u16 keypress)
{
    this->keypress = keypress;
}

u16 IDFrame5::getKeypress() const
{
    return keypress;
}

void IDFrame5::setSeed(u64 seed)
{
    this->seed = seed;
}

u64 IDFrame5::getSeed() const
{
    return seed;
}
