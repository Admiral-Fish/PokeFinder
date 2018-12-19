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

#ifndef TINYMT_HPP
#define TINYMT_HPP

#include <Core/RNG/IRNG.hpp>

class TinyMT : public IRNG
{

public:
    TinyMT(u32 seed, u32 frames = 0);
    TinyMT(const u32 st[], u32 frames = 0);
    void advanceFrames(u32 frames) override;
    void nextState();
    u32 nextUInt() override;
    u16 nextUShort() override;
    u32 temper();
    void setSeed(u32 seed) override;
    void setSeed(u32 seed, u32 frames) override;
    u32 getSeed() override;
    u32 *getState();

private:
    u32 state[4];
    u32 seed;

    void initialize(u32 seed);
    void periodCertification();

};

#endif // TINYMT_HPP
