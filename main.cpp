#include "Objects/LCRNG.hpp"
#include "Objects/MTRNG.hpp"
#include "Objects/NatureLock.hpp"
#include "Objects/LockInfo.hpp"
#include <iostream>

int main ()
{
    NatureLock test = NatureLock(0);
    
    std::cout  << test.ivMethodFirstShadowShinySkip(0xD19AEF0E)<< std::endl;
    std::cout  << test.ivMethodFirstShadowShinySkip(0xAA940E92)<< std::endl;
}