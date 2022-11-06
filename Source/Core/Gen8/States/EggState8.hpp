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

#ifndef EGGSTATE8_HPP
#define EGGSTATE8_HPP

#include <Core/Parents/States/EggState.hpp>

class PersonalInfo;

/**
 * @brief State class for Gen8 static generator encounters
 */
class EggState8 : public EggGeneratorState
{
public:
    /**
     * @brief Construct a new GeneratorState8 object
     *
     * @param advances State advances
     * @param gender Pokemon gender
     * @param nature Pokemon nature
     * @param ability Pokemon ability
     * @param inheritance Pokemon IV inheritance
     * @param ivs Pokemon IVs
     * @param pid PID value
     * @param shiny Pokemon shininess
     * @param info Pokemon personal information
     */
    EggState8(u32 advances, u8 gender, u8 nature, u8 ability, const std::array<u8, 6> &inheritance, const std::array<u8, 6> &ivs, u32 pid,
              u8 shiny, const PersonalInfo *info);
};
#endif // EGGSTATE8_HPP
