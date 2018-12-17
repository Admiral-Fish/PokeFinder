/*
 * This file is part of PokéFinder
 * Copyright (C) 2017 by Admiral_Fish, bumba, and EzPzStreamz
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

#ifndef SFMT_HPP
#define SFMT_HPP

#include <Core/RNG/IRNG64.hpp>

class SFMT : public IRNG64
{

public:
    SFMT(u32 seed, u32 frames = 0);
    void advanceFrames(u32 frames) override;
    u32 nextUInt() override;
    u64 nextULong() override;
    void setSeed(u64 seed) override;
    void setSeed(u64 seed, u32 frames) override;
    u64 getSeed() override;

private:
    static const u32 CMSK1 = 0xdfffffef;
    static const u32 CMSK2 = 0xddfecb7f;
    static const u32 CMSK3 = 0xbffaffff;
    static const u32  CMSK4 = 0xbffffff6;
    static const int CSL1 = 18;
    static const int CSR1 = 11;
    static const int N32 = 624;
    const u32 parity[4] = { 0x1, 0x0, 0x0, 0x13c9e684 };
    u32 sfmt[624];
    u32 seed;
    u32 index;

    void initialize(u32 seed);
    void periodCertificaion();
    void shuffle();

};

#endif // SFMT_HPP
