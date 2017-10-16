#ifndef TINYMT_HPP
#define TINYMT_HPP
#include <cstdint>

class TinyMT
{

private:
    uint32_t const MAT1 = 0x8f7011ee;
    uint32_t const MAT2 = 0xfc78ff1f;
    int const MINLOOP = 8;
    int const PRELOOP = 8;
    uint32_t const TMAT = 0x3793fdff;
    uint32_t const TINYMT32MASK = 0x7FFFFFFF;
    int const TINYMT32SH0 = 1;
    int const TINYMT32SH1 = 10;
    int const TINYMT32SH8 = 8;
    uint32_t state[4];

    void init(uint32_t seed);

    void periodCertification();

public:

    TinyMT(uint32_t seed);

    TinyMT(uint32_t st[]);

    void NextState();

    uint32_t NextUint();

    uint32_t Temper();

};

#endif // TINYMT_HPP
