/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2023 by Admiral_Fish, bumba, and EzPzStreamz
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
#include <Core/Gen4/EncounterArea4.hpp>
#include <Core/Gen4/Profile4.hpp>
#include <Core/Gen4/StaticTemplate4.hpp>
#include <Core/Parents/PersonalInfo.hpp>
#include <Core/Parents/PersonalLoader.hpp>
#include <Core/Parents/Slot.hpp>
#include <Core/Resources/EncounterData4.hpp>
#include <Core/Util/Utilities.hpp>
#include <algorithm>
#include <cstring>
#include <iterator>

constexpr std::array<u16, 15> greatMarshDP = { 55, 183, 194, 195, 298, 315, 397, 399, 400, 451, 453, 455 };
constexpr std::array<u16, 15> greatMarshDPDex = { 46, 55, 102, 115, 193, 285, 315, 316, 397, 451, 452, 453, 454, 455 };

constexpr std::array<u16, 15> greatMarshPt = { 114, 193, 194, 195, 357, 451, 453, 455 };
constexpr std::array<u16, 15> greatMarshPtDex = { 46, 102, 114, 115, 193, 195, 285, 316, 352, 357, 451, 452, 453, 454, 455 };

constexpr std::array<u16, 16> trophyGardenDP = { 35, 39, 52, 113, 133, 137, 173, 174, 183, 298, 311, 312, 351, 438, 439, 440 };
constexpr std::array<u16, 16> trophyGardenPt = { 35, 39, 52, 113, 132, 133, 173, 174, 183, 298, 311, 312, 351, 438, 439, 440 };

/**
 * @brief Modifies encounter slots based on the radio station
 *
 * @param pokemon Vector of original encounters
 * @param data Encounter area data
 * @param info Personal info array pointer
 * @param radio Radio station
 */
static void modifyRadio(std::vector<Slot> &pokemon, const u8 *data, const PersonalInfo *info, int radio)
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
static void modifySwarmHGSS(std::vector<Slot> &pokemon, const u8 *data, const PersonalInfo *info, Encounter encounter, bool swarm)
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
            for (size_t i = 0; i < 5; i++)
            {
                pokemon[i].setSpecie(specie, &info[specie]);
            }
        }
    }
}

/**
 * @brief Gets the encounter area for HGSS Safari zone
 *
 * @param encounter Encounter type
 * @param time Time modifier
 * @param blocks Active pokeblocks in the safari
 * @param info Personal info array pointer
 *
 * @return Vector of encounter areas
 */
static std::vector<EncounterArea4> getHGSSSafari(Encounter encounter, int time, const std::array<u8, 5> &blocks, const PersonalInfo *info)
{
    const u8 *compressedData = hgss_safari.data();
    size_t compressedLength = hgss_safari.size();

    u32 length;
    u8 *data = Utilities::decompress(compressedData, compressedLength, length);

    std::vector<EncounterArea4> encounters;
    for (size_t offset = 0; offset < length;)
    {
        const u8 *entry = data + offset;

        u8 location = entry[0];
        bool water = entry[1];
        size_t block = 0;

        std::vector<Slot> slots;
        slots.reserve(10);
        switch (encounter)
        {
        case Encounter::Grass:
            for (size_t i = 0; i < 10; i++)
            {
                u16 specie = *reinterpret_cast<const u16 *>(entry + 2 + (i * 3) + (30 * time));
                u8 level = entry[4 + (i * 3) + (30 * time)];
                for (; block < 10; block++)
                {
                    if (blocks[entry[182 + (4 * block)]] >= entry[183 + (4 * block)]
                        && blocks[entry[184 + (4 * block)]] >= entry[185 + (4 * block)])
                    {
                        specie = *reinterpret_cast<const u16 *>(entry + 92 + (3 * block) + (30 * time));
                        level = entry[94 + (3 * block) + (30 * time)];
                        block++;
                        break;
                    }
                }
                slots.emplace_back(specie, level, level, &info[specie]);
            }
            encounters.emplace_back(location, 0, encounter, slots);
            break;
        case Encounter::Surfing:
            if (water)
            {
                for (size_t i = 0; i < 10; i++)
                {
                    u16 specie = *reinterpret_cast<const u16 *>(entry + 222 + (i * 3) + (30 * time));
                    u8 level = entry[224 + (i * 3) + (30 * time)];
                    for (; block < 3; block++)
                    {
                        if (blocks[entry[339 + (4 * block)]] >= entry[340 + (4 * block)]
                            && blocks[entry[341 + (4 * block)]] >= entry[342 + (4 * block)])
                        {
                            specie = *reinterpret_cast<const u16 *>(entry + 312 + (3 * block) + (30 * time));
                            level = entry[314 + (3 * block) + (30 * time)];
                            block++;
                            break;
                        }
                    }
                    slots.emplace_back(specie, level, level, &info[specie]);
                }
                encounters.emplace_back(location, 0, encounter, slots);
            }
            break;
        case Encounter::OldRod:
            if (water)
            {
                for (size_t i = 0; i < 10; i++)
                {
                    u16 specie = *reinterpret_cast<const u16 *>(entry + 351 + (i * 3) + (30 * time));
                    u8 level = entry[353 + (i * 3) + (30 * time)];
                    for (; block < 2; block++)
                    {
                        if (blocks[entry[459 + (4 * block)]] >= entry[460 + (4 * block)]
                            && blocks[entry[461 + (4 * block)]] >= entry[462 + (4 * block)])
                        {
                            specie = *reinterpret_cast<const u16 *>(entry + 441 + (3 * block) + (30 * time));
                            level = entry[443 + (3 * block) + (30 * time)];
                            block++;
                            break;
                        }
                    }
                    slots.emplace_back(specie, level, level, &info[specie]);
                }
                encounters.emplace_back(location, 25, encounter, slots);
            }
            break;
        case Encounter::GoodRod:
            if (water)
            {
                for (size_t i = 0; i < 10; i++)
                {
                    u16 specie = *reinterpret_cast<const u16 *>(entry + 467 + (i * 3) + (30 * time));
                    u8 level = entry[469 + (i * 3) + (30 * time)];
                    for (; block < 2; block++)
                    {
                        if (blocks[entry[575 + (4 * block)]] >= entry[576 + (4 * block)]
                            && blocks[entry[577 + (4 * block)]] >= entry[578 + (4 * block)])
                        {
                            specie = *reinterpret_cast<const u16 *>(entry + 557 + (3 * block) + (30 * time));
                            level = entry[559 + (3 * block) + (30 * time)];
                            block++;
                            break;
                        }
                    }
                    slots.emplace_back(specie, level, level, &info[specie]);
                }
                encounters.emplace_back(location, 50, encounter, slots);
            }
            break;
        case Encounter::SuperRod:
            if (water)
            {
                for (size_t i = 0; i < 10; i++)
                {
                    u16 specie = *reinterpret_cast<const u16 *>(entry + 583 + (i * 3) + (30 * time));
                    u8 level = entry[585 + (i * 3) + (30 * time)];
                    for (; block < 2; block++)
                    {
                        if (blocks[entry[691 + (4 * block)]] >= entry[692 + (4 * block)]
                            && blocks[entry[693 + (4 * block)]] >= entry[694 + (4 * block)])
                        {
                            specie = *reinterpret_cast<const u16 *>(entry + 673 + (3 * block) + (30 * time));
                            level = entry[675 + (3 * block) + (30 * time)];
                            block++;
                            break;
                        }
                    }
                    slots.emplace_back(specie, level, level, &info[specie]);
                }
                encounters.emplace_back(location, 75, encounter, slots);
            }
            break;
        default:
            break;
        }
        offset += water ? 699 : 222;
    }

    delete[] data;
    return encounters;
}

/**
 * @brief Gets the encounter area for HGSS
 *
 * @param version Game version
 * @param encounter Encounter type
 * @param profile Profile information
 * @param radio Radio station
 * @param swarm Whether swarm is active
 * @param time Time modifier
 * @param safari Whether the encounter is in the safari zone
 * @param blocks Active pokeblocks in the safari
 * @param info Personal info array pointer
 *
 * @return Vector of encounter areas
 */
static std::vector<EncounterArea4> getHGSS(Game version, Encounter encounter, int radio, bool swarm, bool dex, int time,
                                           const std::array<u8, 5> &blocks, const PersonalInfo *info)
{
    const u8 *compressedData;
    size_t compressedLength;

    u8 *data;
    u32 length;

    std::vector<EncounterArea4> encounters;
    if (encounter == Encounter::BugCatchingContest)
    {
        compressedData = hgss_bug.data();
        compressedLength = hgss_bug.size();

        data = Utilities::decompress(compressedData, compressedLength, length);
        length = dex ? length : 41;
        for (size_t offset = dex ? 41 : 0; offset < length; offset += 41)
        {
            const u8 *entry = data + offset;

            u8 location = entry[0];

            std::vector<Slot> slots;
            slots.reserve(10);
            for (size_t i = 0; i < 10; i++)
            {
                u16 specie = *reinterpret_cast<const u16 *>(entry + 1 + (i * 4));
                u8 min = entry[3 + (i * 4)];
                u8 max = entry[4 + (i * 4)];
                slots.emplace_back(specie, min, max, &info[specie]);
            }
            encounters.emplace_back(location, 0, encounter, slots);
        }
    }
    else if (encounter == Encounter::Headbutt || encounter == Encounter::HeadbuttAlt || encounter == Encounter::HeadbuttSpecial)
    {
        if (version == Game::HeartGold)
        {
            compressedData = hg_headbutt.data();
            compressedLength = hg_headbutt.size();
        }
        else
        {
            compressedData = ss_headbutt.data();
            compressedLength = ss_headbutt.size();
        }

        data = Utilities::decompress(compressedData, compressedLength, length);
        for (size_t offset = 0; offset < length;)
        {
            const u8 *entry = data + offset;

            u8 location = entry[0];
            bool special = entry[1];
            u8 tree = toInt(encounter) - toInt(Encounter::Headbutt);

            if (encounter != Encounter::HeadbuttSpecial || special)
            {
                std::vector<Slot> slots;
                slots.reserve(6);
                for (size_t i = 0; i < 6; i++)
                {
                    u16 specie = *reinterpret_cast<const u16 *>(entry + 2 + (i * 4) + (24 * tree));
                    u8 min = entry[4 + (i * 4) + (24 * tree)];
                    u8 max = entry[5 + (i * 4) + (24 * tree)];
                    slots.emplace_back(specie, min, max, &info[specie]);
                }
                encounters.emplace_back(location, 0, encounter, slots);
            }

            offset += special == 0 ? 50 : 74;
        }
    }
    else
    {
        if (version == Game::HeartGold)
        {
            compressedData = heartgold.data();
            compressedLength = heartgold.size();
        }
        else
        {
            compressedData = soulsilver.data();
            compressedLength = soulsilver.size();
        }

        data = Utilities::decompress(compressedData, compressedLength, length);
        for (size_t offset = 0; offset < length; offset += 195)
        {
            const u8 *entry = data + offset;

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
                    for (size_t i = 0; i < 12; i++)
                    {
                        u8 level = entry[7 + i];
                        u16 specie = *reinterpret_cast<const u16 *>(entry + 19 + (i * 2) + (time * 24));
                        slots.emplace_back(specie, level, level, &info[specie]);
                    }
                    modifyRadio(slots, entry, info, radio);
                    modifySwarmHGSS(slots, entry, info, encounter, swarm);
                    encounters.emplace_back(location, grass, encounter, slots);
                }
                break;
            case Encounter::Surfing:
                if (surf != 0)
                {
                    std::vector<Slot> slots;
                    slots.reserve(5);
                    for (size_t i = 0; i < 5; i++)
                    {
                        u8 min = entry[99 + (i * 4)];
                        u8 max = entry[100 + (i * 4)];
                        u16 specie = *reinterpret_cast<const u16 *>(entry + 101 + (i * 4));
                        slots.emplace_back(specie, min, max, &info[specie]);
                    }
                    modifySwarmHGSS(slots, entry, info, encounter, swarm);
                    encounters.emplace_back(location, surf, encounter, slots);
                }
                break;
            case Encounter::RockSmash:
                if (rock != 0)
                {
                    std::vector<Slot> slots;
                    slots.reserve(2);
                    for (size_t i = 0; i < 2; i++)
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
                    for (size_t i = 0; i < 5; i++)
                    {
                        u8 min = entry[127 + (i * 4)];
                        u8 max = entry[128 + (i * 4)];
                        u16 specie = *reinterpret_cast<const u16 *>(entry + 129 + (i * 4));
                        slots.emplace_back(specie, min, max, &info[specie]);
                    }
                    modifySwarmHGSS(slots, entry, info, encounter, swarm);
                    encounters.emplace_back(location, old, encounter, slots);
                }
                break;
            case Encounter::GoodRod:
                if (good != 0)
                {
                    std::vector<Slot> slots;
                    slots.reserve(5);
                    for (size_t i = 0; i < 5; i++)
                    {
                        u8 min = entry[147 + (i * 4)];
                        u8 max = entry[148 + (i * 4)];
                        u16 specie = *reinterpret_cast<const u16 *>(entry + 149 + (i * 4));

                        // Adjust time based slot
                        if ((time == 0 || time == 1) && i == 3)
                        {
                            specie = *reinterpret_cast<const u16 *>(entry + 191);
                        }

                        slots.emplace_back(specie, min, max, &info[specie]);
                    }
                    modifySwarmHGSS(slots, entry, info, encounter, swarm);
                    encounters.emplace_back(location, good, encounter, slots);
                }
                break;
            case Encounter::SuperRod:
                if (super != 0)
                {
                    std::vector<Slot> slots;
                    slots.reserve(5);
                    for (size_t i = 0; i < 5; i++)
                    {
                        u8 min = entry[167 + (i * 4)];
                        u8 max = entry[168 + (i * 4)];
                        u16 specie = *reinterpret_cast<const u16 *>(entry + 169 + (i * 4));

                        // Adjust time based slot
                        if ((time == 0 || time == 1) && i == 1)
                        {
                            specie = *reinterpret_cast<const u16 *>(entry + 191);
                        }

                        slots.emplace_back(specie, min, max, &info[specie]);
                    }
                    modifySwarmHGSS(slots, entry, info, encounter, swarm);
                    encounters.emplace_back(location, super, encounter, slots);
                }
                break;
            default:
                break;
            }
        }

        std::vector<EncounterArea4> safariEncounters = getHGSSSafari(encounter, time, blocks, info);
        encounters.insert(encounters.end(), safariEncounters.begin(), safariEncounters.end());
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
static void modifyDual(std::vector<Slot> &pokemon, const u8 *data, const PersonalInfo *info, Game dual)
{
    u16 specie1;
    u16 specie2;
    if (dual == Game::Ruby)
    {
        specie1 = *reinterpret_cast<const u16 *>(data + 58);
        specie2 = *reinterpret_cast<const u16 *>(data + 60);
    }
    else if (dual == Game::Sapphire)
    {
        specie1 = *reinterpret_cast<const u16 *>(data + 62);
        specie2 = *reinterpret_cast<const u16 *>(data + 64);
    }
    else if (dual == Game::Emerald)
    {
        specie1 = *reinterpret_cast<const u16 *>(data + 66);
        specie2 = *reinterpret_cast<const u16 *>(data + 68);
    }
    else if (dual == Game::FireRed)
    {
        specie1 = *reinterpret_cast<const u16 *>(data + 70);
        specie2 = *reinterpret_cast<const u16 *>(data + 72);
    }
    else if (dual == Game::LeafGreen)
    {
        specie1 = *reinterpret_cast<const u16 *>(data + 74);
        specie2 = *reinterpret_cast<const u16 *>(data + 76);
    }
    else
    {
        return;
    }

    pokemon[8].setSpecie(specie1, &info[specie1]);
    pokemon[9].setSpecie(specie2, &info[specie2]);
}

/**
 * @brief Modifies encounter slots based on the Great Marsh
 *
 * @param pokemon Vector of original encounters
 * @param replacement Replacement pokemon
 * @param info Personal info array pointer
 * @param location Encounter location
 */
static void modifyGreatMarsh(std::vector<Slot> &pokemon, const std::array<u16, 2> &replacement, const PersonalInfo *info, u8 location)
{
    if (location >= 23 && location <= 28 && replacement[0] != 0)
    {
        u16 specie = replacement[0];
        pokemon[6].setSpecie(specie, &info[specie]);
        pokemon[7].setSpecie(specie, &info[specie]);
    }
}

/**
 * @brief Modifies encounter slots based on the pokeradar
 *
 * @param pokemon Vector of original encounters
 * @param data Encounter area data
 * @param info Personal info array pointer
 * @param radar Whether pokeradar is active or not
 */
static void modifyRadar(std::vector<Slot> &pokemon, const u8 *data, const PersonalInfo *info, bool radar)
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
static void modifySwarmDPPt(std::vector<Slot> &pokemon, const u8 *data, const PersonalInfo *info, bool swarm)
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
static void modifyTime(std::vector<Slot> &pokemon, const u8 *data, const PersonalInfo *info, int time)
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
 * @brief Modifies encounter slots based on the Trophy Garden
 *
 * @param pokemon Vector of original encounters
 * @param replacement Replacement pokemon
 * @param info Personal info array pointer
 * @param location Encounter location
 */
static void modifyTrophyGarden(std::vector<Slot> &pokemon, const std::array<u16, 2> &replacement, const PersonalInfo *info, u8 location)
{
    if (location == 117 && replacement[0] != 0 && replacement[1] != 0)
    {
        u16 specie1 = replacement[0];
        u16 specie2 = replacement[1];
        pokemon[6].setSpecie(specie1, &info[specie1]);
        pokemon[7].setSpecie(specie2, &info[specie2]);
    }
}

/**
 * @brief Gets the encounter area for DPPt
 *
 * @param version Game version
 * @param encounter Encounter type
 * @param dual Dual slot version
 * @param radar Whether pokeradar is active
 * @param swarm Whether swarm is active
 * @param time Time modifier
 * @param replacement Replacement slots used by Great Marsh and Trophy Garden
 * @param info Personal info array pointer
 *
 * @return Vector of encounter areas
 */
static std::vector<EncounterArea4> getDPPt(Game version, Encounter encounter, Game dual, bool radar, bool swarm, int time,
                                           const std::array<u16, 2> &replacement, const PersonalInfo *info)
{
    const u8 *compressedData;
    size_t compressedLength;

    if (version == Game::Diamond)
    {
        compressedData = diamond.data();
        compressedLength = diamond.size();
    }
    else if (version == Game::Pearl)
    {
        compressedData = pearl.data();
        compressedLength = pearl.size();
    }
    else
    {
        compressedData = platinum.data();
        compressedLength = platinum.size();
    }

    u32 length;
    u8 *data = Utilities::decompress(compressedData, compressedLength, length);

    std::vector<EncounterArea4> encounters;
    for (size_t offset = 0; offset < length; offset += 162)
    {
        const u8 *entry = data + offset;

        u8 location = entry[0];
        u8 grass = entry[1];
        u8 surf = entry[78];
        u8 old = entry[99];
        u8 good = entry[120];
        u8 super = entry[141];

        switch (encounter)
        {
        case Encounter::Grass:
            if (grass != 0)
            {
                std::vector<Slot> slots;
                slots.reserve(12);
                for (size_t i = 0; i < 12; i++)
                {
                    u8 level = entry[2 + (i * 3)];
                    u16 specie = *reinterpret_cast<const u16 *>(entry + 3 + (i * 3));
                    slots.emplace_back(specie, level, level, &info[specie]);
                }
                modifySwarmDPPt(slots, entry, info, swarm);
                modifyTime(slots, entry, info, time);
                modifyRadar(slots, entry, info, radar);
                modifyGreatMarsh(slots, replacement, info, location);
                modifyTrophyGarden(slots, replacement, info, location);
                modifyDual(slots, entry, info, dual);
                encounters.emplace_back(location, grass, encounter, slots);
            }
            break;
        case Encounter::Surfing:
            if (surf != 0)
            {
                std::vector<Slot> slots;
                slots.reserve(5);
                for (size_t i = 0; i < 5; i++)
                {
                    u8 max = entry[79 + (i * 4)];
                    u8 min = entry[80 + (i * 4)];
                    u16 specie = *reinterpret_cast<const u16 *>(entry + 81 + (i * 4));
                    slots.emplace_back(specie, min, max, &info[specie]);
                }
                encounters.emplace_back(location, surf, encounter, slots);
            }
            break;
        case Encounter::OldRod:
            if (old != 0)
            {
                std::vector<Slot> slots;
                slots.reserve(5);
                for (size_t i = 0; i < 5; i++)
                {
                    u8 max = entry[100 + (i * 4)];
                    u8 min = entry[101 + (i * 4)];
                    u16 specie = *reinterpret_cast<const u16 *>(entry + 102 + (i * 4));
                    slots.emplace_back(specie, min, max, &info[specie]);
                }
                encounters.emplace_back(location, old, encounter, slots);
            }
            break;
        case Encounter::GoodRod:
            if (good != 0)
            {
                std::vector<Slot> slots;
                slots.reserve(5);
                for (size_t i = 0; i < 5; i++)
                {
                    u8 max = entry[121 + (i * 4)];
                    u8 min = entry[122 + (i * 4)];
                    u16 specie = *reinterpret_cast<const u16 *>(entry + 123 + (i * 4));
                    slots.emplace_back(specie, min, max, &info[specie]);
                }
                encounters.emplace_back(location, good, encounter, slots);
            }
            break;
        case Encounter::SuperRod:
            if (super != 0)
            {
                std::vector<Slot> slots;
                slots.reserve(5);
                for (size_t i = 0; i < 5; i++)
                {
                    u8 max = entry[142 + (i * 4)];
                    u8 min = entry[143 + (i * 4)];
                    u16 specie = *reinterpret_cast<const u16 *>(entry + 144 + (i * 4));
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

namespace Encounters4
{
    std::vector<EncounterArea4> getEncounters(Encounter encounter, int time, Game dual, bool radar, int radio, bool swarm,
                                              const std::array<u16, 2> &replacement, const std::array<u8, 5> &blocks,
                                              const Profile4 *profile)
    {
        Game version = profile->getVersion();
        const auto *info = PersonalLoader::getPersonal(version);
        if ((version & Game::DPPt) != Game::None)
        {
            return getDPPt(version, encounter, dual, radar, swarm, time, replacement, info);
        }
        return getHGSS(version, encounter, radio, swarm, profile->getNationalDex(), time, blocks, info);
    }

    std::array<u16, 15> getGreatMarshPokemon(const Profile4 *profile)
    {
        if ((profile->getVersion() & Game::DP) != Game::None)
        {
            return profile->getNationalDex() ? greatMarshDPDex : greatMarshDP;
        }
        else
        {
            return profile->getNationalDex() ? greatMarshPtDex : greatMarshPt;
        }
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

    std::array<u16, 16> getTrophyGardenPokemon(const Profile4 *profile)
    {
        if ((profile->getVersion() & Game::DP) != Game::None)
        {
            return trophyGardenDP;
        }
        else
        {
            return trophyGardenPt;
        }
    }
}
