#include "LockInfo.hpp"
#include <cstdint>

// Constructor for LockInfo
LockInfo::LockInfo(uint32_t nature, uint32_t genderLower, uint32_t genderUpper)
{
    this->nature = nature;
    this->genderLower = genderLower;
    this->genderUpper = genderUpper;
}

// Gets nature value
uint32_t LockInfo::getNature()
{
    return nature;
}

// Gets lower gender thresh value
uint32_t LockInfo::getGenderLower()
{
    return genderLower;
}

// Gets upper gender thresh value
uint32_t LockInfo::getGenderUpper()
{
    return genderUpper;
}

