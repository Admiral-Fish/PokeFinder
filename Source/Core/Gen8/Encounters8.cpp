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

#include "Encounters8.hpp"
#include <Core/Enum/Encounter.hpp>
#include <Core/Enum/Game.hpp>
#include <Core/Gen8/EncounterArea8.hpp>
#include <Core/Gen8/Profile8.hpp>
#include <Core/Gen8/UndergroundArea.hpp>
#include <Core/Parents/PersonalInfo.hpp>
#include <Core/Parents/PersonalLoader.hpp>
#include <Core/Parents/Slot.hpp>
#include <Core/Resources/EncounterData8.hpp>
#include <Core/Util/Utilities.hpp>
#include <algorithm>
#include <cmath>
#include <iterator>

constexpr std::array<u16, 14> greatMarsh = { 55, 183, 194, 195, 298, 315, 397, 399, 400, 451, 453, 455 };
constexpr std::array<u16, 14> greatMarshDex = { 46, 55, 102, 115, 193, 285, 315, 316, 397, 451, 452, 453, 454, 455 };

constexpr std::array<u16, 16> trophyGarden = { 35, 39, 52, 113, 133, 137, 173, 174, 183, 298, 311, 312, 351, 438, 439, 440 };

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
static void modifyRadar(std::vector<Slot> &mons, const u8 *data, const PersonalInfo *info, bool radar)
{
    if (radar)
    {
        u16 species[4] = { *reinterpret_cast<const u16 *>(data + 50), *reinterpret_cast<const u16 *>(data + 52),
                           *reinterpret_cast<const u16 *>(data + 54), *reinterpret_cast<const u16 *>(data + 56) };
        mons[4].setSpecie(species[0], &info[species[0]]);
        mons[5].setSpecie(species[1], &info[species[1]]);
        mons[10].setSpecie(species[2], &info[species[2]]);
        mons[11].setSpecie(species[3], &info[species[3]]);
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
static void modifySwarm(std::vector<Slot> &mons, const u8 *data, const PersonalInfo *info, bool swarm)
{
    if (swarm)
    {
        u16 species[2] = { *reinterpret_cast<const u16 *>(data + 38), *reinterpret_cast<const u16 *>(data + 40) };
        mons[0].setSpecie(species[0], &info[species[0]]);
        mons[1].setSpecie(species[1], &info[species[1]]);
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
static void modifyTime(std::vector<Slot> &mons, const u8 *data, const PersonalInfo *info, int time)
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

    mons[2].setSpecie(specie1, &info[specie1]);
    mons[3].setSpecie(specie2, &info[specie2]);
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
 * @param encounter Encounter type
 * @param time Time modifier
 * @param radar Whether pokeradar is active
 * @param swarm Whether swarm is active
 * @param version Game version
 * @param replacement Replacement slots used by Great Marsh and Trophy Garden
 * @param info Personal info array pointer
 *
 * @return Vector of encounter areas
 */
static std::vector<EncounterArea8> getBDSP(Encounter encounter, int time, bool radar, bool swarm, Game version,
                                           const std::array<u16, 2> &replacement, const PersonalInfo *info)
{
    const u8 *compressedData;
    size_t compressedLength;
    if (version == Game::BD)
    {
        compressedData = bd.data();
        compressedLength = bd.size();
    }
    else
    {
        compressedData = sp.data();
        compressedLength = sp.size();
    }

    u32 length;
    u8 *data = Utilities::decompress(compressedData, compressedLength, length);

    std::vector<EncounterArea8> encounters;
    for (size_t offset = 0; offset < length; offset += 142)
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
                for (size_t i = 0; i < 12; i++)
                {
                    u8 level = entry[2 + (i * 3)];
                    u16 specie = *reinterpret_cast<const u16 *>(entry + 3 + (i * 3));
                    slots.emplace_back(specie, level, level, &info[specie]);
                }
                modifySwarm(slots, entry, info, swarm);
                modifyTime(slots, entry, info, time);
                modifyRadar(slots, entry, info, radar);
                modifyGreatMarsh(slots, replacement, info, location);
                modifyTrophyGarden(slots, replacement, info, location);
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
                    u8 max = entry[59 + (i * 4)];
                    u8 min = entry[60 + (i * 4)];
                    u16 specie = *reinterpret_cast<const u16 *>(entry + 61 + (i * 4));
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
                    u8 max = entry[80 + (i * 4)];
                    u8 min = entry[81 + (i * 4)];
                    u16 specie = *reinterpret_cast<const u16 *>(entry + 82 + (i * 4));
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
                    u8 max = entry[101 + (i * 4)];
                    u8 min = entry[102 + (i * 4)];
                    u16 specie = *reinterpret_cast<const u16 *>(entry + 103 + (i * 4));
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
                    u8 max = entry[122 + (i * 4)];
                    u8 min = entry[123 + (i * 4)];
                    u16 specie = *reinterpret_cast<const u16 *>(entry + 124 + (i * 4));
                    slots.emplace_back(specie, min, max, &info[specie]);
                }
                encounters.emplace_back(location, super, Encounter::SuperRod, slots);
            }
            break;
        default:
            break;
        }
    }
    delete[] data;
    return encounters;
}

namespace Encounters8
{
    std::vector<EncounterArea8> getEncounters(Encounter encounter, int time, bool radar, bool swarm, const std::array<u16, 2> &replacement,
                                              const Profile8 *profile)
    {
        const auto *info = PersonalLoader::getPersonal(profile->getVersion());
        return getBDSP(encounter, time, radar, swarm, profile->getVersion(), replacement, info);
    }

    std::array<u16, 14> getGreatMarshPokemon(const Profile8 *profile)
    {
        if (profile->getNationalDex())
        {
            return greatMarshDex;
        }
        else
        {
            return greatMarsh;
        }
    }

    const StaticTemplate *getStaticEncounters(int index, int *size)
    {
        if (index == 0)
        {
            if (size)
            {
                *size = starters.size();
            }
            return starters.data();
        }
        else if (index == 1)
        {
            if (size)
            {
                *size = gifts.size();
            }
            return gifts.data();
        }
        else if (index == 2)
        {
            if (size)
            {
                *size = fossils.size();
            }
            return fossils.data();
        }
        else if (index == 3)
        {
            if (size)
            {
                *size = stationary.size();
            }
            return stationary.data();
        }
        else if (index == 4)
        {
            if (size)
            {
                *size = roamers.size();
            }
            return roamers.data();
        }
        else if (index == 5)
        {
            if (size)
            {
                *size = legends.size();
            }
            return legends.data();
        }
        else if (index == 6)
        {
            if (size)
            {
                *size = ramanasParkPureSpace.size();
            }
            return ramanasParkPureSpace.data();
        }
        else if (index == 7)
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

    const StaticTemplate *getStaticEncounter(int type, int index)
    {
        const StaticTemplate *templates = getStaticEncounters(type);
        return &templates[index];
    }

    std::array<u16, 16> getTrophyGardenPokemon()
    {
        return trophyGarden;
    }

    std::vector<UndergroundArea> getUndergroundEncounters(int storyFlag, bool diglett, const Profile8 *profile)
    {
        const u8 *compressedData;
        size_t compressedLength;

        Game version = profile->getVersion();
        if (version == Game::BD)
        {
            compressedData = bd_underground.data();
            compressedLength = bd_underground.size();
        }
        else
        {
            compressedData = sp_underground.data();
            compressedLength = sp_underground.size();
        }

        u32 length;
        u8 *data = Utilities::decompress(compressedData, compressedLength, length);

        std::vector<UndergroundArea> encounters;
        const PersonalInfo *base = PersonalLoader::getPersonal(profile->getVersion());
        for (size_t offset = 0; offset < length;)
        {
            const u8 *entry = data + offset;
            size_t index = 0;

            u8 location = entry[index++];

            u8 specialCount = entry[index++];
            std::vector<SpecialPokemon> specialPokemon;
            for (u8 i = 0; i < specialCount; i++, index += 4)
            {
                u16 specie = *reinterpret_cast<const u16 *>(entry + index);
                u16 rate = *reinterpret_cast<const u16 *>(entry + index + 2);
                SpecialPokemon special = { specie, rate };
                specialPokemon.emplace_back(special);
            }

            u8 min = entry[index++];
            u8 max = entry[index++];

            std::array<u8, 18> rates;
            std::generate(rates.begin(), rates.end(), [entry, &index] { return entry[index++]; });

            std::vector<Pokemon> pokemon;
            std::vector<TypeSize> types;
            u8 pokemonCount = entry[index++];
            for (u8 i = 0; i < pokemonCount; i++)
            {
                u8 flag = entry[index++];
                if (flag <= storyFlag)
                {
                    u16 specie = *reinterpret_cast<const u16 *>(entry + index);
                    index += 2;
                    const PersonalInfo *info = &base[specie];

                    u8 size = entry[index++];
                    u8 typeCount = info->getType(0) == info->getType(1) ? 1 : 2;
                    for (u8 j = 0; j < typeCount; j++)
                    {
                        u8 type = info->getType(j);
                        u16 value = type + std::pow(10, size);

                        TypeSize typeSize = { value, size, type };
                        types.emplace_back(typeSize);
                    }

                    std::array<u8, 6> flagRates;
                    std::generate(flagRates.begin(), flagRates.end(), [entry, &index] { return entry[index++]; });
                    u8 rateup = entry[index++];

                    u16 flagRate = diglett ? flagRates[storyFlag - 1] * rateup : flagRates[storyFlag - 1];
                    Pokemon mon = { flagRate, specie, size, info->getType(0), info->getType(1) };
                    pokemon.emplace_back(mon);
                }
                else
                {
                    index += 10;
                }
            }

            encounters.emplace_back(location, min, max, pokemon, specialPokemon, rates, types);
            offset += index;
        }
        delete[] data;
        return encounters;
    }
}
