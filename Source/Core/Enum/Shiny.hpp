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

#ifndef SHINY_HPP
#define SHINY_HPP

#include <Core/Global.hpp>

/**
 * @brief Enum that encompasses different PID locking mechanisms
 */
enum class Shiny : u8
{
    Random, /// PID is random, not locked
    Never, /// PID is random, forced to not be shiny
    Always, /// PID is random, forced to shiny
    Star, /// PID is random, forced to be star shiny
    Square, /// PID is random, forced to be square shiny
    Static /// PID is set to a fixed value
};

/**
 * @brief Converts enum to number
 *
 * @param shiny Input shiny type
 *
 * @return Converted number
 */
constexpr u8 toInt(Shiny shiny)
{
    return static_cast<u8>(shiny);
};

#endif // SHINY_HPP
