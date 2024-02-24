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

#ifndef NAZOS_HPP
#define NAZOS_HPP

#include <Core/Global.hpp>
#include <array>

enum class DSType : u8;
enum class Game : u32;
enum class Language : u8;
class Profile5;

/**
 * @brief Computes the first past of the Gen 5 seeding SHA1 input
 */
namespace Nazos
{
    /**
     * @brief Calculates the nazo values
     *
     * @param version Game version
     * @param language Language
     * @param dsType DS type
     *
     * @return Array of nazo values
     */
    std::array<u32, 5> getNazo(Game version, Language language, DSType dsType);

    /**
     * @brief Calculates the nazo values
     *
     * @param profile Profile information
     *
     * @return Array of nazo values
     */
    std::array<u32, 5> getNazo(const Profile5 &profile);
}

#endif // NAZOS_HPP
