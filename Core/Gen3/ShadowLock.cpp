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

#include "ShadowLock.hpp"

/* Each non-shadow before a shadow has to match
 * a specific gender/nature and these preset
 * values directly impact what spreads are available */

ShadowLock::ShadowLock(u8 num, Method version)
{
    switchLock(num, version);
}

ShadowType ShadowLock::getType()
{
    return team.getType();
}

bool ShadowLock::firstShadowNormal(u32 seed)
{
    XDRNGR backward(seed);
    backward.advanceFrames(1);

    // Grab PID from first non-shadow going backwards
    // If it doesn't match spread fails
    u32 pidOriginal = getPIDBackward(backward);
    if (!team.getLock(0).compare(pidOriginal))
    {
        return false;
    }

    u32 pid = 0;
    for (x = 1; x < backCount; x++)
    {
        backward.advanceFrames(5);
        compareBackwards(pid, backward);
    }

    XDRNG forward(backward.getSeed());
    forward.advanceFrames(1);

    for (x = frontCount; x >= 0; x--)
    {
        forward.advanceFrames(5);
        compareForwards(pid, forward);
    }

    // Check if we end on the same PID as first non-shadow going backwards
    return pidOriginal == pid;
}

bool ShadowLock::firstShadowSet(u32 seed)
{
    XDRNGR backward(seed);
    backward.advanceFrames(6);

    // Grab PID from first non-shadow going backwards
    // If it doesn't match spread fails
    u32 pidOriginal = getPIDBackward(backward);
    if (!team.getLock(0).compare(pidOriginal))
    {
        return false;
    }

    u32 pid = 0;
    for (x = 1; x < backCount; x++)
    {
        backward.advanceFrames(5);
        compareBackwards(pid, backward);
    }

    XDRNG forward(backward.getSeed());
    forward.advanceFrames(1);

    for (x = frontCount; x >= 0; x--)
    {
        forward.advanceFrames(5);
        compareForwards(pid, forward);
    }

    // Check if we end on the same PID as first non-shadow going backwards
    return pidOriginal == pid;
}

bool ShadowLock::firstShadowShinySkip(u32 seed)
{
    XDRNGR backward(seed);
    backward.advanceFrames(1);

    // Check how many advances from shiny skip
    u16 psv = getPSVReverse(backward);
    u16 psvtemp = getPSVReverse(backward);
    while (psv == psvtemp)
    {
        psvtemp = psv;
        psv = getPSVReverse(backward);
    }

    // Grab PID from first non-shadow going backwards
    // If it doesn't match spread fails
    backward.advanceFrames(5);
    u32 pidOriginal = getPIDBackward(backward);
    if (!team.getLock(0).compare(pidOriginal))
    {
        return false;
    }

    u32 pid = 0;
    for (x = 1; x < backCount; x++)
    {
        backward.advanceFrames(5);
        compareBackwards(pid, backward);
    }

    XDRNG forward(backward.getSeed());
    forward.advanceFrames(1);

    for (x = frontCount; x >= 0; x--)
    {
        forward.advanceFrames(5);
        compareForwards(pid, forward);
    }

    // Check if we end on the same PID as first non-shadow going backwards
    return pidOriginal == pid;
}

bool ShadowLock::firstShadowUnset(u32 seed)
{
    XDRNGR backward(seed);
    backward.advanceFrames(8);

    // Grab PID from first non-shadow going backwards
    // If it doesn't match spread fails
    u32 pidOriginal = getPIDBackward(backward);
    if (!team.getLock(0).compare(pidOriginal))
    {
        return false;
    }

    u32 pid = 0;
    for (x = 1; x < backCount; x++)
    {
        backward.advanceFrames(5);
        compareBackwards(pid, backward);
    }

    XDRNG forward(backward.getSeed());
    forward.advanceFrames(1);

    for (x = frontCount; x >= 0; x--)
    {
        forward.advanceFrames(5);
        compareForwards(pid, forward);
    }

    // Check if we end on the same PID as first non-shadow going backwards
    return pidOriginal == pid;
}

bool ShadowLock::salamenceSet(u32 seed)
{
    XDRNGR backward(seed);
    backward.advanceFrames(6);

    // Build PID of non-shadow
    u32 pid = getPIDBackward(backward);

    return currLock.compare(pid);
}

bool ShadowLock::salamenceShinySkip(u32 seed)
{
    XDRNGR backward(seed);
    backward.advanceFrames(1);

    // Check how many advances from shiny skip
    u16 psv = getPSVReverse(backward);
    u16 psvtemp = getPSVReverse(backward);
    while (psv == psvtemp)
    {
        psvtemp = psv;
        psv = getPSVReverse(backward);
    }

    // Build PID of non-shadow
    backward.advanceFrames(5);
    u32 pid = getPIDBackward(backward);

    // Backwards nature lock check
    return currLock.compare(pid);
}

bool ShadowLock::salamenceUnset(u32 seed)
{
    XDRNGR backward(seed);
    backward.advanceFrames(8);

    // Build PID of non-shadow
    u32 pid = getPIDBackward(backward);

    // Backwards nature lock check
    return currLock.compare(pid);
}

bool ShadowLock::singleNL(u32 seed)
{
    XDRNGR backward(seed);
    backward.advanceFrames(1);

    // Build PID of non-shadow
    u32 pid = getPIDBackward(backward);

    // Backwards nature lock check
    return currLock.compare(pid);
}

// Needs more research
bool ShadowLock::eReader(u32 seed, u32 readerPID)
{
    // Check if PID is even valid for E-Reader
    // E-Reader have set nature/gender
    if (!team.getLock(0).compare(readerPID))
    {
        return false;
    }

    XDRNGR backward(seed);
    backward.advanceFrames(1);

    u32 pid;

    x = 1;
    compareBackwards(pid, backward);

    for (x = 2; x < backCount; x++)
    {
        backward.advanceFrames(3);
        compareBackwards(pid, backward);
    }

    XDRNG forward(backward.getSeed());
    forward.advanceFrames(1);

    for (x = frontCount; x >= 0; x--)
    {
        forward.advanceFrames(3);
        compareForwards(pid, forward);
    }

    // Checks if PID matches original
    return pid == readerPID;
}

void ShadowLock::switchLock(u8 lockNum, Method version)
{
    team = ShadowTeam::loadShadowTeams(version).at(lockNum);

    backCount = team.getSize();
    frontCount = backCount == 1 ? 0 : backCount - 2;
    x = 0;
    if (backCount == 1)
    {
        getCurrLock();
    }
}

void ShadowLock::compareBackwards(u32 &pid, XDRNGR &rng)
{
    getCurrLock();
    do
    {
        pid = getPIDBackward(rng);
    } while (!currLock.compare(pid));
}

void ShadowLock::compareForwards(u32 &pid, XDRNG &rng)
{
    getCurrLock();
    do
    {
        pid = getPIDForward(rng);
    } while (!currLock.compare(pid));
}

void ShadowLock::getCurrLock()
{
    currLock = team.getLock(x);
}

u32 ShadowLock::getPIDForward(XDRNG &rng)
{
    u32 high = rng.nextUInt() & 0xFFFF0000;
    u32 low = rng.nextUShort();
    return high | low;
}

u32 ShadowLock::getPIDBackward(XDRNGR &rng)
{
    u32 low = rng.nextUShort();
    u32 high = rng.nextUInt() & 0xFFFF0000;
    return low | high;
}

u16 ShadowLock::getPSVReverse(XDRNGR &rng)
{
    return (rng.nextUShort() ^ rng.nextUShort()) >> 3;
}
