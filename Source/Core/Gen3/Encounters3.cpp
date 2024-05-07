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
        u32 length;
        u8 *data;

        if (version == Game::Emerald)
        {
            data = Utilities::decompress(EMERALD.data(), EMERALD.size(), length);
        }
        else if (version == Game::FireRed)
        {
            data = Utilities::decompress(FIRERED.data(), FIRERED.size(), length);
        }
        else if (version == Game::LeafGreen)
        {
            data = Utilities::decompress(LEAFGREEN.data(), LEAFGREEN.size(), length);
        }
        else if (version == Game::Ruby)
        {
            data = Utilities::decompress(RUBY.data(), RUBY.size(), length);
        }
        else
        {
            data = Utilities::decompress(SAPPHIRE.data(), SAPPHIRE.size(), length);
        }

        const PersonalInfo *info = PersonalLoader::getPersonal(version);

        std::vector<EncounterArea3> encounters;
        for (size_t offset = 0; offset < length; offset += sizeof(WildEncounter3))
        {
            const auto *entry = reinterpret_cast<const WildEncounter3 *>(data + offset);

            std::array<Slot, 12> slots;
            switch (encounter)
            {
            case Encounter::Grass:
                if (entry->grassRate != 0)
                {
                    for (size_t i = 0; i < 12; i++)
                    {
                        const auto &slot = entry->grass[i];
                        slots[i] = Slot(slot.specie & 0x7ff, slot.specie >> 11, slot.level, slot.level, &info[slot.specie & 0x7ff]);
                    }
                    encounters.emplace_back(entry->location, entry->grassRate, encounter, slots);
                }
                break;
            case Encounter::Surfing:
                if (entry->surfRate != 0)
                {
                    for (size_t i = 0; i < 5; i++)
                    {
                        const auto &slot = entry->surf[i];
                        slots[i] = Slot(slot.specie, slot.minLevel, slot.maxLevel, &info[slot.specie]);
                    }
                    encounters.emplace_back(entry->location, entry->surfRate, encounter, slots);
                }
                break;
            case Encounter::RockSmash:
                if (entry->rockRate != 0)
                {
                    for (size_t i = 0; i < 5; i++)
                    {
                        const auto &slot = entry->rock[i];
                        slots[i] = Slot(slot.specie, slot.minLevel, slot.maxLevel, &info[slot.specie]);
                    }
                    encounters.emplace_back(entry->location, entry->rockRate, encounter, slots);
                }
                break;
            case Encounter::OldRod:
                if (entry->fishRate != 0)
                {
                    for (size_t i = 0; i < 2; i++)
                    {
                        const auto &slot = entry->old[i];
                        slots[i] = Slot(slot.specie, slot.minLevel, slot.maxLevel, &info[slot.specie]);
                    }
                    encounters.emplace_back(entry->location, entry->fishRate, encounter, slots);
                }
                break;
            case Encounter::GoodRod:
                if (entry->fishRate != 0)
                {
                    for (size_t i = 0; i < 3; i++)
                    {
                        const auto &slot = entry->good[i];
                        slots[i] = Slot(slot.specie, slot.minLevel, slot.maxLevel, &info[slot.specie]);
                    }
                    encounters.emplace_back(entry->location, entry->fishRate, encounter, slots);
                }
                break;
            case Encounter::SuperRod:
                if (entry->fishRate != 0)
                {
                    for (size_t i = 0; i < 5; i++)
                    {
                        const auto &slot = entry->super[i];
                        slots[i] = Slot(slot.specie, slot.minLevel, slot.maxLevel, &info[slot.specie]);
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
        u8 *data = Utilities::decompress(XD.data(), XD.size(), length);

        const PersonalInfo *info = PersonalLoader::getPersonal(Game::Gen3);

        std::vector<EncounterArea> encounters;
        for (size_t offset = 0; offset < length; offset += sizeof(WildEncounterPokeSpot))
        {
            const auto *entry = reinterpret_cast<const WildEncounterPokeSpot *>(data + offset);

            std::array<Slot, 12> slots;
            for (size_t i = 0; i < 3; i++)
            {
                const auto &slot = entry->spot[i];
                slots[i] = Slot(slot.specie, slot.minLevel, slot.maxLevel, &info[slot.specie]);
            }
            encounters.emplace_back(entry->location, 0, Encounter::Grass, slots);
        }
        delete[] data;
        return encounters;
    }

    const ShadowTemplate *getShadowTeams(int *size)
    {
        if (size)
        {
            *size = GALESCOLOSHADOW.size();
        }
        return GALESCOLOSHADOW.data();
    }

    const ShadowTemplate *getShadowTeam(int index)
    {
        const ShadowTemplate *templates = getShadowTeams();
        return &templates[index];
    }

    const StaticTemplate3 *getStaticEncounters(int type, int *size)
    {
        if (type == 0)
        {
            if (size)
            {
                *size = STARTERS.size();
            }
            return STARTERS.data();
        }
        else if (type == 1)
        {
            if (size)
            {
                *size = FOSSILS.size();
            }
            return FOSSILS.data();
        }
        else if (type == 2)
        {
            if (size)
            {
                *size = GIFTS.size();
            }
            return GIFTS.data();
        }
        else if (type == 3)
        {
            if (size)
            {
                *size = GAMECORNER.size();
            }
            return GAMECORNER.data();
        }
        else if (type == 4)
        {
            if (size)
            {
                *size = STATIONARY.size();
            }
            return STATIONARY.data();
        }
        else if (type == 5)
        {
            if (size)
            {
                *size = LEGENDS.size();
            }
            return LEGENDS.data();
        }
        else if (type == 6)
        {
            if (size)
            {
                *size = EVENTS.size();
            }
            return EVENTS.data();
        }
        else if (type == 7)
        {
            if (size)
            {
                *size = ROAMERS.size();
            }
            return ROAMERS.data();
        }
        else if (type == 8)
        {
            if (size)
            {
                *size = GALESCOLO.size();
            }
            return GALESCOLO.data();
        }
        else
        {
            if (size)
            {
                *size = CHANNEL.size();
            }
            return CHANNEL.data();
        }
    }

    const StaticTemplate3 *getStaticEncounter(int type, int index)
    {
        const StaticTemplate3 *templates = getStaticEncounters(type);
        return &templates[index];
    }
}
