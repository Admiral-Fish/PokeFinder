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

template <class RNG>
static consteval JumpTable computeJumpTable()
{
    JumpTable table;
    table.jump[0].add = RNG::getAdd();
    table.jump[0].mult = RNG::getMult();

    for (int i = 1; i < 32; i++)
    {
        table.jump[i].add = table.jump[i - 1].add * (table.jump[i - 1].mult + 1);
        table.jump[i].mult = table.jump[i - 1].mult * table.jump[i - 1].mult;
    }

    return table;
}

constexpr JumpTable ARNGTable = computeJumpTable<ARNG>();
constexpr JumpTable ARNGRTable = computeJumpTable<ARNGR>();
constexpr JumpTable PokeRNGTable = computeJumpTable<PokeRNG>();
constexpr JumpTable PokeRNGRTable = computeJumpTable<PokeRNGR>();
constexpr JumpTable XDRNGTable = computeJumpTable<XDRNG>();
constexpr JumpTable XDRNGRTable = computeJumpTable<XDRNGR>();
