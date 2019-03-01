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

#ifndef SHADOWLOCK_HPP
#define SHADOWLOCK_HPP

#include <QFile>
#include <QVector>
#include <Core/RNG/LCRNG.hpp>
#include <Core/Objects/Method.hpp>

// Defines the different types of shadows
enum ShadowType
{
    SingleLock,
    FirstShadow,
    Salamence,
    SecondShadow,
    EReader
};

class LockInfo
{

public:
    LockInfo() = default;
    LockInfo(u8 nature, u8 genderLower, u8 genderUpper);
    bool compare(u32 pid) const;

private:
    u8 genderUpper;
    u8 genderLower;
    u8 nature;
    bool free;

};

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
    QVector<LockInfo> lockInfo;
    LockInfo currLock;
    u32 pid, pidOriginal;
    XDRNG forward;
    XDRNGR backward;
    ShadowType type;
    int x;

    void countBackTwo();
    void countForwardTwo();
    void getCurrLock();
    u32 getPIDForward();
    u32 getPIDReverse();
    u16 getPSVReverse();
    void natureLockSetup(u8 lockNum, Method version);

};

#endif // SHADOWLOCK_HPP
