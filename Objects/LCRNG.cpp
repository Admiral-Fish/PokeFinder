#include "LCRNG.hpp"
#include <string>

// Default constructor for LCRNG
LCRNG::LCRNG()
{
    seed = 0;
    mult = 0;
    add = 0;
    multR = 0;
    addR = 0;
}

// LCRNG constructor with seed
LCRNG::LCRNG(uint32_t seed)
{
    this->seed = seed;
}

// Get method for seed
uint32_t LCRNG::getSeed()
{
    return seed;
}

// Set method for seed
void LCRNG::setSeed(uint32_t newSeed)
{
    seed = newSeed;
}

// Method for setting other constants
void LCRNG::setConst(uint32_t mult, uint32_t add, uint32_t multR, uint32_t addR)
{
    this->mult = mult;
    this->add = add;
    this->multR = multR;
    this->addR = addR;
}

// Method for setting pokeRNG constants
void LCRNG::setpokeRNG()
{
    setConst(0x41c64e6d, 0x6073, 0xeeb9eb65, 0xa3561a1);
}

// Method for setting xdRNG constants
void LCRNG::setxdRNG()
{
    setConst(0x343FD, 0x269EC3, 0xB9B33155, 0xA170F641);
}

// Method for setting aRNG constants
void LCRNG::setaRNG()
{
    setConst(0x6c078965, 0x01, 0x9638806d, 0x69c77f93);
}

// Method for finding next 32 bit seed
uint32_t LCRNG::next32Bit()
{
    seed = seed*mult+add;
    return seed;
}

// Method for finding next 16 bit seed
uint32_t LCRNG::next16Bit()
{
    return (next32Bit() >> 16);
}

// Method for finding previous 32 bit seed
uint32_t LCRNG::prev32Bit()
{
    seed = seed*multR+addR;
    return seed;
}

// Method for finding previous 16 bit seed
uint32_t LCRNG::prev16Bit()
{
    return (prev32Bit() >> 16);
}

// Method for advancing seed by a given number of frames
void LCRNG::advanceFrames(int frames)
{
    for(int i = 0; i < frames; i++)
        seed = seed*mult + add;
}

// Method for reversing seed by a given number of frames
void LCRNG::reverseFrames(int frames)
{
    for(int i = 0; i < frames; i++)
        seed = seed*multR + addR;
}
