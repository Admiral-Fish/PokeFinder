#ifndef LOCKINFO_HPP
#define LOCKINFO_HPP
#include <cstdint>

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

#endif // LOCKINFO_HPP
