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
     * @param gender Gender ratio
     * @param ability1 1st Ability
     * @param ability2 2nd Ability
     * @param abilityH Hidden Ability
     * @param formCount Alternate form count
     * @param formStatIndex Alternate form index
     * @param present Obtainable in-game
     */
    constexpr PersonalInfo(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe, u8 gender, u16 ability1, u16 ability2, u16 abilityH, u8 formCount,
                           u16 formStatIndex, bool present) :
        stats { hp, atk, def, spa, spd, spe },
        gender(gender),
        ability1(ability1),
        ability2(ability2),
        abilityH(abilityH),
        formCount(formCount),
        formStatIndex(formStatIndex),
        present(present)
    {
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
    u16 getGender() const
    {
        return gender;
    }

    /**
     * @brief First pokemon ability
     *
     * @return Ability 1
     */
    u16 getAbility1() const
    {
        return ability1;
    }

    /**
     * @brief Second pokemon ability
     *
     * @return Ability 2
     */
    u16 getAbility2() const
    {
        return ability2;
    }

    /**
     * @brief Hidden pokemon ability
     *
     * @return Hidden ability
     */
    u16 getAbilityH() const
    {
        return abilityH;
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
     * @brief Determines whether the pokemon is obtainable or not
     *
     * @return true Pokemon is obtainable
     * @return false Pokemon is not obtainable
     */
    bool getPresent() const
    {
        return present;
    }

private:
    std::array<u8, 6> stats;
    u8 gender;
    u16 ability1;
    u16 ability2;
    u16 abilityH;
    u8 formCount;
    u16 formStatIndex;
    bool present;
};

#endif // PERSONALINFO_HPP
