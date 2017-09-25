#include "MTRNG.hpp"

// Mersenne Twister

// Constructor for Mersenne Twister
MersenneTwister::MersenneTwister(uint32_t seed)
{
    init(seed);
}

// Initializes
void MersenneTwister::init(uint32_t seed)
{
    mt[0] = seed;

    for (index = 1; index < N; index++)
        mt[index] = (0x6C078965*(mt[index - 1] ^ (mt[index - 1] >> 30)) + index);
}

uint32_t MersenneTwister::temperingShiftL(uint32_t y)
{
    return (y >> 18);
}

uint32_t MersenneTwister::temperingShiftS(uint32_t y)
{
    return (y << 7);
}

uint32_t MersenneTwister::temperingShiftT(uint32_t y)
{
    return (y << 15);
}

uint32_t MersenneTwister::temperingShiftU(uint32_t y)
{
    return (y >> 11);
}

// Calls the next psuedo-random number
uint32_t MersenneTwister::Nextuint()
{
    uint32_t y;

    // Array reshuffle check
    if (index >= N)
    {
        int kk = 0;

        for (; kk < 227; ++kk)
        {
            y = (mt[kk] & UPPERMASK) | (mt[kk + 1] & LOWERMASK);
            mt[kk] = mt[kk + M] ^ (y >> 1) ^ mag01[y & 0x1];
        }

        for (; kk < 623; ++kk)
        {
            y = (mt[kk] & UPPERMASK) | (mt[kk + 1] & LOWERMASK);
            mt[kk] = mt[kk - 227] ^ (y >> 1) ^ mag01[y & 0x1];
        }

        y = (mt[623] & UPPERMASK) | (mt[0] & LOWERMASK);
        mt[623] = mt[396] ^ (y >> 1) ^ mag01[y & 0x1];

        index = 0;
    }

    y = mt[index++];
    y ^= temperingShiftU(y);
    y ^= temperingShiftS(y) & TEMPERINGMASKB;
    y ^= temperingShiftT(y) & TEMPERINGMASKC;
    y ^= temperingShiftL(y);

    return y;
}

// Recreates the Mersenne Twister with a new seed
void MersenneTwister::Reseed(uint32_t seed)
{
    init(seed);
}


// Mersenne Twister Untempered

// Constructor for Mersenne Twister Untempered 
MersenneTwisterUntempered::MersenneTwisterUntempered(uint32_t seed)
{
    init(seed);
}

// Initializes
void MersenneTwisterUntempered::init(uint32_t seed)
{
    mt[0] = seed;

    for (index = 1; index < N; index++)
        mt[index] = (0x6C078965*(mt[index - 1] ^ (mt[index - 1] >> 30)) + index);
}

// Calls the next psuedo-random number
uint32_t MersenneTwisterUntempered::Nextuint()
{
    // Array reshuffle check
    if (index >= N)
    {
        int kk = 0;
        uint32_t y;

        for (; kk < 227; ++kk)
        {
            y = (mt[kk] & UPPERMASK) | (mt[kk + 1] & LOWERMASK);
            mt[kk] = mt[kk + M] ^ (y >> 1) ^ mag01[y & 0x1];
        }

        for (; kk < 623; ++kk)
        {
            y = (mt[kk] & UPPERMASK) | (mt[kk + 1] & LOWERMASK);
            mt[kk] = mt[kk - 227] ^ (y >> 1) ^ mag01[y & 0x1];
        }

        y = (mt[623] & UPPERMASK) | (mt[0] & LOWERMASK);
        mt[623] = mt[396] ^ (y >> 1) ^ mag01[y & 0x1];

        index = 0;
    }

    return mt[index++];
}

// Recreates the Mersenne Twister Untempered with a new seed
void MersenneTwisterUntempered::Reseed(uint32_t seed)
{
    init(seed);
}


// Mersenne Twister Fast

// Constructor for Mersenne Twister Fast
MersenneTwisterFast::MersenneTwisterFast(uint32_t seed, int calls)
{
    maxCalls = calls;
    
    if (maxCalls > 227)
    {
        return;
        //Throw an error eventually
    }
    max = M + maxCalls;
    init(seed);
}

// Initializes
void MersenneTwisterFast::init(uint32_t seed)
{
    mt[0] = seed;

    for (index = 1; index < max; ++index)
        mt[index] = (0x6C078965*(mt[index - 1] ^ (mt[index - 1] >> 30)) + index);
}

uint32_t MersenneTwisterFast::temperingShiftS(uint32_t y)
{
    return (y << 7);
}

uint32_t MersenneTwisterFast::temperingShiftT(uint32_t y)
{
    return (y << 15);
}

uint32_t MersenneTwisterFast::temperingShiftU(uint32_t y)
{
    return (y >> 11);
}

// Calls the next psuedo-random number
uint32_t MersenneTwisterFast::Nextuint()
{
    uint32_t y;

    // Array reshuffle check
    if (index >= max)
    {
        int kk = 0;

        for (; kk < maxCalls; ++kk)
        {
            y = (mt[kk] & UPPERMASK) | (mt[kk + 1] & LOWERMASK);
            mt[kk] = mt[kk + M] ^ (y >> 1) ^ mag01[y & 0x1];
        }

        index = 0;
    }

    y = mt[index++];
    y ^= temperingShiftU(y);
    y ^= temperingShiftS(y) & TEMPERINGMASKB;
    y ^= temperingShiftT(y) & TEMPERINGMASKC2;

    return y;
}

// Recreates the Mersenne Twister Fast with a new seed
void MersenneTwisterFast::Reseed(uint32_t seed)
{
    init(seed);
}


// SFMT

// Constructor for SFMT
SFMT::SFMT(uint32_t seed)
{
    init(seed);
}

// Initializes
void SFMT::init(uint32_t seed)
{
    sfmt[0] = seed;

    for (index = 1; index < N32; index++)
        sfmt[index] = 0x6C078965 * (sfmt[index - 1] ^ (sfmt[index - 1] >> 30)) + index;

    periodCertificaion();
    Shuffle();
}

// Verify internal state vector
void SFMT::periodCertificaion()
{
    uint32_t inner = 0;
    uint32_t work;
    int i, j;

    for (i = 0; i < 4; i++)
        inner ^= sfmt[i] & parity[i];
    for (i = 16; i > 0; i >>= 1)
        inner ^= inner >> i;
    inner &= 1;
    if (inner == 1)
        return;

    for (i = 0; i < 4; i++)
    {
        work = 1;
        for (j = 0; j < 32; j++)
        {
            if ((work & parity[i]) != 0)
            {
                sfmt[i] ^= work;
                return;
            }
            work <<= 1;
        }
    }
}

// Advances by n frames shuffling the correct amount of times
void SFMT::AdvanceFrames(int n)
{
    int temp = index + (n * 2);
    while (temp >= 624)
    {
        temp -= 624;
        Shuffle();
    }
}

// Generates the next psuedo random number
uint32_t SFMT::Nextuint()
{
    // Array reshuffle check
    if (index >= N32)
        Shuffle();
    return sfmt[index++];
}

// Generates the next 64bit psuedo random number
uint64_t SFMT::Nextulong()
{
    return Nextuint() | ((uint64_t)Nextuint() << 32);
}

// Recreates the SFMT with a new seed
void SFMT::Reseed(uint32_t seed)
{
    init(seed);
}

// Shuffles the array once all 624 states have been used
void SFMT::Shuffle()
{
    int a = 0;
    int b = 488;
    int c = 616;
    int d = 620;
    do
    {
        sfmt[a + 3] = sfmt[a + 3] ^ (sfmt[a + 3] << 8) ^ (sfmt[a + 2] >> 24) ^ (sfmt[c + 3] >> 8) ^ ((sfmt[b + 3] >> CSR1) & CMSK4) ^ (sfmt[d + 3] << CSL1);
        sfmt[a + 2] = sfmt[a + 2] ^ (sfmt[a + 2] << 8) ^ (sfmt[a + 1] >> 24) ^ (sfmt[c + 3] << 24) ^ (sfmt[c + 2] >> 8) ^ ((sfmt[b + 2] >> CSR1) & CMSK3) ^ (sfmt[d + 2] << CSL1);
        sfmt[a + 1] = sfmt[a + 1] ^ (sfmt[a + 1] << 8) ^ (sfmt[a] >> 24) ^ (sfmt[c + 2] << 24) ^ (sfmt[c + 1] >> 8) ^ ((sfmt[b + 1] >> CSR1) & CMSK2) ^ (sfmt[d + 1] << CSL1);
        sfmt[a] = sfmt[a] ^ (sfmt[a] << 8) ^ (sfmt[c + 1] << 24) ^ (sfmt[c] >> 8) ^ ((sfmt[b] >> CSR1) & CMSK1) ^ (sfmt[d] << CSL1);
        c = d;
        d = a;
        a += 4;
        b += 4;
        if (b >= N32)
            b = 0;
    }
    while (a < N32);
    index = 0;
}

