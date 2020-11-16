/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2020 by Admiral_Fish, bumba, and EzPzStreamz
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
#include <Core/Parents/PersonalInfo.hpp>
#include <QByteArrayList>
#include <QFile>

namespace Encounters5
{
    namespace
    {
        constexpr u8 bwLocations[12] = { 2, 44, 45, 46, 47, 48, 49, 73, 84, 88, 93, 94 };
        constexpr u8 bw2Locations[13] = { 2, 23, 24, 25, 26, 27, 28, 43, 107, 111, 116, 117, 129 };

        QByteArrayList getData(Game game)
        {
            QString path;
            switch (game)
            {
            case Game::Black:
                path = ":/encounters/black.bin";
                break;
            case Game::Black2:
                path = ":/encounters/black2.bin";
                break;
            case Game::White:
                path = ":/encounters/white.bin";
                break;
            case Game::White2:
            default:
                path = ":/encounters/white2.bin";
                break;
            }

            QByteArray data;
            QFile file(path);
            if (file.open(QIODevice::ReadOnly))
            {
                data = file.readAll();
                file.close();
            }

            QByteArrayList encounters;
            int count = 0;
            for (int i = 0; i < data.size();)
            {
                int size;
                if (game & Game::BW)
                {
                    if (std::binary_search(std::begin(bwLocations), std::end(bwLocations), count))
                    {
                        size = 928;
                    }
                    else
                    {
                        size = 232;
                    }
                }
                else
                {
                    if (std::binary_search(std::begin(bw2Locations), std::end(bw2Locations), count))
                    {
                        size = 928;
                    }
                    else
                    {
                        size = 232;
                    }
                }

                encounters.append(data.mid(i, size));
                i += size;
                count++;
            }

            return encounters;
        }

        u16 getValue(const QByteArray &data, int offset)
        {
            u16 high = static_cast<u8>(data.at(offset + 1)) << 8;
            u16 low = static_cast<u8>(data.at(offset));
            return high | low;
        }

        PersonalInfo getInfo(const std::vector<PersonalInfo> &info, u16 species, u8 form)
        {
            auto personal = info.at(species);
            if (form != 0 && personal.getFormStatIndex() != 0)
            {
                personal = info.at(personal.getFormStatIndex() + form - 1);
            }
            return personal;
        }

        std::vector<EncounterArea5> getAreas(const QByteArray &data, Encounter encounter, const std::vector<PersonalInfo> &info,
                                             u8 location, u8 season)
        {
            std::vector<EncounterArea5> encounters;

            int offset = 0;
            if (season != 0 && data.size() != 232)
            {
                offset = (season - 1) * 232;
            }

            // Grass
            if (data.at(offset + 0) != 0 && encounter == Encounter::Grass)
            {
                std::vector<Slot> grass;
                for (int i = 0; i < 12; i++)
                {
                    u16 species = getValue(data, offset + 8 + i * 4);
                    u8 min = data.at(offset + 10 + i * 4);
                    u8 max = data.at(offset + 11 + i * 4);
                    grass.push_back(Slot(species & 0x7ff, min, max, getInfo(info, species & 0x7ff, species >> 11)));
                }
                encounters.push_back(EncounterArea5(location, Encounter::Grass, grass));
            }

            // Double Grass
            if (data.at(offset + 1) != 0 && encounter == Encounter::DoubleGrass)
            {
                std::vector<Slot> doubleGrass;
                for (int i = 0; i < 12; i++)
                {
                    u16 species = getValue(data, offset + 56 + i * 4);
                    u8 min = data.at(offset + 58 + i * 4);
                    u8 max = data.at(offset + 59 + i * 4);
                    doubleGrass.push_back(Slot(species & 0x7ff, min, max, getInfo(info, species & 0x7ff, species >> 11)));
                }
                encounters.push_back(EncounterArea5(location, Encounter::DoubleGrass, doubleGrass));
            }

            // Special Grass
            if (data.at(offset + 2) != 0 && encounter == Encounter::SpecialGrass)
            {
                std::vector<Slot> specialGrass;
                for (int i = 0; i < 12; i++)
                {
                    u16 species = getValue(data, offset + 104 + i * 4);
                    u8 min = data.at(offset + 106 + i * 4);
                    u8 max = data.at(offset + 107 + i * 4);
                    specialGrass.push_back(Slot(species & 0x7ff, min, max, getInfo(info, species & 0x7ff, species >> 11)));
                }
                encounters.push_back(EncounterArea5(location, Encounter::SpecialGrass, specialGrass));
            }

            // Surf
            if (data.at(offset + 3) != 0 && encounter == Encounter::Surfing)
            {
                std::vector<Slot> surf;
                for (int i = 0; i < 5; i++)
                {
                    u16 species = getValue(data, offset + 152 + i * 4);
                    u8 min = data.at(offset + 154 + i * 4);
                    u8 max = data.at(offset + 155 + i * 4);
                    surf.push_back(Slot(species & 0x7ff, min, max, getInfo(info, species & 0x7ff, species >> 11)));
                }
                encounters.push_back(EncounterArea5(location, Encounter::Surfing, surf));
            }

            // Special Surf
            if (data.at(offset + 4) != 0 && encounter == Encounter::SpecialSurf)
            {
                std::vector<Slot> specialSurf;
                for (int i = 0; i < 5; i++)
                {
                    u16 species = getValue(data, offset + 172 + i * 4);
                    u8 min = data.at(offset + 174 + i * 4);
                    u8 max = data.at(offset + 175 + i * 4);
                    specialSurf.push_back(Slot(species & 0x7ff, min, max, getInfo(info, species & 0x7ff, species >> 11)));
                }
                encounters.push_back(EncounterArea5(location, Encounter::SpecialSurf, specialSurf));
            }

            // Fish
            if (data.at(offset + 5) != 0 && encounter == Encounter::SuperRod)
            {
                std::vector<Slot> fish;
                for (int i = 0; i < 5; i++)
                {
                    u16 species = getValue(data, offset + 192 + i * 4);
                    u8 min = data.at(offset + 194 + i * 4);
                    u8 max = data.at(offset + 195 + i * 4);
                    fish.push_back(Slot(species & 0x7ff, min, max, getInfo(info, species & 0x7ff, species >> 11)));
                }
                encounters.push_back(EncounterArea5(location, Encounter::SuperRod, fish));
            }

            // Special Fish
            if (data.at(offset + 6) != 0 && encounter == Encounter::SpecialSuperRod)
            {
                std::vector<Slot> specialFish;
                for (int i = 0; i < 5; i++)
                {
                    u16 species = getValue(data, offset + 212 + i * 4);
                    u8 min = data.at(offset + 214 + i * 4);
                    u8 max = data.at(offset + 215 + i * 4);
                    specialFish.push_back(Slot(species & 0x7ff, min, max, getInfo(info, species & 0x7ff, species >> 11)));
                }
                encounters.push_back(EncounterArea5(location, Encounter::SpecialSuperRod, specialFish));
            }

            return encounters;
        }
    }

    std::vector<EncounterArea5> getEncounters(Encounter encounter, u8 season, Game version)
    {
        std::vector<EncounterArea5> encounters;
        std::vector<PersonalInfo> info = PersonalInfo::loadPersonal(5);

        const auto &encounterData = getData(version);
        for (u8 i = 0; i < encounterData.size(); i++)
        {
            auto areas = getAreas(encounterData.at(i), encounter, info, i, season);
            encounters.insert(encounters.end(), areas.begin(), areas.end());
        }

        return encounters;
    }
}
