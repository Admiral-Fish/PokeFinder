#include "LCRNG.hpp"

#include <string>

LCRNG::LCRNG() {
    seed = 0;
    mult = 0;
    add = 0;
    multR = 0;
    addR = 0;
}

LCRNG::LCRNG(unsigned long seed) {
    this->seed = seed;
}

unsigned long LCRNG::getSeed() {
    return seed;
}

void LCRNG::reSeed(unsigned long newSeed) {
    seed = newSeed;
}

void LCRNG::setConst(unsigned long mult, unsigned long add, unsigned long multR, unsigned long addR) {
    this->mult = mult;
    this->add = add;
    this->multR = multR;
    this->addR = addR;
}

void LCRNG::setpokeRNG() {
    setConst(0x41c64e6d, 0x6073, 0xeeb9eb65, 0xa3561a1);
}

void LCRNG::setxdRNG() {
    setConst(0x343FD, 0x269EC3, 0xB9B33155, 0xA170F641);
}

void LCRNG::setaRNG() {
    setConst(0x6c078965, 0x01, 0x9638806d, 0x69c77f93);
}

unsigned long LCRNG::next32Bit() {
    seed = (seed*mult+add)&0xFFFFFFFF;
    return seed;
}

unsigned long LCRNG::next16Bit() {
    return (next32Bit() >> 16);
}

unsigned long LCRNG::prev32Bit() {
    seed = (seed*multR+addR)&0xFFFFFFFF;
    return seed;
}

unsigned long LCRNG::prev16Bit() {
    return (prev32Bit() >> 16);
}

unsigned long LCRNG::advanceFrames(int frames) {
    return -1;
}

unsigned long LCRNG::reverseFrames(int frames) {
    return -1;
}

