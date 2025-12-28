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

#ifndef HIDDENGROTTOAREA_HPP
#define HIDDENGROTTOAREA_HPP

#include <Core/Global.hpp>
#include <array>
#include <string>
#include <vector>

class PersonalInfo;

/**
 * @brief Contains information that impact the generation of hidden grotto encounters
 */
class HiddenGrottoSlot
{
public:
    /**
     * @brief Construct a new HiddenGrottoSlot object
     */
    HiddenGrottoSlot() = default;

    /**
     * @brief Construct a new HiddenGrottoSlot object
     *
     * @param specie Species of the pokemon
     * @param gender Gender ratio of the pokemon
     * @param minLevel Minimum level of the pokemon
     * @param maxLevel Maximum level of the pokemon
     * @param info Pokemon information
     */
    HiddenGrottoSlot(u16 specie, u8 gender, u8 minLevel, u8 maxLevel, const PersonalInfo *info) :
        info(info), specie(specie), gender(gender), maxLevel(maxLevel), minLevel(minLevel)
    {
    }

    /**
     * @brief Determines the gender ratio of the pokemon
     *
     * @return Pokemon gender ratio
     */
    u8 getGender() const
    {
        return gender;
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

private:
    const PersonalInfo *info;
    u16 specie;
    u8 gender;
    u8 maxLevel;
    u8 minLevel;
};

/**
 * @brief Contains information about the encounters for an area. This includes location, rate, and the slots.
 */
class HiddenGrottoArea
{
public:
    /**
     * @brief Construct a new HiddenGrottoArea object
     *
     * @param location Location number
     * @param pokemon Available pokemon of the area
     * @param item Available items of the area
     * @param hiddenItem Available hidden items of the area
     */
    HiddenGrottoArea(u8 location, const std::array<HiddenGrottoSlot, 12> &pokemon, const std::array<u16, 16> &item,
                     const std::array<u16, 16> &hiddenItem);

    /**
     * @brief Returns the item of the \p group and \p index
     *
     * @param group Item group
     * @param index Item index
     *
     * @return Item
     */
    u16 getItem(u8 group, u8 index) const;

    /**
     * @brief Return vector of names of all item slots
     *
     * @return Vector of item name
     */
    std::vector<std::string> getItemNames() const;

    /**
     * @brief Returns the item of the \p group and \p index
     *
     * @param group Hidden item group
     * @param index Hidden item index
     *
     * @return Item
     */
    u16 getHiddenItem(u8 group, u8 index) const;

    /**
     * @brief Returns the location of the area
     *
     * @return Location number
     */
    u8 getLocation() const;

    /**
     * @brief Returns the pokemon at the \p group and \p index
     *
     * @param group Pokemon group
     * @param index Pokemon index
     *
     * @return Pokemon
     */
    HiddenGrottoSlot getPokemon(u8 group, u8 index) const;

    /**
     * @brief Return vector of names of all pokemon slots
     *
     * @return Vector of pokemon name
     */
    std::vector<std::string> getSpecieNames() const;

    /**
     * @brief Return the item numbers of unique item of the area
     *
     * @return Vector of item numbers
     */
    std::vector<u16> getUniqueItems() const;

    /**
     * @brief Return the species numbers of unique pokemon of the area
     *
     * @return Vector of pokemon species
     */
    std::vector<u16> getUniqueSpecies() const;

private:
    std::array<HiddenGrottoSlot, 12> pokemon;
    std::array<u16, 16> item;
    std::array<u16, 16> hiddenItem;
    u8 location;
};

#endif // HIDDENGROTTOAREA_HPP
