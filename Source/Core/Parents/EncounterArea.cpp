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

#include "EncounterArea.hpp"
#include <Core/Enum/Encounter.hpp>
#include <Core/Enum/Game.hpp>
#include <Core/Enum/Lead.hpp>
#include <Core/Parents/PersonalInfo.hpp>
#include <Core/Parents/Slot.hpp>
#include <Core/Util/Translator.hpp>
#include <algorithm>

EncounterArea::EncounterArea(u8 location, u8 rate, Encounter encounter, const std::array<Slot, 12> &pokemon) :
    location(location), rate(rate), encounter(encounter), pokemon(pokemon)
{
}

std::pair<u8, u8> EncounterArea::getLevelRange(u16 specie) const
{
    std::pair<u8, u8> range = std::make_pair(100, 0);
    for (size_t i = 0; i < pokemon.size() && pokemon[i].getSpecie() != 0; i++)
    {
        if (pokemon[i].getSpecie() == specie)
        {
            range.first = std::min(range.first, pokemon[i].getMinLevel());
            range.second = std::max(range.second, pokemon[i].getMaxLevel());
        }
    }
    return range;
}

std::vector<bool> EncounterArea::getSlots(u16 specie) const
{
    std::vector<bool> flags(pokemon.size());
    for (size_t i = 0; i < pokemon.size() && pokemon[i].getSpecie() != 0; i++)
    {
        flags[i] = pokemon[i].getSpecie() == (specie & 0x7ff) && pokemon[i].getForm() == (specie >> 11);
    }
    return flags;
}

ModifiedSlots EncounterArea::getSlots(Lead lead) const
{
    ModifiedSlots modifiedSlots;
    modifiedSlots.count = 0;

    u8 type;
    switch (lead)
    {
    case Lead::MagnetPull:
        type = 8;
        break;
    case Lead::Static:
        type = 12;
        break;
    case Lead::Harvest:
        type = 11;
        break;
    case Lead::FlashFire:
        type = 9;
        break;
    case Lead::StormDrain:
        type = 10;
        break;
    default:
        return modifiedSlots;
    }

    u8 count = 0;
    for (size_t i = 0; i < pokemon.size() && pokemon[i].getInfo() != nullptr; i++)
    {
        const PersonalInfo *info = pokemon[i].getInfo();
        if (info->getType(0) == type || info->getType(1) == type)
        {
            modifiedSlots.encounters[modifiedSlots.count++] = static_cast<u8>(i);
        }
        count++;
    }

    if (modifiedSlots.count == count)
    {
        modifiedSlots.count = 0;
    }

    return modifiedSlots;
}

std::vector<std::string> EncounterArea::getSpecieNames() const
{
    return Translator::getSpecies(getUniqueSpecies());
}

std::vector<u16> EncounterArea::getUniqueSpecies() const
{
    std::vector<u16> nums;
    for (size_t i = 0; i < pokemon.size() && pokemon[i].getSpecie(); i++)
    {
        u16 num = (pokemon[i].getForm() << 11) | pokemon[i].getSpecie();
        if (std::find(nums.begin(), nums.end(), num) == nums.end())
        {
            nums.emplace_back(num);
        }
    }
    return nums;
}
