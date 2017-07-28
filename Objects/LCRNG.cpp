#include "LCRNG.hpp"


// Default constructor for LCRNG
LCRNG::LCRNG()
{
    seed = 0;
    setpokeRNG();
}

// LCRNG constructor with seed
LCRNG::LCRNG(uint32_t seed)
{
    this->seed = seed;
}

// LCRNG constructor with seed and RNG type
LCRNG::LCRNG(uint32_t seed, rng_type type): LCRNG(seed)
{
    if (type == POKE_RNG)
      setpokeRNG();
    else if (type == XD_RNG)
      setxdRNG();
    else
      setaRNG();
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
    setConst(POKE_MULT, POKE_ADD, POKE_MULTR, POKE_ADDR);
}

// Method for setting xdRNG constants
void LCRNG::setxdRNG()
{
    setConst(XD_MULT, XD_ADD, XD_MULTR, XD_ADDR);
}

// Method for setting aRNG constants
void LCRNG::setaRNG()
{
    setConst(A_MULT, A_ADD, A_MULTR, A_ADDR);
}

// Method for finding next 32 bit seed
uint32_t LCRNG::next32Bit()
{
    seed = seed*mult + add;
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
    seed = seed*multR + addR;
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
