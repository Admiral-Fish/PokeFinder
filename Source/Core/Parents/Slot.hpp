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

#ifndef SLOT_HPP
#define SLOT_HPP

#include <Core/Global.hpp>

class PersonalInfo;

/**
 * @brief Contains information that impact the generation of wild encounters
 */
class Slot
{
public:
    /**
     * @brief Construct a new Slot object
     *
     * @param specie Species of the pokemon
     * @param minLevel Minimum level of the pokemon
     * @param maxLevel Maximum level of the pokemon
     * @param info Pokemon information
     */
    Slot(u16 specie, u8 minLevel, u8 maxLevel, const PersonalInfo *info) :
        info(info), specie(specie), form(0), maxLevel(maxLevel), minLevel(minLevel)
    {
    }

    /**
     * @brief Construct a new Slot object
     *
     * @param specie Species of the pokemon
     * @param form Form of the pokemon
     * @param minLevel Minimum level of the pokemon
     * @param maxLevel Maximum level of the pokemon
     * @param info Pokemon information
     */
    Slot(u16 specie, u8 form, u8 minLevel, u8 maxLevel, const PersonalInfo *info) :
        info(info), specie(specie), form(form), maxLevel(maxLevel), minLevel(minLevel)
    {
    }

    /**
     * @brief Determines the form of the pokemon
     *
     * @return Pokemon form
     */
    u8 getForm() const
    {
        return form;
    }

    /**
     * @brief Determines the information of the pokemon
     *
     * @return Pokemon information
     */
    const PersonalInfo *getInfo() const
    {
        return info;
    }

    /**
     * @brief Determines the maximum level of the pokemon
     *
     * @return Maximum level
     */
    u8 getMaxLevel() const
    {
        return maxLevel;
    }

    /**
     * @brief Determines the minimum level of the pokemon
     *
     * @return Minimum level
     */
    u8 getMinLevel() const
    {
        return minLevel;
    }

    /**
     * @brief Determines the species of the pokemon
     *
     * @return Pokemon species
     */
    u16 getSpecie() const
    {
        return specie;
    }

    /**
     * @brief Allows the specie to be changed. Typically due to some modifier (radar, swarms, etc.)
     *
     * @param specie Pokemon species
     * @param info Pokemon information
     */
    void setSpecie(u16 specie, const PersonalInfo *info)
    {
        this->specie = specie;
        this->info = info;
    }

private:
    const PersonalInfo *info;
    u16 specie;
    u8 form;
    u8 minLevel;
    u8 maxLevel;
};

#endif // SLOT_HPP
