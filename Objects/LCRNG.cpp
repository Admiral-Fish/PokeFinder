#include "LCRNG.hpp"
#include <string>

LCRNG::LCRNG()
{
    seed = 0;
    mult = 0;
    add = 0;
    multR = 0;
    addR = 0;
}

LCRNG::LCRNG(uint32_t seed)
{
    this->seed = seed;
}

uint32_t LCRNG::getSeed()
{
    return seed;
}

void LCRNG::reSeed(uint32_t newSeed)
{
    seed = newSeed;
}

void LCRNG::setConst(uint32_t mult, uint32_t add, uint32_t multR, uint32_t addR)
{
    this->mult = mult;
    this->add = add;
    this->multR = multR;
    this->addR = addR;
}

void LCRNG::setpokeRNG()
{
    setConst(0x41c64e6d, 0x6073, 0xeeb9eb65, 0xa3561a1);
}

void LCRNG::setxdRNG()
{
    setConst(0x343FD, 0x269EC3, 0xB9B33155, 0xA170F641);
}

void LCRNG::setaRNG()
{
    setConst(0x6c078965, 0x01, 0x9638806d, 0x69c77f93);
}

uint32_t LCRNG::next32Bit()
{
    seed = seed*mult+add;
    return seed;
}

uint32_t LCRNG::next16Bit()
{
    return (next32Bit() >> 16);
}

uint32_t LCRNG::prev32Bit()
{
    seed = seed*multR+addR;
    return seed;
}

uint32_t LCRNG::prev16Bit()
{
    return (prev32Bit() >> 16);
}

void LCRNG::advanceFrames(int frames)
{
    for(int i = 0; i < frames; i++)
        seed = seed*mult + add;
}

void LCRNG::reverseFrames(int frames)
{
    for(int i = 0; i < frames; i++)
        seed = seed*multR + addR;
}
