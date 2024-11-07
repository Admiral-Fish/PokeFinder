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
#include <cstring>

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

struct WildEncounterDPPt
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
    u16 ruby[2];
    u16 sapphire[2];
    u16 emerald[2];
    u16 firered[2];
    u16 leafgreen[2];
    DynamicSlot surf[5];
    DynamicSlot old[5];
    DynamicSlot good[5];
    DynamicSlot super[5];
};
static_assert(sizeof(WildEncounterDPPt) == 174);

struct WildEncounterDPPtHoney
{
    union {
        struct
        {
            DynamicSlot normal[6];
            DynamicSlot rare[6];
            DynamicSlot munchlax[6];
        };
        DynamicSlot slots[18];
    };
};
static_assert(sizeof(WildEncounterDPPtHoney) == 72);

struct HGSSEncounterGrass
{
    union {
        struct
        {
            u16 morning[12];
            u16 day[12];
            u16 night[12];
        };
        u16 slots[36];
    };
    u8 level[12];
};
static_assert(sizeof(HGSSEncounterGrass) == 84);

struct WildEncounterHGSS
{
    u8 location;
    u8 grassRate;
    u8 surfRate;
    u8 rockRate;
    u8 oldRate;
    u8 goodRate;
    u8 superRate;
    HGSSEncounterGrass grass;
    u16 hoennSound[2];
    u16 sinnohSound[2];
    DynamicSlot surf[5];
    DynamicSlot rock[2];
    DynamicSlot old[5];
    DynamicSlot good[5];
    DynamicSlot super[5];
    u16 swarm[4];
};
static_assert(sizeof(WildEncounterHGSS) == 196);

struct WildEncounterHGSSBug
{
    u8 location;
    DynamicSlot bug[10];
};
static_assert(sizeof(WildEncounterHGSSBug) == 42);

struct WildEncounterHGSSHeadbutt
{
    u8 location;
    bool hasSpecial;
    union {
        struct
        {
            DynamicSlot normal1[6];
            DynamicSlot normal2[6];
            DynamicSlot special[6];
        };
        DynamicSlot slots[18];
    };
};
static_assert(sizeof(WildEncounterHGSSHeadbutt) == 74);

template <int size>
struct WildSubEncounterHGSSSafari
{
    union {
        struct
        {
            StaticSlot morning[10];
            StaticSlot day[10];
            StaticSlot night[10];
        };
        StaticSlot slots[30];
    } normal;
    union {
        struct
        {
            StaticSlot morning[size];
            StaticSlot day[size];
            StaticSlot night[size];
        };
        StaticSlot slots[3 * size];
    } block;
    u8 type1[size];
    u8 quantity1[size];
    u8 type2[size];
    u8 quantity2[size];
};
static_assert(sizeof(WildSubEncounterHGSSSafari<10>) == 280);
static_assert(sizeof(WildSubEncounterHGSSSafari<3>) == 168);
static_assert(sizeof(WildSubEncounterHGSSSafari<2>) == 152);

struct WildEncounterHGSSSafari
{
    u8 location;
    bool hasWater;
    WildSubEncounterHGSSSafari<10> grass;
    WildSubEncounterHGSSSafari<3> surf;
    WildSubEncounterHGSSSafari<2> old;
    WildSubEncounterHGSSSafari<2> good;
    WildSubEncounterHGSSSafari<2> super;
};
static_assert(sizeof(WildEncounterHGSSSafari) == 906);

constexpr std::array<u16, 15> greatMarshDP = { 55, 183, 194, 195, 298, 315, 397, 399, 400, 451, 453, 455 };
constexpr std::array<u16, 15> greatMarshDPDex = { 46, 55, 102, 115, 193, 285, 315, 316, 397, 451, 452, 453, 454, 455 };

constexpr std::array<u16, 15> greatMarshPt = { 114, 193, 194, 195, 357, 451, 453, 455 };
constexpr std::array<u16, 15> greatMarshPtDex = { 46, 102, 114, 115, 193, 195, 285, 316, 352, 357, 451, 452, 453, 454, 455 };

constexpr std::array<u16, 16> trophyGardenDP = { 35, 39, 52, 113, 133, 137, 173, 174, 183, 298, 311, 312, 351, 438, 439, 440 };
constexpr std::array<u16, 16> trophyGardenPt = { 35, 39, 52, 113, 132, 133, 173, 174, 183, 298, 311, 312, 351, 438, 439, 440 };

/**
 * @brief Modifies encounter slots based on the dual slot game
 *
 * @param pokemon Vector of original encounters
 * @param entry Encounter area data
 * @param info Personal info array pointer
 * @param dual Dual slot game version
 */
static void modifyDual(std::array<Slot, 12> &pokemon, const WildEncounterDPPt *entry, const PersonalInfo *info, Game dual)
{
    u16 specie1;
    u16 specie2;
    if (dual == Game::Ruby)
    {
        specie1 = entry->ruby[0];
        specie2 = entry->ruby[1];
    }
    else if (dual == Game::Sapphire)
    {
        specie1 = entry->sapphire[0];
        specie2 = entry->sapphire[1];
    }
    else if (dual == Game::Emerald)
    {
        specie1 = entry->emerald[0];
        specie2 = entry->emerald[1];
    }
    else if (dual == Game::FireRed)
    {
        specie1 = entry->firered[0];
        specie2 = entry->firered[1];
    }
    else if (dual == Game::LeafGreen)
    {
        specie1 = entry->leafgreen[0];
        specie2 = entry->leafgreen[1];
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
static void modifyGreatMarsh(std::array<Slot, 12> &pokemon, const std::array<u16, 2> &replacement, const PersonalInfo *info, u8 location)
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
static void modifyRadar(std::array<Slot, 12> &pokemon, const WildEncounterDPPt *entry, const PersonalInfo *info, bool radar)
{
    if (radar)
    {
        pokemon[4].setSpecie(entry->radar[0], &info[entry->radar[0]]);
        pokemon[5].setSpecie(entry->radar[1], &info[entry->radar[1]]);
        pokemon[10].setSpecie(entry->radar[2], &info[entry->radar[2]]);
        pokemon[11].setSpecie(entry->radar[3], &info[entry->radar[3]]);
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
static void modifySwarmDPPt(std::array<Slot, 12> &pokemon, const WildEncounterDPPt *entry, const PersonalInfo *info, bool swarm)
{
    if (swarm)
    {
        pokemon[0].setSpecie(entry->swarm[0], &info[entry->swarm[0]]);
        pokemon[1].setSpecie(entry->swarm[1], &info[entry->swarm[1]]);
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
static void modifyTimeDPPt(std::array<Slot, 12> &pokemon, const WildEncounterDPPt *entry, const PersonalInfo *info, int time)
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
static void modifyTrophyGarden(std::array<Slot, 12> &pokemon, const std::array<u16, 2> &replacement, const PersonalInfo *info, u8 location)
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
 * @param settings Settings that impact wild encounter slots
 * @param info Personal info array pointer
 *
 * @return Vector of encounter areas
 */
static std::vector<EncounterArea4> getDPPt(Game version, Encounter encounter, const EncounterSettings4 &settings, const PersonalInfo *info)
{
    u32 length;
    u8 *data;

    std::vector<EncounterArea4> encounters;
    if (encounter == Encounter::Honey || encounter == Encounter::HoneyRare || encounter == Encounter::HoneyMunchlax)
    {
        if (version == Game::Diamond)
        {
            data = Utilities::decompress(D_HONEY.data(), D_HONEY.size(), length);
        }
        else if (version == Game::Pearl)
        {
            data = Utilities::decompress(P_HONEY.data(), P_HONEY.size(), length);
        }
        else
        {
            data = Utilities::decompress(PT_HONEY.data(), PT_HONEY.size(), length);
        }

        u8 honey = toInt(encounter) - toInt(Encounter::Honey);
        for (size_t offset = 0; offset < length; offset += sizeof(WildEncounterDPPtHoney))
        {
            const auto *entry = reinterpret_cast<const WildEncounterDPPtHoney *>(data + offset);

            std::array<Slot, 12> slots;

            const DynamicSlot *honeySlot = &entry->slots[6 * honey];
            for (size_t i = 0; i < 6; i++)
            {
                const auto &slot = honeySlot[i];
                slots[i] = Slot(slot.specie, slot.minLevel, slot.maxLevel, &info[slot.specie]);
            }
            encounters.emplace_back(0, 0, encounter, slots);
        }
    }
    else
    {
        if (version == Game::Diamond)
        {
            data = Utilities::decompress(DIAMOND.data(), DIAMOND.size(), length);
        }
        else if (version == Game::Pearl)
        {
            data = Utilities::decompress(PEARL.data(), PEARL.size(), length);
        }
        else
        {
            data = Utilities::decompress(PLATINUM.data(), PLATINUM.size(), length);
        }

        for (size_t offset = 0; offset < length; offset += sizeof(WildEncounterDPPt))
        {
            const auto *entry = reinterpret_cast<const WildEncounterDPPt *>(data + offset);

            std::array<Slot, 12> slots;
            switch (encounter)
            {
            case Encounter::Grass:
                if (entry->grassRate != 0)
                {
                    for (size_t i = 0; i < 12; i++)
                    {
                        const auto &slot = entry->grass[i];
                        slots[i] = Slot(slot.specie, slot.level, slot.level, &info[slot.specie]);
                    }
                    modifySwarmDPPt(slots, entry, info, settings.swarm);
                    modifyTimeDPPt(slots, entry, info, settings.time);
                    modifyRadar(slots, entry, info, settings.dppt.radar);
                    modifyGreatMarsh(slots, settings.dppt.replacement, info, entry->location);
                    modifyTrophyGarden(slots, settings.dppt.replacement, info, entry->location);
                    modifyDual(slots, entry, info, settings.dppt.dual);
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
            case Encounter::OldRod:
                if (entry->oldRate != 0)
                {
                    for (size_t i = 0; i < 5; i++)
                    {
                        const auto &slot = entry->old[i];
                        slots[i] = Slot(slot.specie, slot.minLevel, slot.maxLevel, &info[slot.specie]);
                    }

                    // Insert Feebas for Mt Coronet B1F
                    if (settings.dppt.feebasTile && entry->location == 22)
                    {
                        slots[5] = Slot(349, 10, 20, &info[349]);
                    }

                    encounters.emplace_back(entry->location, entry->oldRate, encounter, slots);
                }
                break;
            case Encounter::GoodRod:
                if (entry->goodRate != 0)
                {
                    for (size_t i = 0; i < 5; i++)
                    {
                        const auto &slot = entry->good[i];
                        slots[i] = Slot(slot.specie, slot.minLevel, slot.maxLevel, &info[slot.specie]);
                    }

                    // Insert Feebas for Mt Coronet B1F
                    if (settings.dppt.feebasTile && entry->location == 22)
                    {
                        slots[5] = Slot(349, 10, 20, &info[349]);
                    }

                    encounters.emplace_back(entry->location, entry->goodRate, encounter, slots);
                }
                break;
            case Encounter::SuperRod:
                if (entry->superRate != 0)
                {
                    for (size_t i = 0; i < 5; i++)
                    {
                        const auto &slot = entry->super[i];
                        slots[i] = Slot(slot.specie, slot.minLevel, slot.maxLevel, &info[slot.specie]);
                    }

                    // Insert Feebas for Mt Coronet B1F
                    if (settings.dppt.feebasTile && entry->location == 22)
                    {
                        slots[5] = Slot(349, 10, 20, &info[349]);
                    }

                    encounters.emplace_back(entry->location, entry->superRate, encounter, slots);
                }
                break;
            default:
                break;
            }
        }
    }
    delete[] data;
    return encounters;
}

/**
 * @brief Modifies encounter slots based on the radio station
 *
 * @param pokemon Vector of original encounters
 * @param entry Encounter area data
 * @param info Personal info array pointer
 * @param radio Radio station
 */
static void modifyRadio(std::array<Slot, 12> &pokemon, const WildEncounterHGSS *entry, const PersonalInfo *info, int radio)
{
    u16 specie1;
    u16 specie2;
    if (radio == 1)
    {
        specie1 = entry->hoennSound[0];
        specie2 = entry->hoennSound[1];
    }
    else if (radio == 2)
    {
        specie1 = entry->sinnohSound[0];
        specie2 = entry->sinnohSound[1];
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
 * @param entry Encounter area data
 * @param info Personal info array pointer
 * @param encounter Encounter type
 * @param swarm Whether swarm is active or not
 */
static void modifySwarmHGSS(std::array<Slot, 12> &pokemon, const WildEncounterHGSS *entry, const PersonalInfo *info, Encounter encounter,
                            bool swarm)
{
    if (swarm)
    {
        u16 specie;
        if (encounter == Encounter::Grass)
        {
            specie = entry->swarm[0];
            pokemon[0].setSpecie(specie, &info[specie]);
            pokemon[1].setSpecie(specie, &info[specie]);
        }
        else if (encounter == Encounter::Surfing)
        {
            specie = entry->swarm[1];
            pokemon[0].setSpecie(specie, &info[specie]);
        }
        else if (encounter == Encounter::OldRod)
        {
            specie = entry->swarm[3];
            pokemon[2].setSpecie(specie, &info[specie]);
        }
        else if (encounter == Encounter::GoodRod)
        {
            specie = entry->swarm[3];
            for (int i : { 0, 2, 3 })
            {
                pokemon[i].setSpecie(specie, &info[specie]);
            }
        }
        else if (encounter == Encounter::SuperRod)
        {
            specie = entry->swarm[3];
            for (size_t i = 0; i < 5; i++)
            {
                pokemon[i].setSpecie(specie, &info[specie]);
            }
        }
    }
}

static void modifyTimeHGSS(std::array<Slot, 12> &pokemon, const WildEncounterHGSS *entry, const PersonalInfo *info, Encounter encounter,
                           int time)
{
    if (time != 0 && time != 1)
    {
        return;
    }

    if (encounter == Encounter::GoodRod)
    {
        pokemon[3].setSpecie(entry->swarm[2], &info[entry->swarm[2]]);
    }
    else if (encounter == Encounter::SuperRod)
    {
        pokemon[1].setSpecie(entry->swarm[2], &info[entry->swarm[2]]);
    }
}

/**
 * @brief Gets the encounter area for HGSS Safari zone
 *
 * @param encounter Encounter type
 * @param settings Settings that impact wild encounter slots
 * @param info Personal info array pointer
 *
 * @return Vector of encounter areas
 */
static std::vector<EncounterArea4> getHGSSSafari(Encounter encounter, const EncounterSettings4 &settings, const PersonalInfo *info)
{
    u32 length;
    u8 *data = Utilities::decompress(HGSS_SAFARI.data(), HGSS_SAFARI.size(), length);

    std::vector<EncounterArea4> encounters;
    for (size_t offset = 0; offset < length; offset += sizeof(WildEncounterHGSSSafari))
    {
        const auto *entry = reinterpret_cast<const WildEncounterHGSSSafari *>(data + offset);

        size_t block = 0;
        const StaticSlot *safariSlots;
        const StaticSlot *safariBlockSlots;

        std::array<Slot, 12> slots;
        switch (encounter)
        {
        case Encounter::Grass:
            for (size_t i = 0; i < 10; i++)
            {
                safariSlots = &entry->grass.normal.slots[10 * settings.time];
                safariBlockSlots = &entry->grass.block.slots[10 * settings.time];

                u16 specie = safariSlots[i].specie;
                u8 level = safariSlots[i].level;
                for (; block < 10; block++)
                {
                    if (settings.hgss.blocks[entry->grass.type1[block]] >= entry->grass.quantity1[block]
                        && settings.hgss.blocks[entry->grass.type2[block]] >= entry->grass.quantity2[block])
                    {
                        specie = safariBlockSlots[block].specie;
                        level = safariBlockSlots[block].level;
                        block++;
                        break;
                    }
                }
                slots[i] = Slot(specie, level, level, &info[specie]);
            }
            encounters.emplace_back(entry->location, 0, encounter, slots);
            break;
        case Encounter::Surfing:
            if (entry->hasWater)
            {
                safariSlots = &entry->surf.normal.slots[10 * settings.time];
                safariBlockSlots = &entry->surf.block.slots[3 * settings.time];

                for (size_t i = 0; i < 10; i++)
                {
                    u16 specie = safariSlots[i].specie;
                    u8 level = safariSlots[i].level;
                    for (; block < 3; block++)
                    {
                        if (settings.hgss.blocks[entry->surf.type1[block]] >= entry->surf.quantity1[block]
                            && settings.hgss.blocks[entry->surf.type2[block]] >= entry->surf.quantity2[block])
                        {
                            specie = safariBlockSlots[block].specie;
                            level = safariBlockSlots[block].level;
                            block++;
                            break;
                        }
                    }
                    slots[i] = Slot(specie, level, level, &info[specie]);
                }
                encounters.emplace_back(entry->location, 0, encounter, slots);
            }
            break;
        case Encounter::OldRod:
            if (entry->hasWater)
            {
                safariSlots = &entry->old.normal.slots[10 * settings.time];
                safariBlockSlots = &entry->old.block.slots[2 * settings.time];

                for (size_t i = 0; i < 10; i++)
                {
                    u16 specie = safariSlots[i].specie;
                    u8 level = safariSlots[i].level;
                    for (; block < 2; block++)
                    {
                        if (settings.hgss.blocks[entry->surf.type1[block]] >= entry->old.quantity1[block]
                            && settings.hgss.blocks[entry->surf.type2[block]] >= entry->old.quantity2[block])
                        {
                            specie = safariBlockSlots[block].specie;
                            level = safariBlockSlots[block].level;
                            block++;
                            break;
                        }
                    }
                    slots[i] = Slot(specie, level, level, &info[specie]);
                }
                encounters.emplace_back(entry->location, 25, encounter, slots);
            }
            break;
        case Encounter::GoodRod:
            if (entry->hasWater)
            {
                safariSlots = &entry->good.normal.slots[10 * settings.time];
                safariBlockSlots = &entry->good.block.slots[2 * settings.time];

                for (size_t i = 0; i < 10; i++)
                {
                    u16 specie = safariSlots[i].specie;
                    u8 level = safariSlots[i].level;
                    for (; block < 2; block++)
                    {
                        if (settings.hgss.blocks[entry->good.type1[block]] >= entry->good.quantity1[block]
                            && settings.hgss.blocks[entry->good.type2[block]] >= entry->good.quantity2[block])
                        {
                            specie = safariBlockSlots[block].specie;
                            level = safariBlockSlots[block].level;
                            block++;
                            break;
                        }
                    }
                    slots[i] = Slot(specie, level, level, &info[specie]);
                }
                encounters.emplace_back(entry->location, 50, encounter, slots);
            }
            break;
        case Encounter::SuperRod:
            if (entry->hasWater)
            {
                safariSlots = &entry->super.normal.slots[10 * settings.time];
                safariBlockSlots = &entry->super.block.slots[2 * settings.time];

                for (size_t i = 0; i < 10; i++)
                {
                    u16 specie = safariSlots[i].specie;
                    u8 level = safariSlots[i].level;
                    for (; block < 2; block++)
                    {
                        if (settings.hgss.blocks[entry->super.type1[block]] >= entry->super.quantity1[block]
                            && settings.hgss.blocks[entry->super.type2[block]] >= entry->super.quantity2[block])
                        {
                            specie = safariBlockSlots[block].specie;
                            level = safariBlockSlots[block].level;
                            block++;
                            break;
                        }
                    }
                    slots[i] = Slot(specie, level, level, &info[specie]);
                }
                encounters.emplace_back(entry->location, 75, encounter, slots);
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
 * @brief Gets the encounter area for HGSS
 *
 * @param version Game version
 * @param encounter Encounter type
 * @param profile Profile information
 * @param settings Settings that impact wild encounter slots
 * @param info Personal info array pointer
 *
 * @return Vector of encounter areas
 */
static std::vector<EncounterArea4> getHGSS(Game version, Encounter encounter, const Profile4 *profile, const EncounterSettings4 &settings,
                                           const PersonalInfo *info)
{
    u8 *data;
    u32 length;

    std::vector<EncounterArea4> encounters;
    if (encounter == Encounter::BugCatchingContest)
    {
        data = Utilities::decompress(HGSS_BUG.data(), HGSS_BUG.size(), length);
        length = profile->getNationalDex() ? length : sizeof(WildEncounterHGSSBug);
        for (size_t offset = profile->getNationalDex() ? sizeof(WildEncounterHGSSBug) : 0; offset < length;
             offset += sizeof(WildEncounterHGSSBug))
        {
            const auto *entry = reinterpret_cast<const WildEncounterHGSSBug *>(data + offset);

            std::array<Slot, 12> slots;
            for (size_t i = 0; i < 10; i++)
            {
                const auto &slot = entry->bug[i];
                slots[i] = Slot(slot.specie, slot.minLevel, slot.maxLevel, &info[slot.specie]);
            }
            encounters.emplace_back(entry->location, 0, encounter, slots);
        }
    }
    else if (encounter == Encounter::Headbutt || encounter == Encounter::HeadbuttAlt || encounter == Encounter::HeadbuttSpecial)
    {
        if (version == Game::HeartGold)
        {
            data = Utilities::decompress(HG_HEADBUTT.data(), HG_HEADBUTT.size(), length);
        }
        else
        {
            data = Utilities::decompress(SS_HEADBUTT.data(), SS_HEADBUTT.size(), length);
        }

        u8 tree = toInt(encounter) - toInt(Encounter::Headbutt);
        for (size_t offset = 0; offset < length; offset += sizeof(WildEncounterHGSSHeadbutt))
        {
            const auto *entry = reinterpret_cast<const WildEncounterHGSSHeadbutt *>(data + offset);

            if (encounter != Encounter::HeadbuttSpecial || entry->hasSpecial)
            {
                std::array<Slot, 12> slots;

                const DynamicSlot *treeSlot = &entry->slots[6 * tree];
                for (size_t i = 0; i < 6; i++)
                {
                    const auto &slot = treeSlot[i];
                    slots[i] = Slot(slot.specie, slot.minLevel, slot.maxLevel, &info[slot.specie]);
                }
                encounters.emplace_back(entry->location, 0, encounter, slots);
            }
        }
    }
    else
    {
        if (version == Game::HeartGold)
        {
            data = Utilities::decompress(HEARTGOLD.data(), HEARTGOLD.size(), length);
        }
        else
        {
            data = Utilities::decompress(SOULSILVER.data(), SOULSILVER.size(), length);
        }

        for (size_t offset = 0; offset < length; offset += sizeof(WildEncounterHGSS))
        {
            const auto *entry = reinterpret_cast<const WildEncounterHGSS *>(data + offset);

            std::array<Slot, 12> slots;
            switch (encounter)
            {
            case Encounter::Grass:
                // Skip Ruins of Alph if the unlock requirements aren't met
                if (entry->location == 10)
                {
                    auto unlocked = profile->getUnlockedUnownForms();
                    if (unlocked.size() != 26 || !profile->getUndiscoveredUnownForms(unlocked).empty())
                    {
                        continue;
                    }
                }
                else if (entry->location == 11)
                {
                    if (profile->getUnlockedUnownForms().empty())
                    {
                        continue;
                    }
                }

                if (entry->grassRate != 0)
                {
                    const u16 *species = &entry->grass.slots[settings.time * 12];
                    for (size_t i = 0; i < 12; i++)
                    {
                        slots[i] = Slot(species[i], entry->grass.level[i], entry->grass.level[i], &info[species[i]]);
                    }
                    modifyRadio(slots, entry, info, settings.hgss.radio);
                    modifySwarmHGSS(slots, entry, info, encounter, settings.swarm);
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
                    modifySwarmHGSS(slots, entry, info, encounter, settings.swarm);
                    encounters.emplace_back(entry->location, entry->surfRate, encounter, slots);
                }
                break;
            case Encounter::RockSmash:
                if (entry->rockRate != 0)
                {
                    for (size_t i = 0; i < 2; i++)
                    {
                        const auto &slot = entry->rock[i];
                        slots[i] = Slot(slot.specie, slot.minLevel, slot.maxLevel, &info[slot.specie]);
                    }
                    encounters.emplace_back(entry->location, entry->rockRate, encounter, slots);
                }
                break;
            case Encounter::OldRod:
                if (entry->oldRate != 0)
                {
                    for (size_t i = 0; i < 5; i++)
                    {
                        const auto &slot = entry->old[i];
                        slots[i] = Slot(slot.specie, slot.minLevel, slot.maxLevel, &info[slot.specie]);
                    }
                    modifySwarmHGSS(slots, entry, info, encounter, settings.swarm);
                    encounters.emplace_back(entry->location, entry->oldRate, encounter, slots);
                }
                break;
            case Encounter::GoodRod:
                if (entry->goodRate != 0)
                {
                    for (size_t i = 0; i < 5; i++)
                    {
                        const auto &slot = entry->good[i];
                        slots[i] = Slot(slot.specie, slot.minLevel, slot.maxLevel, &info[slot.specie]);
                    }
                    modifyTimeHGSS(slots, entry, info, encounter, settings.time);
                    modifySwarmHGSS(slots, entry, info, encounter, settings.swarm);
                    encounters.emplace_back(entry->location, entry->goodRate, encounter, slots);
                }
                break;
            case Encounter::SuperRod:
                if (entry->superRate != 0)
                {
                    for (size_t i = 0; i < 5; i++)
                    {
                        const auto &slot = entry->super[i];
                        slots[i] = Slot(slot.specie, slot.minLevel, slot.maxLevel, &info[slot.specie]);
                    }
                    modifyTimeHGSS(slots, entry, info, encounter, settings.time);
                    modifySwarmHGSS(slots, entry, info, encounter, settings.swarm);
                    encounters.emplace_back(entry->location, entry->superRate, encounter, slots);
                }
                break;
            default:
                break;
            }
        }

        std::vector<EncounterArea4> safariEncounters = getHGSSSafari(encounter, settings, info);
        encounters.insert(encounters.end(), safariEncounters.begin(), safariEncounters.end());
    }

    delete[] data;
    return encounters;
}

namespace Encounters4
{
    std::vector<EncounterArea4> getEncounters(Encounter encounter, const EncounterSettings4 &settings, const Profile4 *profile)
    {
        Game version = profile->getVersion();
        const auto *info = PersonalLoader::getPersonal(version);
        if ((version & Game::DPPt) != Game::None)
        {
            return getDPPt(version, encounter, settings, info);
        }
        return getHGSS(version, encounter, profile, settings, info);
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

    const StaticTemplate4 *getStaticEncounters(int type, int *size)
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
        else
        {
            if (size)
            {
                *size = ROAMERS.size();
            }
            return ROAMERS.data();
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
