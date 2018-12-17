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

#include "NatureLock.hpp"

/* Each non-shadow before a shadow has to match
 * a specific gender/nature and these preset
 * values directly impact what spreads are available */

LockInfo::LockInfo(u8 nature, u16 genderLower, u16 genderUpper, bool free)
{
    this->nature = nature;
    this->genderLower = genderLower;
    this->genderUpper = genderUpper;
    this->free = free;
}

bool LockInfo::compare(u32 pid)
{
    if (free)
    {
        return true;
    }

    u16 gender = pid & 255;
    return gender >= genderLower && gender <= genderUpper && nature == (pid % 25);
}


NatureLock::NatureLock(int num, Method version)
{
    if (version == XD)
    {
        switchLockGales(num);
    }
    else
    {
        switchLockColo(num);
    }
}

ShadowType NatureLock::getType()
{
    return type;
}

// Checks if seed is valid for single shadow case
bool NatureLock::firstShadowNormal(u32 seed)
{
    backward.setSeed(seed, 1);

    // Grab PID from first non-shadow going backwards
    // If it doesn't match spread fails
    pidOriginal = getPIDReverse();
    if (!lockInfo[0].compare(pidOriginal))
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
bool NatureLock::firstShadowSet(u32 seed)
{
    backward.setSeed(seed, 6);

    // Grab PID from first non-shadow going backwards
    // If it doesn't match spread fails
    pidOriginal = getPIDReverse();
    if (!lockInfo[0].compare(pidOriginal))
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
bool NatureLock::firstShadowShinySkip(u32 seed)
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
    if (!lockInfo[0].compare(pidOriginal))
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
bool NatureLock::firstShadowUnset(u32 seed)
{
    backward.setSeed(seed, 8);

    // Grab PID from first non-shadow going backwards
    // If it doesn't match spread fails
    pidOriginal = getPIDReverse();
    if (!lockInfo[0].compare(pidOriginal))
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
bool NatureLock::salamenceSet(u32 seed)
{
    backward.setSeed(seed, 6);

    // Build PID of non-shadow
    pid = getPIDReverse();

    // Backwards nature lock check
    return currLock.compare(pid);
}

// Checks if seed is valid for 1st shadow unset and antishiny(aka Shiny Skip) for Salamence
bool NatureLock::salamenceShinySkip(u32 seed)
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
bool NatureLock::salamenceUnset(u32 seed)
{
    backward.setSeed(seed, 8);

    // Build PID of non-shadow
    pid = getPIDReverse();

    // Backwards nature lock check
    return currLock.compare(pid);
}

// Checks if seed is valid for single nature lock
bool NatureLock::singleNL(u32 seed)
{
    backward.setSeed(seed, 1);

    // Build PID of non-shadow
    pid = getPIDReverse();

    // Backwards nature lock check
    return currLock.compare(pid);
}

// Needs more research
bool NatureLock::eReader(u32 seed, u32 readerPID)
{
    // Check if PID is even valid for E-Reader
    if (!lockInfo[0].compare(readerPID))
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

void NatureLock::switchLockColo(int lockNum)
{
    natureLockSetupColo(lockNum);
    backCount = lockInfo.size();
    frontCount = backCount == 1 ? 0 : backCount - 2;
    x = 0;
    if (backCount == 1)
    {
        getCurrLock();
    }
}

void NatureLock::switchLockGales(int lockNum)
{
    natureLockSetupGales(lockNum);
    backCount = lockInfo.size();
    frontCount = backCount == 1 ? 0 : backCount - 2;
    x = 0;
    if (backCount == 1)
    {
        getCurrLock();
    }
}

void NatureLock::countBackTwo()
{
    do
    {
        pid = getPIDReverse();
    }
    while (!currLock.compare(pid));
}

void NatureLock::countForwardTwo()
{
    do
    {
        pid = getPIDForward();
    }
    while (!currLock.compare(pid));
}

void NatureLock::getCurrLock()
{
    currLock = lockInfo[x];
}

u32 NatureLock::getPIDForward()
{
    u32 high = forward.nextUInt() & 0xFFFF0000;
    u32 low = forward.nextUShort();
    return high | low;
}

u32 NatureLock::getPIDReverse()
{
    u32 low = backward.nextUShort();
    u32 high = backward.nextUInt() & 0xFFFF0000;
    return low | high;
}

u16 NatureLock::getPSVReverse()
{
    return (backward.nextUShort() ^ backward.nextUShort()) >> 3;
}

void NatureLock::natureLockSetupColo(int lockNum)
{
    switch (lockNum)
    {
        case 0: // Gligar
            lockInfo = { LockInfo(18, 127, 255), LockInfo(6, 0, 190), LockInfo(12, 127, 255) };
            type = ShadowType::FirstShadow;
            break;
        case 1: // Heracross
            lockInfo = { LockInfo(0, 0, 126), LockInfo(0, 127, 255) };
            type = ShadowType::FirstShadow;
            break;
        case 2: // Makuhita
            lockInfo = { LockInfo(0, 0, 126), LockInfo(24, 127, 255) };
            type = ShadowType::FirstShadow;
            break;
        case 3: // E-Reader Mareep
            lockInfo = { LockInfo(16, 0, 126), LockInfo(12, 0, 126), LockInfo(10, 0, 126), LockInfo(4, 0, 190) };
            type = ShadowType::EReader;
            break;
        case 4: // Murkrow
            lockInfo = { LockInfo(18, 127, 255), LockInfo(12, 0, 126), LockInfo(6, 127, 255 ) };
            type = ShadowType::FirstShadow;
            break;
        case 5: // E-Reader Scizor
            lockInfo = { LockInfo(11, 127, 255), LockInfo(3, 127, 255), LockInfo(2, 0, 255), LockInfo(13, 0, 126) };
            type = ShadowType::EReader;
            break;
        case 6: // E-Reader Togepi
            lockInfo = { LockInfo(22, 0, 30), LockInfo(24, 127, 255), LockInfo(8, 127, 255), LockInfo(23, 127, 255) };
            type = ShadowType::EReader;
            break;
        default: // Ursaring
            lockInfo = { LockInfo(21, 0, 126), LockInfo(16, 31, 255), LockInfo(20, 0, 63) };
            type = ShadowType::FirstShadow;
            break;
    }
}

void NatureLock::natureLockSetupGales(int lockNum)
{
    switch (lockNum)
    {
        case 0: // Altaria
            lockInfo = { LockInfo(24, 127, 255), LockInfo(0, 0, 126), LockInfo(12, 127, 255) };
            type = ShadowType::SecondShadow;
            break;
        case 1: // Arbok
            lockInfo =  { LockInfo(18, 0, 126), LockInfo(12, 0, 126), LockInfo(0, 0, 126), LockInfo(6, 127, 255) };
            type = ShadowType::FirstShadow;
            break;
        case 2: // Banette
            lockInfo =  { LockInfo(12, 0, 255), LockInfo(18, 0, 126), LockInfo(0, 0, 255) };
            type = ShadowType::SecondShadow;
            break;
        case 3: // Butterfree
            lockInfo = { LockInfo(0, 0, 126), LockInfo(6, 127, 255), LockInfo(12, 0, 190) };
            type = ShadowType::SecondShadow;
            break;
        case 4: // Chansey
            lockInfo = { LockInfo(24, 127, 255), LockInfo(6, 0, 126) };
            type = ShadowType::SecondShadow;
            break;
        case 5: // Delcatty
            lockInfo = { LockInfo(24, 127, 255), LockInfo(0, 127, 255), LockInfo(6, 0, 190) };
            type = ShadowType::FirstShadow;
            break;
        case 6: // Dodrio
            lockInfo =  { LockInfo(18, 0, 126) };
            type = ShadowType::SingleLock;
            break;
        case 7: // Dragonite
            lockInfo = { LockInfo(0, 127, 255), LockInfo(12, 0, 126), LockInfo(12, 0, 126), LockInfo(18, 127, 255), LockInfo(0, 127, 255) };
            type = ShadowType::FirstShadow;
            break;
        case 8: // Dugtrio
            lockInfo = { LockInfo(12, 127, 255), LockInfo(6, 0, 126), LockInfo(18, 127, 255), LockInfo(0, 127, 255) };
            type = ShadowType::FirstShadow;
            break;
        case 9: // Duskull
            lockInfo = { LockInfo(24, 127, 255), LockInfo(18, 0, 126), LockInfo(12, 127, 255) };
            type = ShadowType::FirstShadow;
            break;
        case 10: // Electabuzz
            lockInfo = { LockInfo(18, 0, 126), LockInfo(6, 0, 126), LockInfo(24, 63, 255) };
            type = ShadowType::FirstShadow;
            break;
        case 11: // Farfetch'd
            lockInfo = { LockInfo(24, 127, 255), LockInfo(0, 0, 126), LockInfo(12, 127, 255) };
            type = ShadowType::FirstShadow;
            break;
        case 12: // Golduck
            lockInfo = { LockInfo(18, 0, 126), LockInfo(6, 0, 126), LockInfo(24, 127, 255) };
            type = ShadowType::FirstShadow;
            break;
        case 13: // Grimer
            lockInfo = { LockInfo(18, 127, 255), LockInfo(12, 127, 255) };
            type = ShadowType::FirstShadow;
            break;
        case 14: // Growlithe
            lockInfo = { LockInfo(6, 0, 126), LockInfo(24, 127, 255) };
            type = ShadowType::SecondShadow;
            break;
        case 15: // Gulpin 3
        case 16: // Gulpin 1
        case 17: // Gulpin 2
            lockInfo = { LockInfo(6, 127, 255), LockInfo(12, 0, 126) };
            type = ShadowType::FirstShadow;
            break;
        case 18: // Hitmonchan
            lockInfo = { LockInfo(18, 0, 126), LockInfo(6, 0, 126), LockInfo(24, 127, 255) };
            type = ShadowType::FirstShadow;
            break;
        case 19: // Hitmonlee
            lockInfo = { LockInfo(24, 0, 62), LockInfo(6, 0, 255), LockInfo(12, 0, 126), LockInfo(18, 127, 255) };
            type = ShadowType::FirstShadow;
            break;
        case 20: // Hypno
            lockInfo = { LockInfo(24, 127, 255), LockInfo(6, 0, 126), LockInfo(12, 0, 126), LockInfo(18, 0, 126) };
            type = ShadowType::SecondShadow;
            break;
        case 21: // Kangaskhan
            lockInfo = { LockInfo(12, 0, 255), LockInfo(18, 0, 126), LockInfo(0, 0, 255) };
            type = ShadowType::FirstShadow;
            break;
        case 22: // Lapras
            lockInfo = { LockInfo(24, 127, 255), LockInfo(-1, -1, -1, true), LockInfo(-1, -1, -1, true), LockInfo(6, 0, 126) };
            type = ShadowType::SecondShadow;
            break;
        case 23: // Ledyba
            lockInfo = { LockInfo(0, 0, 126) };
            type = ShadowType::SingleLock;
            break;
        case 24: // Lickitung
            lockInfo = { LockInfo(6, 0, 255), LockInfo(24, 127, 255) };
            type = ShadowType::FirstShadow;
            break;
        case 25: // Lunatone
            lockInfo = { LockInfo(18, 127, 255), LockInfo(0, 0, 126) };
            type = ShadowType::FirstShadow;
            break;
        case 26: // Marcargo
            lockInfo = { LockInfo(12, 0, 126), LockInfo(6, 127, 255), LockInfo(24, 127, 255) };
            type = ShadowType::SecondShadow;
            break;
        case 27: // Magmar
            lockInfo = { LockInfo(0, 0, 126), LockInfo(18, 191, 255), LockInfo(18, 127, 255) };
            type = ShadowType::FirstShadow;
            break;
        case 28: // Magneton
            lockInfo = { LockInfo(12, 0, 126), LockInfo(0, 127, 255), LockInfo(18, 0, 255) };
            type = ShadowType::FirstShadow;
            break;
        case 29: // Makuhita
            lockInfo = { LockInfo(18, 0, 126), LockInfo(6, 127, 255) };
            type = ShadowType::FirstShadow;
            break;
        case 30: // Manectric
            lockInfo = { LockInfo(6, 0, 126) };
            type = ShadowType::SingleLock;
            break;
        case 31: // Marowak
            lockInfo = { LockInfo(24, 127, 255), LockInfo(-1, -1, -1, true), LockInfo(-1, -1, -1, true), LockInfo(6, 0, 126) };
            type = ShadowType::FirstShadow;
            break;
        case 32: // Mawile
            lockInfo = { LockInfo(18, 0, 126), LockInfo(6, 127, 255) };
            type = ShadowType::FirstShadow;
            break;
        case 33: // Meowth
            lockInfo =  { LockInfo(18, 0, 126), LockInfo(0, 0, 126), LockInfo(6, 63, 255) };
            type = ShadowType::FirstShadow;
            break;
        case 34: // Mr. Mime
            lockInfo = { LockInfo(6, 0, 126), LockInfo(24, 127, 255), LockInfo(18, 127, 255), LockInfo(18, 127, 255) };
            type = ShadowType::SecondShadow;
            break;
        case 35: // Natu
            lockInfo = { LockInfo(0, 0, 126), LockInfo(24, 127, 255) };
            type = ShadowType::FirstShadow;
            break;
        case 36: // Nosepass
            lockInfo = { LockInfo(12, 0, 126), LockInfo(18, 127, 255), LockInfo(0, 127, 255) };
            type = ShadowType::FirstShadow;
            break;
        case 37: // Numel
            lockInfo = { LockInfo(24, 0, 126), LockInfo(0, 0, 255), LockInfo(6, 127, 255) };
            type = ShadowType::FirstShadow;
            break;
        case 38: // Paras
            lockInfo = { LockInfo(6, 0, 126), LockInfo(24, 127, 255) };
            type = ShadowType::FirstShadow;
            break;
        case 39: // Pidgeotto
            lockInfo = { LockInfo(18, 31, 255), LockInfo(12, 127, 255) };
            type = ShadowType::FirstShadow;
            break;
        case 40: // Pineco
            lockInfo = { LockInfo(6, 127, 255) };
            type = ShadowType::SingleLock;
            break;
        case 41: // Pinsir
            lockInfo = { LockInfo(0, 0, 126), LockInfo(18, 191, 255), LockInfo(18, 127, 255) };
            type = ShadowType::SecondShadow;
            break;
        case 42: // Poliwrath
            lockInfo = { LockInfo(6, 0, 126), LockInfo(24, 127, 255), LockInfo(18, 127, 255), LockInfo(18, 127, 255) };
            type = ShadowType::FirstShadow;
            break;
        case 43: // Poochyena
            lockInfo = { LockInfo(12, 0, 126) };
            type = ShadowType::SingleLock;
            break;
        case 44: // Primeape
            lockInfo = { LockInfo(24, 127, 255), LockInfo(6, 0, 126), LockInfo(12, 0, 126), LockInfo(18, 0, 126) };
            type = ShadowType::FirstShadow;
            break;
        case 45: // Ralts
            lockInfo = { LockInfo(18, 127, 255), LockInfo(6, 0, 126), LockInfo(0, 63, 255) };
            type = ShadowType::FirstShadow;
            break;
        case 46: // Rapidash
            lockInfo = { LockInfo(12, 0, 126), LockInfo(6, 127, 255), LockInfo(24, 127, 255) };
            type = ShadowType::FirstShadow;
            break;
        case 47: // Raticate
            lockInfo = { LockInfo(18, 127, 255), LockInfo(-1, -1, -1, true), LockInfo(18, 0, 126) };
            type = ShadowType::FirstShadow;
            break;
        case 48: // Roselia
            lockInfo = { LockInfo(18, 127, 255), LockInfo(6, 127, 255) };
            type = ShadowType::FirstShadow;
            break;
        case 49: // Sableye
            lockInfo = { LockInfo(18, 0, 126), LockInfo(6, 0, 126), LockInfo(24, 127, 255) };
            type = ShadowType::SecondShadow;
            break;
        case 50: // Salamence
            lockInfo = { LockInfo(6, 0, 126) };
            type = ShadowType::Salamence;
            break;
        case 51: // Scyther
            lockInfo = { LockInfo(24, 127, 255), LockInfo(6, 0, 126) };
            type = ShadowType::FirstShadow;
            break;
        case 52: // Seedot 3
            lockInfo = { LockInfo(12, 127, 255), LockInfo(0, 127, 255), LockInfo(0, 0, 126), LockInfo(24, 0, 126), LockInfo(6, 127, 255) };
            type = ShadowType::FirstShadow;
            break;
        case 53: // Seedot 1
            lockInfo = { LockInfo(12, 127, 255), LockInfo(0, 127, 255), LockInfo(18, 0, 126), LockInfo(24, 0, 126), LockInfo(6, 127, 255) };
            type = ShadowType::FirstShadow;
            break;
        case 54: // Seedot 2
            lockInfo = { LockInfo(6, 127, 255), LockInfo(0, 0, 126), LockInfo(0, 0, 126), LockInfo(24, 0, 126), LockInfo(6, 127, 255) };
            type = ShadowType::FirstShadow;
            break;
        case 55: // Seel
            lockInfo = { LockInfo(18, 0, 126), LockInfo(18, 127, 255), LockInfo(6, 127, 255) };
            type = ShadowType::FirstShadow;
            break;
        case 56: // Shroomish
            lockInfo = { LockInfo(0, 0, 126), LockInfo(24, 0, 190) };
            type = ShadowType::FirstShadow;
            break;
        case 57: // Snorlax
            lockInfo =  { LockInfo(18, 0, 126), LockInfo(6, 0, 126), LockInfo(24, 63, 255) };
            type = ShadowType::SecondShadow;
            break;
        case 58: // Snorunt
            lockInfo = { LockInfo(6, 0, 126) };
            type = ShadowType::SingleLock;
            break;
        case 59: // Solrock
            lockInfo = { LockInfo(0, 0, 126), LockInfo(6, 127, 255), LockInfo(24, 0, 255) };
            type = ShadowType::FirstShadow;
            break;
        case 60: // Spearow
            lockInfo = { LockInfo(6, 0, 126), LockInfo(18, 127, 255) };
            type = ShadowType::FirstShadow;
            break;
        case 61: // Spheal 3
        case 63: // Spheal 2
            lockInfo = { LockInfo(0, 0, 255), LockInfo(12, 0, 126), LockInfo(24, 127, 255) };
            type = ShadowType::FirstShadow;
            break;
        case 62: // Spheal 1
            lockInfo = { LockInfo(12, 0, 126), LockInfo(24, 127, 255) };
            type = ShadowType::FirstShadow;
            break;
        case 64: // Spinarak
            lockInfo = { LockInfo(6, 127, 255), LockInfo(12, 0, 126) };
            type = ShadowType::FirstShadow;
            break;
        case 65: // Starmie
            lockInfo = { LockInfo(18, 127, 255), LockInfo(-1, -1, -1, true), LockInfo(0, 0, 126), LockInfo(6, 127, 255), LockInfo(24, 0, 255) };
            type = ShadowType::FirstShadow;
            break;
        case 66: // Swinub
            lockInfo =  { LockInfo(0, 127, 255), LockInfo(18, 0, 126) };
            type = ShadowType::FirstShadow;
            break;
        case 67: // Tangela
            lockInfo = { LockInfo(0, 0, 126), LockInfo(6, 127, 255), LockInfo(12, 0, 190) };
            type = ShadowType::FirstShadow;
            break;
        case 68: // Venomoth
            lockInfo = { LockInfo(12, 63, 255), LockInfo(24, 0, 255), LockInfo(18, 0, 126) };
            type = ShadowType::FirstShadow;
            break;
        case 69: // Voltorb
            lockInfo = { LockInfo(12, 0, 126), LockInfo(18, 127, 255), LockInfo(0, 127, 255) };
            type = ShadowType::FirstShadow;
            break;
        case 70: // Vulpix
            lockInfo = { LockInfo(18, 127, 255), LockInfo(6, 0, 126), LockInfo(0, 127, 255) };
            type = ShadowType::FirstShadow;
            break;
        default: // Weepinbell
            lockInfo = { LockInfo(12, 63, 255), LockInfo(24, 0, 255), LockInfo(18, 0, 126) };
            type = ShadowType::SecondShadow;
            break;
    }
}
