#include "Objects/NatureLock.hpp"
#include "Objects/EncounterSlot.hpp"
#include <iostream>

int main ()
{
    std::cout << EncounterSlot::KSlot(0x12345678, Encounter::Wild) << std::endl;
}