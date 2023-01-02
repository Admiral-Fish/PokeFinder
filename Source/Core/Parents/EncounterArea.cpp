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
#include <Core/Enum/Game.hpp>
#include <Core/Enum/Lead.hpp>
#include <Core/Parents/PersonalInfo.hpp>
#include <Core/Parents/Slot.hpp>
#include <Core/Util/Translator.hpp>
#include <algorithm>

EncounterArea::EncounterArea(u8 location, u8 rate, Encounter encounter, const std::vector<Slot> &pokemon) :
    location(location), rate(rate), encounter(encounter), pokemon(pokemon)
{
}

u8 EncounterArea::calculateLevel(u8 index, u16 prng) const
{
    u8 range = pokemon[index].getMaxLevel() - pokemon[index].getMinLevel() + 1;
    return (prng % range) + pokemon[index].getMinLevel();
}

u8 EncounterArea::calculateLevel(u8 index) const
{
    return pokemon[index].getMaxLevel();
}

Encounter EncounterArea::getEncounter() const
{
    return encounter;
}

std::pair<u8, u8> EncounterArea::getLevelRange(u16 specie) const
{
    std::pair<u8, u8> range = std::make_pair(100, 0);
    for (const auto &slot : pokemon)
    {
        if (slot.getSpecie() == specie)
        {
            range.first = std::min(range.first, slot.getMinLevel());
            range.second = std::max(range.second, slot.getMaxLevel());
        }
    }
    return range;
}

u8 EncounterArea::getLocation() const
{
    return location;
}

std::vector<Slot> EncounterArea::getPokemon() const
{
    return pokemon;
}

const Slot &EncounterArea::getPokemon(int index) const
{
    return pokemon[index];
}

u8 EncounterArea::getRate() const
{
    return rate;
}

std::vector<bool> EncounterArea::getSlots(u16 specie) const
{
    std::vector<bool> flags(pokemon.size());
    std::transform(pokemon.begin(), pokemon.end(), flags.begin(), [specie](const auto &mon) { return mon.getSpecie() == specie; });
    return flags;
}

std::vector<u8> EncounterArea::getSlots(Game version, Lead lead) const
{
    std::vector<u8> encounters;
    u8 type;
    switch (lead)
    {
    case Lead::MagnetPull:
        type = 8;
        break;
    case Lead::Static:
        type = (version & (Game::Gen3 | Game::Gen4)) != Game::None ? 13 : 12;
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
        return encounters;
    }

    for (size_t i = 0; i < pokemon.size(); i++)
    {
        const PersonalInfo *info = pokemon[i].getInfo();
        if (info->getType(0) == type || info->getType(1) == type)
        {
            encounters.emplace_back(static_cast<u8>(i));
        }
    }
    return encounters;
}

std::vector<std::string> EncounterArea::getSpecieNames() const
{
    return Translator::getSpecies(getUniqueSpecies());
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
