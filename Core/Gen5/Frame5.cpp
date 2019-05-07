/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2019 by Admiral_Fish, bumba, and EzPzStreamz
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

#include "Frame5.hpp"

Frame5::Frame5()
{
    tid = 12345;
    sid = 54321;
    psv = 12345 ^ 54321;
}

Frame5::Frame5(u16 tid, u16 sid, u16 psv)
{
    this->tid = tid;
    this->sid = sid;
    this->psv = psv;
}

void Frame5::setInheritance(u16 iv1, u16 iv2, u16 par1, u16 par2, u16 par3, u16 inh1, u16 inh2, u16 inh3, const QVector<u8> &parent1, const QVector<u8> &parent2, bool broken)
{
    // TODO
}

QString Frame5::chatotPitch() const
{
    // TODO
    return QString();
}

u64 Frame5::getInitialSeed() const
{
    return initialSeed;
}

void Frame5::setInitialSeed(const u64 &value)
{
    initialSeed = value;
}

u64 Frame5::getSeed() const
{
    return seed;
}

void Frame5::setSeed(const u64 &value)
{
    seed = value;
}
