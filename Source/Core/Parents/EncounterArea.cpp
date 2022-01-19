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

#include "EncounterArea.hpp"
#include <Core/Enum/Encounter.hpp>
#include <Core/Parents/Slot.hpp>
#include <Core/Util/Translator.hpp>
#include <algorithm>

EncounterArea::EncounterArea(u16 location, Encounter encounter, const std::vector<Slot> &pokemon) :
    pokemon(pokemon), location(location), encounter(encounter)
{
}

Encounter EncounterArea::getEncounter() const
{
    return encounter;
}

u16 EncounterArea::getLocation() const
{
    return location;
}

std::vector<Slot> EncounterArea::getPokemon() const
{
    return pokemon;
}

std::vector<u16> EncounterArea::getUniqueSpecies() const
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

std::vector<bool> EncounterArea::getSlots(u16 num) const
{
    std::vector<bool> flags(pokemon.size());

    std::transform(pokemon.begin(), pokemon.end(), flags.begin(), [num](const auto &mon) { return mon.getSpecie() == num; });

    return flags;
}

std::pair<u8, u8> EncounterArea::getLevelRange(u16 specie) const
{
    std::pair<u8, u8> range = std::make_pair(100, 0);
    for (auto slot : pokemon)
    {
        if (slot.getSpecie() == specie)
        {
            range.first = std::min(range.first, slot.getMinLevel());
            range.second = std::max(range.second, slot.getMaxLevel());
        }
    }
    return range;
}

std::vector<std::string> EncounterArea::getSpecieNames() const
{
    return Translator::getSpecies(getUniqueSpecies());
}
