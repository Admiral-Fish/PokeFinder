/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2021 by Admiral_Fish, bumba, and EzPzStreamz
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
#include <Core/Resources/Resources.hpp>
#include <algorithm>
#include <cstring>
#include <iterator>

namespace Encounters5
{
    namespace
    {
        constexpr u8 bwLocations[12] = { 2, 44, 45, 46, 47, 48, 49, 73, 84, 88, 93, 94 };
        constexpr u8 bw2Locations[13] = { 2, 23, 24, 25, 26, 27, 28, 43, 107, 111, 116, 117, 129 };

        std::vector<std::vector<u8>> getData(Game game)
        {
            const u8 *data;
            size_t size;

            if (game == Game::Black)
            {
                data = black.data();
                size = black.size();
            }
            else if (game == Game::Black2)
            {
                data = black2.data();
                size = black2.size();
            }
            else if (game == Game::White)
            {
                data = white.data();
                size = white.size();
            }
            else
            {
                data = white2.data();
                size = white2.size();
            }

            std::vector<std::vector<u8>> encounters;
            int count = 0;
            for (size_t i = 0; i < size;)
            {
                int length;
                if (game & Game::BW)
                {
                    if (std::binary_search(std::begin(bwLocations), std::end(bwLocations), count))
                    {
                        length = 928;
                    }
                    else
                    {
                        length = 232;
                    }
                }
                else
                {
                    if (std::binary_search(std::begin(bw2Locations), std::end(bw2Locations), count))
                    {
                        length = 928;
                    }
                    else
                    {
                        length = 232;
                    }
                }

                std::vector<u8> entry(length);
                std::memcpy(entry.data(), data + i, size);
                encounters.emplace_back(entry);

                i += size;
                count++;
            }

            return encounters;
        }

        u16 getValue(const std::vector<u8> &data, int offset)
        {
            return static_cast<u16>(data[offset + 1] << 8) | data[offset];
        }

        PersonalInfo getInfo(const std::vector<PersonalInfo> &info, u16 species, u8 form)
        {
            auto personal = info[species];
            if (form != 0 && personal.getFormStatIndex() != 0)
            {
                personal = info[personal.getFormStatIndex() + form - 1];
            }
            return personal;
        }

        std::vector<EncounterArea5> getAreas(const std::vector<u8> &data, Encounter encounter, const std::vector<PersonalInfo> &info,
                                             u8 location, u8 season)
        {
            std::vector<EncounterArea5> encounters;

            int offset = 0;
            if (season != 0 && data.size() != 232)
            {
                offset = (season - 1) * 232;
            }

            // Grass
            if (data[offset] != 0 && encounter == Encounter::Grass)
            {
                std::vector<Slot> grass;
                for (int i = 0; i < 12; i++)
                {
                    u16 species = getValue(data, offset + 8 + i * 4);
                    u8 min = data[offset + 10 + i * 4];
                    u8 max = data[offset + 11 + i * 4];
                    grass.emplace_back(species & 0x7ff, min, max, getInfo(info, species & 0x7ff, species >> 11));
                }
                encounters.emplace_back(location, Encounter::Grass, grass);
            }

            // Double Grass
            if (data[offset + 1] != 0 && encounter == Encounter::DoubleGrass)
            {
                std::vector<Slot> doubleGrass;
                for (int i = 0; i < 12; i++)
                {
                    u16 species = getValue(data, offset + 56 + i * 4);
                    u8 min = data[offset + 58 + i * 4];
                    u8 max = data[offset + 59 + i * 4];
                    doubleGrass.emplace_back(species & 0x7ff, min, max, getInfo(info, species & 0x7ff, species >> 11));
                }
                encounters.emplace_back(location, Encounter::DoubleGrass, doubleGrass);
            }

            // Special Grass
            if (data[offset + 2] != 0 && encounter == Encounter::SpecialGrass)
            {
                std::vector<Slot> specialGrass;
                for (int i = 0; i < 12; i++)
                {
                    u16 species = getValue(data, offset + 104 + i * 4);
                    u8 min = data[offset + 106 + i * 4];
                    u8 max = data[offset + 107 + i * 4];
                    specialGrass.emplace_back(species & 0x7ff, min, max, getInfo(info, species & 0x7ff, species >> 11));
                }
                encounters.emplace_back(location, Encounter::SpecialGrass, specialGrass);
            }

            // Surf
            if (data[offset + 3] != 0 && encounter == Encounter::Surfing)
            {
                std::vector<Slot> surf;
                for (int i = 0; i < 5; i++)
                {
                    u16 species = getValue(data, offset + 152 + i * 4);
                    u8 min = data[offset + 154 + i * 4];
                    u8 max = data[offset + 155 + i * 4];
                    surf.emplace_back(species & 0x7ff, min, max, getInfo(info, species & 0x7ff, species >> 11));
                }
                encounters.emplace_back(location, Encounter::Surfing, surf);
            }

            // Special Surf
            if (data[offset + 4] != 0 && encounter == Encounter::SpecialSurf)
            {
                std::vector<Slot> specialSurf;
                for (int i = 0; i < 5; i++)
                {
                    u16 species = getValue(data, offset + 172 + i * 4);
                    u8 min = data[offset + 174 + i * 4];
                    u8 max = data[offset + 175 + i * 4];
                    specialSurf.emplace_back(species & 0x7ff, min, max, getInfo(info, species & 0x7ff, species >> 11));
                }
                encounters.emplace_back(location, Encounter::SpecialSurf, specialSurf);
            }

            // Fish
            if (data[offset + 5] != 0 && encounter == Encounter::SuperRod)
            {
                std::vector<Slot> fish;
                for (int i = 0; i < 5; i++)
                {
                    u16 species = getValue(data, offset + 192 + i * 4);
                    u8 min = data[offset + 194 + i * 4];
                    u8 max = data[offset + 195 + i * 4];
                    fish.emplace_back(species & 0x7ff, min, max, getInfo(info, species & 0x7ff, species >> 11));
                }
                encounters.emplace_back(location, Encounter::SuperRod, fish);
            }

            // Special Fish
            if (data[offset + 6] != 0 && encounter == Encounter::SpecialSuperRod)
            {
                std::vector<Slot> specialFish;
                for (int i = 0; i < 5; i++)
                {
                    u16 species = getValue(data, offset + 212 + i * 4);
                    u8 min = data[offset + 214 + i * 4];
                    u8 max = data[offset + 215 + i * 4];
                    specialFish.emplace_back(species & 0x7ff, min, max, getInfo(info, species & 0x7ff, species >> 11));
                }
                encounters.emplace_back(location, Encounter::SpecialSuperRod, specialFish);
            }

            return encounters;
        }
    }

    std::vector<EncounterArea5> getEncounters(Encounter encounter, u8 season, Game version)
    {
        std::vector<EncounterArea5> encounters;
        std::vector<PersonalInfo> info = PersonalLoader5::getPersonal();

        const auto &encounterData = getData(version);
        for (size_t i = 0; i < encounterData.size(); i++)
        {
            auto areas = getAreas(encounterData[i], encounter, info, i, season);
            encounters.insert(encounters.end(), areas.begin(), areas.end());
        }

        return encounters;
    }
}
