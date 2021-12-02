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

#include "Encounters3.hpp"
#include <Core/Enum/Encounter.hpp>
#include <Core/Enum/Game.hpp>
#include <Core/Gen3/EncounterArea3.hpp>
#include <Core/Gen3/Profile3.hpp>
#include <Core/Parents/PersonalLoader.hpp>
#include <Core/Parents/Slot.hpp>
#include <Core/Resources/Encounters.hpp>
#include <algorithm>
#include <array>
#include <cstring>
#include <iterator>

namespace Encounters3
{
    namespace
    {
        std::vector<std::array<u8, 120>> getData(Game game)
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

            std::vector<std::array<u8, 120>> encounters;
            for (size_t i = 0; i < size; i += 120)
            {
                std::array<u8, 120> entry;
                std::memcpy(entry.data(), data + i, 120);
                encounters.emplace_back(entry);
            }

            return encounters;
        }

        u16 getValue(const std::array<u8, 120> &data, int offset)
        {
            return static_cast<u16>(data[offset] << 8) | data[offset + 1];
        }

        std::vector<EncounterArea3> getArea(const std::array<u8, 120> &data, const PersonalInfo *info)
        {
            std::vector<EncounterArea3> encounters;

            u8 location = data[0];

            if (data[1] == 1 || data[1] == 2)
            {
                std::vector<Slot> grass;
                for (u8 i = 0; i < 12; i++)
                {
                    u8 level = data[4 + i * 3];
                    u16 specie = getValue(data, 5 + i * 3);
                    grass.emplace_back(specie, level, info[specie]);
                }

                u8 val = data[1];
                encounters.emplace_back(location, val == 1 ? Encounter::Grass : Encounter::SafariZone, grass);
            }
            if (data[2] == 1)
            {
                std::vector<Slot> rock;
                for (u8 i = 0; i < 5; i++)
                {
                    u8 minLevel = data[40 + i * 4];
                    u8 maxLevel = data[41 + i * 4];
                    u16 specie = getValue(data, 42 + i * 4);
                    rock.emplace_back(specie, minLevel, maxLevel, info[specie]);
                }
                encounters.emplace_back(location, Encounter::RockSmash, rock);
            }
            if (data[3] == 1)
            {
                std::vector<Slot> surf;
                for (u8 i = 0; i < 5; i++)
                {
                    u8 minLevel = data[60 + i * 4];
                    u8 maxLevel = data[61 + i * 4];
                    u16 specie = getValue(data, 62 + i * 4);
                    surf.emplace_back(specie, minLevel, maxLevel, info[specie]);
                }
                encounters.emplace_back(location, Encounter::Surfing, surf);

                std::vector<Slot> old;
                for (u8 i = 0; i < 2; i++)
                {
                    u8 minLevel = data[80 + i * 4];
                    u8 maxLevel = data[81 + i * 4];
                    u16 specie = getValue(data, 82 + i * 4);
                    old.emplace_back(specie, minLevel, maxLevel, info[specie]);
                }
                encounters.emplace_back(location, Encounter::OldRod, old);

                std::vector<Slot> good;
                for (u8 i = 0; i < 3; i++)
                {
                    u8 minLevel = data[88 + i * 4];
                    u8 maxLevel = data[89 + i * 4];
                    u16 specie = getValue(data, 90 + i * 4);
                    good.emplace_back(specie, minLevel, maxLevel, info[specie]);
                }
                encounters.emplace_back(location, Encounter::GoodRod, good);

                std::vector<Slot> super;
                for (u8 i = 0; i < 5; i++)
                {
                    u8 minLevel = data[100 + i * 4];
                    u8 maxLevel = data[101 + i * 4];
                    u16 specie = getValue(data, 102 + i * 4);
                    super.emplace_back(specie, minLevel, maxLevel, info[specie]);
                }
                encounters.emplace_back(location, Encounter::SuperRod, super);
            }
            return encounters;
        }
    }

    std::vector<EncounterArea3> getEncounters(Encounter encounter, Game version)
    {
        const auto *info = PersonalLoader::getPersonal(version);

        std::vector<EncounterArea3> encounters;
        for (const auto &data : getData(version))
        {
            auto areas = getArea(data, info);
            std::copy_if(areas.begin(), areas.end(), std::back_inserter(encounters),
                         [&encounter](const EncounterArea3 &area) { return area.getEncounter() == encounter; });
        }

        return encounters;
    }
}
