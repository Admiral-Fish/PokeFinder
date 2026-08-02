/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2024 by Admiral_Fish, bumba, and EzPzStreamz
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

#ifndef PASSPOWER_HPP
#define PASSPOWER_HPP

#include <Core/Global.hpp>

/**
 * @brief Enum to encompass different encounter methods
 */
enum class PassPower : u8
{
    None,
    Level1,
    Level2,
    Level3,
    LevelS
};

/**
 * @brief Converts enum to number
 *
 * @param power Input pass power
 *
 * @return Converted number
 */
constexpr u8 toInt(PassPower power)
{
    return static_cast<u8>(power);
}

/**
 * @brief Computes subtraction of two \ref PassPower
 *
 * @param left First pass power
 * @param right Second pass power
 *
 * @return Computed substraction pass power
 */
constexpr PassPower operator-(PassPower left, PassPower right)
{
    return static_cast<PassPower>(toInt(left) - toInt(right));
}

/**
 * @brief Determines if \p left is less than or equal to \p right
 *
 * @param left First pass power
 * @param right Second pass power
 *
 * @return true Left is less than or equal to right 
 * @return false Left is not less than or equal to right
 */
constexpr bool operator<=(PassPower left, PassPower right)
{
    return toInt(left) <= toInt(right);
}

#endif // PASSPOWER_HPP
