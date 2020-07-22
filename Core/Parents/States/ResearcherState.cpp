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

#include "ResearcherState.hpp"

ResearcherState::ResearcherState(bool rng64Bit, u32 advance) : custom {}, rng64Bit(rng64Bit), advance(advance)
{
}

u64 ResearcherState::getState() const
{
    return state;
}

void ResearcherState::setState(u64 seed)
{
    state = seed;
}

u32 ResearcherState::getAdvance() const
{
    return advance;
}

u64 ResearcherState::getCustom(u8 x) const
{
    return custom[x];
}

void ResearcherState::setCustom(u8 x, u64 val)
{
    custom[x] = val;
}

u32 ResearcherState::getHigh32() const
{
    return state >> 32;
}

u32 ResearcherState::getLow32() const
{
    return state & 0xffffffff;
}

u32 ResearcherState::getHigh16() const
{
    return rng64Bit ? getHigh32() >> 16 : state >> 16;
}

u32 ResearcherState::getLow16() const
{
    return rng64Bit ? getHigh32() & 0xFFFF : state & 0xFFFF;
}

u32 ResearcherState::getMod25() const
{
    return rng64Bit ? getHigh32() % 25 : getHigh16() % 25;
}

u32 ResearcherState::getMod100() const
{
    return rng64Bit ? getHigh32() % 100 : getHigh16() % 100;
}

u32 ResearcherState::getMod3() const
{
    return rng64Bit ? getHigh32() % 3 : getHigh16() % 3;
}

u32 ResearcherState::getDiv656() const
{
    return getHigh16() / 656;
}

u32 ResearcherState::getHighBit() const
{
    return rng64Bit ? getHigh32() >> 31 : getHigh16() >> 15;
}

u32 ResearcherState::getLowBit() const
{
    return rng64Bit ? getHigh32() & 1 : getHigh16() & 1;
}
