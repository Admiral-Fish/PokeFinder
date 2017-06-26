#ifndef NATURELOCK_HPP
#define NATURELOCK_HPP
#include <cstdint>
#include "LCRNG.hpp"
#include "LockInfo.hpp"

class NatureLock
{
    
private:
    LCRNG rng;
    LockInfo lockInfo[5] = { LockInfo(0 , 0, 0), LockInfo(0 , 0, 0), LockInfo(0 , 0, 0), LockInfo(0 , 0, 0), LockInfo(0 , 0, 0) };
    int count;
    int count2;
    int type;
    int x;
    uint32_t nature;
    uint32_t genderLower;
    uint32_t genderUpper;
    uint32_t pid;
    uint32_t gender;
    uint32_t pidOriginal;
    
    void natureLockSetup(int lockNum);
    
    uint32_t getPIDForward();
    
    uint32_t getPIDReverse();
    
    void countBackTwo();
    
    void countForwardTwo();
    
    void getCurrLock();
    
public:

    NatureLock(int lockNum);
    
    bool ivMethodSingleNL(uint32_t seed);
    
    bool ivMethodSalamenceSet(uint32_t seed);
    
    bool ivMethodSalamenceUnset(uint32_t seed);
    
    bool ivMethodSalamenceShinySkip(uint32_t seed);
    
    bool ivMethodFirstShadow(uint32_t seed);
    
    bool ivMethodFirstShadowSet(uint32_t seed);
    
    bool ivMethodFirstShadowUnset(uint32_t seed);
    
    bool ivMethodFirstShadowShinySkip(uint32_t seed);

};

#endif // NATURELOCK_HPP
