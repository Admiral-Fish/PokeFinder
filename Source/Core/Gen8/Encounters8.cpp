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
#include <cstddef>
#include <iterator>

constexpr std::array<u16, 14> greatMarsh = { 55, 183, 194, 195, 298, 315, 397, 399, 400, 451, 453, 455 };
constexpr std::array<u16, 14> greatMarshDex = { 46, 55, 102, 115, 193, 285, 315, 316, 397, 451, 452, 453, 454, 455 };

constexpr std::array<u16, 16> trophyGarden = { 35, 39, 52, 113, 133, 137, 173, 174, 183, 298, 311, 312, 351, 438, 439, 440 };

struct DynamicSlot
{
    u16 specie;
    u8 maxLevel;
    u8 minLevel;
};
static_assert(sizeof(DynamicSlot) == 4);

struct StaticSlot
{
    u16 specie;
    u8 level;
};
static_assert(sizeof(StaticSlot) == 4);

struct UndergroundSlot
{
    u16 specie;
    std::array<u8, 6> flagRates;
    u8 flag;
    u8 rateup;
    u8 size;
};
static_assert(sizeof(UndergroundSlot) == 12);

struct WildEncounter8
{
    u8 location;
    u8 grassRate;
    u8 surfRate;
    u8 oldRate;
    u8 goodRate;
    u8 superRate;
    StaticSlot grass[12];
    u16 swarm[2];
    u16 day[2];
    u16 night[2];
    u16 radar[4];
    DynamicSlot surf[5];
    DynamicSlot old[5];
    DynamicSlot good[5];
    DynamicSlot super[5];
};
static_assert(sizeof(WildEncounter8) == 154);

struct WildEncounterUnderground
{
    u8 location;
    u8 minPokemon;
    u8 maxPokemon;
    u8 pokemonCount;
    u8 specialRateCount;
    std::array<u8, 18> typeRates;
    u8 data[0];
};
static_assert(sizeof(WildEncounterUnderground) == 23);

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
 * @param entry Encounter area data
 * @param info Personal info array pointer
 * @param radar Whether pokeradar is active or not
 */
static void modifyRadar(std::vector<Slot> &mons, const WildEncounter8 *entry, const PersonalInfo *info, bool radar)
{
    if (radar)
    {
        mons[4].setSpecie(entry->radar[0], &info[entry->radar[0]]);
        mons[5].setSpecie(entry->radar[1], &info[entry->radar[1]]);
        mons[10].setSpecie(entry->radar[2], &info[entry->radar[2]]);
        mons[11].setSpecie(entry->radar[3], &info[entry->radar[3]]);
    }
}

/**
 * @brief Modifies encounter slots based on the swarm
 *
 * @param pokemon Vector of original encounters
 * @param entry Encounter area data
 * @param info Personal info array pointer
 * @param swarm Whether swarm is active or not
 */
static void modifySwarm(std::vector<Slot> &mons, const WildEncounter8 *entry, const PersonalInfo *info, bool swarm)
{
    if (swarm)
    {
        mons[0].setSpecie(entry->swarm[0], &info[entry->swarm[0]]);
        mons[1].setSpecie(entry->swarm[1], &info[entry->swarm[1]]);
    }
}

/**
 * @brief Modifies encounter slots based on the time
 *
 * @param pokemon Vector of original encounters
 * @param entry Encounter area data
 * @param info Personal info array pointer
 * @param time Time of day
 */
static void modifyTime(std::vector<Slot> &mons, const WildEncounter8 *entry, const PersonalInfo *info, int time)
{
    u16 specie1;
    u16 specie2;
    if (time == 1)
    {
        specie1 = entry->day[0];
        specie2 = entry->day[1];
    }
    else if (time == 2)
    {
        specie1 = entry->night[0];
        specie2 = entry->night[1];
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
    for (size_t offset = 0; offset < length; offset += sizeof(WildEncounter8))
    {
        const auto *entry = reinterpret_cast<const WildEncounter8 *>(data + offset);

        switch (encounter)
        {
        case Encounter::Grass:
            if (entry->grassRate != 0)
            {
                std::vector<Slot> slots;
                slots.reserve(12);
                for (const auto &slot : entry->grass)
                {
                    slots.emplace_back(slot.specie, slot.level, slot.level, &info[slot.specie]);
                }
                modifySwarm(slots, entry, info, swarm);
                modifyTime(slots, entry, info, time);
                modifyRadar(slots, entry, info, radar);
                modifyGreatMarsh(slots, replacement, info, entry->location);
                modifyTrophyGarden(slots, replacement, info, entry->location);
                encounters.emplace_back(entry->location, entry->grassRate, encounter, slots);
            }
            break;
        case Encounter::Surfing:
            if (entry->surfRate != 0)
            {
                std::vector<Slot> slots;
                slots.reserve(5);
                for (const auto &slot : entry->surf)
                {
                    slots.emplace_back(slot.specie, slot.minLevel, slot.maxLevel, &info[slot.specie]);
                }
                encounters.emplace_back(entry->location, entry->surfRate, encounter, slots);
            }
            break;
        case Encounter::OldRod:
            if (entry->oldRate != 0)
            {
                std::vector<Slot> slots;
                slots.reserve(5);
                for (const auto &slot : entry->old)
                {
                    slots.emplace_back(slot.specie, slot.minLevel, slot.maxLevel, &info[slot.specie]);
                }
                encounters.emplace_back(entry->location, entry->oldRate, encounter, slots);
            }
            break;
        case Encounter::GoodRod:
            if (entry->goodRate != 0)
            {
                std::vector<Slot> slots;
                slots.reserve(5);
                for (const auto &slot : entry->good)
                {
                    slots.emplace_back(slot.specie, slot.minLevel, slot.maxLevel, &info[slot.specie]);
                }
                encounters.emplace_back(entry->location, entry->goodRate, encounter, slots);
            }
            break;
        case Encounter::SuperRod:
            if (entry->superRate != 0)
            {
                std::vector<Slot> slots;
                slots.reserve(5);
                for (const auto &slot : entry->super)
                {
                    slots.emplace_back(slot.specie, slot.minLevel, slot.maxLevel, &info[slot.specie]);
                }
                encounters.emplace_back(entry->location, entry->superRate, Encounter::SuperRod, slots);
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
            auto *entry = reinterpret_cast<WildEncounterUnderground *>(data + offset);

            std::vector<SpecialPokemon> specialPokemon;
            const auto *specialPokemonSlots
                = reinterpret_cast<const SpecialPokemon *>(data + offset + offsetof(WildEncounterUnderground, data));
            for (u8 i = 0; i < entry->specialRateCount; i++)
            {
                specialPokemon.emplace_back(specialPokemonSlots[i]);
            }

            std::vector<Pokemon> pokemon;
            std::vector<TypeSize> types;
            const auto *pokemonSlots = reinterpret_cast<const UndergroundSlot *>(data + offset + offsetof(WildEncounterUnderground, data)
                                                                                 + entry->specialRateCount * sizeof(SpecialPokemon));
            for (u8 i = 0; i < entry->pokemonCount; i++)
            {
                const auto *pokemonSlot = &pokemonSlots[i];
                if (pokemonSlot->flag <= storyFlag)
                {
                    u16 specie = pokemonSlot->specie;
                    const PersonalInfo *info = &base[specie];

                    u8 typeCount = info->getType(0) == info->getType(1) ? 1 : 2;
                    for (u8 j = 0; j < typeCount; j++)
                    {
                        u8 type = info->getType(j);
                        u16 value = type + std::pow(10, pokemonSlot->size);

                        TypeSize typeSize = { value, pokemonSlot->size, type };
                        types.emplace_back(typeSize);
                    }

                    u16 flagRate
                        = diglett ? pokemonSlot->flagRates[storyFlag - 1] * pokemonSlot->rateup : pokemonSlot->flagRates[storyFlag - 1];
                    Pokemon mon = { flagRate, specie, pokemonSlot->size, info->getType(0), info->getType(1) };
                    pokemon.emplace_back(mon);
                }
            }

            encounters.emplace_back(entry->location, entry->minPokemon, entry->maxPokemon, pokemon, specialPokemon, entry->typeRates,
                                    types);
            offset += sizeof(WildEncounterUnderground) + entry->specialRateCount * sizeof(SpecialPokemon)
                + entry->pokemonCount * sizeof(UndergroundSlot);
        }
        delete[] data;
        return encounters;
    }
}
