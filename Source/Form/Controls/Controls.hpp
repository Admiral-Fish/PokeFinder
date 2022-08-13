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

#ifndef CONTROLS_HPP
#define CONTROLS_HPP

#include <Core/Global.hpp>

/**
 * @brief Enum that encompasses which controls can be enabled/disabled
 */
enum class Controls : u16
{
    None = 0,
    IVs = 1 << 0,
    Ability = 1 << 1,
    Gender = 1 << 2,
    EncounterSlots = 1 << 3,
    HiddenPowers = 1 << 4,
    Natures = 1 << 5,
    Shiny = 1 << 6,
    UseDelay = 1 << 7,
    DisableFilter = 1 << 8
};

/**
 * @brief Computes bitwise AND of two \ref Controls
 *
 * @param left First control
 * @param right Second control
 *
 * @return Computed bitwise AND control
 */
constexpr Controls operator&(Controls left, Controls right)
{
    return static_cast<Controls>(static_cast<u16>(left) & static_cast<u16>(right));
};

/**
 * @brief Computes bitwise OR of two \ref Controls
 *
 * @param left First control
 * @param right Second control
 *
 * @return Computed bitwise OR control
 */
constexpr Controls operator|(Controls left, Controls right)
{
    return static_cast<Controls>(static_cast<u16>(left) | static_cast<u16>(right));
};

#endif // CONTROLS_HPP
