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

#include "HiddenGrottoArea.hpp"
#include <Core/Enum/Game.hpp>
#include <Core/Parents/PersonalInfo.hpp>
#include <Core/Parents/PersonalLoader.hpp>
#include <Core/Util/Translator.hpp>
#include <algorithm>

namespace
{
    constexpr u8 pinwheelForestExterior = 8;
    constexpr u8 pinwheelForestInterior = 9;
    constexpr u8 lostlornForest = 126;
    constexpr u8 route6Cave = 135;

    constexpr u16 butterfree = 12;
    constexpr u16 beedrill = 15;
    constexpr u16 nosepass = 299;
    constexpr u16 pinsir = 127;
    constexpr u16 heracross = 214;
    constexpr u16 hariyama = 297;
    constexpr u16 medicham = 308;

    bool isBlack2(Game version)
    {
        return version == Game::Black2;
    }

    HiddenGrottoSlot replaceSpecie(const HiddenGrottoSlot &slot, u16 specie)
    {
        const PersonalInfo *info = PersonalLoader::getPersonal(Game::BW2);
        return HiddenGrottoSlot(specie, slot.getGender(), slot.getMinLevel(), slot.getMaxLevel(), &info[specie]);
    }

    HiddenGrottoSlot replaceGender(const HiddenGrottoSlot &slot, u8 gender)
    {
        return HiddenGrottoSlot(slot.getSpecie(), gender, slot.getMinLevel(), slot.getMaxLevel(), slot.getInfo());
    }
}

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

HiddenGrottoSlot HiddenGrottoArea::getPokemon(u8 group, u8 index, Game version) const
{
    HiddenGrottoSlot slot = getPokemon(group, index);
    if (!isBlack2(version))
    {
        return slot;
    }

    u16 specie = slot.getSpecie();
    if (location == pinwheelForestInterior && specie == butterfree)
    {
        return replaceSpecie(slot, beedrill);
    }
    if (location == lostlornForest)
    {
        if (specie == heracross)
        {
            return replaceSpecie(slot, pinsir);
        }
        if (specie == pinsir)
        {
            return replaceSpecie(slot, heracross);
        }
    }
    if (location == pinwheelForestExterior)
    {
        if (specie == hariyama)
        {
            return replaceSpecie(slot, medicham);
        }
        if (specie == medicham)
        {
            return replaceSpecie(slot, hariyama);
        }
    }
    if (location == route6Cave && specie == nosepass)
    {
        return replaceGender(slot, 30);
    }

    return slot;
}

std::vector<std::string> HiddenGrottoArea::getSpecieNames() const
{
    return Translator::getSpecies(getUniqueSpecies());
}

std::vector<std::string> HiddenGrottoArea::getSpecieNames(Game version) const
{
    return Translator::getSpecies(getUniqueSpecies(version));
}

std::vector<u16> HiddenGrottoArea::getUniqueItems() const
{
    std::vector<u16> nums;
    for (u16 i : item)
    {
        if (std::ranges::find(nums, i) == nums.end())
        {
            nums.emplace_back(i);
        }
    }

    for (u16 i : hiddenItem)
    {
        if (std::ranges::find(nums, i) == nums.end())
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
        if (std::ranges::find(nums, mon.getSpecie()) == nums.end())
        {
            nums.emplace_back(mon.getSpecie());
        }
    }
    return nums;
}

std::vector<u16> HiddenGrottoArea::getUniqueSpecies(Game version) const
{
    std::vector<u16> nums;
    for (u8 group = 0; group < 4; group++)
    {
        for (u8 slot = 0; slot < 3; slot++)
        {
            HiddenGrottoSlot mon = getPokemon(group, slot, version);
            if (std::ranges::find(nums, mon.getSpecie()) == nums.end())
            {
                nums.emplace_back(mon.getSpecie());
            }
        }
    }
    return nums;
}
