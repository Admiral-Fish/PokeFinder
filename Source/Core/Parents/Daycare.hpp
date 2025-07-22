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

#ifndef DAYCARE_HPP
#define DAYCARE_HPP

#include <Core/Global.hpp>
#include <array>

/**
 * @brief Contains information about the parents in the daycare
 */
class Daycare
{
public:
    /**
     * @brief Daycare Construct a new Daycare object
     *
     * @param parentIVs Parent IVs
     * @param parentAbility Parent abilities
     * @param parentGender Parent gender
     * @param parentItem Parent items
     * @param parentNature Parent natures
     * @param specie Egg specie
     * @param masuda Parents form international combination
     */
    Daycare(const std::array<std::array<u8, 6>, 2> &parentIVs, const std::array<u8, 2> &parentAbility,
            const std::array<u8, 2> &parentGender, const std::array<u8, 2> &parentItem, const std::array<u8, 2> &parentNature, u16 specie,
            bool masuda) :
        specie(specie),
        parentIVs(parentIVs),
        parentAbility(parentAbility),
        parentGender(parentGender),
        parentItem(parentItem),
        parentNature(parentNature),
        masuda(masuda)
    {
    }

    /**
     * @brief Determines if either of the parents is Ditto
     *
     * @return true Either of the parents is Ditto
     * @return false Neither of the parents is Ditto
     */
    bool getDitto() const
    {
        return parentGender[0] == 3 || parentGender[1] == 3;
    }

    /**
     * @brief Determines the specie of the egg
     *
     * @return Egg specie
     */
    u16 getEggSpecie() const
    {
        return specie;
    }

    /**
     * @brief Determines how many everstones the parents are holding
     *
     * @return Everstone count
     */
    u8 getEverstoneCount() const
    {
        return (parentItem[0] == 1) + (parentItem[1] == 1);
    }

    /**
     * @brief Determines if the parents form a international combo
     *
     * @return true Parents are a Masuda pair
     * @return false Parents are not a Masuda pair
     */
    bool getMasuda() const
    {
        return masuda;
    }

    /**
     * @brief Determines the ability of the \p parent
     *
     * @param parent Parent index
     *
     * @return Parent ability
     */
    u8 getParentAbility(u8 parent) const
    {
        return parentAbility[parent];
    }

    /**
     * @brief Determines the gender of the \p parent
     *
     * @param parent Parent index
     *
     * @return Parent gender
     */
    u8 getParentGender(u8 parent) const
    {
        return parentGender[parent];
    }

    /**
     * @brief Determines the item of the \p parent
     *
     * @param parent Parent index
     *
     * @return Parent item
     */
    u8 getParentItem(u8 parent) const
    {
        return parentItem[parent];
    }

    /**
     * @brief Determines the IV of the \p parent
     *
     * @param parent Parent index
     * @param index IV index
     *
     * @return Parent IV
     */
    u8 getParentIV(u8 parent, u8 index) const
    {
        return parentIVs[parent][index];
    }

    /**
     * @brief Determines the nature of the \p parent
     *
     * @param parent Parent index
     *
     * @return Parent nature
     */
    u8 getParentNature(u8 parent) const
    {
        return parentNature[parent];
    }

    /**
     * @brief Determines how many power items the parents are holding
     *
     * @return Power item count
     */
    u8 getPowerItemCount() const
    {
        return (parentItem[0] >= 2 && parentItem[0] <= 7) + (parentItem[1] >= 2 && parentItem[1] <= 7);
    }

private:
    u16 specie;
    std::array<std::array<u8, 6>, 2> parentIVs;
    std::array<u8, 2> parentAbility;
    std::array<u8, 2> parentGender; // 0 - Male, 1 - Female, 2 - Genderless, 3 - Ditto
    std::array<u8, 2> parentItem; // 0 - No item, 1 - Everstone, 2-7 Power items, 8 Destiny knot
    std::array<u8, 2> parentNature;
    bool masuda;
};

#endif // DAYCARE_HPP
