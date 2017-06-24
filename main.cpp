#include "Objects/LCRNG.hpp"
#include "Objects/MTRNG.hpp"
#include <iostream>

int main ()
{
    MersenneTwisterFast test = MersenneTwisterFast(0, 227);
    for (int i = 0; i < 10; i++)
        std::cout << std::hex << test.Nextuint() << std::endl;

}