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
#include <Core/Gen3/EncounterArea3.hpp>
#include <Core/Gen3/Profile3.hpp>
#include <Core/Parents/PersonalLoader.hpp>
#include <Core/Parents/Slot.hpp>
#include <Core/Resources/Encounters.hpp>
#include <algorithm>
#include <cstring>
#include <iterator>

namespace Encounters3
{
    namespace
    {
        std::vector<EncounterArea3> getAreas(Encounter encounter, Game game, const PersonalInfo *info)
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

            std::vector<EncounterArea3> encounters;
            for (size_t i = 0; i < size; i += 121)
            {
                const u8 *entry = data + i;

                u8 location = entry[0];

                u8 grass = entry[1];
                if (grass != 0 && encounter == Encounter::Grass) {
                    std::vector<Slot> slots;
                    for (int i = 0; i < 12; i++) {
                        u8 level = entry[5 + (i * 3)];
                        u16 specie = *reinterpret_cast<const u16 *>(entry + 6 + (i * 3));
                        slots.emplace_back(specie, level, info[specie]);
                    }
                    encounters.emplace_back(location, grass, encounter, slots);
                }

                u8 water = entry[2];
                if (water != 0 && encounter == Encounter::Surfing) {
                    std::vector<Slot> slots;
                    for (int i = 0; i < 5; i++)
                    {
                        u8 min = entry[41 + (i * 4)];
                        u8 max = entry[42 + (i * 4)];
                        u16 specie = *reinterpret_cast<const u16 *>(entry + 43 + (i * 4));
                        slots.emplace_back(specie, min, max, info[specie]);
                    }
                    encounters.emplace_back(location, water, encounter, slots);
                }

                u8 rock = entry[3];
                if (rock != 0 && encounter == Encounter::RockSmash) {
                    std::vector<Slot> slots;
                    for (int i = 0; i < 5; i++)
                    {
                        u8 min = entry[61 + (i * 4)];
                        u8 max = entry[62 + (i * 4)];
                        u16 specie = *reinterpret_cast<const u16 *>(entry + 63 + (i * 4));
                        slots.emplace_back(specie, min, max, info[specie]);
                    }
                    encounters.emplace_back(location, rock, encounter, slots);
                }

                u8 fish = entry[4];
                if (fish != 0) {
                    if (encounter == Encounter::OldRod) {
                        std::vector<Slot> slots;
                        for (int i = 0; i < 2; i++)
                        {
                            u8 min = entry[81 + (i * 4)];
                            u8 max = entry[82 + (i * 4)];
                            u16 specie = *reinterpret_cast<const u16 *>(entry + 83 + (i * 4));
                            slots.emplace_back(specie, min, max, info[specie]);
                        }
                        encounters.emplace_back(location, fish, encounter, slots);
                    }
                    else if (encounter == Encounter::GoodRod) {
                        std::vector<Slot> slots;
                        for (int i = 0; i < 3; i++)
                        {
                            u8 min = entry[89 + (i * 4)];
                            u8 max = entry[90 + (i * 4)];
                            u16 specie = *reinterpret_cast<const u16 *>(entry + 91 + (i * 4));
                            slots.emplace_back(specie, min, max, info[specie]);
                        }
                        encounters.emplace_back(location, fish, encounter, slots);
                    }
                    else if (encounter == Encounter::SuperRod)
                    {
                        std::vector<Slot> slots;
                        for (int i = 0; i < 5; i++)
                        {
                            u8 min = entry[101 + (i * 4)];
                            u8 max = entry[102 + (i * 4)];
                            u16 specie = *reinterpret_cast<const u16 *>(entry + 103 + (i * 4));
                            slots.emplace_back(specie, min, max, info[specie]);
                        }
                        encounters.emplace_back(location, fish, encounter, slots);
                    }
                }
            }
            return encounters;
        }
    }

    std::vector<EncounterArea3> getEncounters(Encounter encounter, Game version)
    {
        const auto *info = PersonalLoader::getPersonal(version);
        return getAreas(encounter, version, info);
    }
}
