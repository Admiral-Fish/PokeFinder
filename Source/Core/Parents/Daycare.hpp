/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2021 by Admiral_Fish, bumba, and EzPzStreamz
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
#include <array>

enum Game : u16;

class Daycare
{
public:
    Daycare() = default;
    Daycare(const std::array<std::array<u8, 6>, 2> &parentIVs, const std::array<u8, 2> &parentAbility,
            const std::array<u8, 2> &parentGender, const std::array<u8, 2> &parentItem, const std::array<u8, 2> &parentNature, bool masuda,
            bool nidoranVolbeat);
    u8 getParentIV(u8 parent, u8 index) const;
    u8 getParentAbility(u8 parent) const;
    u8 getParentGender(u8 parent) const;
    u8 getParentItem(u8 parent) const;
    u8 getParentNature(u8 parent) const;
    u8 getEverstoneCount(Game version) const;
    u8 getPowerItemCount() const;
    bool getDitto() const;
    bool getMasuda() const;
    bool getNidoranVolbeat() const;

private:
    std::array<std::array<u8, 6>, 2> parentIVs;
    std::array<u8, 2> parentAbility;
    std::array<u8, 2> parentGender; // 3 - Ditto
    std::array<u8, 2> parentItem; // 0 - No item, 1 - Everstone, 2-7 Power items
    std::array<u8, 2> parentNature;
    bool masuda;
    bool nidoranVolbeat;
};

#endif // DAYCARE_HPP
