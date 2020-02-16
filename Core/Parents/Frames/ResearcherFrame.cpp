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

#include "ResearcherFrame.hpp"

ResearcherFrame::ResearcherFrame(bool rng64Bit, u32 frame) : custom {}, rng64Bit(rng64Bit), frame(frame)
{
}

u32 ResearcherFrame::getFull32() const
{
    return full32;
}

void ResearcherFrame::setFull32(u32 seed)
{
    full32 = seed;
}

u64 ResearcherFrame::getFull64() const
{
    return full64;
}

void ResearcherFrame::setFull64(u64 seed)
{
    full64 = seed;
}

u32 ResearcherFrame::getFrame() const
{
    return frame;
}

u64 ResearcherFrame::getCustom(u8 x) const
{
    return custom[x];
}

void ResearcherFrame::setCustom(u8 x, u64 val)
{
    custom[x] = val;
}

u32 ResearcherFrame::getHigh32() const
{
    return full64 >> 32;
}

u32 ResearcherFrame::getLow32() const
{
    return full64 & 0xffffffff;
}

u32 ResearcherFrame::getHigh16() const
{
    return rng64Bit ? getHigh32() >> 16 : full32 >> 16;
}

u32 ResearcherFrame::getLow16() const
{
    return rng64Bit ? getHigh32() & 0xFFFF : full32 & 0xFFFF;
}

u32 ResearcherFrame::getMod25() const
{
    return rng64Bit ? getHigh32() % 25 : getHigh16() % 25;
}

u32 ResearcherFrame::getMod100() const
{
    return rng64Bit ? getHigh32() % 100 : getHigh16() % 100;
}

u32 ResearcherFrame::getMod3() const
{
    return rng64Bit ? getHigh32() % 3 : getHigh16() % 3;
}

u32 ResearcherFrame::getDiv656() const
{
    return getHigh16() / 656;
}

u32 ResearcherFrame::getHighBit() const
{
    return rng64Bit ? getHigh32() >> 31 : getHigh16() >> 15;
}

u32 ResearcherFrame::getLowBit() const
{
    return rng64Bit ? getHigh32() & 1 : getHigh16() & 1;
}
