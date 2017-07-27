#ifndef LCRNG_HPP
#define LCRNG_HPP

#include <string>
#include <cstdint>

enum rng_type {
    POKE_RNG,
    XD_RNG
}

const uint32_t POKE_MULT = 0x41c64e6d;
const uint32_t POKE_ADD = 0x6073;
const uint32_t POKE_MULTR = 0xeeb9eb65;
const uint32_t POKE_ADDR = 0xa3561a1;
const uint32_t XD_MULT = 0x343FD;
const uint32_t XD_ADD = 0x269EC3;
const uint32_t XD_MULTR = 0xB9B33155;
const uint32_t XD_ADDR = 0xA170F641;
const uint32_t A_MULT = 0x6c078965;
const uint32_t A_ADD = 0x01;
const uint32_t A_MULTR = 0x9638806d;
const uint32_t A_ADDR = 0x69c77f93;


class LCRNG
{

private:
    uint32_t add;
    uint32_t mult;
    uint32_t multR;
    uint32_t addR;
    uint32_t seed;

public:

    LCRNG();

    LCRNG(uint32_t seed);

    LCRNG(uint32_t seed, rng_type type);

    void setSeed(uint32_t newSeed);

    void setConst(uint32_t mult, uint32_t add, uint32_t multR, uint32_t addR);

    void setpokeRNG();

    void setxdRNG();

    void setaRNG();

    uint32_t getSeed();

    uint32_t next32Bit();

    uint32_t next16Bit();

    uint32_t prev32Bit();

    uint32_t prev16Bit();

    void advanceFrames(int frames);

    void reverseFrames(int frames);

};

#endif //LCRNG_HPP
