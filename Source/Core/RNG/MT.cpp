/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2022 by Admiral_Fish, bumba, and EzPzStreamz
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

#include "MT.hpp"
#include <Core/RNG/SIMD.hpp>

MT::MT(u32 seed) : index(624)
{
    mt[0] = seed;

    for (u32 i = 1; i < 624; i++)
    {
        seed = 0x6c078965 * (seed ^ (seed >> 30)) + i;
        mt[i] = seed;
    }
}

void MT::advance(u32 advances)
{
    advances += index;
    while (advances >= 624)
    {
        shuffle();
        advances -= 624;
    }
    index = advances;
}

u32 MT::next()
{
    if (index == 624)
    {
        shuffle();
        index = 0;
    }

    u32 y = mt[index++];
    y ^= (y >> 11);
    y ^= (y << 7) & 0x9d2c5680;
    y ^= (y << 15) & 0xefc60000;
    y ^= (y >> 18);

    return y;
}

u16 MT::nextUShort()
{
    return next() >> 16;
}

void MT::shuffle()
{
    vuint32x4 upperMask = v32x4_set(0x80000000);
    vuint32x4 lowerMask = v32x4_set(0x7fffffff);
    vuint32x4 matrix = v32x4_set(0x9908b0df);
    vuint32x4 one = v32x4_set(1);

    for (int i = 0; i < 224; i += 4)
    {
        vuint32x4 m0 = v32x4_load(&mt[i]);
        vuint32x4 m1 = v32x4_load(&mt[i + 1]);
        vuint32x4 m2 = v32x4_load(&mt[i + 397]);

        vuint32x4 y = v32x4_or(v32x4_and(m0, upperMask), v32x4_and(m1, lowerMask));
        vuint32x4 y1 = v32x4_shr<1>(y);
        vuint32x4 mag01 = v32x4_and(v32x4_cmpeq(v32x4_and(y, one), one), matrix);

        v32x4_store(&mt[i], v32x4_xor(v32x4_xor(y1, mag01), m2));
    }

    vuint32x4 last = v32x4_insert<3>(v32x4_load(&mt[621]), mt[0]);
    {
        vuint32x4 m0 = v32x4_load(&mt[224]);
        vuint32x4 m1 = v32x4_load(&mt[225]);

        vuint32x4 y = v32x4_or(v32x4_and(m0, upperMask), v32x4_and(m1, lowerMask));
        vuint32x4 y1 = v32x4_shr<1>(y);
        vuint32x4 mag01 = v32x4_and(v32x4_cmpeq(v32x4_and(y, one), one), matrix);

        v32x4_store(&mt[224], v32x4_xor(v32x4_xor(y1, mag01), last));
    }

    for (int i = 228; i < 620; i += 4)
    {
        vuint32x4 m0 = v32x4_load(&mt[i]);
        vuint32x4 m1 = v32x4_load(&mt[i + 1]);
        vuint32x4 m2 = v32x4_load(&mt[i - 227]);

        vuint32x4 y = v32x4_or(v32x4_and(m0, upperMask), v32x4_and(m1, lowerMask));
        vuint32x4 y1 = v32x4_shr<1>(y);
        vuint32x4 mag01 = v32x4_and(v32x4_cmpeq(v32x4_and(y, one), one), matrix);

        v32x4_store(&mt[i], v32x4_xor(v32x4_xor(y1, mag01), m2));
    }

    {
        vuint32x4 m0 = v32x4_load(&mt[620]);
        vuint32x4 m2 = v32x4_load(&mt[393]);

        vuint32x4 y = v32x4_or(v32x4_and(m0, upperMask), v32x4_and(last, lowerMask));
        vuint32x4 y1 = v32x4_shr<1>(y);
        vuint32x4 mag01 = v32x4_and(v32x4_cmpeq(v32x4_and(y, one), one), matrix);

        v32x4_store(&mt[620], v32x4_xor(v32x4_xor(y1, mag01), m2));
    }
}
