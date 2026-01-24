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
#include <algorithm>
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
    u8 location;
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
static_assert(sizeof(WildEncounterDPPtHoney) == 74);

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
    u16 grassSwarm;
    u16 waterSwarm;
    u16 fishNight;
    u16 fishSwarm;
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

constexpr std::array<u8, 21> honeyTreeMapIDs = { 145, 146, 147, 148, 149, 150, 156, 157, 159, 160, 161, 162, 163, 164, 167, 169, 170, 7, 8, 9, 183 };

constexpr std::array<u16, 16> trophyGardenDP = { 35, 39, 52, 113, 133, 137, 173, 174, 183, 298, 311, 312, 351, 438, 439, 440 };
constexpr std::array<u16, 16> trophyGardenPt = { 35, 39, 52, 113, 132, 133, 173, 174, 183, 298, 311, 312, 351, 438, 439, 440 };

/**
 * @brief Gets the tree ID from location
 *
 * @param location Location
 *
 * @return Tree ID
 */
static u8 getTreeIDFromMapID(u8 location)
{
    for (u8 i = 0; i < honeyTreeMapIDs.size(); i++)
    {
        if (location == honeyTreeMapIDs[i])
        {
            return i;
        }
    }

    return -1;
}

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
 * @brief Gets the encounter area for DPPt honey trees
 *
 * @param version Game version
 * @param profile Profile information
 * @param info Personal info array pointer
 *
 * @return Vector of encounter areas
 */
static std::vector<EncounterArea4> getDPPtHoney(Game version, const Profile4 *profile, const PersonalInfo *info)
{
    u32 length;
    const WildEncounterDPPtHoney *data;
    if (version == Game::Diamond)
    {
        data = Utilities::decompress<WildEncounterDPPtHoney>(D_HONEY.data(), D_HONEY.size(), length);
    }
    else if (version == Game::Pearl)
    {
        data = Utilities::decompress<WildEncounterDPPtHoney>(P_HONEY.data(), P_HONEY.size(), length);
    }
    else
    {
        data = Utilities::decompress<WildEncounterDPPtHoney>(PT_HONEY.data(), PT_HONEY.size(), length);
    }

    u16 tid = profile->getTID();
    u16 sid = profile->getSID();
    std::array<u8, 4> munchlaxTreeIDs = { static_cast<u8>((sid & 0xff) % 21), static_cast<u8>((sid >> 8) % 21),
                                          static_cast<u8>((tid & 0xff) % 21), static_cast<u8>((tid >> 8) % 21) };
    for (size_t i = 1; i < 4; i++)
    {
        for (size_t j = 0; j < i; j++)
        {
            if (munchlaxTreeIDs[j] == munchlaxTreeIDs[i])
            {
                ++munchlaxTreeIDs[i] %= 21;
            }
        }
    }

    std::vector<EncounterArea4> encounters;
    for (size_t i = 0; i < length; i++)
    {
        std::array<Slot, 12> slots;

        int max = 12;
        u8 treeID = getTreeIDFromMapID(data[i].location);
        if (std::any_of(munchlaxTreeIDs.begin(), munchlaxTreeIDs.end(), [treeID](u8 tree) { return tree == treeID; }))
        {
            max = 18;
        }

        // While we technically have 18 slots with the number of duplicates it will always be below 12
        int count = 0;
        for (int j = 0; j < max; j++)
        {
            const auto &slot = data[i].slots[j];
            if (std::all_of(slots.begin(), slots.begin() + count, [slot](const auto &s) { return s.getSpecie() != slot.specie; }))
            {
                slots[count++] = Slot(slot.specie, slot.minLevel, slot.maxLevel, &info[slot.specie]);
            }
        }

        encounters.emplace_back(data[i].location, 0, Encounter::HoneyTree, slots);
    }

    delete[] data;
    return encounters;
}

/**
 * @brief Gets the encounter area for DPPt
 *
 * @param version Game version
 * @param encounter Encounter type
 * @param profile Profile information
 * @param settings Settings that impact wild encounter slots
 * @param info Personal info array pointer
 *
 * @return Vector of encounter areas
 */
static std::vector<EncounterArea4> getDPPt(Game version, Encounter encounter, const Profile4 *profile, const EncounterSettings4 &settings,
                                           const PersonalInfo *info)
{
    std::vector<EncounterArea4> encounters;
    if (encounter == Encounter::HoneyTree)
    {
        encounters = getDPPtHoney(version, profile, info);
    }
    else
    {
        u32 length;
        const WildEncounterDPPt *data;

        if (version == Game::Diamond)
        {
            data = Utilities::decompress<WildEncounterDPPt>(DIAMOND.data(), DIAMOND.size(), length);
        }
        else if (version == Game::Pearl)
        {
            data = Utilities::decompress<WildEncounterDPPt>(PEARL.data(), PEARL.size(), length);
        }
        else
        {
            data = Utilities::decompress<WildEncounterDPPt>(PLATINUM.data(), PLATINUM.size(), length);
        }

        for (size_t i = 0; i < length; i++)
        {
            std::array<Slot, 12> slots;
            switch (encounter)
            {
            case Encounter::Grass:
                if (data[i].grassRate != 0)
                {
                    for (size_t j = 0; j < 12; j++)
                    {
                        const auto &slot = data[i].grass[j];
                        slots[j] = Slot(slot.specie, slot.level, slot.level, &info[slot.specie]);
                    }
                    modifySwarmDPPt(slots, &data[i], info, settings.swarm);
                    modifyTimeDPPt(slots, &data[i], info, settings.time);
                    modifyRadar(slots, &data[i], info, settings.dppt.radar);
                    modifyGreatMarsh(slots, settings.dppt.replacement, info, data[i].location);
                    modifyTrophyGarden(slots, settings.dppt.replacement, info, data[i].location);
                    modifyDual(slots, &data[i], info, settings.dppt.dual);
                    encounters.emplace_back(data[i].location, data[i].grassRate, encounter, slots);
                }
                break;
            case Encounter::Surfing:
                if (data[i].surfRate != 0)
                {
                    for (size_t j = 0; j < 5; j++)
                    {
                        const auto &slot = data[i].surf[j];
                        slots[j] = Slot(slot.specie, slot.minLevel, slot.maxLevel, &info[slot.specie]);
                    }
                    encounters.emplace_back(data[i].location, data[i].surfRate, encounter, slots);
                }
                break;
            case Encounter::OldRod:
                if (data[i].oldRate != 0)
                {
                    for (size_t j = 0; j < 5; j++)
                    {
                        const auto &slot = data[i].old[j];
                        slots[j] = Slot(slot.specie, slot.minLevel, slot.maxLevel, &info[slot.specie]);
                    }

                    // Insert Feebas for Mt Coronet B1F
                    if (settings.dppt.feebasTile && data[i].location == 22)
                    {
                        slots[5] = Slot(349, 10, 20, &info[349]);
                    }

                    encounters.emplace_back(data[i].location, data[i].oldRate, encounter, slots);
                }
                break;
            case Encounter::GoodRod:
                if (data[i].goodRate != 0)
                {
                    for (size_t j = 0; j < 5; j++)
                    {
                        const auto &slot = data[i].good[j];
                        slots[j] = Slot(slot.specie, slot.minLevel, slot.maxLevel, &info[slot.specie]);
                    }

                    // Insert Feebas for Mt Coronet B1F
                    if (settings.dppt.feebasTile && data[i].location == 22)
                    {
                        slots[5] = Slot(349, 10, 20, &info[349]);
                    }

                    encounters.emplace_back(data[i].location, data[i].goodRate, encounter, slots);
                }
                break;
            case Encounter::SuperRod:
                if (data[i].superRate != 0)
                {
                    for (size_t j = 0; j < 5; j++)
                    {
                        const auto &slot = data[i].super[j];
                        slots[j] = Slot(slot.specie, slot.minLevel, slot.maxLevel, &info[slot.specie]);
                    }

                    // Insert Feebas for Mt Coronet B1F
                    if (settings.dppt.feebasTile && data[i].location == 22)
                    {
                        slots[5] = Slot(349, 10, 20, &info[349]);
                    }

                    encounters.emplace_back(data[i].location, data[i].superRate, encounter, slots);
                }
                break;
            default:
                break;
            }
        }
        delete[] data;
    }
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
            specie = entry->grassSwarm;
            pokemon[0].setSpecie(specie, &info[specie]);
            pokemon[1].setSpecie(specie, &info[specie]);
        }
        else if (encounter == Encounter::Surfing)
        {
            specie = entry->waterSwarm;
            pokemon[0].setSpecie(specie, &info[specie]);
        }
        else if (encounter == Encounter::OldRod)
        {
            specie = entry->fishSwarm;
            pokemon[2].setSpecie(specie, &info[specie]);
        }
        else if (encounter == Encounter::GoodRod)
        {
            specie = entry->fishSwarm;
            for (int i : { 0, 2, 3 })
            {
                pokemon[i].setSpecie(specie, &info[specie]);
            }
        }
        else if (encounter == Encounter::SuperRod)
        {
            specie = entry->fishSwarm;
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
        pokemon[3].setSpecie(entry->fishNight, &info[entry->fishNight]);
    }
    else if (encounter == Encounter::SuperRod)
    {
        pokemon[1].setSpecie(entry->fishNight, &info[entry->fishNight]);
    }
}

/**
 * @brief Gets the encounter area for HGSS bug catching contest
 *
 * @param profile Profile information
 * @param info Personal info array pointer
 *
 * @return Vector of encounter areas
 */
static std::vector<EncounterArea4> getHGSSBug(const Profile4 *profile, const PersonalInfo *info)
{
    u32 length;
    auto *data = Utilities::decompress<WildEncounterHGSSBug>(HGSS_BUG.data(), HGSS_BUG.size(), length);
    length = profile->getNationalDex() ? length : 1;

    std::vector<EncounterArea4> encounters;
    for (size_t i = profile->getNationalDex() ? 1 : 0; i < length; i++)
    {
        std::array<Slot, 12> slots;
        for (size_t j = 0; j < 10; j++)
        {
            const auto &slot = data[i].bug[j];
            slots[j] = Slot(slot.specie, slot.minLevel, slot.maxLevel, &info[slot.specie]);
        }
        encounters.emplace_back(data[i].location, 0, Encounter::BugCatchingContest, slots);
    }

    delete[] data;
    return encounters;
}

/**
 * @brief Gets the encounter area for HGSS headbutt
 *
 * @param version Game version
 * @param encounter Encounter type
 * @param info Personal info array pointer
 *
 * @return Vector of encounter areas
 */
static std::vector<EncounterArea4> getHGSSHeadbutt(Game version, Encounter encounter, const PersonalInfo *info)
{
    u32 length;
    const WildEncounterHGSSHeadbutt *data;
    if (version == Game::HeartGold)
    {
        data = Utilities::decompress<WildEncounterHGSSHeadbutt>(HG_HEADBUTT.data(), HG_HEADBUTT.size(), length);
    }
    else
    {
        data = Utilities::decompress<WildEncounterHGSSHeadbutt>(SS_HEADBUTT.data(), SS_HEADBUTT.size(), length);
    }

    u8 tree = toInt(encounter) - toInt(Encounter::Headbutt);
    std::vector<EncounterArea4> encounters;
    for (size_t i = 0; i < length; i++)
    {
        if (encounter != Encounter::HeadbuttSpecial || data[i].hasSpecial)
        {
            std::array<Slot, 12> slots;

            auto *treeSlot = &data[i].slots[6 * tree];
            for (size_t j = 0; j < 6; j++)
            {
                const auto &slot = treeSlot[j];
                slots[j] = Slot(slot.specie, slot.minLevel, slot.maxLevel, &info[slot.specie]);
            }
            encounters.emplace_back(data[i].location, 0, encounter, slots);
        }
    }

    delete[] data;
    return encounters;
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
    auto *data = Utilities::decompress<WildEncounterHGSSSafari>(HGSS_SAFARI.data(), HGSS_SAFARI.size(), length);

    std::vector<EncounterArea4> encounters;
    for (size_t i = 0; i < length; i++)
    {
        size_t block = 0;

        std::array<Slot, 12> slots;
        switch (encounter)
        {
        case Encounter::Grass:
            for (size_t j = 0; j < 10; j++)
            {
                auto *safariSlots = &data[i].grass.normal.slots[10 * settings.time];
                auto *safariBlockSlots = &data[i].grass.block.slots[10 * settings.time];

                u16 specie = safariSlots[j].specie;
                u8 level = safariSlots[j].level;
                for (; block < 10; block++)
                {
                    if (settings.hgss.blocks[data[i].grass.type1[block]] >= data[i].grass.quantity1[block]
                        && settings.hgss.blocks[data[i].grass.type2[block]] >= data[i].grass.quantity2[block])
                    {
                        specie = safariBlockSlots[block].specie;
                        level = safariBlockSlots[block].level;
                        block++;
                        break;
                    }
                }
                slots[j] = Slot(specie, level, level, &info[specie]);
            }
            encounters.emplace_back(data[i].location, 0, encounter, slots);
            break;
        case Encounter::Surfing:
            if (data[i].hasWater)
            {
                auto *safariSlots = &data[i].surf.normal.slots[10 * settings.time];
                auto *safariBlockSlots = &data[i].surf.block.slots[3 * settings.time];

                for (size_t j = 0; j < 10; j++)
                {
                    u16 specie = safariSlots[j].specie;
                    u8 level = safariSlots[j].level;
                    for (; block < 3; block++)
                    {
                        if (settings.hgss.blocks[data[i].surf.type1[block]] >= data[i].surf.quantity1[block]
                            && settings.hgss.blocks[data[i].surf.type2[block]] >= data[i].surf.quantity2[block])
                        {
                            specie = safariBlockSlots[block].specie;
                            level = safariBlockSlots[block].level;
                            block++;
                            break;
                        }
                    }
                    slots[j] = Slot(specie, level, level, &info[specie]);
                }
                encounters.emplace_back(data[i].location, 0, encounter, slots);
            }
            break;
        case Encounter::OldRod:
            if (data[i].hasWater)
            {
                auto *safariSlots = &data[i].old.normal.slots[10 * settings.time];
                auto *safariBlockSlots = &data[i].old.block.slots[2 * settings.time];

                for (size_t j = 0; j < 10; j++)
                {
                    u16 specie = safariSlots[j].specie;
                    u8 level = safariSlots[j].level;
                    for (; block < 2; block++)
                    {
                        if (settings.hgss.blocks[data[i].surf.type1[block]] >= data[i].old.quantity1[block]
                            && settings.hgss.blocks[data[i].surf.type2[block]] >= data[i].old.quantity2[block])
                        {
                            specie = safariBlockSlots[block].specie;
                            level = safariBlockSlots[block].level;
                            block++;
                            break;
                        }
                    }
                    slots[j] = Slot(specie, level, level, &info[specie]);
                }
                encounters.emplace_back(data[i].location, 25, encounter, slots);
            }
            break;
        case Encounter::GoodRod:
            if (data[i].hasWater)
            {
                auto *safariSlots = &data[i].good.normal.slots[10 * settings.time];
                auto *safariBlockSlots = &data[i].good.block.slots[2 * settings.time];

                for (size_t j = 0; j < 10; j++)
                {
                    u16 specie = safariSlots[j].specie;
                    u8 level = safariSlots[j].level;
                    for (; block < 2; block++)
                    {
                        if (settings.hgss.blocks[data[i].good.type1[block]] >= data[i].good.quantity1[block]
                            && settings.hgss.blocks[data[i].good.type2[block]] >= data[i].good.quantity2[block])
                        {
                            specie = safariBlockSlots[block].specie;
                            level = safariBlockSlots[block].level;
                            block++;
                            break;
                        }
                    }
                    slots[j] = Slot(specie, level, level, &info[specie]);
                }
                encounters.emplace_back(data[i].location, 50, encounter, slots);
            }
            break;
        case Encounter::SuperRod:
            if (data[i].hasWater)
            {
                auto *safariSlots = &data[i].super.normal.slots[10 * settings.time];
                auto *safariBlockSlots = &data[i].super.block.slots[2 * settings.time];

                for (size_t j = 0; j < 10; j++)
                {
                    u16 specie = safariSlots[j].specie;
                    u8 level = safariSlots[j].level;
                    for (; block < 2; block++)
                    {
                        if (settings.hgss.blocks[data[i].super.type1[block]] >= data[i].super.quantity1[block]
                            && settings.hgss.blocks[data[i].super.type2[block]] >= data[i].super.quantity2[block])
                        {
                            specie = safariBlockSlots[block].specie;
                            level = safariBlockSlots[block].level;
                            block++;
                            break;
                        }
                    }
                    slots[j] = Slot(specie, level, level, &info[specie]);
                }
                encounters.emplace_back(data[i].location, 75, encounter, slots);
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
    std::vector<EncounterArea4> encounters;
    if (encounter == Encounter::BugCatchingContest)
    {
        encounters = getHGSSBug(profile, info);
    }
    else if (encounter == Encounter::Headbutt || encounter == Encounter::HeadbuttAlt || encounter == Encounter::HeadbuttSpecial)
    {
        encounters = getHGSSHeadbutt(version, encounter, info);
    }
    else
    {
        u32 length;
        const WildEncounterHGSS *data;
        if (version == Game::HeartGold)
        {
            data = Utilities::decompress<WildEncounterHGSS>(HEARTGOLD.data(), HEARTGOLD.size(), length);
        }
        else
        {
            data = Utilities::decompress<WildEncounterHGSS>(SOULSILVER.data(), SOULSILVER.size(), length);
        }

        for (size_t i = 0; i < length; i++)
        {
            std::array<Slot, 12> slots;
            switch (encounter)
            {
            case Encounter::Grass:
                // Skip Ruins of Alph if the unlock requirements aren't met
                if (data[i].location == 10)
                {
                    auto unlocked = profile->getUnlockedUnownForms();
                    if (unlocked.size() != 26 || !profile->getUndiscoveredUnownForms(unlocked).empty())
                    {
                        continue;
                    }
                }
                else if (data[i].location == 11)
                {
                    if (profile->getUnlockedUnownForms().empty())
                    {
                        continue;
                    }
                }

                if (data[i].grassRate != 0)
                {
                    const u16 *species = &data[i].grass.slots[settings.time * 12];
                    for (size_t j = 0; j < 12; j++)
                    {
                        slots[j] = Slot(species[j], data[i].grass.level[j], data[i].grass.level[j], &info[species[j]]);
                    }
                    modifyRadio(slots, &data[i], info, settings.hgss.radio);
                    modifySwarmHGSS(slots, &data[i], info, encounter, settings.swarm);
                    encounters.emplace_back(data[i].location, data[i].grassRate, encounter, slots);
                }
                break;
            case Encounter::Surfing:
                if (data[i].surfRate != 0)
                {
                    for (size_t j = 0; j < 5; j++)
                    {
                        const auto &slot = data[i].surf[j];
                        slots[j] = Slot(slot.specie, slot.minLevel, slot.maxLevel, &info[slot.specie]);
                    }
                    modifySwarmHGSS(slots, &data[i], info, encounter, settings.swarm);
                    encounters.emplace_back(data[i].location, data[i].surfRate, encounter, slots);
                }
                break;
            case Encounter::RockSmash:
                if (data[i].rockRate != 0)
                {
                    for (size_t j = 0; j < 2; j++)
                    {
                        const auto &slot = data[i].rock[j];
                        slots[j] = Slot(slot.specie, slot.minLevel, slot.maxLevel, &info[slot.specie]);
                    }
                    encounters.emplace_back(data[i].location, data[i].rockRate, encounter, slots);
                }
                break;
            case Encounter::OldRod:
                if (data[i].oldRate != 0)
                {
                    for (size_t j = 0; j < 5; j++)
                    {
                        const auto &slot = data[i].old[j];
                        slots[j] = Slot(slot.specie, slot.minLevel, slot.maxLevel, &info[slot.specie]);
                    }
                    modifySwarmHGSS(slots, &data[i], info, encounter, settings.swarm);
                    encounters.emplace_back(data[i].location, data[i].oldRate, encounter, slots);
                }
                break;
            case Encounter::GoodRod:
                if (data[i].goodRate != 0)
                {
                    for (size_t j = 0; j < 5; j++)
                    {
                        const auto &slot = data[i].good[j];
                        slots[j] = Slot(slot.specie, slot.minLevel, slot.maxLevel, &info[slot.specie]);
                    }
                    modifyTimeHGSS(slots, &data[i], info, encounter, settings.time);
                    modifySwarmHGSS(slots, &data[i], info, encounter, settings.swarm);
                    encounters.emplace_back(data[i].location, data[i].goodRate, encounter, slots);
                }
                break;
            case Encounter::SuperRod:
                if (data[i].superRate != 0)
                {
                    for (size_t j = 0; j < 5; j++)
                    {
                        const auto &slot = data[i].super[j];
                        slots[j] = Slot(slot.specie, slot.minLevel, slot.maxLevel, &info[slot.specie]);
                    }
                    modifyTimeHGSS(slots, &data[i], info, encounter, settings.time);
                    modifySwarmHGSS(slots, &data[i], info, encounter, settings.swarm);
                    encounters.emplace_back(data[i].location, data[i].superRate, encounter, slots);
                }
                break;
            default:
                break;
            }
        }

        delete[] data;

        std::vector<EncounterArea4> safariEncounters = getHGSSSafari(encounter, settings, info);
        encounters.insert(encounters.end(), safariEncounters.begin(), safariEncounters.end());
    }

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
            return getDPPt(version, encounter, profile, settings, info);
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
