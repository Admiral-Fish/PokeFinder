// Copyright 2007-2008 Rory Plaire (codekaizen@gmail.com)

// Adapted from:

/* C# Version Copyright (C) 2001-2004 Akihilo Kramot (Takel).  */
/* C# porting from a C-program for MT19937, originaly coded by */
/* Takuji Nishimura and Makoto Matsumoto, considering the suggestions by */
/* Topher Cooper and Marc Rieffel in July-Aug. 1997.           */
/* This library is free software under the Artistic license:   */
/*                                                             */
/* You can find the original C-program at                      */
/*     http://www.math.keio.ac.jp/~matumoto/mt.html            */
/*                                                             */

// and:

/////////////////////////////////////////////////////////////////////////////
// C# Version Copyright (c) 2003 CenterSpace Software, LLC                 //
//                                                                         //
// This code is free software under the Artistic license.                  //
//                                                                         //
// CenterSpace Software                                                    //
// 2098 NW Myrtlewood Way                                                  //
// Corvallis, Oregon, 97330                                                //
// USA                                                                     //
// http://www.centerspace.net                                              //
/////////////////////////////////////////////////////////////////////////////

// and, of course:
/*
   A C-program for MT19937, with initialization improved 2002/2/10.
   Coded by Takuji Nishimura and Makoto Matsumoto.
   This is a faster version by taking Shawn Cokus's optimization,
   Matthe Bellew's simplification, Isaku Wada's real version.
   Before using, initialize the state by using init_genrand(seed)
   or init_by_array(init_key, key_length).
   Copyright (C) 1997 - 2002, Makoto Matsumoto and Takuji Nishimura,
   All rights reserved.
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:
     1. Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.
     2. Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.
     3. The names of its contributors may not be used to endorse or promote
        products derived from this software without specific prior written
        permission.
   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
   Any feedback is very welcome.
   http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/emt.html
   email: m-mat @ math.sci.hiroshima-u.ac.jp (remove space)
*/

#include "MTRNG.hpp"

// Mersenne Twister

// Constructor for Mersenne Twister
MersenneTwister::MersenneTwister(uint32_t seed)
{
    init(seed);
}

// Recreates the Mersenne Twister with a new seed
void MersenneTwister::Reseed(uint32_t seed)
{
    init(seed);
}

// Initializes
void MersenneTwister::init(uint32_t seed)
{
    _mt[0] = seed;

    for (_mti = 1; _mti < 624; _mti++)
        _mt[_mti] = (0x6C078965*(_mt[_mti - 1] ^ (_mt[_mti - 1] >> 30)) + _mti);
}

// Calls the next psuedo-random number
uint32_t MersenneTwister::Nextuint()
{
    return Generateuint();
}

// Generate the next psuedo-random number
uint32_t MersenneTwister::Generateuint()
{
    uint32_t y;

    //Array reshuffle check
    if (_mti >= 624) 
    {
        int kk = 0;

        for (; kk < 227; ++kk)
        {
            y = (_mt[kk] & 0x80000000) | (_mt[kk + 1] & 0x7FFFFFFF);
            _mt[kk] = _mt[kk + 397] ^ (y >> 1) ^ _mag01[y & 0x1];
        }

        for (; kk < 623; ++kk)
        {
            y = (_mt[kk] & 0x80000000) | (_mt[kk + 1] & 0x7FFFFFFF);
            _mt[kk] = _mt[kk - 227] ^ (y >> 1) ^ _mag01[y & 0x1];
        }

        y = (_mt[623] & 0x80000000) | (_mt[0] & 0x7FFFFFFF);
        _mt[623] = _mt[396] ^ (y >> 1) ^ _mag01[y & 0x1];

        _mti = 0;
    }

    y = _mt[_mti++];
    y ^= temperingShiftU(y);
    y ^= temperingShiftS(y) & 0x9D2C5680;
    y ^= temperingShiftT(y) & 0xEFC60000;
    y ^= temperingShiftL(y);

    return y;
}

uint32_t MersenneTwister::temperingShiftU(uint32_t y)
{
    return (y >> 11);
}

uint32_t MersenneTwister::temperingShiftS(uint32_t y)
{
    return (y << 7);
}

uint32_t MersenneTwister::temperingShiftT(uint32_t y)
{
    return (y << 15);
}

uint32_t MersenneTwister::temperingShiftL(uint32_t y)
{
    return (y >> 18);
}


// Mersenne Twister Untempered

// Constructor for Mersenne Twister Untempered 
MersenneTwisterUntempered::MersenneTwisterUntempered(uint32_t seed)
{
    init(seed);
}

// Recreates the Mersenne Twister Untempered with a new seed
void MersenneTwisterUntempered::Reseed(uint32_t seed)
{
    init(seed);
}

// Initializes
void MersenneTwisterUntempered::init(uint32_t seed)
{
    _mt[0] = seed;

    for (_mti = 1; _mti < 624; _mti++)
        _mt[_mti] = (0x6C078965*(_mt[_mti - 1] ^ (_mt[_mti - 1] >> 30)) + _mti);
}

// Calls the next psuedo-random number
uint32_t MersenneTwisterUntempered::Nextuint()
{
    return Generateuint();
}

// Generate the next psuedo-random number
uint32_t MersenneTwisterUntempered::Generateuint()
{
    uint32_t y;

    //Array reshuffle check
    if (_mti >= 624) 
    {
        int kk = 0;

        for (; kk < 227; ++kk)
        {
            y = (_mt[kk] & 0x80000000) | (_mt[kk + 1] & 0x7FFFFFFF);
            _mt[kk] = _mt[kk + 397] ^ (y >> 1) ^ _mag01[y & 0x1];
        }

        for (; kk < 623; ++kk)
        {
            y = (_mt[kk] & 0x80000000) | (_mt[kk + 1] & 0x7FFFFFFF);
            _mt[kk] = _mt[kk - 227] ^ (y >> 1) ^ _mag01[y & 0x1];
        }

        y = (_mt[623] & 0x80000000) | (_mt[0] & 0x7FFFFFFF);
        _mt[623] = _mt[396] ^ (y >> 1) ^ _mag01[y & 0x1];

        _mti = 0;
    }

    y = _mt[_mti++];

    return y;
}


// Mersenne Twister Fast

// Constructor for Mersenne Twister Fast
MersenneTwisterFast::MersenneTwisterFast(uint32_t seed, int calls)
{
    maxCalls = calls;
    
    if (maxCalls > 227)
    {
        return;
        //Throw an error eventually
    }
    max = 397 + maxCalls;
    init(seed);
}

// Recreates the Mersenne Twister Fast with a new seed
void MersenneTwisterFast::Reseed(uint32_t seed)
{
    init(seed);
}

// Initializes
void MersenneTwisterFast::init(uint32_t seed)
{
    _mt[0] = seed;
    
    for (_mti = 1; _mti < max; ++_mti)
        _mt[_mti] = (0x6C078965*(_mt[_mti - 1] ^ (_mt[_mti - 1] >> 30)) + _mti);
}

// Calls the next psuedo-random number
uint32_t MersenneTwisterFast::Nextuint()
{
    return Generateuint();
}

// Generate the next psuedo-random number
uint32_t MersenneTwisterFast::Generateuint()
{
    uint32_t y;

    //Array reshuffle check
    if (_mti >= max) 
    {
        int kk = 0;

        for (; kk < maxCalls; ++kk)
        {
            y = (_mt[kk] & 0x80000000) | (_mt[kk + 1] & 0x7FFFFFFF);
            _mt[kk] = _mt[kk + 397] ^ (y >> 1) ^ _mag01[y & 0x1];
        }

        _mti = 0;
    }

    y = _mt[_mti++];
    y ^= temperingShiftU(y);
    y ^= temperingShiftS(y) & 0x9D2C5680;
    y ^= temperingShiftT(y) & 0xEF000000;

    return y;
}

uint32_t MersenneTwisterFast::temperingShiftU(uint32_t y)
{
    return (y >> 11);
}

uint32_t MersenneTwisterFast::temperingShiftS(uint32_t y)
{
    return (y << 7);
}

uint32_t MersenneTwisterFast::temperingShiftT(uint32_t y)
{
    return (y << 15);
}