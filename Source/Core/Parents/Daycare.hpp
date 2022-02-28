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

#ifndef DAYCARE_HPP
#define DAYCARE_HPP

#include <Core/Util/Global.hpp>
#include <algorithm>
#include <array>

class Daycare
{
public:
    Daycare(const std::array<std::array<u8, 6>, 2> &parentIVs, const std::array<u8, 2> &parentAbility,
            const std::array<u8, 2> &parentGender, const std::array<u8, 2> &parentItem, const std::array<u8, 2> &parentNature, bool masuda,
            bool nidoranVolbeat) :
        parentIVs(parentIVs),
        parentAbility(parentAbility),
        parentGender(parentGender),
        parentItem(parentItem),
        parentNature(parentNature),
        masuda(masuda),
        nidoranVolbeat(nidoranVolbeat)
    {
    }

    u8 getParentIV(u8 parent, u8 index) const
    {
        return parentIVs[parent][index];
    }

    u8 getParentAbility(u8 parent) const
    {
        return parentAbility[parent];
    }

    u8 getParentGender(u8 parent) const
    {
        return parentGender[parent];
    }

    u8 getParentItem(u8 parent) const
    {
        return parentItem[parent];
    }

    u8 getParentNature(u8 parent) const
    {
        return parentNature[parent];
    }

    u8 getEverstoneCount() const
    {
        return std::count_if(parentItem.begin(), parentItem.end(), [](u8 item) { return item == 1; });
    }

    u8 getPowerItemCount() const
    {
        return std::count_if(parentItem.begin(), parentItem.end(), [](u8 item) { return item >= 2 && item <= 7; });
    }

    bool getDitto() const
    {
        return std::any_of(parentGender.begin(), parentGender.end(), [](u8 gender) { return gender == 3; });
    }

    bool getMasuda() const
    {
        return masuda;
    }

    bool getNidoranVolbeat() const
    {
        return nidoranVolbeat;
    }

private:
    std::array<std::array<u8, 6>, 2> parentIVs;
    std::array<u8, 2> parentAbility;
    std::array<u8, 2> parentGender; // 0 - Male, 1 - Female, 2 - Genderless, 3 - Ditto
    std::array<u8, 2> parentItem; // 0 - No item, 1 - Everstone, 2-7 Power items, 8 Destiny knot
    std::array<u8, 2> parentNature;
    bool masuda;
    bool nidoranVolbeat;
};

#endif // DAYCARE_HPP
