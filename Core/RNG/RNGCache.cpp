/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2020 by Admiral_Fish, bumba, and EzPzStreamz
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3
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

#include "RNGCache.hpp"
#include <Core/Enum/Method.hpp>
#include <cstring>

// See https://crypto.stackexchange.com/a/10609 for how the following math works
// Uses a brute force meet in the middle attack using precomputated data

RNGCache::RNGCache(Method method)
{
    if (method == Method::Method4)
    {
        k = 0xa29a6900; // Mult * Mult << 8
        mult = 0xc2a29a69; // Mult * Mult
        add = 0xe97e7b6a; // Add * (Mult + 1)
    }
    // Method 1/2
    else
    {
        k = 0xc64e6d00; // Mult << 8
        mult = 0x41c64e6d; // pokerng constant
        add = 0x6073; // pokerng constant
    }

    std::memset(low, 0, sizeof(low));
    std::memset(flags, false, sizeof(flags));
    for (u16 i = 0; i < 256; i++)
    {
        u32 right = mult * i + add;
        u16 val = right >> 16;

        flags[val] = true;
        low[val--] = static_cast<u8>(i);
        flags[val] = true;
        low[val] = static_cast<u8>(i);
    }
}

// Recovers origin seeds for two 16 bit calls(15 bits known) with or without gap based on the cache
QVector<u32> RNGCache::recoverLower16BitsIV(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe) const
{
    QVector<u32> origin;

    u32 first = static_cast<u32>((hp | (atk << 5) | (def << 10)) << 16);
    u32 second = static_cast<u32>((spe | (spa << 5) | (spd << 10)) << 16);

    // Check with the top bit of the first call both
    // flipped and unflipped to account for only knowing 15 bits
    u32 search1 = second - first * mult;
    u32 search2 = second - (first ^ 0x80000000) * mult;

    for (u16 i = 0; i < 256; i++, search1 -= k, search2 -= k)
    {
        if (flags[search1 >> 16])
        {
            u32 test = first | static_cast<u32>(i << 8) | low[search1 >> 16];
            // Verify IV calls line up
            if (((test * mult + add) & 0x7fff0000) == second)
            {
                origin.append(test);
            }
        }

        if (flags[search2 >> 16])
        {
            u32 test = first | static_cast<u32>(i << 8) | low[search2 >> 16];
            // Verify IV calls line up
            if (((test * mult + add) & 0x7fff0000) == second)
            {
                origin.append(test);
            }
        }
    }

    return origin;
}

// Recovers origin seeds for two 16 bit calls based on the cache
QVector<u32> RNGCache::recoverLower16BitsPID(u32 pid) const
{
    QVector<u32> origin;

    u32 first = pid << 16;
    u32 second = pid & 0xFFFF0000;
    u32 search = second - first * mult;

    for (u16 i = 0; i < 256; i++, search -= k)
    {
        if (flags[search >> 16])
        {
            u32 test = first | static_cast<u32>(i << 8) | low[search >> 16];
            // Verify PID calls line up
            if (((test * mult + add) & 0xffff0000) == second)
            {
                origin.append(test);
            }
        }
    }
    return origin;
}
