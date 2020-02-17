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

#ifndef RESEARCHERFRAME_HPP
#define RESEARCHERFRAME_HPP

#include <Core/Util/Global.hpp>

class ResearcherFrame
{
public:
    ResearcherFrame() = default;
    ResearcherFrame(bool rng64Bit, u32 frame);
    u32 getFull32() const;
    void setFull32(u32 seed);
    u64 getFull64() const;
    void setFull64(u64 seed);
    u32 getFrame() const;
    u64 getCustom(u8 x) const;
    void setCustom(u8 x, u64 val);
    u32 getHigh32() const;
    u32 getLow32() const;
    u32 getHigh16() const;
    u32 getLow16() const;
    u32 getMod25() const;
    u32 getMod100() const;
    u32 getMod3() const;
    u32 getDiv656() const;
    u32 getHighBit() const;
    u32 getLowBit() const;

private:
    u64 custom[10];
    bool rng64Bit;
    u64 full64;
    u32 full32;
    u32 frame;
};

#endif // RESEARCHERFRAME_HPP
