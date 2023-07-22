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

#include "Encounters5.hpp"
#include <Core/Enum/Encounter.hpp>
#include <Core/Enum/Game.hpp>
#include <Core/Gen5/EncounterArea5.hpp>
#include <Core/Gen5/HiddenGrottoArea.hpp>
#include <Core/Gen5/Profile5.hpp>
#include <Core/Parents/PersonalInfo.hpp>
#include <Core/Parents/PersonalLoader.hpp>
#include <Core/Parents/Slot.hpp>
#include <Core/Resources/EncounterData5.hpp>
#include <Core/Util/Utilities.hpp>
#include <cstddef>

struct DynamicSlot
{
    u16 specie;
    u8 maxLevel;
    u8 minLevel;
};
static_assert(sizeof(DynamicSlot) == 4);

struct GrottoSlot
{
    u16 specie;
    u8 maxLevel;
    u8 minLevel;
    u8 gender;
};
static_assert(sizeof(GrottoSlot) == 6);

struct StaticSlot
{
    u16 specie;
    u8 level;
};
static_assert(sizeof(StaticSlot) == 4);

struct WildEncounter5Season
{
    u8 grassRate;
    u8 grassDoubleRate;
    u8 grassSpecialRate;
    u8 surfRate;
    u8 surfSpecialRate;
    u8 fishRate;
    u8 fishSpecialRate;
    StaticSlot grass[12];
    StaticSlot grassDouble[12];
    StaticSlot grassSpecial[12];
    DynamicSlot surf[5];
    DynamicSlot surfSpecial[5];
    DynamicSlot fish[5];
    DynamicSlot fishSpecial[5];
};
static_assert(sizeof(WildEncounter5Season) == 232);

struct WildEncounter5
{
    u8 location;
    u8 seasonCount;
    WildEncounter5Season seasons[0];
};
static_assert(sizeof(WildEncounter5) == 2);

struct WildEncounterGrotto
{
    u8 location;
    GrottoSlot pokemon[12];
    std::array<u16, 16> items;
    std::array<u16, 16> hiddenItems;
};
static_assert(sizeof(WildEncounterGrotto) == 138);

namespace Encounters5
{
    const DreamRadarTemplate *getDreamRadarEncounters(int *size)
    {
        if (size)
        {
            *size = dreamRadar.size();
        }
        return dreamRadar.data();
    }

    const DreamRadarTemplate *getDreamRadarEncounters(int index)
    {
        return &dreamRadar[index];
    }

    std::vector<HiddenGrottoArea> getHiddenGrottoEncounters()
    {
        u32 length;
        const u8 *data = Utilities::decompress(bw2_grotto.data(), bw2_grotto.size(), length);

        const PersonalInfo *info = PersonalLoader::getPersonal(Game::BW2);

        std::vector<HiddenGrottoArea> encounters;
        for (size_t offset = 0; offset < length; offset += sizeof(WildEncounterGrotto))
        {
            const auto *entry = reinterpret_cast<const WildEncounterGrotto *>(data + offset);

            std::array<HiddenGrottoSlot, 12> pokemon;
            for (size_t i = 0; i < 12; i++)
            {
                const auto &slot = entry->pokemon[i];
                pokemon[i] = HiddenGrottoSlot(slot.specie, slot.gender, slot.minLevel, slot.maxLevel, &info[slot.specie]);
            }

            encounters.emplace_back(entry->location, pokemon, entry->items, entry->hiddenItems);
        }
        delete[] data;
        return encounters;
    }

    std::vector<EncounterArea5> getEncounters(Encounter encounter, u8 season, const Profile5 *profile)
    {
        const u8 *compressedData;
        size_t compressedLength;

        Game version = profile->getVersion();
        if (version == Game::Black)
        {
            compressedData = black.data();
            compressedLength = black.size();
        }
        else if (version == Game::Black2)
        {
            compressedData = black2.data();
            compressedLength = black2.size();
        }
        else if (version == Game::White)
        {
            compressedData = white.data();
            compressedLength = white.size();
        }
        else
        {
            compressedData = white2.data();
            compressedLength = white2.size();
        }

        u32 length;
        u8 *data = Utilities::decompress(compressedData, compressedLength, length);

        std::vector<EncounterArea5> encounters;
        for (size_t offset = 0; offset < length;)
        {
            const auto *entry = reinterpret_cast<const WildEncounter5 *>(data + offset);

            const auto *entrySeason = &entry->seasons[0];
            if (season < entry->seasonCount)
            {
                entrySeason = &entry->seasons[season];
            }

            switch (encounter)
            {
            case Encounter::Grass:
                if (entrySeason->grassRate != 0)
                {
                    std::vector<Slot> slots;
                    slots.reserve(12);
                    for (const auto &slot : entrySeason->grass)
                    {
                        slots.emplace_back(slot.specie & 0x7ff, slot.specie >> 11, slot.level, slot.level,
                                           PersonalLoader::getPersonal(version, slot.specie & 0x7ff, slot.specie >> 11));
                    }
                    encounters.emplace_back(entry->location, entrySeason->grassRate, encounter, slots);
                }
                break;
            case Encounter::DoubleGrass:
                if (entrySeason->grassDoubleRate != 0)
                {
                    std::vector<Slot> slots;
                    slots.reserve(12);
                    for (const auto &slot : entrySeason->grassDouble)
                    {
                        slots.emplace_back(slot.specie & 0x7ff, slot.specie >> 11, slot.level, slot.level,
                                           PersonalLoader::getPersonal(version, slot.specie & 0x7ff, slot.specie >> 11));
                    }
                    encounters.emplace_back(entry->location, entrySeason->grassDoubleRate, encounter, slots);
                }
                break;
            case Encounter::SpecialGrass:
                if (entrySeason->grassSpecialRate != 0)
                {
                    std::vector<Slot> slots;
                    slots.reserve(12);
                    for (const auto &slot : entrySeason->grassSpecial)
                    {
                        slots.emplace_back(slot.specie & 0x7ff, slot.specie >> 11, slot.level, slot.level,
                                           PersonalLoader::getPersonal(version, slot.specie & 0x7ff, slot.specie >> 11));
                    }
                    encounters.emplace_back(entry->location, entrySeason->grassSpecialRate, encounter, slots);
                }
                break;
            case Encounter::Surfing:
                if (entrySeason->surfRate != 0)
                {
                    std::vector<Slot> slots;
                    slots.reserve(5);
                    for (const auto &slot : entrySeason->surf)
                    {
                        slots.emplace_back(slot.specie & 0x7ff, slot.specie >> 11, slot.minLevel, slot.maxLevel,
                                           PersonalLoader::getPersonal(version, slot.specie & 0x7ff, slot.specie >> 11));
                    }
                    encounters.emplace_back(entry->location, entrySeason->surfRate, encounter, slots);
                }
                break;
            case Encounter::SpecialSurf:
                if (entrySeason->surfSpecialRate != 0)
                {
                    std::vector<Slot> slots;
                    slots.reserve(5);
                    for (const auto &slot : entrySeason->surfSpecial)
                    {
                        slots.emplace_back(slot.specie & 0x7ff, slot.specie >> 11, slot.minLevel, slot.maxLevel,
                                           PersonalLoader::getPersonal(version, slot.specie & 0x7ff, slot.specie >> 11));
                    }
                    encounters.emplace_back(entry->location, entrySeason->surfSpecialRate, encounter, slots);
                }
                break;
            case Encounter::SuperRod:
                if (entrySeason->fishRate != 0)
                {
                    std::vector<Slot> slots;
                    slots.reserve(5);
                    for (const auto &slot : entrySeason->fish)
                    {
                        slots.emplace_back(slot.specie & 0x7ff, slot.specie >> 11, slot.minLevel, slot.maxLevel,
                                           PersonalLoader::getPersonal(version, slot.specie & 0x7ff, slot.specie >> 11));
                    }
                    encounters.emplace_back(entry->location, entrySeason->fishRate, encounter, slots);
                }
                break;
            case Encounter::SpecialSuperRod:
                if (entrySeason->fishSpecialRate != 0)
                {
                    std::vector<Slot> slots;
                    slots.reserve(5);
                    for (const auto &slot : entrySeason->fishSpecial)
                    {
                        slots.emplace_back(slot.specie & 0x7ff, slot.specie >> 11, slot.minLevel, slot.maxLevel,
                                           PersonalLoader::getPersonal(version, slot.specie & 0x7ff, slot.specie >> 11));
                    }
                    encounters.emplace_back(entry->location, entrySeason->fishSpecialRate, encounter, slots);
                }
                break;
            default:
                break;
            }

            offset += sizeof(WildEncounter5) + entry->seasonCount * sizeof(WildEncounter5Season);
        }
        return encounters;
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
                *size = fossils.size();
            }
            return fossils.data();
        }
        else if (index == 2)
        {
            if (size)
            {
                *size = gifts.size();
            }
            return gifts.data();
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
                *size = legends.size();
            }
            return legends.data();
        }
        else if (index == 5)
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

    const StaticTemplate *getStaticEncounter(int type, int index)
    {
        const StaticTemplate *templates = getStaticEncounters(type);
        return &templates[index];
    }
}
