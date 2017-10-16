/*
 * This file is part of PokéFinder
 * Copyright (C) 2017 by Admiral_Fish and bumba
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

#ifndef NATURELOCK_HPP
#define NATURELOCK_HPP
#include <libPokeFinder/RNG/LCRNG.hpp>
#include <cstdint>
#include <vector>

// Defines the different types of shadows
enum ShadowType
{
    NoLock,
    SingleLock,
    FirstShadow,
    Salamence,
    SecondShadow
};

class LockInfo
{
    
private:
    uint32_t genderUpper;
    uint32_t genderLower;
    uint32_t nature;
    
public:
    
    LockInfo(uint32_t nature, uint32_t genderLower, uint32_t genderUpper);

    uint32_t GetGenderLower();

    uint32_t GetGenderUpper();
    
    uint32_t GetNature();
    
};

class NatureLock
{
    
private:
    int backCount;
    int frontCount;
    uint32_t gender;
    uint32_t genderLower;
    uint32_t genderUpper;
    std::vector<LockInfo> lockInfo;
    uint32_t nature;
    uint32_t pid;
    uint32_t pidOriginal;
    LCRNG rng = XDRNG(0);
    ShadowType type;
    int x;

    void countBackTwo();

    void countForwardTwo();

    void getCurrLock();

    uint32_t getPIDForward();

    uint32_t getPIDReverse();

    uint32_t getPSVReverse();

    void natureLockSetupColo(int lockNum);
    
    void natureLockSetupGales(int lockNum);
    
public:

    NatureLock();

    ShadowType GetType();

    bool IVMethodFirstShadow(uint32_t seed);

    bool IVMethodFirstShadowSet(uint32_t seed);

    bool IVMethodFirstShadowShinySkip(uint32_t seed);

    bool IVMethodFirstShadowUnset(uint32_t seed);

    bool IVMethodSalamenceSet(uint32_t seed);

    bool IVMethodSalamenceShinySkip(uint32_t seed);

    bool IVMethodSalamenceUnset(uint32_t seed);

    bool IVMethodSingleNL(uint32_t seed);

    void SwitchLockColo(int lockNum);
    
    void SwitchLockGales(int lockNum);

};

#endif // NATURELOCK_HPP
