#ifndef MTRNG_HPP
#define MTRNG_HPP
#include <cstdint>

class MTRNG
{

public:

    virtual void Reseed(uint32_t seed) = 0;

    virtual uint32_t Nextuint() = 0;

};

class MersenneTwister : public MTRNG
{

private:
    uint32_t const LOWERMASK = 0x7FFFFFFF;
    int const M = 397;
    uint32_t const MATRIXA = 0x9908B0DF;
    int const N = 624;
    uint32_t const UPPERMASK = 0x80000000;
    uint32_t const TEMPERINGMASKB = 0x9D2C5680;
    uint32_t const TEMPERINGMASKC = 0xEFC60000;
    uint32_t mag01[2] = { 0x0, MATRIXA };
    uint32_t mt[624];
    int index;

    void init(uint32_t seed);

    uint32_t temperingShiftL(uint32_t y);

    uint32_t temperingShiftS(uint32_t y);

    uint32_t temperingShiftT(uint32_t y);

    uint32_t temperingShiftU(uint32_t y);

public:

    MersenneTwister(uint32_t seed);

    virtual uint32_t Nextuint();

    virtual void Reseed(uint32_t seed);
    
};

class MersenneTwisterUntempered : public MTRNG
{
    
private:
    int const M = 397;
    int const N = 624;
    uint32_t const MATRIXA = 0x9908B0DF;
    uint32_t const LOWERMASK = 0x7FFFFFFF;
    uint32_t const UPPERMASK = 0x80000000;
    uint32_t mt[624];
    uint32_t mag01[2] = { 0x0, MATRIXA };
    int index;

    void init(uint32_t seed);

public:

    MersenneTwisterUntempered(uint32_t seed);

    virtual void Reseed(uint32_t seed);

    virtual uint32_t Nextuint();
    
};

class MersenneTwisterFast : public MTRNG
{
    
private:
    int const M = 397;
    int const N = 624;
    uint32_t const LOWERMASK = 0x7FFFFFFF;
    uint32_t const MATRIXA = 0x9908B0DF;
    uint32_t const TEMPERINGMASKB = 0x9D2C5680;
    uint32_t const TEMPERINGMASKC2 = 0xEFC00000;
    uint32_t const UPPERMASK = 0x80000000;
    uint32_t mag01[2] = { 0x0, MATRIXA };
    int max;
    int maxCalls;
    uint32_t mt[624];
    int index;

    void init(uint32_t seed);

    uint32_t temperingShiftS(uint32_t y);

    uint32_t temperingShiftT(uint32_t y);

    uint32_t temperingShiftU(uint32_t y);

public:

    MersenneTwisterFast(uint32_t seed, int calls);

    virtual uint32_t Nextuint();

    virtual void Reseed(uint32_t seed);
    
};

class SFMT : public MTRNG
{

private:
    uint32_t const CMSK1 = 0xdfffffef;
    uint32_t const CMSK2 = 0xddfecb7f;
    uint32_t const CMSK3 = 0xbffaffff;
    uint32_t const CMSK4 = 0xbffffff6;
    int const CSL1 = 18;
    int const CSR1 = 11;
    int const N32 = 624;
    int index;
    uint32_t parity[4] = { 0x1, 0x0, 0x0, 0x13c9e684 };
    uint32_t sfmt[624];

    void init(uint32_t seed);

    void periodCertificaion();

public:

    SFMT(uint32_t seed);

    void AdvanceFrames(int n);

    virtual uint32_t Nextuint();

    uint64_t Nextulong();

    virtual void Reseed(uint32_t seed);

    void Shuffle();

};

#endif //MTRNG_HPP
