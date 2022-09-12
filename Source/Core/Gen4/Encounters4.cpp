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

#include "Encounters4.hpp"
#include <Core/Enum/Encounter.hpp>
#include <Core/Enum/Method.hpp>
#include <Core/Gen4/Profile4.hpp>
#include <Core/Gen4/StaticTemplate4.hpp>
#include <Core/Parents/EncounterArea.hpp>
#include <Core/Parents/PersonalLoader.hpp>
#include <Core/Parents/Slot.hpp>
#include <Core/Resources/Encounters.hpp>
#include <algorithm>
#include <bzlib.h>
#include <cstring>
#include <iterator>

constexpr std::array<StaticTemplate4, 12> starters = {
    StaticTemplate4(Game::DPPt, 387, 5, Method::Method1), // Turtwig @ Lake Verity / Route 201
    StaticTemplate4(Game::DPPt, 390, 5, Method::Method1), // Chimchar
    StaticTemplate4(Game::DPPt, 393, 5, Method::Method1), // Piplup

    StaticTemplate4(Game::HGSS, 1, 5, Method::Method1), // Bulbasaur @ Pallet Town
    StaticTemplate4(Game::HGSS, 4, 5, Method::Method1), // Charmander
    StaticTemplate4(Game::HGSS, 7, 5, Method::Method1), // Squirtle
    StaticTemplate4(Game::HGSS, 152, 5, Method::Method1), // Chikorita @ New Bark Town
    StaticTemplate4(Game::HGSS, 155, 5, Method::Method1), // Cyndaquil
    StaticTemplate4(Game::HGSS, 158, 5, Method::Method1), // Totodile
    StaticTemplate4(Game::HGSS, 252, 5, Method::Method1), // Treecko @ Saffron City
    StaticTemplate4(Game::HGSS, 255, 5, Method::Method1), // Torchic
    StaticTemplate4(Game::HGSS, 258, 5, Method::Method1) // Mudkip
};

constexpr std::array<StaticTemplate4, 7> fossils = {
    StaticTemplate4(Game::Gen4, 138, 20, Method::Method1), // Omanyte @ Mining Museum / Pewter City
    StaticTemplate4(Game::Gen4, 140, 20, Method::Method1), // Kabuto
    StaticTemplate4(Game::Gen4, 142, 20, Method::Method1), // Aerodactyl
    StaticTemplate4(Game::Gen4, 345, 20, Method::Method1), // Lileep
    StaticTemplate4(Game::Gen4, 347, 20, Method::Method1), // Anorith
    StaticTemplate4(Game::Gen4, 408, 20, Method::Method1), // Cranidos
    StaticTemplate4(Game::Gen4, 410, 20, Method::Method1) // Shieldon
};

constexpr std::array<StaticTemplate4, 14> gifts = {
    StaticTemplate4(Game::DP, 133, 5, Method::Method1), // Eevee @ Hearthome City
    StaticTemplate4(Game::Platinum, 133, 20, Method::Method1), // Eevee
    StaticTemplate4(Game::Platinum, 137, 25, Method::Method1), // Porygon @ Veilstone City
    StaticTemplate4(Game::Platinum, 175, 1, Method::Method1), // Togepi egg @ Cynthia
    StaticTemplate4(Game::DP, 440, 1, Method::Method1), // Happiny egg @ Traveling Man
    StaticTemplate4(Game::DPPt, 447, 1, Method::Method1), // Riolu egg @ Riley

    StaticTemplate4(Game::HGSS, 72, 15, Method::Method1), // Tentacool @ Cianwood City
    StaticTemplate4(Game::HGSS, 133, 5, Method::Method1), // Eevee @ Goldenrod City
    StaticTemplate4(Game::HGSS, 147, 15, Method::Method1), // Dratini @ Dragon's Den
    StaticTemplate4(Game::HGSS, 236, 10, Method::Method1), // Tyrogue @ Mt. Mortar
    StaticTemplate4(Game::HGSS, 175, 1, Method::Method1), // Togepi Egg @ Mr. Pokemon
    StaticTemplate4(Game::HGSS, 179, 1, Method::Method1), // Mareep Egg @ Primo
    StaticTemplate4(Game::HGSS, 194, 1, Method::Method1), // Wooper Egg
    StaticTemplate4(Game::HGSS, 218, 1, Method::Method1) // Slugma Egg
};

constexpr std::array<StaticTemplate4, 7> gameCorner = {
    StaticTemplate4(Game::HGSS, 112, 15, Method::Method1), // Mr. Mime @ Celadon City Game Corner
    StaticTemplate4(Game::HGSS, 133, 15, Method::Method1), // Eevee
    StaticTemplate4(Game::HGSS, 137, 15, Method::Method1), // Porygon

    StaticTemplate4(Game::HGSS, 63, 15, Method::Method1), // Abra @ Goldenrod City Game Corner
    StaticTemplate4(Game::HeartGold, 23, 15, Method::Method1), // Ekans
    StaticTemplate4(Game::SoulSilver, 27, 15, Method::Method1), // Sandshrew
    StaticTemplate4(Game::HGSS, 147, 15, Method::Method1) // Dratini
};

constexpr std::array<StaticTemplate4, 13> stationary = {
    StaticTemplate4(Game::DP, 425, 22, Method::MethodJ), // Drifloon @ Valley Windworks
    StaticTemplate4(Game::Platinum, 425, 15, Method::MethodJ), // Drifloon
    StaticTemplate4(Game::DP, 479, 15, Method::MethodJ), // Rotom @ Old Chateau
    StaticTemplate4(Game::Platinum, 479, 20, Method::MethodJ), // Rotom
    StaticTemplate4(Game::DPPt, 442, 25, Method::MethodJ), // Spiritomb @ Route 209

    StaticTemplate4(Game::HGSS, 100, 23, Method::MethodK), // Voltorb @ Team Rocket HQ Trap Floor
    StaticTemplate4(Game::HGSS, 74, 21, Method::MethodK), // Geodude
    StaticTemplate4(Game::HGSS, 109, 21, Method::MethodK), // Koffing
    StaticTemplate4(Game::HGSS, 130, 30, Method::Method1, Shiny::Always), // Gyrados @ Lake of Rage
    StaticTemplate4(Game::HGSS, 131, 20, Method::MethodK), // Lapras @ Union Cave
    StaticTemplate4(Game::HGSS, 101, 23, Method::MethodK), // Electrode @ Team Rocket HQ
    StaticTemplate4(Game::HGSS, 143, 50, Method::MethodK), // Snorlax @ Route 11 / Route 12
    StaticTemplate4(Game::HGSS, 185, 20, Method::MethodK) // Sudowoodo @ Route 36
    // TODO: maybe add spiky-eared pichu
};

constexpr std::array<StaticTemplate4, 33> legends = {
    StaticTemplate4(Game::Platinum, 377, 30, Method::MethodJ), // Regirock @ Rock Peak Ruins
    StaticTemplate4(Game::Platinum, 378, 30, Method::MethodJ), // Regice @ Iceberg Ruins
    StaticTemplate4(Game::Platinum, 379, 30, Method::MethodJ), // Registeel @ Iron Ruins
    StaticTemplate4(Game::DPPt, 480, 50, Method::MethodJ), // Uxie @ Acuity Cavern
    StaticTemplate4(Game::DPPt, 482, 50, Method::MethodJ), // Azelf @ Valor Cavern
    StaticTemplate4(Game::Diamond, 483, 47, Method::MethodJ), // Diagla @ Spear Pillar
    StaticTemplate4(Game::Pearl, 484, 47, Method::MethodJ), // Palkia
    StaticTemplate4(Game::Platinum, 483, 70, Method::MethodJ), // Diagla
    StaticTemplate4(Game::Platinum, 484, 70, Method::MethodJ), // Palkia
    StaticTemplate4(Game::DP, 485, 70, Method::MethodJ), // Heatran @ Stark Mountain
    StaticTemplate4(Game::Platinum, 485, 50, Method::MethodJ), // Heatran
    StaticTemplate4(Game::DP, 486, 70, Method::MethodJ), // Regigigas @ Snowpoint Temple
    StaticTemplate4(Game::Platinum, 486, 1, Method::MethodJ), // Regigigas
    StaticTemplate4(Game::DP, 487, 70, Method::MethodJ), // Giratina @ Turnback Cave
    StaticTemplate4(Game::Platinum, 487, 47, Method::MethodJ), // Giratina
    StaticTemplate4(Game::Platinum, 487, 47, Method::MethodJ, 1), // Giratina @ Distortion World

    StaticTemplate4(Game::HGSS, 144, 50, Method::MethodK), // Articuno @ Seafoam Islands
    StaticTemplate4(Game::HGSS, 145, 50, Method::MethodK), // Zapdos @ Route 10
    StaticTemplate4(Game::HGSS, 146, 50, Method::MethodK), // Moltres @ Mt. Silver Cave
    StaticTemplate4(Game::HGSS, 150, 70, Method::MethodK), // Mewtwo @ Cerulean Cave
    StaticTemplate4(Game::HGSS, 245, 40, Method::MethodK), // Suicune @ Route 25 / Burned Tower
    StaticTemplate4(Game::HeartGold, 249, 70, Method::MethodK), // Lugia @ Whirl Islands
    StaticTemplate4(Game::SoulSilver, 249, 45, Method::MethodK), // Lugia
    StaticTemplate4(Game::HeartGold, 250, 45, Method::MethodK), // Ho-Oh @ Bell Tower
    StaticTemplate4(Game::SoulSilver, 250, 70, Method::MethodK), // Ho-Oh
    StaticTemplate4(Game::HeartGold, 381, 40, Method::MethodK), // Latios @ Pewter City
    StaticTemplate4(Game::SoulSilver, 380, 40, Method::MethodK), // Latias
    StaticTemplate4(Game::HeartGold, 382, 50, Method::MethodK), // Kyogre @ Embedded Tower
    StaticTemplate4(Game::SoulSilver, 383, 50, Method::MethodK), // Groudon
    StaticTemplate4(Game::HGSS, 384, 50, Method::MethodK), // Rayquaza
    StaticTemplate4(Game::HGSS, 483, 1, Method::Method1), // Dialga @ Sinjoh Ruins
    StaticTemplate4(Game::HGSS, 484, 1, Method::Method1), // Palkia
    StaticTemplate4(Game::HGSS, 487, 1, Method::Method1, 1) // Giratina
};

constexpr std::array<StaticTemplate4, 3> events = {
    StaticTemplate4(Game::Gen4, 490, 1, Method::Method1, Shiny::Never), // Manaphy @ Pokemon Ranger
    StaticTemplate4(Game::Platinum, 491, 50, Method::MethodJ), // Darkrai @ Newmoon Island
    StaticTemplate4(Game::Platinum, 492, 30, Method::MethodJ) // Shaymin @ Flower Paradise
};

constexpr std::array<StaticTemplate4, 9> roamers = {
    StaticTemplate4(Game::DPPt, 481, 50, Method::Method1), // Mesprit
    StaticTemplate4(Game::DPPt, 488, 50, Method::Method1), // Cresselia
    StaticTemplate4(Game::Platinum, 144, 60, Method::Method1), // Articuno
    StaticTemplate4(Game::Platinum, 145, 60, Method::Method1), // Zapdos
    StaticTemplate4(Game::Platinum, 146, 60, Method::Method1), // Moltres

    StaticTemplate4(Game::HGSS, 243, 40, Method::Method1), // Raikou
    StaticTemplate4(Game::HGSS, 244, 40, Method::Method1), // Entei
    StaticTemplate4(Game::HeartGold, 380, 35, Method::Method1), // Latias
    StaticTemplate4(Game::SoulSilver, 381, 35, Method::Method1) // Latios
};

namespace Encounters4
{
    namespace
    {
        /**
         * @brief Decompressed provided data
         *
         * @param compressedData Compressed data
         * @param compressedSize Size of compressed data
         * @param size Size of uncompressed data
         *
         * @return Uncompressed data
         */
        u8 *decompress(const u8 *compressedData, size_t compressedSize, u32 &size)
        {
            size = *reinterpret_cast<const u16 *>(compressedData);
            u8 *data = new u8[size];

            BZ2_bzBuffToBuffDecompress(reinterpret_cast<char *>(data), &size,
                                       reinterpret_cast<char *>(const_cast<u8 *>(compressedData + sizeof(u16))), compressedSize, 0, 0);

            return data;
        }

        /**
         * @brief Modifies encounter slots based on the radio station
         *
         * @param pokemon Vector of original encounters
         * @param data Encounter area data
         * @param info Personal info array pointer
         * @param radio Radio station
         */
        void modifyRadio(std::vector<Slot> &pokemon, const u8 *data, const PersonalInfo *info, int radio)
        {
            u16 specie1;
            u16 specie2;
            if (radio == 1)
            {
                specie1 = *reinterpret_cast<const u16 *>(data + 91);
                specie2 = *reinterpret_cast<const u16 *>(data + 93);
            }
            else if (radio == 2)
            {
                specie1 = *reinterpret_cast<const u16 *>(data + 95);
                specie2 = *reinterpret_cast<const u16 *>(data + 97);
            }
            else
            {
                return;
            }

            pokemon[2].setSpecie(specie1, &info[specie1]);
            pokemon[3].setSpecie(specie1, &info[specie1]);
            pokemon[4].setSpecie(specie2, &info[specie2]);
            pokemon[5].setSpecie(specie2, &info[specie2]);
        }

        /**
         * @brief Modifies encounter slots based on the swarm
         *
         * @param pokemon Vector of original encounters
         * @param data Encounter area data
         * @param info Personal info array pointer
         * @param encounter Encounter type
         * @param swarm Whether swarm is active or not
         */
        void modifySwarmHGSS(std::vector<Slot> &pokemon, const u8 *data, const PersonalInfo *info, Encounter encounter, bool swarm)
        {
            if (swarm)
            {
                u16 specie;
                if (encounter == Encounter::Grass)
                {
                    specie = *reinterpret_cast<const u16 *>(data + 187);
                    pokemon[0].setSpecie(specie, &info[specie]);
                    pokemon[1].setSpecie(specie, &info[specie]);
                }
                else if (encounter == Encounter::Surfing)
                {
                    specie = *reinterpret_cast<const u16 *>(data + 189);
                    pokemon[0].setSpecie(specie, &info[specie]);
                }
                else if (encounter == Encounter::OldRod)
                {
                    specie = *reinterpret_cast<const u16 *>(data + 193);
                    pokemon[2].setSpecie(specie, &info[specie]);
                }
                else if (encounter == Encounter::GoodRod)
                {
                    specie = *reinterpret_cast<const u16 *>(data + 193);
                    for (int i : { 0, 2, 3 })
                    {
                        pokemon[i].setSpecie(specie, &info[specie]);
                    }
                }
                else if (encounter == Encounter::SuperRod)
                {
                    specie = *reinterpret_cast<const u16 *>(data + 193);
                    for (int i = 0; i < 5; i++)
                    {
                        pokemon[i].setSpecie(specie, &info[specie]);
                    }
                }
            }
        }

        /**
         * @brief Gets the encounter area for HGSS
         *
         * @param version Game version
         * @param encounter Encounter type
         * @param profile Profile information
         * @param info Personal info array pointer
         * @param modifier Time modifier
         *
         * @return Vector of encounter areas
         */
        std::vector<EncounterArea> getHGSS(Game version, Encounter encounter, const Profile4 &profile, const PersonalInfo *info,
                                           int modifier)
        {
            const u8 *compressedData;
            size_t compressedSize;

            if (version == Game::HeartGold)
            {
                compressedData = heartgold.data();
                compressedSize = heartgold.size();
            }
            else
            {
                compressedData = soulsilver.data();
                compressedSize = soulsilver.size();
            }

            u8 *data;
            u32 size;

            data = decompress(compressedData, compressedSize, size);

            std::vector<EncounterArea> encounters;
            for (size_t offset = 0; offset < size; offset += 195)
            {
                u8 const *entry = data + offset;

                u8 location = entry[0];
                u8 grass = entry[1];
                u8 surf = entry[2];
                u8 rock = entry[3];
                u8 old = entry[4];
                u8 good = entry[5];
                u8 super = entry[6];

                switch (encounter)
                {
                case Encounter::Grass:
                    if (grass != 0)
                    {
                        std::vector<Slot> slots;
                        slots.reserve(12);
                        for (int i = 0; i < 12; i++)
                        {
                            u8 level = entry[7 + i];
                            u16 specie = *reinterpret_cast<const u16 *>(entry + 19 + (i * 2) + (modifier * 24));
                            slots.emplace_back(specie, level, &info[specie]);
                        }
                        modifyRadio(slots, entry, info, profile.getRadio());
                        modifySwarmHGSS(slots, entry, info, encounter, profile.getSwarm());
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
                            u8 min = entry[99 + (i * 4)];
                            u8 max = entry[100 + (i * 4)];
                            u16 specie = *reinterpret_cast<const u16 *>(entry + 101 + (i * 4));
                            slots.emplace_back(specie, min, max, &info[specie]);
                        }
                        modifySwarmHGSS(slots, entry, info, encounter, profile.getSwarm());
                        encounters.emplace_back(location, surf, encounter, slots);
                    }
                    break;
                case Encounter::RockSmash:
                    if (rock != 0)
                    {
                        std::vector<Slot> slots;
                        slots.reserve(2);
                        for (int i = 0; i < 2; i++)
                        {
                            u8 min = entry[119 + (i * 4)];
                            u8 max = entry[120 + (i * 4)];
                            u16 specie = *reinterpret_cast<const u16 *>(entry + 121 + (i * 4));
                            slots.emplace_back(specie, min, max, &info[specie]);
                        }
                        encounters.emplace_back(location, rock, encounter, slots);
                    }
                    break;
                case Encounter::OldRod:
                    if (old != 0)
                    {
                        std::vector<Slot> slots;
                        slots.reserve(5);
                        for (int i = 0; i < 5; i++)
                        {
                            u8 min = entry[127 + (i * 4)];
                            u8 max = entry[128 + (i * 4)];
                            u16 specie = *reinterpret_cast<const u16 *>(entry + 129 + (i * 4));
                            slots.emplace_back(specie, min, max, &info[specie]);
                        }
                        modifySwarmHGSS(slots, entry, info, encounter, profile.getSwarm());
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
                            u8 min = entry[147 + (i * 4)];
                            u8 max = entry[148 + (i * 4)];
                            u16 specie = *reinterpret_cast<const u16 *>(entry + 149 + (i * 4));

                            // Adjust time based slot
                            if ((modifier == 0 || modifier == 1) && i == 3)
                            {
                                specie = *reinterpret_cast<const u16 *>(entry + 191);
                            }

                            slots.emplace_back(specie, min, max, &info[specie]);
                        }
                        modifySwarmHGSS(slots, entry, info, encounter, profile.getSwarm());
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
                            u8 min = entry[167 + (i * 4)];
                            u8 max = entry[168 + (i * 4)];
                            u16 specie = *reinterpret_cast<const u16 *>(entry + 169 + (i * 4));

                            // Adjust time based slot
                            if ((modifier == 0 || modifier == 1) && i == 1)
                            {
                                specie = *reinterpret_cast<const u16 *>(entry + 191);
                            }

                            slots.emplace_back(specie, min, max, &info[specie]);
                        }
                        modifySwarmHGSS(slots, entry, info, encounter, profile.getSwarm());
                        encounters.emplace_back(location, super, encounter, slots);
                    }
                    break;
                default:
                    break;
                }
            }
            delete[] data;
            return encounters;
        }

        /**
         * @brief Modifies encounter slots based on the dual slot game
         *
         * @param pokemon Vector of original encounters
         * @param data Encounter area data
         * @param info Personal info array pointer
         * @param dual Dual slot game version
         */
        void modifyDual(std::vector<Slot> &pokemon, const u8 *data, const PersonalInfo *info, Game dual)
        {
            u16 specie1;
            u16 specie2;
            if (dual == Game::Ruby)
            {
                specie1 = *reinterpret_cast<const u16 *>(data + 59);
                specie2 = *reinterpret_cast<const u16 *>(data + 61);
            }
            else if (dual == Game::Sapphire)
            {
                specie1 = *reinterpret_cast<const u16 *>(data + 63);
                specie2 = *reinterpret_cast<const u16 *>(data + 65);
            }
            else if (dual == Game::Emerald)
            {
                specie1 = *reinterpret_cast<const u16 *>(data + 67);
                specie2 = *reinterpret_cast<const u16 *>(data + 69);
            }
            else if (dual == Game::FireRed)
            {
                specie1 = *reinterpret_cast<const u16 *>(data + 71);
                specie2 = *reinterpret_cast<const u16 *>(data + 73);
            }
            else if (dual == Game::LeafGreen)
            {
                specie1 = *reinterpret_cast<const u16 *>(data + 75);
                specie2 = *reinterpret_cast<const u16 *>(data + 77);
            }
            else
            {
                return;
            }

            pokemon[8].setSpecie(specie1, &info[specie1]);
            pokemon[9].setSpecie(specie2, &info[specie2]);
        }

        /**
         * @brief Modifies encounter slots based on the pokeradar
         *
         * @param pokemon Vector of original encounters
         * @param data Encounter area data
         * @param info Personal info array pointer
         * @param radar Whether pokeradar is active or not
         */
        void modifyRadar(std::vector<Slot> &pokemon, const u8 *data, const PersonalInfo *info, bool radar)
        {
            if (radar)
            {
                u16 species[4] = { *reinterpret_cast<const u16 *>(data + 50), *reinterpret_cast<const u16 *>(data + 52),
                                   *reinterpret_cast<const u16 *>(data + 54), *reinterpret_cast<const u16 *>(data + 56) };
                pokemon[4].setSpecie(species[0], &info[species[0]]);
                pokemon[5].setSpecie(species[1], &info[species[1]]);
                pokemon[10].setSpecie(species[2], &info[species[2]]);
                pokemon[11].setSpecie(species[3], &info[species[3]]);
            }
        }

        /**
         * @brief Modifies encounter slots based on the swarm
         *
         * @param pokemon Vector of original encounters
         * @param data Encounter area data
         * @param info Personal info array pointer
         * @param swarm Whether swarm is active or not
         */
        void modifySwarmDPPt(std::vector<Slot> &pokemon, const u8 *data, const PersonalInfo *info, bool swarm)
        {
            if (swarm)
            {
                u16 specie1 = *reinterpret_cast<const u16 *>(data + 38);
                u16 specie2 = *reinterpret_cast<const u16 *>(data + 40);
                pokemon[0].setSpecie(specie1, &info[specie1]);
                pokemon[1].setSpecie(specie2, &info[specie2]);
            }
        }

        /**
         * @brief Modifies encounter slots based on the time
         *
         * @param pokemon Vector of original encounters
         * @param data Encounter area data
         * @param info Personal info array pointer
         * @param time Time of day
         */
        void modifyTime(std::vector<Slot> &pokemon, const u8 *data, const PersonalInfo *info, int time)
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

            pokemon[2].setSpecie(specie1, &info[specie1]);
            pokemon[3].setSpecie(specie2, &info[specie2]);
        }

        /**
         * @brief Gets the encounter area for DPPt
         *
         * @param version Game version
         * @param encounter Encounter type
         * @param profile Profile information
         * @param info Personal info array pointer
         * @param time Time modifier
         *
         * @return Vector of encounter areas
         */
        std::vector<EncounterArea> getDPPt(Game version, Encounter encounter, const Profile4 &profile, const PersonalInfo *info, int time)
        {
            const u8 *compressedData;
            size_t compressedSize;
            u8 *data;
            u32 size;

            if (version == Game::Diamond)
            {
                compressedData = diamond.data();
                compressedSize = diamond.size();
            }
            else if (version == Game::Pearl)
            {
                compressedData = pearl.data();
                compressedSize = pearl.size();
            }
            else
            {
                compressedData = platinum.data();
                compressedSize = platinum.size();
            }

            data = decompress(compressedData, compressedSize, size);
            std::vector<EncounterArea> encounters;
            for (size_t offset = 0; offset < size; offset += 163)
            {
                const u8 *entry = data + offset;

                u8 location = entry[0];
                u8 grass = entry[1];
                u8 surf = entry[79];
                u8 old = entry[100];
                u8 good = entry[121];
                u8 super = entry[142];

                switch (encounter)
                {
                case Encounter::Grass:
                    if (grass != 0)
                    {
                        std::vector<Slot> slots;
                        for (int i = 0; i < 12; i++)
                        {
                            u8 level = entry[2 + (i * 3)];
                            u16 specie = *reinterpret_cast<const u16 *>(entry + 3 + (i * 3));
                            slots.emplace_back(specie, level, &info[specie]);
                        }
                        modifySwarmDPPt(slots, entry, info, profile.getSwarm());
                        modifyTime(slots, entry, info, time);
                        modifyRadar(slots, entry, info, profile.getRadar());
                        modifyDual(slots, entry, info, profile.getDualSlot());
                        encounters.emplace_back(location, grass, encounter, slots);
                    }
                    break;
                case Encounter::Surfing:
                    if (surf != 0)
                    {
                        std::vector<Slot> slots;
                        for (int i = 0; i < 5; i++)
                        {
                            u8 max = entry[80 + (i * 4)];
                            u8 min = entry[81 + (i * 4)];
                            u16 specie = *reinterpret_cast<const u16 *>(entry + 82 + (i * 4));
                            slots.emplace_back(specie, min, max, &info[specie]);
                        }
                        encounters.emplace_back(location, surf, encounter, slots);
                    }
                    break;
                case Encounter::OldRod:
                    if (old != 0)
                    {
                        std::vector<Slot> slots;
                        for (int i = 0; i < 5; i++)
                        {
                            u8 max = entry[101 + (i * 4)];
                            u8 min = entry[102 + (i * 4)];
                            u16 specie = *reinterpret_cast<const u16 *>(entry + 103 + (i * 4));
                            slots.emplace_back(specie, min, max, &info[specie]);
                        }
                        encounters.emplace_back(location, old, encounter, slots);
                    }
                    break;
                case Encounter::GoodRod:
                    if (good != 0)
                    {
                        std::vector<Slot> slots;
                        for (int i = 0; i < 5; i++)
                        {
                            u8 max = entry[122 + (i * 4)];
                            u8 min = entry[123 + (i * 4)];
                            u16 specie = *reinterpret_cast<const u16 *>(entry + 124 + (i * 4));
                            slots.emplace_back(specie, min, max, &info[specie]);
                        }
                        encounters.emplace_back(location, good, encounter, slots);
                    }
                    break;
                case Encounter::SuperRod:
                    if (super != 0)
                    {
                        std::vector<Slot> slots;
                        for (int i = 0; i < 5; i++)
                        {
                            u8 max = entry[143 + (i * 4)];
                            u8 min = entry[144 + (i * 4)];
                            u16 specie = *reinterpret_cast<const u16 *>(entry + 145 + (i * 4));
                            slots.emplace_back(specie, min, max, &info[specie]);
                        }
                        encounters.emplace_back(location, super, encounter, slots);
                    }
                    break;
                default:
                    break;
                }
            }
            delete[] data;
            return encounters;
        }
    }

    std::vector<EncounterArea> getEncounters(Encounter encounter, int modifier, const Profile4 &profile)
    {
        Game version = profile.getVersion();
        const auto *info = PersonalLoader::getPersonal(version);
        if ((version & Game::DPPt) != Game::None)
        {
            return getDPPt(version, encounter, profile, info, modifier);
        }
        return getHGSS(version, encounter, profile, info, modifier);
    }

    const StaticTemplate4 *getStaticEncounters(int type, size_t *size)
    {
        if (type == 0)
        {
            if (size)
            {
                *size = starters.size();
            }
            return starters.data();
        }
        else if (type == 1)
        {
            if (size)
            {
                *size = fossils.size();
            }
            return fossils.data();
        }
        else if (type == 2)
        {
            if (size)
            {
                *size = gifts.size();
            }
            return gifts.data();
        }
        else if (type == 3)
        {
            if (size)
            {
                *size = gameCorner.size();
            }
            return gameCorner.data();
        }
        else if (type == 4)
        {
            if (size)
            {
                *size = stationary.size();
            }
            return stationary.data();
        }
        else if (type == 5)
        {
            if (size)
            {
                *size = legends.size();
            }
            return legends.data();
        }
        else if (type == 6)
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

    const StaticTemplate4 *getStaticEncounter(int type, int index)
    {
        const StaticTemplate4 *templates = getStaticEncounters(type);
        return &templates[index];
    }
}
