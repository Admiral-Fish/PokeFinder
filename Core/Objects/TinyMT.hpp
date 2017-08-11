/*
Copyright (c) 2011, 2013 Mutsuo Saito, Makoto Matsumoto,
Hiroshima University and The University of Tokyo.
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
    * Neither the name of the Hiroshima University nor the names of
      its contributors may be used to endorse or promote products
      derived from this software without specific prior written
      permission.

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

#ifndef TINYMT_HPP
#define TINYMT_HPP
#include <cstdint>

class TinyMT
{

private:
    //uint32_t mat1 = 0x8f7011ee;
    //uint32_t const mat2 = 0xfc78ff1f;
    //uint32_t const tmat = 0x3793fdff;
    //int const MIN_LOOP = 8;
    //int const PRE_LOOP = 8;
    //uint32_t const TINYMT32_MASK = 0x7FFFFFFF;
    //int const TINYMT32_SH0 = 1;
    //int const TINYMT32_SH1 = 10;
    //int const TINYMT32_SH8 = 8;
    uint32_t status[4] = { 0, 0, 0, 0 };

public:

    TinyMT(uint32_t seed);

    TinyMT(uint32_t st[]);

    void init(uint32_t seed);

    void nextState();

    uint32_t temper();

    void period_certification();

    uint32_t Nextuint();

};

#endif // TINYMT_HPP
