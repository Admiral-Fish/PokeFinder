/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2024 by Admiral_Fish, bumba, and EzPzStreamz
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
            *size = DREAMRADAR.size();
        }
        return DREAMRADAR.data();
    }

    const DreamRadarTemplate *getDreamRadarEncounters(int index)
    {
        return &DREAMRADAR[index];
    }

    std::vector<EncounterArea5> getEncounters(Encounter encounter, u8 season, const Profile5 *profile)
    {
        u32 length;
        u8 *data;

        Game version = profile->getVersion();
        if (version == Game::Black)
        {
            data = Utilities::decompress(BLACK.data(), BLACK.size(), length);
        }
        else if (version == Game::Black2)
        {
            data = Utilities::decompress(BLACK2.data(), BLACK2.size(), length);
        }
        else if (version == Game::White)
        {
            data = Utilities::decompress(WHITE.data(), WHITE.size(), length);
        }
        else
        {
            data = Utilities::decompress(WHITE2.data(), WHITE2.size(), length);
        }

        std::vector<EncounterArea5> encounters;
        for (size_t offset = 0; offset < length;)
        {
            const auto *entry = reinterpret_cast<const WildEncounter5 *>(data + offset);

            const auto *entrySeason = &entry->seasons[0];
            bool seasons = entry->seasonCount > 1;
            if (season < entry->seasonCount)
            {
                entrySeason = &entry->seasons[season];
            }

            std::array<Slot, 12> slots;
            switch (encounter)
            {
            case Encounter::Grass:
                if (entrySeason->grassRate != 0)
                {
                    for (size_t i = 0; i < 12; i++)
                    {
                        const auto &slot = entrySeason->grass[i];
                        slots[i] = Slot(slot.specie & 0x7ff, slot.specie >> 11, slot.level, slot.level,
                                        PersonalLoader::getPersonal(version, slot.specie & 0x7ff, slot.specie >> 11));
                    }
                    encounters.emplace_back(entry->location, entrySeason->grassRate, seasons, encounter, slots);
                }
                break;
            case Encounter::DoubleGrass:
                if (entrySeason->grassDoubleRate != 0)
                {
                    for (size_t i = 0; i < 12; i++)
                    {
                        const auto &slot = entrySeason->grassDouble[i];
                        slots[i] = Slot(slot.specie & 0x7ff, slot.specie >> 11, slot.level, slot.level,
                                        PersonalLoader::getPersonal(version, slot.specie & 0x7ff, slot.specie >> 11));
                    }
                    encounters.emplace_back(entry->location, entrySeason->grassDoubleRate, seasons, encounter, slots);
                }
                break;
            case Encounter::SpecialGrass:
                if (entrySeason->grassSpecialRate != 0)
                {
                    for (size_t i = 0; i < 12; i++)
                    {
                        const auto &slot = entrySeason->grassSpecial[i];
                        slots[i] = Slot(slot.specie & 0x7ff, slot.specie >> 11, slot.level, slot.level,
                                        PersonalLoader::getPersonal(version, slot.specie & 0x7ff, slot.specie >> 11));
                    }
                    encounters.emplace_back(entry->location, entrySeason->grassSpecialRate, seasons, encounter, slots);
                }
                break;
            case Encounter::Surfing:
                if (entrySeason->surfRate != 0)
                {
                    for (size_t i = 0; i < 5; i++)
                    {
                        const auto &slot = entrySeason->surf[i];
                        slots[i] = Slot(slot.specie & 0x7ff, slot.specie >> 11, slot.minLevel, slot.maxLevel,
                                        PersonalLoader::getPersonal(version, slot.specie & 0x7ff, slot.specie >> 11));
                    }
                    encounters.emplace_back(entry->location, entrySeason->surfRate, seasons, encounter, slots);
                }
                break;
            case Encounter::SpecialSurf:
                if (entrySeason->surfSpecialRate != 0)
                {
                    for (size_t i = 0; i < 5; i++)
                    {
                        const auto &slot = entrySeason->surfSpecial[i];
                        slots[i] = Slot(slot.specie & 0x7ff, slot.specie >> 11, slot.minLevel, slot.maxLevel,
                                        PersonalLoader::getPersonal(version, slot.specie & 0x7ff, slot.specie >> 11));
                    }
                    encounters.emplace_back(entry->location, entrySeason->surfSpecialRate, seasons, encounter, slots);
                }
                break;
            case Encounter::SuperRod:
                if (entrySeason->fishRate != 0)
                {
                    for (size_t i = 0; i < 5; i++)
                    {
                        const auto &slot = entrySeason->fish[i];
                        slots[i] = Slot(slot.specie & 0x7ff, slot.specie >> 11, slot.minLevel, slot.maxLevel,
                                        PersonalLoader::getPersonal(version, slot.specie & 0x7ff, slot.specie >> 11));
                    }
                    encounters.emplace_back(entry->location, entrySeason->fishRate, seasons, encounter, slots);
                }
                break;
            case Encounter::SpecialSuperRod:
                if (entrySeason->fishSpecialRate != 0)
                {
                    for (size_t i = 0; i < 5; i++)
                    {
                        const auto &slot = entrySeason->fishSpecial[i];
                        slots[i] = Slot(slot.specie & 0x7ff, slot.specie >> 11, slot.minLevel, slot.maxLevel,
                                        PersonalLoader::getPersonal(version, slot.specie & 0x7ff, slot.specie >> 11));
                    }
                    encounters.emplace_back(entry->location, entrySeason->fishSpecialRate, seasons, encounter, slots);
                }
                break;
            default:
                break;
            }

            offset += sizeof(WildEncounter5) + entry->seasonCount * sizeof(WildEncounter5Season);
        }
        delete[] data;
        return encounters;
    }

    std::vector<HiddenGrottoArea> getHiddenGrottoEncounters()
    {
        u32 length;
        const u8 *data = Utilities::decompress(BW2_GROTTO.data(), BW2_GROTTO.size(), length);

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

    const StaticTemplate5 *getStaticEncounters(int index, int *size)
    {
        if (index == 0)
        {
            if (size)
            {
                *size = STARTERS.size();
            }
            return STARTERS.data();
        }
        else if (index == 1)
        {
            if (size)
            {
                *size = FOSSILS.size();
            }
            return FOSSILS.data();
        }
        else if (index == 2)
        {
            if (size)
            {
                *size = GIFTS.size();
            }
            return GIFTS.data();
        }
        else if (index == 3)
        {
            if (size)
            {
                *size = STATIONARY.size();
            }
            return STATIONARY.data();
        }
        else if (index == 4)
        {
            if (size)
            {
                *size = LEGENDS.size();
            }
            return LEGENDS.data();
        }
        else if (index == 5)
        {
            if (size)
            {
                *size = EVENTS.size();
            }
            return EVENTS.data();
        }
        else
        {
            if (size)
            {
                *size = ROAMERS.size();
            }
            return ROAMERS.data();
        }
    }

    const StaticTemplate5 *getStaticEncounter(int type, int index)
    {
        const StaticTemplate5 *templates = getStaticEncounters(type);
        return &templates[index];
    }
}
