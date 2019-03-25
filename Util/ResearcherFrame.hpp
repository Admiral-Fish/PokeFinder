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

#ifndef RESEARCHERFRAME_HPP
#define RESEARCHERFRAME_HPP

#include <Core/Util/Global.hpp>

class ResearcherFrame
{

private:
    u64 custom[10];
    bool rng64Bit;
    u64 full64;
    u32 full32;
    u32 frame;

public:
    ResearcherFrame();
    ResearcherFrame(bool rng64Bit, u32 frame);
    u32 getFull32() const { return full32; }
    void setFull32(u32 seed) { full32 = seed; }
    u64 getFull64() const { return full64; }
    void setFull64(u64 seed) { full64 = seed; }
    u32 getFrame() const { return frame; }
    void setFrame(u32 val) { frame = val; }
    u64 getCustom(int x) const { return custom[x]; }
    void setCustom(int x, u64 val) { custom[x] = val; }
    inline u32 getHigh32() const { return full64 >> 32; }
    inline u32 getLow32() const { return full64 & 0xFFFFFFFF; }
    inline u32 getHigh16() const { return rng64Bit ? getHigh32() >> 16 : full32 >> 16; }
    inline u32 getLow16() const { return rng64Bit ? getHigh32() & 0xFFFF : full32 & 0xFFFF; }
    inline u32 getMod25() const { return rng64Bit ? getHigh32() % 25 : getHigh16() % 25; }
    inline u32 getMod100() const { return rng64Bit ? getHigh32() % 100 : getHigh16() % 100; }
    inline u32 getMod3() const { return rng64Bit ? getHigh32() % 3 : getHigh16() % 3; }
    inline u32 getDiv656() const { return getHigh16() / 656; }
    inline u32 getHighBit() const { return rng64Bit ? getHigh32() >> 31 : getHigh16() >> 15; }
    inline u32 getLowBit() const { return rng64Bit ? getHigh32() & 1 : getHigh16() & 1; }

};

#endif // RESEARCHERFRAME_HPP
