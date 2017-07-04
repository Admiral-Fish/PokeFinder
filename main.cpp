#include "Objects/NatureLock.hpp"
#include <iostream>

int main ()
{
    NatureLock test = NatureLock(67);
    
    std::cout  << test.ivMethodSalamenceShinySkip(0x44426368)<< std::endl;
    std::cout  << test.ivMethodSalamenceShinySkip(0xB5E07FD7)<< std::endl;
    std::cout  << test.ivMethodSalamenceShinySkip(0x293FFEA7)<< std::endl;
    std::cout  << test.getType()<< std::endl;
}