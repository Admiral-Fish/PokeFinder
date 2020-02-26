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

#ifndef SHADOWLOCK_HPP
#define SHADOWLOCK_HPP

#include <Core/Gen3/LockInfo.hpp>
#include <Core/RNG/LCRNG.hpp>
#include <QVector>

class ShadowLock
{
public:
    ShadowLock() = default;
    ShadowLock(u8 num, Method version);
    ShadowType getType();
    bool firstShadowNormal(u32 seed);
    bool firstShadowSet(u32 seed);
    bool firstShadowShinySkip(u32 seed);
    bool firstShadowUnset(u32 seed);
    bool salamenceSet(u32 seed);
    bool salamenceShinySkip(u32 seed);
    bool salamenceUnset(u32 seed);
    bool singleNL(u32 seed);
    bool eReader(u32 seed, u32 readerPID);
    void switchLock(u8 lockNum, Method version);

private:
    int backCount, frontCount;
    LockInfo currLock;
    ShadowTeam team;
    int x;

    void compareBackwards(u32 &pid, XDRNGR &rng);
    void compareForwards(u32 &pid, XDRNG &rng);
    void getCurrLock();
    u32 getPIDForward(XDRNG &rng);
    u32 getPIDBackward(XDRNGR &rng);
    u16 getPSVReverse(XDRNGR &rng);
};

#endif // SHADOWLOCK_HPP
