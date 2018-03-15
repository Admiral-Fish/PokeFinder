/*
 * This file is part of PokéFinder
 * Copyright (C) 2017 by Admiral_Fish, bumba, and EzPzStreamz
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
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

#include <QObject>

typedef uint32_t u32;
typedef uint64_t u64;

class ResearcherFrame
{

public:
    quint64 custom[10];
    bool rng64Bit;
    u64 full64;
    u32 full32;
    u32 frame;

    ResearcherFrame(bool rng64Bit, u32 frame);
    inline u32 high32() { return (u32)(full64 >> 32); }
    inline u32 low32() { return (u32)(full64 & 0xFFFFFFFF); }
    inline u32 high16() { return rng64Bit ? high32() >> 16 : full32 >> 16; }
    inline u32 low16() { return rng64Bit ? high32() & 0xFFFF : full32 & 0xFFFF; }
    inline u32 mod25() { return rng64Bit ? high32() % 25 : high16() % 25; }
    inline u32 mod100() { return rng64Bit ? high32() % 100 : high16() % 100; }
    inline u32 mod3() { return rng64Bit ? high32() % 3 : high16() % 3; }
    inline u32 div656() { return high16() / 656; }
    inline u32 highBit() { return rng64Bit ? high32() >> 31 : high16() >> 15; }
    inline u32 lowBit() { return rng64Bit ? high32() & 1 : high16() & 1; }

};

#endif // RESEARCHERFRAME_HPP
