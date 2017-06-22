#ifndef LCRNG_HPP
#define LCRNG_HPP

#include <string>

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

    LCRNG(uint32_t seed, std::string type);

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
