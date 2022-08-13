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

#ifndef BUTTONS_HPP
#define BUTTONS_HPP

#include <Core/Global.hpp>

/**
 * @brief Enum to encompass keypresses for Gen5
 */
enum class Buttons : u16
{
    None = 0,
    R = 1 << 0, // -0x10000
    L = 1 << 1, // -0x20000
    X = 1 << 2, // -0x40000
    Y = 1 << 3, // -0x80000
    A = 1 << 4, // -0x1000000
    B = 1 << 5, // -0x2000000
    Select = 1 << 6, // -0x4000000
    Start = 1 << 7, // -0x8000000
    SelectStart = Select | Start,
    Right = 1 << 8, // -0x10000000
    Left = 1 << 9, // -0x20000000
    Up = 1 << 10, // -0x40000000
    Down = 1 << 11, // -0x80000000
    RightUp = Right | Up,
    LeftUp = Left | Up,
    RightDown = Right | Down,
    LeftDown = Left | Down
};

/**
 * @brief Converts enum to number
 *
 * @param button Input button
 *
 * @return Converted number
 */
constexpr u16 toInt(Buttons button)
{
    return static_cast<u16>(button);
}

/**
 * @brief Computes bitwise AND of two \ref Buttons
 *
 * @param left First button
 * @param right Second button
 *
 * @return Computed bitwise AND button
 */
constexpr Buttons operator&(Buttons left, Buttons right)
{
    return static_cast<Buttons>(toInt(left) & toInt(right));
}

/**
 * @brief Computes bitwise OR of two \ref Buttons
 *
 * @param left First button
 * @param right Second button
 *
 * @return Computed bitwise OR button
 */
constexpr Buttons operator|(Buttons left, Buttons right)
{
    return static_cast<Buttons>(toInt(left) | toInt(right));
}

#endif // BUTTONS_HPP
