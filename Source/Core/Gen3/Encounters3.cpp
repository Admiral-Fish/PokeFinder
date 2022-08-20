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

#include "Encounters3.hpp"
#include <Core/Enum/Encounter.hpp>
#include <Core/Enum/Game.hpp>
#include <Core/Gen3/Profile3.hpp>
#include <Core/Parents/EncounterArea.hpp>
#include <Core/Parents/PersonalLoader.hpp>
#include <Core/Parents/Slot.hpp>
#include <Core/Parents/StaticTemplate.hpp>
#include <Core/Resources/Encounters.hpp>
#include <algorithm>
#include <cstring>
#include <iterator>

constexpr std::array<StaticTemplate, 33> gifts = {
    // RSE
    // Starters @ Littleroot Town
    StaticTemplate(Game::Emerald, 152, 5), // Chikorita
    StaticTemplate(Game::Emerald, 155, 5), // Cyndaquil
    StaticTemplate(Game::Emerald, 158, 5), // Totodile

    // Starters @ Route 101
    StaticTemplate(Game::RSE, 252, 5), // Treecko
    StaticTemplate(Game::RSE, 255, 5), // Torchic
    StaticTemplate(Game::RSE, 258, 5), // Mudkip

    // Fossils
    StaticTemplate(Game::RSE, 345, 20), // Lileep
    StaticTemplate(Game::RSE, 347, 20), // Anorith

    // Gift
    StaticTemplate(Game::RSE, 351, 25), // Castform @ Weather Institute
    StaticTemplate(Game::RSE, 374, 5), // Beldum @ Mossdeep City
    StaticTemplate(Game::RSE, 360, 5), // Wynaut Egg

    // FRLG
    // Starters @ Pallet Town
    StaticTemplate(Game::FRLG, 1, 5), // Bulbasaur
    StaticTemplate(Game::FRLG, 4, 5), // Squirtle
    StaticTemplate(Game::FRLG, 7, 5), // Charmander

    // Fossils @ Cinnabar Island
    StaticTemplate(Game::FRLG, 138, 5), // Omanyte
    StaticTemplate(Game::FRLG, 140, 5), // Kabuto
    StaticTemplate(Game::FRLG, 142, 5), // Aerodactyl

    // Gift
    StaticTemplate(Game::FRLG, 106, 25), // Hitmonlee @ Saffron City
    StaticTemplate(Game::FRLG, 107, 25), // Hitmonchan @ Saffron City
    StaticTemplate(Game::FRLG, 129, 5), // Magikarp @ Route 4
    StaticTemplate(Game::FRLG, 131, 25), // Lapras @ Silph Co.
    StaticTemplate(Game::FRLG, 133, 25), // Eevee @ Celadon City
    StaticTemplate(Game::FRLG, 175, 5), // Togepi Egg

    // Celadon City Game Corner
    StaticTemplate(Game::FireRed, 63, 9), // Abra
    StaticTemplate(Game::FireRed, 35, 8), // Clefairy
    StaticTemplate(Game::FireRed, 123, 25), // Scyther
    StaticTemplate(Game::FireRed, 147, 18), // Dratini
    StaticTemplate(Game::FireRed, 137, 26), // Porygon
    StaticTemplate(Game::LeafGreen, 63, 7), // Abra
    StaticTemplate(Game::LeafGreen, 35, 12), // Clefairy
    StaticTemplate(Game::LeafGreen, 127, 18), // Pinsir
    StaticTemplate(Game::LeafGreen, 147, 24), // Dratini
    StaticTemplate(Game::LeafGreen, 137, 18) // Porygon
};

constexpr std::array<StaticTemplate, 6> stationary = {
    // RSE
    StaticTemplate(Game::RSE, 352, 30), // Kecleon @ Route 119/120
    StaticTemplate(Game::RSE, 101, 30), // Electrode @ Magma/Aqua Hideout
    StaticTemplate(Game::Emerald, 185, 40), // Sudowoodo @ Battle Frontier

    // FRLG
    StaticTemplate(Game::FRLG, 143, 30), // Snorlax @ Route 12/16
    StaticTemplate(Game::FRLG, 101, 34), // Electrode @ Power Plant
    StaticTemplate(Game::FRLG, 97, 30) // Hypno @ Berry Forest
};

constexpr std::array<StaticTemplate, 14> legends = {
    // RSE
    StaticTemplate(Game::RSE, 377, 40), // Regirock @ Desert Ruins
    StaticTemplate(Game::RSE, 378, 40), // Regice @ Island Cave
    StaticTemplate(Game::RSE, 379, 40), // Registeel @ Ancient Tomb
    StaticTemplate(Game::Emerald | Game::Ruby, 380, 50), // Latias @ Southern Island
    StaticTemplate(Game::Emerald | Game::Sapphire, 381, 50), // Latios @ Southern Island
    StaticTemplate(Game::Sapphire, 382, 45), // Kyogre @ Cave of Origin
    StaticTemplate(Game::Emerald, 382, 70), // Kyogre @ Marine Cave
    StaticTemplate(Game::Ruby, 383, 45), // Groudon @ Cave of Origin
    StaticTemplate(Game::Emerald, 383, 70), // Groudon @ Terra Cave
    StaticTemplate(Game::RSE, 384, 70), // Rayquaza @ Sky Pillar

    // FRLG
    StaticTemplate(Game::FRLG, 144, 50), // Articuno @ Seafoam Islands
    StaticTemplate(Game::FRLG, 145, 50), // Zapdos @ Power Plant
    StaticTemplate(Game::FRLG, 146, 50), // Moltres @ Mt. Ember
    StaticTemplate(Game::FRLG, 150, 70) // Mewtwo @ Cerulean Cave
};

constexpr std::array<StaticTemplate, 7> mythics = {
    // RSE
    StaticTemplate(Game::Emerald, 151, 30), // Mew @ Faraway Island
    StaticTemplate(Game::Emerald, 249, 70), // Lugia @ Navel Rock
    StaticTemplate(Game::Emerald, 250, 70), // Ho-Oh @ Navel Rock
    StaticTemplate(Game::Emerald, 386, 30), // Deoxys @ Birth Island

    // FRLG
    StaticTemplate(Game::FRLG, 249, 70), // Lugia @ Navel Rock
    StaticTemplate(Game::FRLG, 250, 70), // Ho-Oh @ Navel Rock
    StaticTemplate(Game::FRLG, 386, 30) // Deoxys @ Birth Islands
};

namespace
{
    /**
     * @brief Gets wild encounters for the \p encouner and \p game
     *
     * @param encounter Encounter type
     * @param game Game version
     * @param info Pointer to PersonalInfo array
     *
     * @return Vector of wild encounters
     */
    std::vector<EncounterArea> getAreas(Encounter encounter, Game game, const PersonalInfo *info)
    {
        const u8 *data;
        size_t size;

        if (game == Game::Emerald)
        {
            data = emerald.data();
            size = emerald.size();
        }
        else if (game == Game::FireRed)
        {
            data = firered.data();
            size = firered.size();
        }
        else if (game == Game::LeafGreen)
        {
            data = leafgreen.data();
            size = leafgreen.size();
        }
        else if (game == Game::Ruby)
        {
            data = ruby.data();
            size = ruby.size();
        }
        else
        {
            data = sapphire.data();
            size = sapphire.size();
        }

        std::vector<EncounterArea> encounters;
        for (size_t offset = 0; offset < size; offset += 121)
        {
            const u8 *entry = data + offset;

            u8 location = entry[0];
            u8 grass = entry[1];
            u8 water = entry[2];
            u8 rock = entry[3];
            u8 fish = entry[4];

            switch (encounter)
            {
            case Encounter::Grass:
                if (grass != 0)
                {
                    std::vector<Slot> slots;
                    slots.reserve(12);
                    for (size_t i = 0; i < 12; i++)
                    {
                        u8 level = entry[5 + (i * 3)];
                        u16 specie = *reinterpret_cast<const u16 *>(entry + 6 + (i * 3));
                        slots.emplace_back(specie, level, info[specie]);
                    }
                    encounters.emplace_back(location, grass, encounter, slots);
                }
                break;
            case Encounter::Surfing:
                if (water != 0)
                {
                    std::vector<Slot> slots;
                    slots.reserve(5);
                    for (size_t i = 0; i < 5; i++)
                    {
                        u8 min = entry[41 + (i * 4)];
                        u8 max = entry[42 + (i * 4)];
                        u16 specie = *reinterpret_cast<const u16 *>(entry + 43 + (i * 4));
                        slots.emplace_back(specie, min, max, info[specie]);
                    }
                    encounters.emplace_back(location, water, encounter, slots);
                }
                break;
            case Encounter::RockSmash:
                if (rock != 0)
                {
                    std::vector<Slot> slots;
                    slots.reserve(5);
                    for (size_t i = 0; i < 5; i++)
                    {
                        u8 min = entry[61 + (i * 4)];
                        u8 max = entry[62 + (i * 4)];
                        u16 specie = *reinterpret_cast<const u16 *>(entry + 63 + (i * 4));
                        slots.emplace_back(specie, min, max, info[specie]);
                    }
                    encounters.emplace_back(location, rock, encounter, slots);
                }
                break;
            case Encounter::OldRod:
                if (fish != 0)
                {
                    std::vector<Slot> slots;
                    slots.reserve(2);
                    for (size_t i = 0; i < 2; i++)
                    {
                        u8 min = entry[81 + (i * 4)];
                        u8 max = entry[82 + (i * 4)];
                        u16 specie = *reinterpret_cast<const u16 *>(entry + 83 + (i * 4));
                        slots.emplace_back(specie, min, max, info[specie]);
                    }
                    encounters.emplace_back(location, fish, encounter, slots);
                }
                break;
            case Encounter::GoodRod:
                if (fish != 0)
                {
                    std::vector<Slot> slots;
                    slots.reserve(3);
                    for (size_t i = 0; i < 3; i++)
                    {
                        u8 min = entry[89 + (i * 4)];
                        u8 max = entry[90 + (i * 4)];
                        u16 specie = *reinterpret_cast<const u16 *>(entry + 91 + (i * 4));
                        slots.emplace_back(specie, min, max, info[specie]);
                    }
                    encounters.emplace_back(location, fish, encounter, slots);
                }
                break;
            case Encounter::SuperRod:
                if (fish != 0)
                {
                    std::vector<Slot> slots;
                    slots.reserve(5);
                    for (size_t i = 0; i < 5; i++)
                    {
                        u8 min = entry[101 + (i * 4)];
                        u8 max = entry[102 + (i * 4)];
                        u16 specie = *reinterpret_cast<const u16 *>(entry + 103 + (i * 4));
                        slots.emplace_back(specie, min, max, info[specie]);
                    }
                    encounters.emplace_back(location, fish, encounter, slots);
                }
                break;
            default:
                break;
            }
        }
        return encounters;
    }
}

namespace Encounters3
{
    std::vector<EncounterArea> getEncounters(Encounter encounter, Game version)
    {
        return getAreas(encounter, version, PersonalLoader::getPersonal(version));
    }

    const StaticTemplate *getStaticEncounters(int type, size_t *size)
    {
        if (type == 0)
        {
            if (size)
            {
                *size = gifts.size();
            }
            return gifts.data();
        }
        else if (type == 1)
        {
            if (size)
            {
                *size = stationary.size();
            }
            return stationary.data();
        }
        else if (type == 2)
        {
            if (size)
            {
                *size = legends.size();
            }
            return legends.data();
        }
        else
        {
            if (size)
            {
                *size = mythics.size();
            }
            return mythics.data();
        }
    }

    const StaticTemplate *getStaticEncounter(int type, int index)
    {
        const StaticTemplate *templates = getStaticEncounters(type);
        return &templates[index];
    }
}
