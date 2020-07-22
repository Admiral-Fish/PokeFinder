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

#include "SFMT.hpp"

SFMT::SFMT(u32 seed)
{
    sfmt[0] = seed;

    for (index = 1; index < 624; index++)
    {
        sfmt[index] = 0x6C078965 * (sfmt[index - 1] ^ (sfmt[index - 1] >> 30)) + index;
    }

    u32 inner = (sfmt[0] & 1) ^ (sfmt[3] & 0x13c9e684);
    for (u8 i = 16; i > 0; i >>= 1)
    {
        inner ^= inner >> i;
    }

    if ((inner & 1) == 0)
    {
        sfmt[0] ^= 1;
    }
}

void SFMT::advance(u32 advances)
{
    index += (advances * 2);
    while (index >= 624)
    {
        shuffle();
    }
}

u64 SFMT::next()
{
    if (index >= 624)
    {
        shuffle();
    }

    u32 high = sfmt[index++];
    u32 low = sfmt[index++];
    return high | (static_cast<u64>(low) << 32);
}

u32 SFMT::nextUInt()
{
    if (index >= 624)
    {
        shuffle();
    }

    return sfmt[index++];
}

void SFMT::shuffle()
{
    u16 a = 0;
    u16 b = 488;
    u16 c = 616;
    u16 d = 620;

    do
    {
        // clang-format off
        sfmt[a + 3] ^= (sfmt[a + 3] << 8) ^ (sfmt[a + 2] >> 24) ^ (sfmt[c + 3] >> 8) ^ ((sfmt[b + 3] >> 11) & 0xbffffff6) ^ (sfmt[d + 3] << 18);
        sfmt[a + 2] ^= (sfmt[a + 2] << 8) ^ (sfmt[a + 1] >> 24) ^ (sfmt[c + 3] << 24) ^ (sfmt[c + 2] >> 8) ^ ((sfmt[b + 2] >> 11) & 0xbffaffff) ^ (sfmt[d + 2] << 18);
        sfmt[a + 1] ^= (sfmt[a + 1] << 8) ^ (sfmt[a] >> 24) ^ (sfmt[c + 2] << 24) ^ (sfmt[c + 1] >> 8) ^ ((sfmt[b + 1] >> 11) & 0xddfecb7f) ^ (sfmt[d + 1] << 18);
        sfmt[a] ^= (sfmt[a] << 8) ^ (sfmt[c + 1] << 24) ^ (sfmt[c] >> 8) ^ ((sfmt[b] >> 11) & 0xdfffffef) ^ (sfmt[d] << 18);
        // clang-format on

        c = d;
        d = a;
        a += 4;
        b += 4;
        if (b == 624)
        {
            b = 0;
        }
    } while (a < 624);
    index -= 624;
}
