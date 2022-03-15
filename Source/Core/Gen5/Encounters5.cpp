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
#include <Core/Gen5/EncounterArea5.hpp>
#include <Core/Parents/PersonalLoader.hpp>
#include <Core/Parents/Slot.hpp>
#include <Core/Resources/Encounters.hpp>
#include <algorithm>
#include <cstring>
#include <iterator>

namespace Encounters5
{
    namespace
    {
        constexpr u8 bwLocations[12] = { 2, 44, 45, 46, 47, 48, 49, 73, 84, 88, 93, 94 };
        constexpr u8 bw2Locations[13] = { 2, 23, 24, 25, 26, 27, 28, 43, 107, 111, 116, 117, 129 };

        std::vector<EncounterArea5> getAreas(Encounter encounter, Game version, u8 season)
        {
            const u8 *data;
            size_t size;

            if (version == Game::Black)
            {
                data = black.data();
                size = black.size();
            }
            else if (version == Game::Black2)
            {
                data = black2.data();
                size = black2.size();
            }
            else if (version == Game::White)
            {
                data = white.data();
                size = white.size();
            }
            else
            {
                data = white2.data();
                size = white2.size();
            }

            std::vector<EncounterArea5> encounters;
            for (size_t offset = 0; offset < size;)
            {
                const u8 *entry = data + offset;

                u8 location = entry[0];

                int length = 1;
                if ((version & Game::BW) != Game::None)
                {
                    length += std::binary_search(std::begin(bwLocations), std::end(bwLocations), location) ? 928 : 232;
                }
                else
                {
                    length += std::binary_search(std::begin(bw2Locations), std::end(bw2Locations), location) ? 928 : 232;
                }

                int offset = 0;
                if (season != 0 && length != 232)
                {
                    offset = (season - 1) * 232;
                }

                u8 grass = entry[offset + 1];
                u8 grassDouble = entry[offset + 2];
                u8 grassSpecial = entry[offset + 3];
                u8 surf = entry[offset + 4];
                u8 surfSpecial = entry[offset + 5];
                u8 fish = entry[offset + 6];
                u8 fishSpecial = entry[offset + 7];

                if (grass != 0 && encounter == Encounter::Grass)
                {
                    std::vector<Slot> slots;
                    for (int i = 0; i < 12; i++)
                    {
                        u16 species = *reinterpret_cast<const u16 *>(entry + offset + 9 + (i * 4));
                        u8 level = entry[offset + 10 + (i * 4)];
                        slots.emplace_back(species & 0x7ff, level, PersonalLoader::getPersonal(version, species & 0x7ff, species >> 11));
                    }
                    encounters.emplace_back(location, grass, Encounter::Grass, slots);
                }

                if (grassDouble != 0 && encounter == Encounter::DoubleGrass)
                {
                    std::vector<Slot> slots;
                    for (int i = 0; i < 12; i++)
                    {
                        u16 species = *reinterpret_cast<const u16 *>(entry + offset + 57 + (i * 4));
                        u8 level = entry[offset + 58 + (i * 4)];
                        slots.emplace_back(species & 0x7ff, level, PersonalLoader::getPersonal(version, species & 0x7ff, species >> 11));
                    }
                    encounters.emplace_back(location, grassDouble, Encounter::DoubleGrass, slots);
                }

                if (grassSpecial != 0 && encounter == Encounter::SpecialGrass)
                {
                    std::vector<Slot> slots;
                    for (int i = 0; i < 12; i++)
                    {
                        u16 species = *reinterpret_cast<const u16 *>(entry + offset + 105 + (i * 4));
                        u8 level = entry[offset + 106 + (i * 4)];
                        slots.emplace_back(species & 0x7ff, level, PersonalLoader::getPersonal(version, species & 0x7ff, species >> 11));
                    }
                    encounters.emplace_back(location, grassSpecial, Encounter::SpecialGrass, slots);
                }

                if (surf && encounter == Encounter::Surfing)
                {
                    std::vector<Slot> slots;
                    for (int i = 0; i < 5; i++)
                    {
                        u16 species = *reinterpret_cast<const u16 *>(entry + offset + 153 + (i * 4));
                        u8 min = entry[offset + 154 + (i * 4)];
                        u8 max = entry[offset + 155 + (i * 4)];
                        slots.emplace_back(species & 0x7ff, min, max, PersonalLoader::getPersonal(version, species & 0x7ff, species >> 11));
                    }
                    encounters.emplace_back(location, surf, Encounter::Surfing, slots);
                }

                if (surfSpecial && encounter == Encounter::SpecialSurf)
                {
                    std::vector<Slot> slots;
                    for (int i = 0; i < 5; i++)
                    {
                        u16 species = *reinterpret_cast<const u16 *>(entry + offset + 173 + (i * 4));
                        u8 min = entry[offset + 174 + (i * 4)];
                        u8 max = entry[offset + 175 + (i * 4)];
                        slots.emplace_back(species & 0x7ff, min, max, PersonalLoader::getPersonal(version, species & 0x7ff, species >> 11));
                    }
                    encounters.emplace_back(location, surfSpecial, Encounter::SpecialSurf, slots);
                }

                if (fish && encounter == Encounter::SuperRod)
                {
                    std::vector<Slot> slots;
                    for (int i = 0; i < 5; i++)
                    {
                        u16 species = *reinterpret_cast<const u16 *>(entry + offset + 193 + (i * 4));
                        u8 min = entry[offset + 194 + (i * 4)];
                        u8 max = entry[offset + 195 + (i * 4)];
                        slots.emplace_back(species & 0x7ff, min, max, PersonalLoader::getPersonal(version, species & 0x7ff, species >> 11));
                    }
                    encounters.emplace_back(location, fish, Encounter::SuperRod, slots);
                }

                if (fishSpecial && encounter == Encounter::SpecialSuperRod)
                {
                    std::vector<Slot> slots;
                    for (int i = 0; i < 5; i++)
                    {
                        u16 species = *reinterpret_cast<const u16 *>(entry + offset + 213 + (i * 4));
                        u8 min = entry[offset + 214 + (i * 4)];
                        u8 max = entry[offset + 215 + (i * 4)];
                        slots.emplace_back(species & 0x7ff, min, max, PersonalLoader::getPersonal(version, species & 0x7ff, species >> 11));
                    }
                    encounters.emplace_back(location, fishSpecial, Encounter::SpecialSuperRod, slots);
                }

                offset += length;
            }
            return encounters;
        }
    }

    std::vector<EncounterArea5> getEncounters(Encounter encounter, u8 season, Game version)
    {
        return getAreas(encounter, version, season);
    }
}
