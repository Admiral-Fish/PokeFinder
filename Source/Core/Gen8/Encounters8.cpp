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

#include "Encounters8.hpp"
#include <Core/Enum/Encounter.hpp>
#include <Core/Enum/Game.hpp>
#include <Core/Gen8/EncounterArea8.hpp>
#include <Core/Gen8/Profile8.hpp>
#include <Core/Parents/PersonalLoader.hpp>
#include <Core/Parents/Slot.hpp>
#include <Core/Resources/Encounters.hpp>
#include <algorithm>
#include <cstring>
#include <iterator>

namespace Encounters4
{
    namespace
    {
        std::vector<std::vector<u8>> getData(Game game)
        {
            const u8 *data;
            size_t size;
            int offset;

            if (game == Game::Diamond)
            {
                data = bd.data();
                size = bd.size();
                offset = 140;
            }
            else if (game == Game::Pearl)
            {
                data = sp.data();
                size = sp.size();
                offset = 140;
            }

            std::vector<std::vector<u8>> encounters;
            for (size_t i = 0; i < size; i += offset)
            {
                std::vector<u8> entry(offset);
                std::memcpy(entry.data(), data + i, offset);
                encounters.emplace_back(entry);
            }

            return encounters;
        }

        u16 getValue(const std::vector<u8> &data, int offset)
        {
            return static_cast<u16>(data[offset] << 8) | data[offset + 1];
        }

        void modifySwarm(std::vector<Slot> &mons, const std::vector<u8> &data, const PersonalInfo *info, bool swarm)
        {
            if (swarm)
            {
                u16 species[2] = { getValue(data, 40), getValue(data, 42) };
                mons[0].setSpecie(species[0], info[species[0]]);
                mons[1].setSpecie(species[1], info[species[1]]);
            }
        }

        void modifyTime(std::vector<Slot> &mons, const std::vector<u8> &data, const PersonalInfo *info, int time)
        {
            u16 specie1;
            u16 specie2;
            if (time == 1)
            {
                specie1 = getValue(data, 44);
                specie2 = getValue(data, 46);
            }
            else if (time == 2)
            {
                specie1 = getValue(data, 48);
                specie2 = getValue(data, 50);
            }
            else
            {
                return;
            }

            mons[2].setSpecie(specie1, info[specie1]);
            mons[3].setSpecie(specie2, info[specie2]);
        }

        void modifyRadar(std::vector<Slot> &mons, const std::vector<u8> &data, const PersonalInfo *info, bool radar)
        {
            if (radar)
            {
                u16 species[4] = { getValue(data, 52), getValue(data, 54), getValue(data, 56), getValue(data, 58) };
                mons[4].setSpecie(species[0], info[species[0]]);
                mons[5].setSpecie(species[1], info[species[1]]);
                mons[10].setSpecie(species[2], info[species[2]]);
                mons[11].setSpecie(species[3], info[species[3]]);
            }
        }

        std::vector<EncounterArea8> getBDSP(const std::vector<u8> &data, const PersonalInfo *info, const Profile8 &profile, int time)
        {
            std::vector<EncounterArea8> encounters;
            u16 location = getValue(data, 0);

            // Grass
            if (data[2] == 1)
            {
                std::vector<Slot> grass;
                for (int i = 0; i < 12; i++)
                {
                    u8 level = data[4 + i * 3];
                    u16 specie = getValue(data, 5 + i * 3);
                    grass.emplace_back(specie, level, info[specie]);
                }

                modifyTime(grass, data, info, time);
                modifyRadar(grass, data, info, profile.getRadar());
                modifySwarm(grass, data, info, profile.getSwarm());

                encounters.emplace_back(location, Encounter::Grass, grass);
            }

            // Water
            if (data[3] == 1)
            {
                std::vector<Slot> surf;
                std::vector<Slot> old;
                std::vector<Slot> good;
                std::vector<Slot> super;
                for (int i = 0; i < 5; i++)
                {
                    u8 min = data[60 + i * 4];
                    u8 max = data[61 + i * 4];
                    u16 specie = getValue(data, 62 + i * 4);
                    surf.emplace_back(specie, min, max, info[specie]);

                    min = data[80 + i * 4];
                    max = data[81 + i * 4];
                    specie = getValue(data, 82 + i * 4);
                    old.emplace_back(specie, min, max, info[specie]);

                    min = data[100 + i * 4];
                    max = data[101 + i * 4];
                    specie = getValue(data, 102 + i * 4);
                    good.emplace_back(specie, min, max, info[specie]);

                    min = data[120 + i * 4];
                    max = data[121 + i * 4];
                    specie = getValue(data, 122 + i * 4);
                    super.emplace_back(specie, min, max, info[specie]);
                }

                encounters.emplace_back(location, Encounter::Surfing, surf);
                encounters.emplace_back(location, Encounter::OldRod, old);
                encounters.emplace_back(location, Encounter::GoodRod, good);
                encounters.emplace_back(location, Encounter::SuperRod, super);
            }
            return encounters;
        }
    }

    std::vector<EncounterArea8> getEncounters(Encounter encounter, int time, const Profile8 &profile)
    {
        auto *info = PersonalLoader::getPersonal(profile.getVersion());

        std::vector<EncounterArea8> encounters;
        for (const auto &data : getData(profile.getVersion()))
        {
            auto areas = getBDSP(data, info, profile, time);
            std::copy_if(areas.begin(), areas.end(), std::back_inserter(encounters),
                         [&encounter](const EncounterArea8 &area) { return area.getEncounter() == encounter; });
        }

        return encounters;
    }
}
