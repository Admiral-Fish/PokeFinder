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
        status[i] = st[i];
    period_certification();
}

// Creates Tiny status given seed
void TinyMT::init(uint32_t seed)
{
    status[0] = seed;
    status[1] = 0x8f7011ee;
    status[2] = 0xfc78ff1f;
    status[3] = 0x3793fdff;

    int i;
    for (i = 1; i < 8; i++)
        status[i & 3] ^= i + 0x6c078965 * (status[(i - 1) & 3] ^ (status[(i - 1) & 3] >> 30));

    period_certification();

    for (i = 0; i < 8; i++)
        nextState();
}

// Generates the next Tiny state
void TinyMT::nextState()
{
    uint32_t y = status[3];
    uint32_t x = (status[0] & 0x7FFFFFFF) ^ status[1] ^ status[2];
    x ^= (x << 1);
    y ^= (y >> 1) ^ x;
    status[0] = status[1];
    status[1] = status[2];
    status[2] = x ^ (y << 10);
    status[3] = y;

    if ((y & 1) == 1)
    {
        status[1] ^= 0x8f7011ee;
        status[2] ^= 0xfc78ff1f;
    }
}

// Generates the psuedo random number from the Tiny state
uint32_t TinyMT::temper()
{
    uint32_t t0 = status[3];
    uint32_t t1 = status[0] + (status[2] >> 8);

    t0 ^= t1;
    if ((t1 & 1) == 1)
        t0 ^= 0x3793fdff;
    return t0;
}

// Calls the next state and next psuedo random number
uint32_t TinyMT::Nextuint()
{
    nextState();
    return temper();
}

// Verifies not all 4 Tiny states are 0
void TinyMT::period_certification()
{
    if (status[0] == 0 && status[1] == 0 && status[2] == 0 && status[3] == 0)
    {
        status[0] = 84;
        status[1] = 73;
        status[2] = 78;
        status[3] = 89;
    }
}
