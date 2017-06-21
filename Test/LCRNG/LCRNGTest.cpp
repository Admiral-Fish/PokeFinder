#include "LCRNG.hpp"
#include <iostream>

int main ()
{
    LCRNG test = LCRNG();
    test.setpokeRNG();
    std::cout << std::hex << test.getSeed() << std::endl;
    std::cout << std::hex << test.next32Bit() << std::endl;
    std::cout << std::hex << test.next32Bit() << std::endl;
    std::cout << std::hex << test.next32Bit() << std::endl;
    std::cout << std::hex << test.next32Bit() << std::endl;
    std:: cout << std::endl;
    uint32_t seed = 0;
    for(int i = 0;i<5;i++)
    {
        seed = (seed*0x41C64E6D + 0x00006073)&0xFFFFFFFF;
        std::cout << std::hex << seed << std::endl;
    }


}
