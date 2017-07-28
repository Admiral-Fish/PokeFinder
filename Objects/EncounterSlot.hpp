#ifndef ENCOUNTERSLOT_HPP
#define ENCOUNTERSLOT_HPP
#include <cstdint>
#include <vector>
#include "Objects/Encounter.hpp"

class Range
{
    
private:
    uint32_t min;
    uint32_t max;
    
public:

    Range(uint32_t min, uint32_t max);
    
    uint32_t getMin();
    
    uint32_t getMax();
    

};

class EncounterSlot
{
    
private:
    static int calcSlot(int compare, std::vector<Range> ranges);
    
public:

    static int HSlot(uint32_t result, Encounter encounterType);
    
    static int KSlot(uint32_t result, Encounter encounterType);
    
    static int JSlot(uint32_t result, Encounter encounterType);
};

#endif // ENCOUNTERSLOT_HPP