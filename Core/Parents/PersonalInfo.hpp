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

#ifndef PERSONALINFO_HPP
#define PERSONALINFO_HPP

#include <Core/Global.hpp>
#include <array>

/**
 * @brief Contains species information such as base stats, gender, abilities, forms, etc.
 */
class PersonalInfo
{
public:
    /**
     * @brief Construct a new PersonalInfo object
     *
     * @param stats Base stats
     * @param types Pokemon types
     * @param items Pokemon items
     * @param gender Gender ratio
     * @param ability Pokemon abilities
     * @param formCount Alternate form count
     * @param formStatIndex Alternate form index
     * @param hatchSpecie Base egg specie
     * @param present Obtainable in-game
     */
    constexpr PersonalInfo(const std::array<u8, 6> &stats, const std::array<u8, 2> &types, const std::array<u16, 3> &item, u8 gender,
                           const std::array<u16, 3> &ability, u8 formCount, u16 formStatIndex, u16 hatchSpecie, bool present) :
        ability(ability),
        item(item),
        formCount(formCount),
        formStatIndex(formStatIndex),
        hatchSpecie(hatchSpecie),
        stats(stats),
        present(present),
        gender(gender),
        types(types)
    {
    }

    /**
     * @brief Returns the ability specified by \p index
     *
     * @param index Ability index
     *
     * @return Ability
     */
    u16 getAbility(int index) const
    {
        return ability[index];
    }

    /**
     * @brief Returns the item specified by \p index
     *
     * @param index Item index
     *
     * @return Item
     */
    u16 getItem(int index) const
    {
        return item[index];
    }

    /**
     * @brief Determines if the pokemon has a fixed gender
     *
     * @return true Pokemon has a fixed gender
     * @return false Pokemon does not have a fixed gender
     */
    bool getFixedGender() const
    {
        return gender == 0 || gender == 254 || gender == 255;
    }

    /**
     * @brief Number of alternate forms the pokemon has
     *
     * @return Form count
     */
    u8 getFormCount() const
    {
        return formCount;
    }

    /**
     * @brief Base index for the alternative form
     *
     * @return Form index
     */
    u16 getFormStatIndex() const
    {
        return formStatIndex;
    }

    /**
     * @brief Gender ratio of the pokemon
     *
     * @return Gender ratio value
     * - 0: Male only
     * - 31: 12.5% female
     * - 63: 25% female
     * - 127: 50% female
     * - 191: 75% female
     * - 254: Female only
     * - 255: Genderless
     */
    u8 getGender() const
    {
        return gender;
    }

    /**
     * @brief Hatch specie of the pokemon
     *
     * @return Pokemon base hatch specie
     */
    u16 getHatchSpecie() const
    {
        return hatchSpecie;
    }

    /**
     * @brief Determines whether the pokemon is obtainable or not
     *
     * @return true Pokemon is obtainable
     * @return false Pokemon is not obtainable
     */
    bool getPresent() const
    {
        return present;
    }

    /**
     * @brief Base stats of the pokemon
     *
     * @return Array of base stats
     */
    std::array<u8, 6> getStats() const
    {
        return stats;
    }

    /**
     * @brief Individual base stat of the pokemon
     *
     * @param index Base stat index
     *
     * @return Base stat
     */
    u8 getStat(int index) const
    {
        return stats[index];
    }

    /**
     * @brief Type of the pokemon
     *
     * @param index Type index
     *
     * @return Pokemon type
     */
    u8 getType(int index) const
    {
        return types[index];
    }

private:
    std::array<u16, 3> ability;
    std::array<u16, 3> item;
    u16 formCount : 5;
    u16 formStatIndex : 11;
    u16 hatchSpecie;
    std::array<u8, 6> stats;
    bool present;
    u8 gender;
    std::array<u8, 2> types;
};

#endif // PERSONALINFO_HPP
