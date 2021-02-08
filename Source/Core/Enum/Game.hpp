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

#ifndef GAME_HPP
#define GAME_HPP

#include <Core/Util/Global.hpp>

enum Game : u16
{
    Blank = 0, // Placeholder
    Ruby = 1 << 0,
    Sapphire = 1 << 1,
    RS = Ruby | Sapphire,
    Emerald = 1 << 2,
    RSE = RS | Emerald,
    FireRed = 1 << 3,
    LeafGreen = 1 << 4,
    FRLG = FireRed | LeafGreen,
    Gales = 1 << 5,
    Colosseum = 1 << 6,
    GC = Gales | Colosseum,
    Diamond = 1 << 7,
    Pearl = 1 << 8,
    Platinum = 1 << 9,
    DPPt = Diamond | Pearl | Platinum,
    HeartGold = 1 << 10,
    SoulSilver = 1 << 11,
    HGSS = HeartGold | SoulSilver,
    Black = 1 << 12,
    White = 1 << 13,
    BW = Black | White,
    Black2 = 1 << 14,
    White2 = 1 << 15,
    BW2 = Black2 | White2
};

#endif // GAME_HPP
