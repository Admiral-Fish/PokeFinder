#include "Objects/NatureLock.hpp"
#include <iostream>

int main ()
{
    NatureLock test = NatureLock(67);
    
    std::cout << test.ivMethodSalamenceShinySkip(0xEB4943E4) << std::endl;
    std::cout << test.ivMethodSalamenceShinySkip(0x3280558B) << std::endl;
    std::cout << test.ivMethodSalamenceShinySkip(0x44426368) << std::endl;
    std::cout << test.getType() << std::endl;
}