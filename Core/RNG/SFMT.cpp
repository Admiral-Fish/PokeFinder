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

#include "SFMT.hpp"

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

// Skips through the SFMT without returning psuedo random number
void SFMT::Next()
{
    if (index >= 622)
        Shuffle();
    else
        index += 2;
}

// Generates the next psuedo random number
uint32_t SFMT::NextUint()
{
    // Array reshuffle check
    if (index >= N32)
        Shuffle();
    return sfmt[index++];
}

// Generates the next 64bit psuedo random number
uint64_t SFMT::NextUlong()
{
    return NextUint() | ((uint64_t)NextUint() << 32);
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
