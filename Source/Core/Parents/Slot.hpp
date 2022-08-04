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

#ifndef SLOT_HPP
#define SLOT_HPP

#include <Core/Global.hpp>
#include <Core/Parents/PersonalInfo.hpp>

/**
 * @brief Contains information that impact the generation of wild encounters
 */
class Slot
{
public:
    /**
     * @brief Creates a new Slot
     * @param specie Species of the pokemon
     * @param minLevel Minimum level of the pokemon
     * @param maxLevel Maximum level of the pokemon
     * @param info Pokemon information
     */
    Slot(u16 specie, u8 minLevel, u8 maxLevel, const PersonalInfo &info) :
        minLevel(minLevel), maxLevel(maxLevel), specie(specie), info(info)
    {
    }

    /**
     * @brief Creates a new Slot
     * @param specie Species of the pokemon
     * @param level Level of the pokemon
     * @param info Pokemon information
     */
    Slot(u16 specie, u8 level, const PersonalInfo &info) : minLevel(level), maxLevel(level), specie(specie), info(info)
    {
    }

    /**
     * @brief Determines the minimum level of the pokemon
     * @return Minimum level
     */
    u8 getMinLevel() const
    {
        return minLevel;
    }

    /**
     * @brief Determines the maximum level of the pokemon
     * @return Maximum level
     */
    u8 getMaxLevel() const
    {
        return maxLevel;
    }

    /**
     * @brief Determines the species of the pokemon
     * @return Pokemon species
     */
    u16 getSpecie() const
    {
        return specie;
    }

    /**
     * @brief Determines the information of the pokemon
     * @return Pokemon information
     */
    PersonalInfo getInfo() const
    {
        return info;
    }

    /**
     * @brief Allows the specie to be changed. Typically due to some modifier (radar, swarms, etc.)
     * @param specie Pokemon species
     * @param info Pokemon information
     */
    void setSpecie(u16 specie, const PersonalInfo &info)
    {
        this->specie = specie;
        this->info = info;
    }

private:
    u8 minLevel;
    u8 maxLevel;
    u16 specie;
    PersonalInfo info;
};

#endif // SLOT_HPP
