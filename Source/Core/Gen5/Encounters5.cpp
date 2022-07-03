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

#include "Encounters5.hpp"
#include <Core/Enum/Encounter.hpp>
#include <Core/Enum/Game.hpp>
#include <Core/Gen5/EncounterArea5.hpp>
#include <Core/Gen5/HiddenGrottoArea.hpp>
#include <Core/Gen5/Profile5.hpp>
#include <Core/Parents/PersonalInfo.hpp>
#include <Core/Parents/PersonalLoader.hpp>
#include <Core/Parents/Slot.hpp>
#include <Core/Resources/EncounterData5.hpp>
#include <Core/Util/Utilities.hpp>
#include <algorithm>
#include <iterator>

constexpr std::array<u8, 12> bwLocations = { 2, 44, 45, 46, 47, 48, 49, 73, 84, 88, 93, 94 };
constexpr std::array<u8, 13> bw2Locations = { 2, 23, 24, 25, 26, 27, 28, 43, 107, 111, 116, 117, 129 };

namespace Encounters5
{
    const DreamRadarTemplate *getDreamRadarEncounters(int *size)
    {
        if (size)
        {
            *size = dreamRadar.size();
        }
        return dreamRadar.data();
    }

    const DreamRadarTemplate *getDreamRadarEncounters(int index)
    {
        return &dreamRadar[index];
    }

    std::vector<HiddenGrottoArea> getHiddenGrottoEncounters()
    {
        u32 length;
        const u8 *data = Utilities::decompress(bw2_grotto.data(), bw2_grotto.size(), length);

        const PersonalInfo *info = PersonalLoader::getPersonal(Game::BW2);

        std::vector<HiddenGrottoArea> encounters;
        for (size_t offset = 0; offset < length; offset += 125)
        {
            const u8 *entry = data + offset;

            u8 location = entry[0];

            std::array<HiddenGrottoSlot, 12> pokemon;
            for (size_t i = 0; i < 12; i++)
            {
                u16 specie = *reinterpret_cast<const u16 *>(entry + 1 + i * 5);
                u8 maxLevel = entry[3 + i * 5];
                u8 minLevel = entry[4 + i * 5];
                u8 gender = entry[5 + i * 5];
                pokemon[i] = HiddenGrottoSlot(specie, gender, minLevel, maxLevel, &info[specie]);
            }

            std::array<u16, 16> items;
            for (size_t i = 0; i < 16; i++)
            {
                items[i] = *reinterpret_cast<const u16 *>(entry + 61 + i * 2);
            }

            std::array<u16, 16> hiddenItems;
            for (size_t i = 0; i < 16; i++)
            {
                hiddenItems[i] = *reinterpret_cast<const u16 *>(entry + 93 + i * 2);
            }

            encounters.emplace_back(location, pokemon, items, hiddenItems);
        }
        delete[] data;
        return encounters;
    }

    std::vector<EncounterArea5> getEncounters(Encounter encounter, u8 season, const Profile5 *profile)
    {
        const u8 *compressedData;
        size_t compressedLength;

        Game version = profile->getVersion();
        if (version == Game::Black)
        {
            compressedData = black.data();
            compressedLength = black.size();
        }
        else if (version == Game::Black2)
        {
            compressedData = black2.data();
            compressedLength = black2.size();
        }
        else if (version == Game::White)
        {
            compressedData = white.data();
            compressedLength = white.size();
        }
        else
        {
            compressedData = white2.data();
            compressedLength = white2.size();
        }

        u32 length;
        u8 *data = Utilities::decompress(compressedData, compressedLength, length);

        std::vector<EncounterArea5> encounters;
        for (size_t offset = 0; offset < length;)
        {
            const u8 *entry = data + offset;

            u8 location = entry[0];

            int size;
            if ((version & Game::BW) != Game::None)
            {
                size = std::binary_search(bwLocations.begin(), bwLocations.end(), location) ? 928 : 232;
            }
            else
            {
                size = std::binary_search(bw2Locations.begin(), bw2Locations.end(), location) ? 928 : 232;
            }

            int seasonOffset = 0;
            if (season != 0 && size != 232)
            {
                seasonOffset = (season - 1) * 232;
            }

            u8 grass = entry[seasonOffset + 1];
            u8 grassDouble = entry[seasonOffset + 2];
            u8 grassSpecial = entry[seasonOffset + 3];
            u8 surf = entry[seasonOffset + 4];
            u8 surfSpecial = entry[seasonOffset + 5];
            u8 fish = entry[seasonOffset + 6];
            u8 fishSpecial = entry[seasonOffset + 7];

            switch (encounter)
            {
            case Encounter::Grass:
                if (grass != 0)
                {
                    std::vector<Slot> slots;
                    slots.reserve(12);
                    for (int i = 0; i < 12; i++)
                    {
                        u16 species = *reinterpret_cast<const u16 *>(entry + seasonOffset + 9 + (i * 4));
                        u8 level = entry[seasonOffset + 10 + (i * 4)];
                        slots.emplace_back(species & 0x7ff, level, PersonalLoader::getPersonal(version, species & 0x7ff, species >> 11));
                    }
                    encounters.emplace_back(location, grass, encounter, slots);
                }
                break;
            case Encounter::DoubleGrass:
                if (grassDouble != 0)
                {
                    std::vector<Slot> slots;
                    slots.reserve(12);
                    for (int i = 0; i < 12; i++)
                    {
                        u16 species = *reinterpret_cast<const u16 *>(entry + seasonOffset + 57 + (i * 4));
                        u8 level = entry[seasonOffset + 58 + (i * 4)];
                        slots.emplace_back(species & 0x7ff, level, PersonalLoader::getPersonal(version, species & 0x7ff, species >> 11));
                    }
                    encounters.emplace_back(location, grassDouble, encounter, slots);
                }
                break;
            case Encounter::SpecialGrass:
                if (grassSpecial != 0)
                {
                    std::vector<Slot> slots;
                    slots.reserve(12);
                    for (int i = 0; i < 12; i++)
                    {
                        u16 species = *reinterpret_cast<const u16 *>(entry + seasonOffset + 105 + (i * 4));
                        u8 level = entry[seasonOffset + 106 + (i * 4)];
                        slots.emplace_back(species & 0x7ff, level, PersonalLoader::getPersonal(version, species & 0x7ff, species >> 11));
                    }
                    encounters.emplace_back(location, grassSpecial, encounter, slots);
                }
                break;
            case Encounter::Surfing:
                if (surf != 0)
                {
                    std::vector<Slot> slots;
                    slots.reserve(5);
                    for (int i = 0; i < 5; i++)
                    {
                        u16 species = *reinterpret_cast<const u16 *>(entry + seasonOffset + 153 + (i * 4));
                        u8 min = entry[seasonOffset + 154 + (i * 4)];
                        u8 max = entry[seasonOffset + 155 + (i * 4)];
                        slots.emplace_back(species & 0x7ff, min, max, PersonalLoader::getPersonal(version, species & 0x7ff, species >> 11));
                    }
                    encounters.emplace_back(location, surf, encounter, slots);
                }
                break;
            case Encounter::SpecialSurf:
                if (surfSpecial != 0)
                {
                    std::vector<Slot> slots;
                    slots.reserve(5);
                    for (int i = 0; i < 5; i++)
                    {
                        u16 species = *reinterpret_cast<const u16 *>(entry + seasonOffset + 173 + (i * 4));
                        u8 min = entry[seasonOffset + 174 + (i * 4)];
                        u8 max = entry[seasonOffset + 175 + (i * 4)];
                        slots.emplace_back(species & 0x7ff, min, max, PersonalLoader::getPersonal(version, species & 0x7ff, species >> 11));
                    }
                    encounters.emplace_back(location, surfSpecial, encounter, slots);
                }
                break;
            case Encounter::SuperRod:
                if (fish != 0)
                {
                    std::vector<Slot> slots;
                    slots.reserve(5);
                    for (int i = 0; i < 5; i++)
                    {
                        u16 species = *reinterpret_cast<const u16 *>(entry + seasonOffset + 193 + (i * 4));
                        u8 min = entry[seasonOffset + 194 + (i * 4)];
                        u8 max = entry[seasonOffset + 195 + (i * 4)];
                        slots.emplace_back(species & 0x7ff, min, max, PersonalLoader::getPersonal(version, species & 0x7ff, species >> 11));
                    }
                    encounters.emplace_back(location, fish, encounter, slots);
                }
                break;
            case Encounter::SpecialSuperRod:
                if (fishSpecial != 0)
                {
                    std::vector<Slot> slots;
                    slots.reserve(5);
                    for (int i = 0; i < 5; i++)
                    {
                        u16 species = *reinterpret_cast<const u16 *>(entry + seasonOffset + 213 + (i * 4));
                        u8 min = entry[seasonOffset + 214 + (i * 4)];
                        u8 max = entry[seasonOffset + 215 + (i * 4)];
                        slots.emplace_back(species & 0x7ff, min, max, PersonalLoader::getPersonal(version, species & 0x7ff, species >> 11));
                    }
                    encounters.emplace_back(location, fishSpecial, encounter, slots);
                }
                break;
            default:
                break;
            }

            offset += 1 + size;
        }
        return encounters;
    }

    const StaticTemplate *getStaticEncounters(int index, int *size)
    {
        if (index == 0)
        {
            if (size)
            {
                *size = starters.size();
            }
            return starters.data();
        }
        else if (index == 1)
        {
            if (size)
            {
                *size = fossils.size();
            }
            return fossils.data();
        }
        else if (index == 2)
        {
            if (size)
            {
                *size = gifts.size();
            }
            return gifts.data();
        }
        else if (index == 3)
        {
            if (size)
            {
                *size = stationary.size();
            }
            return stationary.data();
        }
        else if (index == 4)
        {
            if (size)
            {
                *size = legends.size();
            }
            return legends.data();
        }
        else if (index == 5)
        {
            if (size)
            {
                *size = events.size();
            }
            return events.data();
        }
        else
        {
            if (size)
            {
                *size = roamers.size();
            }
            return roamers.data();
        }
    }

    const StaticTemplate *getStaticEncounter(int type, int index)
    {
        const StaticTemplate *templates = getStaticEncounters(type);
        return &templates[index];
    }
}
