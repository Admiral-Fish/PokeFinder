#include "Objects/LCRNG.hpp"
#include "Objects/MTRNG.hpp"
#include "Objects/NatureLock.hpp"
#include "Objects/LockInfo.hpp"
#include <iostream>

int main ()
{
    NatureLock test = NatureLock(59);
    std::cout  << test.ivMethodSingleNL(0x6D0AA645)<< std::endl;

}