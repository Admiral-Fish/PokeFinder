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

#include "ShadowLock.hpp"

/* Each non-shadow before a shadow has to match
 * a specific gender/nature and these preset
 * values directly impact what spreads are available */

LockInfo::LockInfo(u8 nature, u8 genderLower, u8 genderUpper, bool free)
{
    this->nature = nature;
    this->genderLower = genderLower;
    this->genderUpper = genderUpper;
    this->free = free;
}

bool LockInfo::compare(u32 pid) const
{
    if (free)
    {
        return true;
    }

    u8 gender = pid & 255;
    return gender >= genderLower && gender <= genderUpper && nature == (pid % 25);
}


ShadowLock::ShadowLock(u8 num, Method version)
{
    switchLock(num, version);
    backward = XDRNGR(0);
    forward = XDRNG(0);
}

ShadowType ShadowLock::getType()
{
    return type;
}

// Checks if seed is valid for single shadow case
bool ShadowLock::firstShadowNormal(u32 seed)
{
    backward.setSeed(seed, 1);

    // Grab PID from first non-shadow going backwards
    // If it doesn't match spread fails
    pidOriginal = getPIDReverse();
    if (!lockInfo.at(0).compare(pidOriginal))
    {
        return false;
    }

    // Backwards nature lock check loop
    for (x = 1; x < backCount; x++)
    {
        backward.advanceFrames(5);
        pid = getPIDReverse();
        getCurrLock();
        if (!currLock.compare(pid))
        {
            countBackTwo();
        }
    }

    forward.setSeed(backward.getSeed(), 1);
    // Forwards nature lock check loop
    for (x = frontCount; x >= 0; x--)
    {
        forward.advanceFrames(5);
        pid = getPIDForward();
        getCurrLock();
        if (!currLock.compare(pid))
        {
            countForwardTwo();
        }
    }

    // Check if we end on the same PID as first non-shadow going backwards
    return pidOriginal == pid;
}

// Checks if seed is valid for second shadow with first shadow set
bool ShadowLock::firstShadowSet(u32 seed)
{
    backward.setSeed(seed, 6);

    // Grab PID from first non-shadow going backwards
    // If it doesn't match spread fails
    pidOriginal = getPIDReverse();
    if (!lockInfo.at(0).compare(pidOriginal))
    {
        return false;
    }

    // Backwards nature lock check loop
    for (x = 1; x < backCount; x++)
    {
        backward.advanceFrames(5);
        pid = getPIDReverse();
        getCurrLock();
        if (!currLock.compare(pid))
        {
            countBackTwo();
        }
    }

    forward.setSeed(backward.getSeed(), 1);

    // Forwards nature lock check
    for (x = frontCount; x >= 0; x--)
    {
        forward.advanceFrames(5);
        pid = getPIDForward();
        getCurrLock();
        if (!currLock.compare(pid))
        {
            countForwardTwo();
        }
    }

    // Check if we end on the same PID as first non-shadow going backwards
    return pidOriginal == pid;
}

// Checks if seed is valid for second shadow with first shadow unset and antishiny(aka Shiny Skip)
bool ShadowLock::firstShadowShinySkip(u32 seed)
{
    backward.setSeed(seed, 1);

    u16 psv, psvtemp;

    // Check how many advances from shiny skip
    psv = getPSVReverse();
    psvtemp = getPSVReverse();
    while (psv == psvtemp)
    {
        psvtemp = psv;
        psv = getPSVReverse();
    }

    // Grab PID from first non-shadow going backwards
    // If it doesn't match spread fails
    backward.advanceFrames(5);
    pidOriginal = getPIDReverse();
    if (!lockInfo.at(0).compare(pidOriginal))
    {
        return false;
    }

    // Backwards nature lock check loop
    for (x = 1; x < backCount; x++)
    {
        backward.advanceFrames(5);
        pid = getPIDReverse();
        getCurrLock();
        if (!currLock.compare(pid))
        {
            countBackTwo();
        }
    }

    forward.setSeed(backward.getSeed(), 1);

    // Forwards nature lock check loop
    for (x = frontCount; x >= 0; x--)
    {
        forward.advanceFrames(5);
        pid = getPIDForward();
        getCurrLock();
        if (!currLock.compare(pid))
        {
            countForwardTwo();
        }
    }

    // Check if we end on the same PID as first non-shadow going backwards
    return pidOriginal == pid;
}

// Checks if seed is valid for second shadow with first shadow unset
bool ShadowLock::firstShadowUnset(u32 seed)
{
    backward.setSeed(seed, 8);

    // Grab PID from first non-shadow going backwards
    // If it doesn't match spread fails
    pidOriginal = getPIDReverse();
    if (!lockInfo.at(0).compare(pidOriginal))
    {
        return false;
    }

    // Backwards nature lock check loop
    for (x = 1; x < backCount; x++)
    {
        backward.advanceFrames(5);
        pid = getPIDReverse();
        getCurrLock();
        if (!currLock.compare(pid))
        {
            countBackTwo();
        }
    }

    forward.setSeed(backward.getSeed(), 1);

    // Forwards nature lock check loop
    for (x = frontCount; x >= 0; x--)
    {
        forward.advanceFrames(5);
        pid = getPIDForward();
        getCurrLock();
        if (!currLock.compare(pid))
        {
            countForwardTwo();
        }
    }

    // Check if we end on the same PID as first non-shadow going backwards
    return pidOriginal == pid;
}

// Checks if seed is valid for 1st shadow set for Salamence
bool ShadowLock::salamenceSet(u32 seed)
{
    backward.setSeed(seed, 6);

    // Build PID of non-shadow
    pid = getPIDReverse();

    // Backwards nature lock check
    return currLock.compare(pid);
}

// Checks if seed is valid for 1st shadow unset and antishiny(aka Shiny Skip) for Salamence
bool ShadowLock::salamenceShinySkip(u32 seed)
{
    backward.setSeed(seed, 1);

    u16 psv, psvtemp;

    // Check how many advances from shiny skip
    psv = getPSVReverse();
    psvtemp = getPSVReverse();
    while (psv == psvtemp)
    {
        psvtemp = psv;
        psv = getPSVReverse();
    }

    // Build PID of non-shadow
    backward.advanceFrames(5);
    pid = getPIDReverse();

    // Backwards nature lock check
    return currLock.compare(pid);
}

// Checks if seed is valid for 1st shadow unset for Salamence
bool ShadowLock::salamenceUnset(u32 seed)
{
    backward.setSeed(seed, 8);

    // Build PID of non-shadow
    pid = getPIDReverse();

    // Backwards nature lock check
    return currLock.compare(pid);
}

// Checks if seed is valid for single nature lock
bool ShadowLock::singleNL(u32 seed)
{
    backward.setSeed(seed, 1);

    // Build PID of non-shadow
    pid = getPIDReverse();

    // Backwards nature lock check
    return currLock.compare(pid);
}

// Needs more research
bool ShadowLock::eReader(u32 seed, u32 readerPID)
{
    // Check if PID is even valid for E-Reader
    if (!lockInfo.at(0).compare(readerPID))
    {
        return false;
    }

    backward.setSeed(seed, 1);

    // Get first non-shadow PID
    x = 1;
    getCurrLock();
    getPIDReverse();
    if (!currLock.compare(pid))
    {
        countBackTwo();
    }

    // Backwards nature lock check loop
    for (x = 2; x < backCount; x++)
    {
        backward.advanceFrames(3);
        pid = getPIDReverse();
        getCurrLock();
        if (!currLock.compare(pid))
        {
            countBackTwo();
        }
    }

    forward.setSeed(backward.getSeed(), 1);

    // Forwards nature lock check loop
    for (x = frontCount; x >= 0; x--)
    {
        forward.advanceFrames(3);
        pid = getPIDForward();
        getCurrLock();
        if (!currLock.compare(pid))
        {
            countForwardTwo();
        }
    }

    // Checks if first NL PID back from target matches
    return pid == readerPID;
}

void ShadowLock::switchLock(u8 lockNum, Method version)
{
    natureLockSetup(lockNum, version);

    backCount = lockInfo.size();
    frontCount = backCount == 1 ? 0 : backCount - 2;
    x = 0;
    if (backCount == 1)
    {
        getCurrLock();
    }
}

void ShadowLock::countBackTwo()
{
    do
    {
        pid = getPIDReverse();
    }
    while (!currLock.compare(pid));
}

void ShadowLock::countForwardTwo()
{
    do
    {
        pid = getPIDForward();
    }
    while (!currLock.compare(pid));
}

void ShadowLock::getCurrLock()
{
    currLock = lockInfo.at(x);
}

u32 ShadowLock::getPIDForward()
{
    u32 high = forward.nextUInt() & 0xFFFF0000;
    u32 low = forward.nextUShort();
    return high | low;
}

u32 ShadowLock::getPIDReverse()
{
    u32 low = backward.nextUShort();
    u32 high = backward.nextUInt() & 0xFFFF0000;
    return low | high;
}

u16 ShadowLock::getPSVReverse()
{
    return (backward.nextUShort() ^ backward.nextUShort()) >> 3;
}

void ShadowLock::natureLockSetup(u8 lockNum, Method version)
{
    QString path = version == Method::XD ? ":/encounters/gales.bin" : ":/encounters/colo.bin";

    QByteArray data;
    QFile file(path);
    if (file.open(QIODevice::ReadOnly))
    {
        data = file.readAll();
        file.close();
    }

    lockInfo.clear();
    u16 offset = 0;
    u8 count = 0;
    while (offset < data.size())
    {
        if (count != lockNum)
        {
            offset += data[offset] * 4 + 2;
            count++;
        }
        else
        {
            type = static_cast<ShadowType>(data.at(offset + 1));
            u8 size = data.at(offset);
            for (u8 i = 0; i < size; i++)
            {
                u8 nature = data.at(offset + 2 + i * 4);
                u8 genderLower = data.at(offset + 3 + i * 4);
                u8 genderUpper = data.at(offset + 4 + i * 4);
                bool free = data.at(offset + 5 + i * 4);
                lockInfo.append(LockInfo(nature, genderLower, genderUpper, free));
            }
            break;
        }
    }
}
