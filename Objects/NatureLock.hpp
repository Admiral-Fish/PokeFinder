#ifndef NATURELOCK_HPP
#define NATURELOCK_HPP
#include <cstdint>
#include <vector>
#include "LCRNG.hpp"

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
    LCRNG rng;
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
