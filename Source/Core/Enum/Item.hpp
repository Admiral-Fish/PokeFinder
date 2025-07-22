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

#ifndef ITEM_HPP
#define ITEM_HPP

#include <Core/Global.hpp>

/**
 * @brief Enum to encompass different daycare items
 */
enum class Item : u8
{
    None,
    Everstone,
    PowerItemStart,
    PowerWeight = PowerItemStart,
    PowerBracer,
    PowerBelt,
    PowerLens,
    PowerBand,
    PowerAnklet,
    PowerItemEnd = PowerAnklet,
    DestinyKnot
};

/**
 * @brief Converts enum to number
 *
 * @param item Input item
 *
 * @return Converted number
 */
constexpr u8 toInt(Item item)
{
    return static_cast<u8>(item);
}

/**
 * @brief Computes subtraction of two \ref Item
 *
 * @param left First item
 * @param right Second item
 *
 * @return Computed bitwise AND game
 */
constexpr Item operator-(Item left, Item right)
{
    return static_cast<Item>(toInt(left) - toInt(right));
}

#endif // ITEM_HPP
