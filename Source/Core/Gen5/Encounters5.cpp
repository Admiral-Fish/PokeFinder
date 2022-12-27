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

#include "Encounters5.hpp"
#include <Core/Enum/Encounter.hpp>
#include <Core/Enum/Game.hpp>
#include <Core/Gen5/DreamRadarTemplate.hpp>
#include <Core/Gen5/EncounterArea5.hpp>
#include <Core/Gen5/HiddenGrottoArea.hpp>
#include <Core/Gen5/Profile5.hpp>
#include <Core/Parents/PersonalLoader.hpp>
#include <Core/Parents/Slot.hpp>
#include <Core/Resources/Encounters.hpp>
#include <Core/Util/Utilities.hpp>
#include <algorithm>
#include <iterator>

constexpr u8 bwLocations[12] = { 2, 44, 45, 46, 47, 48, 49, 73, 84, 88, 93, 94 };
constexpr u8 bw2Locations[13] = { 2, 23, 24, 25, 26, 27, 28, 43, 107, 111, 116, 117, 129 };

constexpr std::array<DreamRadarTemplate, 26> dreamRadar = {
    DreamRadarTemplate(79), // Slowpoke
    DreamRadarTemplate(120), // Staryu
    DreamRadarTemplate(137), // Porygon
    DreamRadarTemplate(163), // Hoothoot
    DreamRadarTemplate(174), // Igglybuff
    DreamRadarTemplate(175), // Togepi
    DreamRadarTemplate(213), // Shuckle
    DreamRadarTemplate(238), // Smoochum
    DreamRadarTemplate(249), // Lugia
    DreamRadarTemplate(250), // Ho-Oh
    DreamRadarTemplate(280), // Ralts
    DreamRadarTemplate(333), // Swablu
    DreamRadarTemplate(374), // Beldum
    DreamRadarTemplate(425), // Drifloon
    DreamRadarTemplate(436), // Bronzor
    DreamRadarTemplate(442), // Spiritomb
    DreamRadarTemplate(447), // Riolu
    DreamRadarTemplate(479, 255), // Rotom
    DreamRadarTemplate(483), // Dialga
    DreamRadarTemplate(484), // Palkia
    DreamRadarTemplate(487), // Giratina
    DreamRadarTemplate(517), // Munna
    DreamRadarTemplate(561), // Sigilyph
    DreamRadarTemplate(641), // Tornadus
    DreamRadarTemplate(642), // Thundurus
    DreamRadarTemplate(645) // Landorus
};

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
                size = std::binary_search(std::begin(bwLocations), std::end(bwLocations), location) ? 928 : 232;
            }
            else
            {
                size = std::binary_search(std::begin(bw2Locations), std::end(bw2Locations), location) ? 928 : 232;
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
}
