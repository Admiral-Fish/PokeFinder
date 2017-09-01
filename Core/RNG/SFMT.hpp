/*
Copyright (c) 2006,2007 Mutsuo Saito, Makoto Matsumoto and Hiroshima
University.
Copyright (c) 2012 Mutsuo Saito, Makoto Matsumoto, Hiroshima University
and The University of Tokyo.
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above
      copyright notice, this list of conditions and the following
      disclaimer in the documentation and/or other materials provided
      with the distribution.
    * Neither the names of Hiroshima University, The University of
      Tokyo nor the names of its contributors may be used to endorse
      or promote products derived from this software without specific
      prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef SFMT_HPP
#define SFMT_HPP
#include <cstdint>

class SFMT
{

private:
    //int const N32 = 624;
    //uint32_t const cMSK1 = 0xdfffffef;
    //uint32_t const cMSK2 = 0xddfecb7f;
    //uint32_t const cMSK3 = 0xbffaffff;
    //uint32_t const cMSK4 = 0xbffffff6;
    //int const cSL1 = 18;
    //int const cSR1 = 11;
    uint32_t _sfmt[624];
    uint32_t _parity[4] = { 0x1, 0x0, 0x0, 0x13c9e684 };
    int _sfmti;

    void period_certificaion();

public:

    void init(uint32_t seed);

    SFMT(uint32_t seed);

    uint32_t Nextuint();

    uint64_t Nextulong();

    void Reseed(uint32_t seed);

    void Shuffle();

    void Next();

};

#endif // SFMT_HPP
