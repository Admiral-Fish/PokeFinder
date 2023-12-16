/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2023 by Admiral_Fish, bumba, and EzPzStreamz
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

#include "State.hpp"
#include <Core/Util/Nature.hpp>

constexpr u8 order[6] = { 0, 1, 2, 5, 3, 4 };
constexpr u8 charOrder[11] = { 0, 1, 2, 3, 4, 5, 0, 1, 2, 3, 4 };

void State::updateStats(const PersonalInfo *info)
{
    u8 h = 0;
    u8 p = 0;
    u8 ecIndex = ec % 6;
    u8 charIndex = ecIndex;
    u8 maxIV = 0;
    for (int i = 0; i < 6; i++)
    {
        h |= (ivs[order[i]] & 1) << i;
        p |= ((ivs[order[i]] >> 1) & 1) << i;

        stats[i] = Nature::computeStat(info->getStat(i), ivs[i], nature, level, i);

        u8 index = charOrder[ecIndex + i];
        if (ivs[order[index]] > maxIV)
        {
            charIndex = index;
            maxIV = ivs[order[index]];
        }
    }
    hiddenPower = h * 15 / 63;
    hiddenPowerStrength = 30 + (p * 40 / 63);
    characteristic = (charIndex * 5) + (maxIV % 5);
}
