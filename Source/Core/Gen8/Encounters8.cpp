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

#include "Encounters8.hpp"
#include <Core/Enum/Encounter.hpp>
#include <Core/Enum/Game.hpp>
#include <Core/Gen8/EncounterArea8.hpp>
#include <Core/Gen8/Profile8.hpp>
#include <Core/Parents/PersonalLoader.hpp>
#include <Core/Parents/Slot.hpp>
#include <Core/Parents/StaticTemplate.hpp>
#include <Core/Resources/Encounters.hpp>
#include <algorithm>
#include <cstring>
#include <iterator>

constexpr std::array<StaticTemplate, 8> gifts = {
    StaticTemplate(Game::BDSP, 387, 5), // Turtwig
    StaticTemplate(Game::BDSP, 390, 5), // Chimchar
    StaticTemplate(Game::BDSP, 393, 5), // Piplup
    StaticTemplate(Game::BDSP, 133, 5), // Eevee
    StaticTemplate(Game::BDSP, 440, 1), // Happiny egg
    StaticTemplate(Game::BDSP, 447, 1), // Riolu egg
    StaticTemplate(Game::BDSP, 151, 1, 1, Shiny::Never, 3), // Mew
    StaticTemplate(Game::BDSP, 385, 5, 1, Shiny::Never, 3) // Jirachi
};

constexpr std::array<StaticTemplate, 7> fossils = {
    StaticTemplate(Game::BDSP, 138, 1, 3), // Omanyte
    StaticTemplate(Game::BDSP, 140, 1, 3), // Kabuto
    StaticTemplate(Game::BDSP, 142, 1, 3), // Aerodactyl
    StaticTemplate(Game::BDSP, 345, 1, 3), // Lileep
    StaticTemplate(Game::BDSP, 347, 1, 3), // Anorith
    StaticTemplate(Game::BDSP, 408, 1, 3), // Cranidos
    StaticTemplate(Game::BDSP, 410, 1, 3) // Shieldon
};

constexpr std::array<StaticTemplate, 3> stationary = {
    StaticTemplate(Game::BDSP, 425, 22), // Drifloon
    StaticTemplate(Game::BDSP, 442, 25), // Spiritomb
    StaticTemplate(Game::BDSP, 479, 15) // Rotom
};

constexpr std::array<StaticTemplate, 2> roamers = {
    StaticTemplate(Game::BDSP, 481, 50, 3), // Mespirit
    StaticTemplate(Game::BDSP, 488, 50, 3) // Cresselia
};

constexpr std::array<StaticTemplate, 7> legends = {
    StaticTemplate(Game::BDSP, 480, 50, 3), // Uxie
    StaticTemplate(Game::BDSP, 482, 50, 3), // Azelf
    StaticTemplate(Game::BD, 483, 47, 3), // Dialga
    StaticTemplate(Game::SP, 484, 47, 3), // Palkia
    StaticTemplate(Game::BDSP, 485, 70, 3), // Heatran
    StaticTemplate(Game::BDSP, 486, 70, 3), // Regigigas
    StaticTemplate(Game::BDSP, 487, 70, 3) // Giratina
};

constexpr std::array<StaticTemplate, 11> ramanasParkPureSpace = {
    StaticTemplate(Game::SP, 144, 70, 2, 3), // Articuno
    StaticTemplate(Game::SP, 145, 70, 2, 3), // Zapdos
    StaticTemplate(Game::SP, 146, 70, 2, 3), // Moltres
    StaticTemplate(Game::BD, 243, 70, 2, 3), // Raikou
    StaticTemplate(Game::BD, 244, 70, 2, 3), // Entei
    StaticTemplate(Game::BD, 245, 70, 2, 3), // Suicune
    StaticTemplate(Game::BDSP, 377, 70, 2, 3), // Regirock
    StaticTemplate(Game::BDSP, 378, 70, 2, 3), // Regice
    StaticTemplate(Game::BDSP, 379, 70, 2, 3), // Registeel
    StaticTemplate(Game::BDSP, 380, 70, 3), // Latias
    StaticTemplate(Game::BDSP, 381, 70, 3) // Latios
};

constexpr std::array<StaticTemplate, 6> ramanasParkStrangeSpace = {
    StaticTemplate(Game::BDSP, 150, 70, 2, 3), // Mewtwo
    StaticTemplate(Game::SP, 249, 70, 2, 3), // Lugia
    StaticTemplate(Game::BD, 250, 70, 2, 3), // Ho-Oh
    StaticTemplate(Game::BDSP, 382, 70, 3), // Kyogre
    StaticTemplate(Game::BDSP, 383, 70, 3), // Groudon
    StaticTemplate(Game::BDSP, 384, 70, 3) // Rayquaza
};

constexpr std::array<StaticTemplate, 3> mythics = {
    StaticTemplate(Game::BDSP, 491, 50, 3), // Darkrai
    StaticTemplate(Game::BDSP, 492, 30, 3), // Shaymin
    StaticTemplate(Game::BDSP, 493, 80, 3) // Arceus
};

namespace Encounters8
{
    namespace
    {

        void modifySwarm(std::vector<Slot> &mons, const u8 *data, const PersonalInfo *info, bool swarm)
        {
            if (swarm)
            {
                u16 species[2] = { *reinterpret_cast<const u16 *>(data + 38), *reinterpret_cast<const u16 *>(data + 40) };
                mons[0].setSpecie(species[0], info[species[0]]);
                mons[1].setSpecie(species[1], info[species[1]]);
            }
        }

        void modifyTime(std::vector<Slot> &mons, const u8 *data, const PersonalInfo *info, int time)
        {
            u16 specie1;
            u16 specie2;
            if (time == 1)
            {
                specie1 = *reinterpret_cast<const u16 *>(data + 42);
                specie2 = *reinterpret_cast<const u16 *>(data + 44);
            }
            else if (time == 2)
            {
                specie1 = *reinterpret_cast<const u16 *>(data + 46);
                specie2 = *reinterpret_cast<const u16 *>(data + 48);
            }
            else
            {
                return;
            }

            mons[2].setSpecie(specie1, info[specie1]);
            mons[3].setSpecie(specie2, info[specie2]);
        }

        void modifyRadar(std::vector<Slot> &mons, const u8 *data, const PersonalInfo *info, bool radar)
        {
            if (radar)
            {
                u16 species[4] = { *reinterpret_cast<const u16 *>(data + 50), *reinterpret_cast<const u16 *>(data + 52),
                                   *reinterpret_cast<const u16 *>(data + 54), *reinterpret_cast<const u16 *>(data + 56) };
                mons[4].setSpecie(species[0], info[species[0]]);
                mons[5].setSpecie(species[1], info[species[1]]);
                mons[10].setSpecie(species[2], info[species[2]]);
                mons[11].setSpecie(species[3], info[species[3]]);
            }
        }

        std::vector<EncounterArea8> getBDSP(Encounter encounter, const PersonalInfo *info, const Profile8 &profile, int time)
        {

            std::vector<EncounterArea8> encounters;
            if (encounter == Encounter::GrandUnderground)
            {
                const u8 *randMarkData = ug_rand_mark.data();
                size_t randMarkSize = ug_rand_mark.size();

                const u8 *encountData = ug_encount.data();

                const u8 *specialEncountData = ug_special_pokemon.data();
                size_t specialEncountSize = ug_special_pokemon.size();

                u8 version = profile.getVersion() == Game::BD ? 2 : 3;

                for (size_t offset = 0; offset < randMarkSize; offset += 26)
                {
                    const u8 *entry = randMarkData + offset;
                    std::vector<Slot> slots;
                    u8 location = entry[0] + 181;
                    u8 encount_id = entry[1];
                    const u8 *encount_start = encountData;

                    for (size_t i = 2; i < encount_id; i += 1)
                    {
                        encount_start += encount_start[0] * 4 + 1;
                    }

                    size_t encountSize = encount_start[0];
                    const u8 *encount = encount_start + 1;

                    for (size_t i = 0; i < encountSize; i += 1)
                    {
                        if (encount[2] == 1 || encount[2] == version)
                        {
                            u16 specie = *reinterpret_cast<const u16 *>(encount);
                            slots.emplace_back(specie, 16, 63, info[specie]);
                        }
                        encount += 4;
                    }

                    for (size_t offset2 = 0; offset2 < specialEncountSize; offset2 += 8)
                    {
                        const u8 *special = specialEncountData + offset2;
                        if (special[0] == entry[0] && (special[3] == 1 || special[3] == version))
                        {
                            u16 specie = *reinterpret_cast<const u16 *>(special + 1);
                            slots.emplace_back(specie, 16, 63, info[specie]);
                        }
                    }
                    encounters.emplace_back(location, 0, Encounter::GrandUnderground, slots);
                }
            }
            else
            {
                const u8 *data;
                size_t size;

                if (profile.getVersion() == Game::BD)
                {
                    data = bd.data();
                    size = bd.size();
                }
                else
                {
                    data = sp.data();
                    size = sp.size();
                }

                for (size_t offset = 0; offset < size; offset += 142)
                {
                    const u8 *entry = data + offset;

                    u8 location = entry[0];
                    u8 grass = entry[1];
                    u8 surf = entry[58];
                    u8 old = entry[79];
                    u8 good = entry[100];
                    u8 super = entry[121];

                    switch (encounter)
                    {
                    case Encounter::Grass:
                        if (grass != 0)
                        {
                            std::vector<Slot> slots;
                            slots.reserve(12);
                            for (int i = 0; i < 12; i++)
                            {
                                u8 level = entry[2 + (i * 3)];
                                u16 specie = *reinterpret_cast<const u16 *>(entry + 3 + (i * 3));
                                slots.emplace_back(specie, level, info[specie]);
                            }
                            modifySwarm(slots, entry, info, profile.getSwarm());
                            modifyTime(slots, entry, info, time);
                            modifyRadar(slots, entry, info, profile.getRadar());
                            encounters.emplace_back(location, grass, encounter, slots);
                        }
                        break;
                    case Encounter::Surfing:
                        if (surf != 0)
                        {
                            std::vector<Slot> slots;
                            slots.reserve(5);
                            for (int i = 0; i < 5; i++)
                            {
                                u8 max = entry[59 + (i * 4)];
                                u8 min = entry[60 + (i * 4)];
                                u16 specie = *reinterpret_cast<const u16 *>(entry + 61 + (i * 4));
                                slots.emplace_back(specie, min, max, info[specie]);
                            }
                            encounters.emplace_back(location, surf, encounter, slots);
                        }
                        break;
                    case Encounter::OldRod:
                        if (old != 0)
                        {
                            std::vector<Slot> slots;
                            slots.reserve(5);
                            for (int i = 0; i < 5; i++)
                            {
                                u8 max = entry[80 + (i * 4)];
                                u8 min = entry[81 + (i * 4)];
                                u16 specie = *reinterpret_cast<const u16 *>(entry + 82 + (i * 4));
                                slots.emplace_back(specie, min, max, info[specie]);
                            }
                            encounters.emplace_back(location, old, encounter, slots);
                        }
                        break;
                    case Encounter::GoodRod:
                        if (good != 0)
                        {
                            std::vector<Slot> slots;
                            slots.reserve(5);
                            for (int i = 0; i < 5; i++)
                            {
                                u8 max = entry[101 + (i * 4)];
                                u8 min = entry[102 + (i * 4)];
                                u16 specie = *reinterpret_cast<const u16 *>(entry + 103 + (i * 4));
                                slots.emplace_back(specie, min, max, info[specie]);
                            }
                            encounters.emplace_back(location, good, encounter, slots);
                        }
                        break;
                    case Encounter::SuperRod:
                        if (super != 0)
                        {
                            std::vector<Slot> slots;
                            slots.reserve(5);
                            for (int i = 0; i < 5; i++)
                            {
                                u8 max = entry[122 + (i * 4)];
                                u8 min = entry[123 + (i * 4)];
                                u16 specie = *reinterpret_cast<const u16 *>(entry + 124 + (i * 4));
                                slots.emplace_back(specie, min, max, info[specie]);
                            }
                            encounters.emplace_back(location, super, Encounter::SuperRod, slots);
                        }
                        break;
                    default:
                        break;
                    }
                }
            }

            return encounters;
        }
    }

    std::vector<EncounterArea8> getEncounters(Encounter encounter, int time, const Profile8 &profile)
    {
        const auto *info = PersonalLoader::getPersonal(profile.getVersion());
        return getBDSP(encounter, info, profile, time);
    }

    const StaticTemplate *getStaticEncounters(int index, int *size)
    {
        if (index == 0)
        {
            if (size)
            {
                *size = gifts.size();
            }
            return gifts.data();
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
                *size = stationary.size();
            }
            return stationary.data();
        }
        else if (index == 3)
        {
            if (size)
            {
                *size = roamers.size();
            }
            return roamers.data();
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
                *size = ramanasParkPureSpace.size();
            }
            return ramanasParkPureSpace.data();
        }
        else if (index == 6)
        {
            if (size)
            {
                *size = ramanasParkStrangeSpace.size();
            }
            return ramanasParkStrangeSpace.data();
        }
        else
        {
            if (size)
            {
                *size = mythics.size();
            }
            return mythics.data();
        }
    }
}
