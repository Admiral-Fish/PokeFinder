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

#ifndef GAME_HPP
#define GAME_HPP

#include <Core/Util/Global.hpp>

enum class Game : u32
{
    None = 0,
    Ruby = 1 << 0,
    Sapphire = 1 << 1,
    RS = Ruby | Sapphire,
    Emerald = 1 << 2,
    RSE = RS | Emerald,
    FireRed = 1 << 3,
    LeafGreen = 1 << 4,
    FRLG = FireRed | LeafGreen,
    Gen3 = RSE | FRLG,
    Gales = 1 << 5,
    Colosseum = 1 << 6,
    GC = Gales | Colosseum,
    Diamond = 1 << 7,
    Pearl = 1 << 8,
    DP = Diamond | Pearl,
    Platinum = 1 << 9,
    DPPt = Diamond | Pearl | Platinum,
    HeartGold = 1 << 10,
    SoulSilver = 1 << 11,
    HGSS = HeartGold | SoulSilver,
    Gen4 = DPPt | HGSS,
    Black = 1 << 12,
    White = 1 << 13,
    BW = Black | White,
    Black2 = 1 << 14,
    White2 = 1 << 15,
    BW2 = Black2 | White2,
    Gen5 = BW | BW2,
    Sword = 1 << 24,
    Shield = 1 << 25,
    SwSh = Sword | Shield,
    BD = 1 << 26,
    SP = 1 << 27,
    BDSP = BD | SP,
    Gen8 = SwSh | BDSP
};

constexpr u32 toInt(Game game)
{
    return static_cast<u32>(game);
}

constexpr Game operator&(Game left, Game right)
{
    return static_cast<Game>(toInt(left) & toInt(right));
}

#endif // GAME_HPP
