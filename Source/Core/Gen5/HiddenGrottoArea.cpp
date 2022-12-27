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

#include "HiddenGrottoArea.hpp"
#include <Core/Util/Translator.hpp>

HiddenGrottoArea::HiddenGrottoArea(u8 location, const std::array<HiddenGrottoSlot, 12> &pokemon, const std::array<u16, 16> &item,
                                   const std::array<u16, 16> &hiddenItem) :
    pokemon(pokemon), item(item), hiddenItem(hiddenItem), location(location)
{
}

u16 HiddenGrottoArea::getItem(u8 group, u8 index) const
{
    return item[group * 4 + index];
}

std::vector<std::string> HiddenGrottoArea::getItemNames() const
{
    return Translator::getItems(getUniqueItems());
}

u16 HiddenGrottoArea::getHiddenItem(u8 group, u8 index) const
{
    return hiddenItem[group * 4 + index];
}

u8 HiddenGrottoArea::getLocation() const
{
    return location;
}

HiddenGrottoSlot HiddenGrottoArea::getPokemon(u8 group, u8 index) const
{
    return pokemon[group * 3 + index];
}

std::vector<std::string> HiddenGrottoArea::getSpecieNames() const
{
    return Translator::getSpecies(getUniqueSpecies());
}

std::vector<u16> HiddenGrottoArea::getUniqueItems() const
{
    std::vector<u16> nums;
    for (u16 i : item)
    {
        if (std::find(nums.begin(), nums.end(), i) == nums.end())
        {
            nums.emplace_back(i);
        }
    }

    for (u16 i : hiddenItem)
    {
        if (std::find(nums.begin(), nums.end(), i) == nums.end())
        {
            nums.emplace_back(i);
        }
    }

    return nums;
}

std::vector<u16> HiddenGrottoArea::getUniqueSpecies() const
{
    std::vector<u16> nums;
    for (const auto &mon : pokemon)
    {
        if (std::find(nums.begin(), nums.end(), mon.getSpecie()) == nums.end())
        {
            nums.emplace_back(mon.getSpecie());
        }
    }
    return nums;
}
