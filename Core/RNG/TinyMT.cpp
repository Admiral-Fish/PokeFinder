#include "TinyMT.hpp"

// Constructor
TinyMT::TinyMT(uint32_t seed)
{
    init(seed);
}

// Constructor given Tiny states
TinyMT::TinyMT(uint32_t st[])
{
    for (int i = 0; i < 4; i++)
        state[i] = st[i];
    periodCertification();
}

// Creates Tiny state given seed
void TinyMT::init(uint32_t seed)
{
    state[0] = seed;
    state[1] = MAT1;
    state[2] = MAT2;
    state[3] = TMAT;

    int i;
    for (i = 1; i < MINLOOP; i++)
        state[i & 3] ^= i + 0x6c078965 * (state[(i - 1) & 3] ^ (state[(i - 1) & 3] >> 30));

    periodCertification();

    for (i = 0; i < PRELOOP; i++)
        NextState();
}

// Verifies not all 4 Tiny states are 0
void TinyMT::periodCertification()
{
    if (state[0] == 0 && state[1] == 0 && state[2] == 0 && state[3] == 0)
    {
        state[0] = 'T';
        state[1] = 'I';
        state[2] = 'N';
        state[3] = 'Y';
    }
}

// Generates the next Tiny state
void TinyMT::NextState()
{
    uint32_t y = state[3];
    uint32_t x = (state[0] & TINYMT32MASK) ^ state[1] ^ state[2];
    x ^= (x << TINYMT32SH0);
    y ^= (y >> TINYMT32SH0) ^ x;
    state[0] = state[1];
    state[1] = state[2];
    state[2] = x ^ (y << TINYMT32SH1);
    state[3] = y;

    if ((y & 1) == 1)
    {
        state[1] ^= MAT1;
        state[2] ^= MAT2;
    }
}

// Calls the next state and next psuedo random number
uint32_t TinyMT::NextUint()
{
    NextState();
    return Temper();
}

// Generates the psuedo random number from the Tiny state
uint32_t TinyMT::Temper()
{
    uint32_t t0 = state[3];
    uint32_t t1 = state[0] + (state[2] >> TINYMT32SH8);

    t0 ^= t1;
    if ((t1 & 1) == 1)
        t0 ^= TMAT;
    return t0;
}
