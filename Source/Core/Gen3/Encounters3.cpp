/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2023 by Admiral_Fish, bumba, and EzPzStreamz
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
#include <Core/Gen3/EncounterArea3.hpp>
#include <Core/Gen3/LockInfo.hpp>
#include <Core/Gen3/Profile3.hpp>
#include <Core/Parents/PersonalInfo.hpp>
#include <Core/Parents/PersonalLoader.hpp>
#include <Core/Parents/Slot.hpp>
#include <Core/Resources/EncounterData3.hpp>
#include <Core/Util/Utilities.hpp>
#include <algorithm>
#include <iterator>

namespace Encounters3
{
    std::vector<EncounterArea3> getEncounters(Encounter encounter, Game version)
    {
        const u8 *compressedData;
        size_t compressedLength;

        if (version == Game::Emerald)
        {
            compressedData = emerald.data();
            compressedLength = emerald.size();
        }
        else if (version == Game::FireRed)
        {
            compressedData = firered.data();
            compressedLength = firered.size();
        }
        else if (version == Game::LeafGreen)
        {
            compressedData = leafgreen.data();
            compressedLength = leafgreen.size();
        }
        else if (version == Game::Ruby)
        {
            compressedData = ruby.data();
            compressedLength = ruby.size();
        }
        else
        {
            compressedData = sapphire.data();
            compressedLength = sapphire.size();
        }

        u32 length;
        u8 *data = Utilities::decompress(compressedData, compressedLength, length);

        const PersonalInfo *info = PersonalLoader::getPersonal(version);

        std::vector<EncounterArea3> encounters;
        for (size_t offset = 0; offset < length; offset += 121)
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
                        u16 species = *reinterpret_cast<const u16 *>(entry + 6 + (i * 3));
                        slots.emplace_back(species & 0x7ff, species >> 11, level, level, &info[species & 0x7ff]);
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
                        slots.emplace_back(specie, min, max, &info[specie]);
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
                        slots.emplace_back(specie, min, max, &info[specie]);
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
                        slots.emplace_back(specie, min, max, &info[specie]);
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
                        slots.emplace_back(specie, min, max, &info[specie]);
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
                        slots.emplace_back(specie, min, max, &info[specie]);
                    }
                    encounters.emplace_back(location, fish, encounter, slots);
                }
                break;
            default:
                break;
            }
        }
        delete[] data;
        return encounters;
    }

    std::vector<EncounterArea> getPokeSpotEncounters()
    {
        u32 length;
        u8 *data = Utilities::decompress(xd.data(), xd.size(), length);

        const PersonalInfo *info = PersonalLoader::getPersonal(Game::Gen3);

        std::vector<EncounterArea> encounters;
        for (size_t offset = 0; offset < length; offset += 13)
        {
            const u8 *entry = data + offset;

            u8 location = entry[0];

            std::vector<Slot> slots;
            slots.reserve(3);
            for (size_t i = 0; i < 3; i++)
            {
                u8 min = entry[1 + (i * 4)];
                u8 max = entry[2 + (i * 4)];
                u16 specie = *reinterpret_cast<const u16 *>(entry + 3 + (i * 4));
                slots.emplace_back(specie, min, max, &info[specie]);
            }
            encounters.emplace_back(location, 0, Encounter::Grass, slots);
        }
        delete[] data;
        return encounters;
    }

    const ShadowTemplate *getShadowTeams(size_t *size)
    {
        if (size)
        {
            *size = galesColoShadow.size();
        }
        return galesColoShadow.data();
    }

    const ShadowTemplate *getShadowTeam(int index)
    {
        const ShadowTemplate *templates = getShadowTeams();
        return &templates[index];
    }

    const StaticTemplate *getStaticEncounters(int type, size_t *size)
    {
        if (type == 0)
        {
            if (size)
            {
                *size = starters.size();
            }
            return starters.data();
        }
        else if (type == 1)
        {
            if (size)
            {
                *size = fossils.size();
            }
            return fossils.data();
        }
        else if (type == 2)
        {
            if (size)
            {
                *size = gifts.size();
            }
            return gifts.data();
        }
        else if (type == 3)
        {
            if (size)
            {
                *size = gameCorner.size();
            }
            return gameCorner.data();
        }
        else if (type == 4)
        {
            if (size)
            {
                *size = stationary.size();
            }
            return stationary.data();
        }
        else if (type == 5)
        {
            if (size)
            {
                *size = legends.size();
            }
            return legends.data();
        }
        else if (type == 6)
        {
            if (size)
            {
                *size = events.size();
            }
            return events.data();
        }
        else if (type == 7)
        {
            if (size)
            {
                *size = galesColo.size();
            }
            return galesColo.data();
        }
        else
        {
            if (size)
            {
                *size = channel.size();
            }
            return channel.data();
        }
    }

    const StaticTemplate *getStaticEncounter(int type, int index)
    {
        const StaticTemplate *templates = getStaticEncounters(type);
        return &templates[index];
    }
}
