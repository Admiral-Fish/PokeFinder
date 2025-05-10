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

#ifndef ENCOUNTER_HPP
#define ENCOUNTER_HPP

#include <Core/Global.hpp>

/**
 * @brief Enum to encompass different encounter types
 */
enum class Encounter : u8
{
    Grass,
    GrassDark,
    GrassRustling,
    RockSmash,
    Surfing,
    SurfingRippling,
    OldRod,
    GoodRod,
    SuperRod,
    SuperRodRippling,
    Static,
    Honey,
    HoneyRare,
    HoneyMunchlax,
    BugCatchingContest,
    Headbutt,
    HeadbuttAlt,
    HeadbuttSpecial,
    Roamer,
    Gift,
    EntraLink,
    GiftEgg,
    HiddenGrotto
};

/**
 * @brief Converts enum to number
 *
 * @param encounter Input encounter
 *
 * @return Converted number
 */
constexpr u8 toInt(Encounter encounter)
{
    return static_cast<u8>(encounter);
}

/**
 * @brief Computes subtraction of two \ref Encounter
 *
 * @param left First encounter
 * @param right Second encounter
 *
 * @return Computed substraction encounter
 */
constexpr Encounter operator-(Encounter left, Encounter right)
{
    return static_cast<Encounter>(toInt(left) - toInt(right));
}

#endif // ENCOUNTER_HPP
