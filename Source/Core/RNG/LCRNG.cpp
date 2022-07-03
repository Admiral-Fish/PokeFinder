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

#include "LCRNG.hpp"

static consteval JumpTable computeJumpTable(u32 add, u32 mult)
{
    JumpTable table;
    table.add[0] = add;
    table.mult[0] = mult;

    for (int i = 1; i < 32; i++)
    {
        table.add[i] = table.add[i - 1] * (table.mult[i - 1] + 1);
        table.mult[i] = table.mult[i - 1] * table.mult[i - 1];
    }

    return table;
}

constexpr JumpTable ARNGTable = computeJumpTable(0x01, 0x6C078965);
constexpr JumpTable ARNGRTable = computeJumpTable(0x69C77F93, 0x9638806D);
constexpr JumpTable PokeRNGTable = computeJumpTable(0x6073, 0x41C64E6D);
constexpr JumpTable PokeRNGRTable = computeJumpTable(0xA3561A1, 0xEEB9EB65);
constexpr JumpTable XDRNGTable = computeJumpTable(0x269EC3, 0x343FD);
constexpr JumpTable XDRNGRTable = computeJumpTable(0xA170F641, 0xB9B33155);
