/*
 * This file is part of PokéFinder
 * Copyright (C) 2017 by Admiral_Fish and bumba
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "RNGEuclidean.hpp"

// See https://crypto.stackexchange.com/a/10629 for how the following math works
// Uses Euclidean divison to reduce the search space (kmax) even further then RNGCache
// Only beneficial for smaller multipliers such as XDRNG

// Constructor for RNGEuclidean
RNGEuclidean::RNGEuclidean(Method FrameType)
{
    setupEuclidean(FrameType);
}

// Sets up the Euclidean constants
void RNGEuclidean::setupEuclidean(Method FrameType)
{
    if (FrameType == Channel)
    {
        // Channel is a unique situation having 6 rng calls with each call 5 bits known
        // It is unable to use the cache method and uses a modified Euclidean approach to keep kmax as low as possible
        // Using the first and last calls we can produce a modified adder and multiplier
        // Multj = Mult^j
        // Addj = Add * (1 + Mult + ... + Mult^(j-1))
        // Using j = 5 and XDRNG gives Mult = 0x284A930D and Add = 0xa2974c77
        sub1 = 0x284A930D; // Modified mult
        sub2 = 0x9A974C78; // -Modified add + 0x8000000 - 1
        base = 0x142549847b56cf2; // 0x7ffffff * (Modified mult + 1)
    }
    // XDColo, XD, Colo
    else
    {
        sub1 = 0x343fd; // XDRNG mult
        sub2 = 0x259ec4; // -XDRNG add + 0x10000 - 1
        base = 0x343fabc02; // 0xffff * (XDRNG mult + 1)
    }
}

// Recovers origin seeds for two 16 bit calls(15 bits known)
std::vector<uint32_t> RNGEuclidean::RecoverLower16BitsIV(uint32_t first, uint32_t second)
{
    std::vector<uint32_t> origin;
    int64_t t, kmax;
    uint32_t fullFirst, fullSecond;

    t = ((second - sub1 * first) - sub2) % 0x80000000;
    kmax = (base - t) / 0x80000000;

    for (int64_t k = 0; k <= kmax; k++, t += 0x80000000)
    {
        if ((t % sub1) < 0x10000)
        {
            fullFirst = (uint32_t)(first | (t / sub1));
            fullSecond = fullFirst * MULT + ADD;
            origin.push_back(fullFirst);
            origin.push_back(fullSecond);
        }
    }
    return origin;
}

// Recovers origin seeds for two 16 bit calls
std::vector<uint32_t> RNGEuclidean::RecoverLower16BitsPID(uint32_t first, uint32_t second)
{
    std::vector<uint32_t> origin;
    int64_t t, kmax;
    uint32_t fullFirst, fullSecond;

    t = ((second - sub1 * first) - sub2) % 0x100000000;
    kmax = (base - t) / 0x100000000;

    for (int64_t k = 0; k <= kmax; k++, t += 0x100000000)
    {
        if ((t % sub1) < 0x10000)
        {
            fullFirst = (uint32_t)(first | (t / sub1));
            fullSecond = fullFirst * MULT + ADD;
            origin.push_back(fullFirst);
            origin.push_back(fullSecond);
        }
    }
    return origin;
}

// Recovers origin seeds for six 5 bit calls
std::vector<uint32_t> RNGEuclidean::RecoverLower27BitsChannel(uint32_t hp, uint32_t atk, uint32_t def, uint32_t spa, uint32_t spd, uint32_t spe)
{
    std::vector<uint32_t> origin;
    uint32_t first = hp << 27, fullFirst;
    int64_t kmax, t;

    t = (((spd << 27) - sub1 * first) - sub2) % 0x100000000;
    kmax = (base - t) / 0x100000000;

    for (int64_t k = 0; k <= kmax; k++, t += 0x100000000)
    {
        if ((t % sub1) >= 0x8000000)
            continue;

        fullFirst = first | (uint32_t)(t / sub1);
        // Check if the next 4 IVs lineup
        // The euclidean divisor assures the first and last call match up
        // so there is no need to check if the last call lines up
        uint32_t call = fullFirst * MULT + ADD;
        if ((call >> 27) != atk)
            continue;

        call = call * MULT + ADD;
        if ((call >> 27) != def)
            continue;

        call = call * MULT + ADD;
        if ((call >> 27) != spe)
            continue;

        call = call * MULT + ADD;
        if ((call >> 27) != spa)
            continue;

        origin.push_back(fullFirst);
    }
    return origin;
}

// Switches the Euclidean being used
void RNGEuclidean::SwitchEuclidean(Method FrameType)
{
    setupEuclidean(FrameType);
}
