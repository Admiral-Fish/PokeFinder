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
     * @param hp Base HP
     * @param atk Base Atk
     * @param def Base Def
     * @param spa Base SpA
     * @param spd Base SpD
     * @param spe Base Spe
     * @param type1 1st type
     * @param type2 2nd type
     * @param item1 1st item
     * @param item2 2nd item
     * @param item3 3rd item
     * @param gender Gender ratio
     * @param ability1 1st Ability
     * @param ability2 2nd Ability
     * @param abilityH Hidden Ability
     * @param formCount Alternate form count
     * @param formStatIndex Alternate form index
     * @param present Obtainable in-game
     */
    constexpr PersonalInfo(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe, u8 type1, u8 type2, u16 item1, u16 item2, u16 item3, u8 gender,
                           u16 ability1, u16 ability2, u16 abilityH, u8 formCount, u16 formStatIndex, bool present) :
        ability { ability1, ability2, abilityH },
        item { item1, item2, item3 },
        formCount(formCount),
        formStatIndex(formStatIndex),
        stats { hp, atk, def, spa, spd, spe },
        present(present),
        gender(gender),
        types { type1, type2 }
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

    u16 getItem(int index) const
    {
        return item[index];
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
    u16 ability[3];
    u16 item[3];
    u16 formCount : 5;
    u16 formStatIndex : 11;
    std::array<u8, 6> stats;
    bool present;
    u8 gender;
    u8 types[2];
};

#endif // PERSONALINFO_HPP
