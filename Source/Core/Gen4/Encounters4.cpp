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

#include "Encounters4.hpp"
#include <Core/Enum/Encounter.hpp>
#include <Core/Enum/Game.hpp>
#include <Core/Gen4/EncounterArea4.hpp>
#include <Core/Gen4/Profile4.hpp>
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
                data = diamond.data();
                size = diamond.size();
                offset = 159;
            }
            else if (game == Game::Pearl)
            {
                data = pearl.data();
                size = pearl.size();
                offset = 159;
            }
            else if (game == Game::Platinum)
            {
                data = platinum.data();
                size = platinum.size();
                offset = 159;
            }
            else if (game == Game::HeartGold)
            {
                data = heartgold.data();
                size = heartgold.size();
                offset = 192;
            }
            else
            {
                data = soulsilver.data();
                size = soulsilver.size();
                offset = 192;
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

        void modifyRadio(std::vector<Slot> &mons, const std::vector<u8> &data, const PersonalInfo *info, int radio)
        {
            u16 specie1;
            u16 specie2;
            if (radio == 1)
            {
                specie1 = getValue(data, 88);
                specie2 = getValue(data, 90);
            }
            else if (radio == 2)
            {
                specie1 = getValue(data, 92);
                specie2 = getValue(data, 94);
            }
            else
            {
                return;
            }

            mons[2].setSpecie(specie1, info[specie1]);
            mons[3].setSpecie(specie1, info[specie1]);
            mons[4].setSpecie(specie2, info[specie2]);
            mons[5].setSpecie(specie2, info[specie2]);
        }

        void modifyTime(std::vector<Slot> &mons, const std::vector<u8> &data, const PersonalInfo *info, int time)
        {
            u16 specie1;
            u16 specie2;
            if (time == 1)
            {
                specie1 = getValue(data, 43);
                specie2 = getValue(data, 45);
            }
            else if (time == 2)
            {
                specie1 = getValue(data, 47);
                specie2 = getValue(data, 49);
            }
            else
            {
                return;
            }

            mons[2].setSpecie(specie1, info[specie1]);
            mons[3].setSpecie(specie2, info[specie2]);
        }

        void modifyDual(std::vector<Slot> &mons, const std::vector<u8> &data, const PersonalInfo *info, Game dual)
        {
            u16 specie1;
            u16 specie2;
            if (dual == Game::Ruby)
            {
                specie1 = getValue(data, 59);
                specie2 = getValue(data, 61);
            }
            else if (dual == Game::Sapphire)
            {
                specie1 = getValue(data, 63);
                specie2 = getValue(data, 65);
            }
            else if (dual == Game::Emerald)
            {
                specie1 = getValue(data, 67);
                specie2 = getValue(data, 69);
            }
            else if (dual == Game::FireRed)
            {
                specie1 = getValue(data, 71);
                specie2 = getValue(data, 73);
            }
            else if (dual == Game::LeafGreen)
            {
                specie1 = getValue(data, 75);
                specie2 = getValue(data, 77);
            }
            else
            {
                return;
            }

            mons[8].setSpecie(specie1, info[specie1]);
            mons[9].setSpecie(specie2, info[specie2]);
        }

        void modifyRadar(std::vector<Slot> &mons, const std::vector<u8> &data, const PersonalInfo *info, bool radar)
        {
            if (radar)
            {
                u16 species[4] = { getValue(data, 51), getValue(data, 53), getValue(data, 55), getValue(data, 57) };
                mons[4].setSpecie(species[0], info[species[0]]);
                mons[5].setSpecie(species[1], info[species[1]]);
                mons[10].setSpecie(species[2], info[species[2]]);
                mons[11].setSpecie(species[3], info[species[3]]);
            }
        }

        void modifySwarmHGSS(std::vector<Slot> &mons, const std::vector<u8> &data, const PersonalInfo *info, Encounter encounter,
                             bool swarm)
        {
            if (swarm)
            {
                u16 specie;
                if (encounter == Encounter::Grass)
                {
                    specie = getValue(data, 184);
                }
                else if (encounter == Encounter::Surfing)
                {
                    specie = getValue(data, 186);
                }
                else if (encounter == Encounter::GoodRod)
                {
                    specie = getValue(data, 188);
                }
                else if (encounter == Encounter::SuperRod)
                {
                    specie = getValue(data, 190);
                }
                else
                {
                    return;
                }

                mons[0].setSpecie(specie, info[specie]);
                mons[1].setSpecie(specie, info[specie]);
            }
        }

        void modifySwarmDPPt(std::vector<Slot> &mons, const std::vector<u8> &data, const PersonalInfo *info, bool swarm)
        {
            if (swarm)
            {
                u16 species[2] = { getValue(data, 39), getValue(data, 41) };
                mons[0].setSpecie(species[0], info[species[0]]);
                mons[1].setSpecie(species[1], info[species[1]]);
            }
        }

        std::vector<EncounterArea4> getHGSS(const std::vector<u8> &data, const Profile4 &profile, const PersonalInfo *info,
                                            Encounter encounter, int time)
        {
            std::vector<EncounterArea4> encounters;
            u8 location = data[0];

            // Grass
            if (data[1] == 1)
            {
                int timeOffset = (time * 2) + 1;

                std::vector<Slot> grass;
                for (int i = 0; i < 12; i++)
                {
                    u8 level = data[4 + i * 7];
                    u16 specie = getValue(data, 4 + timeOffset + i * 7);
                    grass.emplace_back(specie, level, info[specie]);
                }

                modifyRadio(grass, data, info, profile.getRadio());
                modifySwarmHGSS(grass, data, info, encounter, profile.getSwarm());

                encounters.emplace_back(location, Encounter::Grass, grass);
            }

            // Rock Smash
            if (data[2] == 1)
            {
                std::vector<Slot> rock;
                for (int i = 0; i < 2; i++)
                {
                    u8 min = data[96 + i * 4];
                    u8 max = data[97 + i * 4];
                    u16 specie = getValue(data, 98 + i * 4);
                    rock.emplace_back(specie, min, max, info[specie]);
                }
                encounters.emplace_back(location, Encounter::RockSmash, rock);
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
                    u8 min = data[104 + i * 4];
                    u8 max = data[105 + i * 4];
                    u16 specie = getValue(data, 106 + i * 4);
                    surf.emplace_back(specie, min, max, info[specie]);

                    min = data[124 + i * 4];
                    max = data[125 + i * 4];
                    specie = getValue(data, 126 + i * 4);
                    old.emplace_back(specie, min, max, info[specie]);

                    min = data[144 + i * 4];
                    max = data[145 + i * 4];
                    specie = getValue(data, 146 + i * 4);

                    // Replace Corsula with Staryu at night
                    // Replace slot 3 with Gyrados at Mt. Silver 2F at night
                    if (time == 2)
                    {
                        if (specie == 222)
                        {
                            specie = 120;
                        }
                        else if (location == 68 && i == 3)
                        {
                            specie = 130;
                        }
                    }

                    good.emplace_back(specie, min, max, info[specie]);

                    min = data[164 + i * 4];
                    max = data[165 + i * 4];
                    specie = getValue(data, 166 + i * 4);

                    // Replace Corsula with Staryu at night
                    // Replace slot 1 with Gyrados at Mt. Silver 2F at night
                    if (time == 2)
                    {
                        if (specie == 222)
                        {
                            specie = 120;
                        }
                        else if (location == 68 && i == 1)
                        {
                            specie = 130;
                        }
                    }

                    super.emplace_back(specie, min, max, info[specie]);
                }

                modifySwarmHGSS(surf, data, info, encounter, profile.getSwarm());
                encounters.emplace_back(location, Encounter::Surfing, surf);

                encounters.emplace_back(location, Encounter::OldRod, old);

                modifySwarmHGSS(good, data, info, encounter, profile.getSwarm());
                encounters.emplace_back(location, Encounter::GoodRod, good);

                modifySwarmHGSS(super, data, info, encounter, profile.getSwarm());
                encounters.emplace_back(location, Encounter::SuperRod, super);
            }
            return encounters;
        }

        std::vector<EncounterArea4> getDPPt(const std::vector<u8> &data, const Profile4 &profile, const PersonalInfo *info, int time)
        {
            std::vector<EncounterArea4> encounters;
            u8 location = data[0];

            // Grass
            if (data[1] == 1)
            {
                std::vector<Slot> grass;
                for (int i = 0; i < 12; i++)
                {
                    u8 level = data[3 + i * 3];
                    u16 specie = getValue(data, 4 + i * 3);
                    grass.emplace_back(specie, level, info[specie]);
                }

                modifyTime(grass, data, info, time);
                modifyDual(grass, data, info, profile.getDualSlot());
                modifyRadar(grass, data, info, profile.getRadar());
                modifySwarmDPPt(grass, data, info, profile.getSwarm());

                encounters.emplace_back(location, Encounter::Grass, grass);
            }

            // Water
            if (data[2] == 1)
            {
                std::vector<Slot> surf;
                std::vector<Slot> old;
                std::vector<Slot> good;
                std::vector<Slot> super;
                for (int i = 0; i < 5; i++)
                {
                    u8 min = data[79 + i * 4];
                    u8 max = data[80 + i * 4];
                    u16 specie = getValue(data, 81 + i * 4);
                    surf.emplace_back(specie, min, max, info[specie]);

                    min = data[99 + i * 4];
                    max = data[100 + i * 4];
                    specie = getValue(data, 101 + i * 4);
                    old.emplace_back(specie, min, max, info[specie]);

                    min = data[119 + i * 4];
                    max = data[120 + i * 4];
                    specie = getValue(data, 121 + i * 4);
                    good.emplace_back(specie, min, max, info[specie]);

                    min = data[139 + i * 4];
                    max = data[140 + i * 4];
                    specie = getValue(data, 141 + i * 4);
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

    std::vector<EncounterArea4> getEncounters(Encounter encounter, int time, const Profile4 &profile)
    {
        Game version = profile.getVersion();
        auto *info = PersonalLoader::getPersonal(version);

        std::vector<EncounterArea4> encounters;
        for (const auto &data : getData(version))
        {
            auto areas = (version & Game::HGSS) == Game::HGSS ? getHGSS(data, profile, info, encounter, time) : getDPPt(data, profile, info, time);
            std::copy_if(areas.begin(), areas.end(), std::back_inserter(encounters),
                         [&encounter](const EncounterArea4 &area) { return area.getEncounter() == encounter; });
        }

        return encounters;
    }
}
