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

#ifndef PERSONALLOADER_HPP
#define PERSONALLOADER_HPP

#include <Core/Global.hpp>

class PersonalInfo;
enum class Game : u32;

namespace PersonalLoader
{
    /**
     * @brief Gets pointer to PersonalInfo array for \p version
     *
     * @param version Game version
     *
     * @return Pointer to PersonalInfo array
     */
    const PersonalInfo *getPersonal(Game version);

    /**
     * @brief Gets PersonalInfo for \p specie and \p form based on the \p version
     *
     * @param version Game version
     * @param specie Pokemon specie
     * @param form Pokemon form
     *
     * @return PersonalInfo entry pointer
     */
    const PersonalInfo *getPersonal(Game version, u16 specie, u8 form = 0);
}

#endif // PERSONALLOADER_HPP
