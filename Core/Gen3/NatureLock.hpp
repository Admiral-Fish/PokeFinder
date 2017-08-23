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
#include <cstdint>
#include <vector>
#include "Core/Objects/LCRNG.hpp"

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
    uint32_t nature;
    uint32_t genderLower;
    uint32_t genderUpper;
    
public:
    
    LockInfo(uint32_t nature, uint32_t genderLower, uint32_t genderUpper);
    
    uint32_t getNature();
    
    uint32_t getGenderLower();
    
    uint32_t getGenderUpper();
    
};

class NatureLock
{
    
private:
    LCRNG rng = LCRNG(0, XD_RNG);
    std::vector<LockInfo> lockInfo;
    int backCount;
    int frontCount;
    ShadowType type;
    int x;
    uint32_t nature;
    uint32_t genderLower;
    uint32_t genderUpper;
    uint32_t pid;
    uint32_t gender;
    uint32_t pidOriginal;
    
    void natureLockSetup(int lockNum);
    
    void countBackTwo();
    
    void countForwardTwo();
    
    void getCurrLock();
    
    uint32_t getPIDForward();
    
    uint32_t getPIDReverse();
    
    uint32_t getPSVReverse();
    
public:

    NatureLock(int lockNum);
    
    void switchLock(int lockNum);
    
    bool ivMethodSingleNL(uint32_t seed);
    
    bool ivMethodSalamenceSet(uint32_t seed);
    
    bool ivMethodSalamenceUnset(uint32_t seed);
    
    bool ivMethodSalamenceShinySkip(uint32_t seed);
    
    bool ivMethodFirstShadow(uint32_t seed);
    
    bool ivMethodFirstShadowSet(uint32_t seed);
    
    bool ivMethodFirstShadowUnset(uint32_t seed);
    
    bool ivMethodFirstShadowShinySkip(uint32_t seed);
    
    ShadowType getType();

};

#endif // NATURELOCK_HPP
