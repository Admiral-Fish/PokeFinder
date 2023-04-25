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

#include "Encounters3.hpp"
#include <Core/Enum/Encounter.hpp>
#include <Core/Enum/Game.hpp>
#include <Core/Gen3/EncounterArea3.hpp>
#include <Core/Gen3/LockInfo.hpp>
#include <Core/Gen3/Profile3.hpp>
#include <Core/Parents/PersonalInfo.hpp>
#include <Core/Parents/PersonalLoader.hpp>
#include <Core/Parents/Slot.hpp>
#include <Core/Resources/EncounterData3.hpp>
#include <Core/Util/Utilities.hpp>
#include <algorithm>
#include <iterator>

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

struct WildEncounter3
{
    u8 location;
    u8 grassRate;
    u8 surfRate;
    u8 rockRate;
    u8 fishRate;
    StaticSlot grass[12];
    DynamicSlot surf[5];
    DynamicSlot rock[5];
    DynamicSlot old[2];
    DynamicSlot good[3];
    DynamicSlot super[5];
};
static_assert(sizeof(WildEncounter3) == 134);

struct WildEncounterPokeSpot
{
    u8 location;
    DynamicSlot spot[3];
};
static_assert(sizeof(WildEncounterPokeSpot) == 14);

namespace Encounters3
{
    std::vector<EncounterArea3> getEncounters(Encounter encounter, Game version)
    {
        const u8 *compressedData;
        std::size_t compressedLength;

        if (version == Game::Emerald)
        {
            compressedData = emerald.data();
            compressedLength = emerald.size();
        }
        else if (version == Game::FireRed)
        {
            compressedData = firered.data();
            compressedLength = firered.size();
        }
        else if (version == Game::LeafGreen)
        {
            compressedData = leafgreen.data();
            compressedLength = leafgreen.size();
        }
        else if (version == Game::Ruby)
        {
            compressedData = ruby.data();
            compressedLength = ruby.size();
        }
        else
        {
            compressedData = sapphire.data();
            compressedLength = sapphire.size();
        }

        u32 length;
        u8 *data = Utilities::decompress(compressedData, compressedLength, length);

        const PersonalInfo *info = PersonalLoader::getPersonal(version);

        std::vector<EncounterArea3> encounters;
        for (std::size_t offset = 0; offset < length; offset += sizeof(WildEncounter3))
        {
            const auto *entry = reinterpret_cast<const WildEncounter3 *>(data + offset);

            switch (encounter)
            {
            case Encounter::Grass:
                if (entry->grassRate != 0)
                {
                    std::vector<Slot> slots;
                    slots.reserve(12);
                    for (const auto &slot : entry->grass)
                    {
                        slots.emplace_back(slot.specie & 0x7ff, slot.specie >> 11, slot.level, slot.level, &info[slot.specie & 0x7ff]);
                    }
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
            case Encounter::RockSmash:
                if (entry->rockRate != 0)
                {
                    std::vector<Slot> slots;
                    slots.reserve(5);
                    for (const auto &slot : entry->rock)
                    {
                        slots.emplace_back(slot.specie, slot.minLevel, slot.maxLevel, &info[slot.specie]);
                    }
                    encounters.emplace_back(entry->location, entry->rockRate, encounter, slots);
                }
                break;
            case Encounter::OldRod:
                if (entry->fishRate != 0)
                {
                    std::vector<Slot> slots;
                    slots.reserve(2);
                    for (const auto &slot : entry->old)
                    {
                        slots.emplace_back(slot.specie, slot.minLevel, slot.maxLevel, &info[slot.specie]);
                    }
                    encounters.emplace_back(entry->location, entry->fishRate, encounter, slots);
                }
                break;
            case Encounter::GoodRod:
                if (entry->fishRate != 0)
                {
                    std::vector<Slot> slots;
                    slots.reserve(3);
                    for (const auto &slot : entry->good)
                    {
                        slots.emplace_back(slot.specie, slot.minLevel, slot.maxLevel, &info[slot.specie]);
                    }
                    encounters.emplace_back(entry->location, entry->fishRate, encounter, slots);
                }
                break;
            case Encounter::SuperRod:
                if (entry->fishRate != 0)
                {
                    std::vector<Slot> slots;
                    slots.reserve(5);
                    for (const auto &slot : entry->super)
                    {
                        slots.emplace_back(slot.specie, slot.minLevel, slot.maxLevel, &info[slot.specie]);
                    }
                    encounters.emplace_back(entry->location, entry->fishRate, encounter, slots);
                }
                break;
            default:
                break;
            }
        }
        delete[] data;
        return encounters;
    }

    std::vector<EncounterArea> getPokeSpotEncounters()
    {
        u32 length;
        u8 *data = Utilities::decompress(xd.data(), xd.size(), length);

        const PersonalInfo *info = PersonalLoader::getPersonal(Game::Gen3);

        std::vector<EncounterArea> encounters;
        for (std::size_t offset = 0; offset < length; offset += sizeof(WildEncounterPokeSpot))
        {
            const auto *entry = reinterpret_cast<const WildEncounterPokeSpot *>(data + offset);

            std::vector<Slot> slots;
            slots.reserve(3);
            for (const auto &slot : entry->spot)
            {
                slots.emplace_back(slot.specie, slot.minLevel, slot.maxLevel, &info[slot.specie]);
            }
            encounters.emplace_back(entry->location, 0, Encounter::Grass, slots);
        }
        delete[] data;
        return encounters;
    }

    const ShadowTemplate *getShadowTeams(std::size_t *size)
    {
        if (size)
        {
            *size = galesColoShadow.size();
        }
        return galesColoShadow.data();
    }

    const ShadowTemplate *getShadowTeam(int index)
    {
        const ShadowTemplate *templates = getShadowTeams();
        return &templates[index];
    }

    const StaticTemplate *getStaticEncounters(int type, std::size_t *size)
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
        else if (type == 7)
        {
            if (size)
            {
                *size = galesColo.size();
            }
            return galesColo.data();
        }
        else
        {
            if (size)
            {
                *size = channel.size();
            }
            return channel.data();
        }
    }

    const StaticTemplate *getStaticEncounter(int type, int index)
    {
        const StaticTemplate *templates = getStaticEncounters(type);
        return &templates[index];
    }
}
