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
#include <Core/Gen4/EncounterArea4.hpp>
#include <Core/Gen4/Profile4.hpp>
#include <Core/Parents/PersonalLoader.hpp>
#include <Core/Parents/Slot.hpp>
#include <Core/Resources/Encounters.hpp>
#include <algorithm>
#include <cstring>
#include <iterator>

namespace Encounters4
{
    namespace
    {
        void modifyRadio(std::vector<Slot> &mons, const u8 *data, const PersonalInfo *info, int radio)
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

            mons[2].setSpecie(specie1, info[specie1]);
            mons[3].setSpecie(specie1, info[specie1]);
            mons[4].setSpecie(specie2, info[specie2]);
            mons[5].setSpecie(specie2, info[specie2]);
        }

        void modifySwarmHGSS(std::vector<Slot> &mons, const u8 *data, const PersonalInfo *info, Encounter encounter, bool swarm)
        {
            if (swarm)
            {
                u16 specie;
                if (encounter == Encounter::Grass)
                {
                    specie = *reinterpret_cast<const u16 *>(data + 187);
                    mons[0].setSpecie(specie, info[specie]);
                    mons[1].setSpecie(specie, info[specie]);
                }
                else if (encounter == Encounter::Surfing)
                {
                    specie = *reinterpret_cast<const u16 *>(data + 189);
                    mons[0].setSpecie(specie, info[specie]);
                }
                else if (encounter == Encounter::OldRod)
                {
                    specie = *reinterpret_cast<const u16 *>(data + 193);
                    mons[2].setSpecie(specie, info[specie]);
                }
                else if (encounter == Encounter::GoodRod)
                {
                    specie = *reinterpret_cast<const u16 *>(data + 193);
                    for (int i : { 0, 2, 3 })
                    {
                        mons[i].setSpecie(specie, info[specie]);
                    }
                }
                else if (encounter == Encounter::SuperRod)
                {
                    specie = *reinterpret_cast<const u16 *>(data + 193);
                    for (int i = 0; i < 5; i++)
                    {
                        mons[i].setSpecie(specie, info[specie]);
                    }
                }
            }
        }

        std::vector<EncounterArea4> getHGSS(Game version, Encounter encounter, const Profile4 &profile, const PersonalInfo *info,
                                            int modifier, Encounter safariEncounter, std::array<u8, 5> blocks)
        {
            const u8 *data;
            size_t size;

            if (version == Game::HeartGold)
            {
                data = heartgold.data();
                size = heartgold.size();
            }
            else
            {
                data = soulsilver.data();
                size = soulsilver.size();
            }

            std::vector<EncounterArea4> encounters;
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

                if (grass != 0 && encounter == Encounter::Grass)
                {
                    std::vector<Slot> slots;
                    for (int i = 0; i < 12; i++)
                    {
                        u8 level = entry[7 + i];
                        u16 specie = *reinterpret_cast<const u16 *>(entry + 19 + (i * 2) + (modifier * 24));
                        slots.emplace_back(specie, level, info[specie]);
                    }
                    modifyRadio(slots, entry, info, profile.getRadio());
                    modifySwarmHGSS(slots, entry, info, encounter, profile.getSwarm());
                    encounters.emplace_back(location, grass, Encounter::Grass, slots);
                }

                if (surf != 0 && encounter == Encounter::Surfing)
                {
                    std::vector<Slot> slots;
                    for (int i = 0; i < 5; i++)
                    {
                        u8 min = entry[99 + (i * 4)];
                        u8 max = entry[100 + (i * 4)];
                        u16 specie = *reinterpret_cast<const u16 *>(entry + 101 + (i * 4));
                        slots.emplace_back(specie, min, max, info[specie]);
                    }
                    modifySwarmHGSS(slots, entry, info, encounter, profile.getSwarm());
                    encounters.emplace_back(location, surf, Encounter::Surfing, slots);
                }

                if (rock != 0 && encounter == Encounter::RockSmash)
                {
                    std::vector<Slot> slots;
                    for (int i = 0; i < 2; i++)
                    {
                        u8 min = entry[119 + (i * 4)];
                        u8 max = entry[120 + (i * 4)];
                        u16 specie = *reinterpret_cast<const u16 *>(entry + 121 + (i * 4));
                        slots.emplace_back(specie, min, max, info[specie]);
                    }
                    encounters.emplace_back(location, rock, Encounter::RockSmash, slots);
                }

                if (old != 0 && encounter == Encounter::OldRod)
                {
                    std::vector<Slot> slots;
                    for (int i = 0; i < 5; i++)
                    {
                        u8 min = entry[127 + (i * 4)];
                        u8 max = entry[128 + (i * 4)];
                        u16 specie = *reinterpret_cast<const u16 *>(entry + 129 + (i * 4));
                        slots.emplace_back(specie, min, max, info[specie]);
                    }
                    modifySwarmHGSS(slots, entry, info, encounter, profile.getSwarm());
                    encounters.emplace_back(location, old, Encounter::OldRod, slots);
                }

                if (good != 0 && encounter == Encounter::GoodRod)
                {
                    std::vector<Slot> slots;
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

                        slots.emplace_back(specie, min, max, info[specie]);
                    }
                    modifySwarmHGSS(slots, entry, info, encounter, profile.getSwarm());
                    encounters.emplace_back(location, good, Encounter::GoodRod, slots);
                }

                if (super != 0 && encounter == Encounter::SuperRod)
                {
                    std::vector<Slot> slots;
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

                        slots.emplace_back(specie, min, max, info[specie]);
                    }
                    modifySwarmHGSS(slots, entry, info, encounter, profile.getSwarm());
                    encounters.emplace_back(location, super, Encounter::SuperRod, slots);
                }
            }

            if (encounter == Encounter::Headbutt)
            {
                size_t size = hg_headbutt.size();

                for (size_t offset = 0; offset < size; )
                {
                    const u8 *entry = (version == Game::HeartGold ? hg_headbutt.data() : ss_headbutt.data()) + offset;

                    u8 location = entry[0];
                    u8 specialTreesFlag = entry[1];
                    u8 treesType = specialTreesFlag == 0 && modifier == 2 ? 0 : modifier;

                    std::vector<Slot> slots;
                    for (int i = 0; i < 6; i++)
                    {
                        u16 specie = *reinterpret_cast<const u16 *>(entry + (24 * treesType) + 2 + (i * 4));
                        u8 min = entry[(24 * treesType) + 4 + (i * 4)];
                        u8 max = entry[(24 * treesType) + 5 + (i * 4)];
                        slots.emplace_back(specie, min, max, info[specie]);
                    }
                    encounters.emplace_back(location, 0, Encounter::Headbutt, slots);

                    offset += specialTreesFlag == 0 ? 50 : 74;
                }
            }

            if (encounter == Encounter::BugCatchingContest)
            {
                size_t size = profile.getNationalDex() ? hgss_bug.size() : 41;

                for (size_t offset = profile.getNationalDex() ? 41 : 0; offset < size; offset += 41)
                {
                    const u8 *entry = hgss_bug.data() + offset;
                    u8 location = entry[0];

                    std::vector<Slot> slots;
                    for (int i = 0; i < 10; i++)
                    {
                        u16 specie = *reinterpret_cast<const u16 *>(entry + 1 + (i * 4));
                        u8 min = entry[3 + (i * 4)];
                        u8 max = entry[4 + (i * 4)];
                        slots.emplace_back(specie, min, max, info[specie]);
                    }
                    encounters.emplace_back(location, 0, Encounter::BugCatchingContest, slots);
                }
            }

            if (encounter == Encounter::SafariZone)
            {
                size_t size = hgss_safari.size();

                for (size_t offset = 0; offset < size; )
                {
                    const u8 *entry = hgss_safari.data() + offset;
                    int grassBlockSlot = 0;
                    int waterBlockSlot = 0;

                    u8 location = entry[0];
                    u8 waterFlag = entry[1];

                    std::vector<Slot> slots;
                    for (int i = 0; i < 10; i++)
                    {
                        u16 specie = *reinterpret_cast<const u16 *>(entry + (30 * modifier) + 2 + (i * 3));
                        u8 level = entry[(30 * modifier) + 4 + (i * 3)];

                        while (grassBlockSlot < 10)
                        {
                            if (blocks[entry[182 + (grassBlockSlot * 4)]] >= entry[183 + (grassBlockSlot * 4)] &&
                                    blocks[entry[184 + (grassBlockSlot * 4)]] >= entry[185 + (grassBlockSlot * 4)])
                            {
                                specie = *reinterpret_cast<const u16 *>(entry + (30 * modifier) + (grassBlockSlot * 3) + 92);
                                level = entry[(30 * modifier) + (grassBlockSlot * 3) + 94];
                                grassBlockSlot++;
                                break;
                            }

                            grassBlockSlot++;
                        }

                        // Adjust slot according to Safari encounter type
                        if (waterFlag != 0 && safariEncounter == Encounter::Surfing)
                        {
                            specie = *reinterpret_cast<const u16 *>(entry + (30 * modifier) + (i * 3) + 222);
                            level = entry[(30 * modifier) + (i * 3) + 224];

                            while (waterBlockSlot < 3)
                            {
                                if (blocks[entry[339 + (waterBlockSlot * 4)]] >= entry[340 + (waterBlockSlot * 4)] &&
                                        blocks[entry[341 + (waterBlockSlot * 4)]] >= entry[342 + (waterBlockSlot * 4)])
                                {
                                    specie = *reinterpret_cast<const u16 *>(entry + (9 * modifier) + (waterBlockSlot * 3) + 312);
                                    level = entry[(9 * modifier) + (waterBlockSlot * 3) + 314];
                                    waterBlockSlot++;
                                    break;
                                }

                                waterBlockSlot++;
                            }
                        }
                        else if (waterFlag != 0 && safariEncounter == Encounter::OldRod)
                        {
                            specie = *reinterpret_cast<const u16 *>(entry + (30 * modifier) + (i * 3) + 351);
                            level = entry[(30 * modifier) + (i * 3) + 353];

                            while (waterBlockSlot < 2)
                            {
                                if (blocks[entry[459 + (waterBlockSlot * 4)]] >= entry[460 + (waterBlockSlot * 4)] &&
                                        blocks[entry[461 + (waterBlockSlot * 4)]] >= entry[462 + (waterBlockSlot * 4)])
                                {
                                    specie = *reinterpret_cast<const u16 *>(entry + (6 * modifier) + (waterBlockSlot * 3) + 441);
                                    level = entry[(6 * modifier) + (waterBlockSlot * 3) + 443];
                                    waterBlockSlot++;
                                    break;
                                }

                                waterBlockSlot++;
                            }
                        }
                        else if (waterFlag != 0 && safariEncounter == Encounter::GoodRod)
                        {
                            specie = *reinterpret_cast<const u16 *>(entry + (30 * modifier) + (i * 3) + 467);
                            level = entry[(30 * modifier) + (i * 3) + 469];

                            while (waterBlockSlot < 2)
                            {
                                if (blocks[entry[575 + (waterBlockSlot * 4)]] >= entry[576 + (waterBlockSlot * 4)] &&
                                        blocks[entry[577 + (waterBlockSlot * 4)]] >= entry[578 + (waterBlockSlot * 4)])
                                {
                                    specie = *reinterpret_cast<const u16 *>(entry + (6 * modifier) + (waterBlockSlot * 3) + 557);
                                    level = entry[(6 * modifier) + (waterBlockSlot * 3) + 559];
                                    waterBlockSlot++;
                                    break;
                                }

                                waterBlockSlot++;
                            }
                        }
                        else if (waterFlag != 0 && safariEncounter == Encounter::SuperRod)
                        {
                            specie = *reinterpret_cast<const u16 *>(entry + (30 * modifier) + (i * 3) + 583);
                            level = entry[(30 * modifier) + (i * 3) + 585];

                            while (waterBlockSlot < 2)
                            {
                                if (blocks[entry[691 + (waterBlockSlot * 4)]] >= entry[692 + (waterBlockSlot * 4)] &&
                                        blocks[entry[693 + (waterBlockSlot * 4)]] >= entry[694 + (waterBlockSlot * 4)])
                                {
                                    specie = *reinterpret_cast<const u16 *>(entry + (6 * modifier) + (waterBlockSlot * 3) + 673);
                                    level = entry[(6 * modifier) + (waterBlockSlot * 3) + 675];
                                    waterBlockSlot++;
                                    break;
                                }

                                waterBlockSlot++;
                            }
                        }

                        slots.emplace_back(specie, level, level, info[specie]);
                    }
                    encounters.emplace_back(location, 0, Encounter::SafariZone, slots);

                    offset += waterFlag == 0 ? 222 : 699;
                }
            }

            return encounters;
        }

        void modifySwarmDPPt(std::vector<Slot> &mons, const u8 *data, const PersonalInfo *info, bool swarm)
        {
            if (swarm)
            {
                u16 specie1 = *reinterpret_cast<const u16 *>(data + 38);
                u16 specie2 = *reinterpret_cast<const u16 *>(data + 40);
                mons[0].setSpecie(specie1, info[specie1]);
                mons[1].setSpecie(specie2, info[specie2]);
            }
        }

        void modifyTime(std::vector<Slot> &mons, const u8 *data, const PersonalInfo *info, int time)
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

            mons[2].setSpecie(specie1, info[specie1]);
            mons[3].setSpecie(specie2, info[specie2]);
        }

        void modifyRadar(std::vector<Slot> &mons, const u8 *data, const PersonalInfo *info, bool radar)
        {
            if (radar)
            {
                u16 species[4] = { *reinterpret_cast<const u16 *>(data + 50), *reinterpret_cast<const u16 *>(data + 52),
                                   *reinterpret_cast<const u16 *>(data + 54), *reinterpret_cast<const u16 *>(data + 56) };
                mons[4].setSpecie(species[0], info[species[0]]);
                mons[5].setSpecie(species[1], info[species[1]]);
                mons[10].setSpecie(species[2], info[species[2]]);
                mons[11].setSpecie(species[3], info[species[3]]);
            }
        }

        void modifyDual(std::vector<Slot> &mons, const u8 *data, const PersonalInfo *info, Game dual)
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

            mons[8].setSpecie(specie1, info[specie1]);
            mons[9].setSpecie(specie2, info[specie2]);
        }

        std::vector<EncounterArea4> getDPPt(Game version, Encounter encounter, const Profile4 &profile, const PersonalInfo *info, int time)
        {
            const u8 *data;
            size_t size;

            if (version == Game::Diamond)
            {
                data = diamond.data();
                size = diamond.size();
            }
            else if (version == Game::Pearl)
            {
                data = pearl.data();
                size = pearl.size();
            }
            else
            {
                data = platinum.data();
                size = platinum.size();
            }

            std::vector<EncounterArea4> encounters;
            for (size_t offset = 0; offset < size; offset += 163)
            {
                const u8 *entry = data + offset;

                u8 location = entry[0];

                u8 grass = entry[1];
                if (grass != 0 && encounter == Encounter::Grass)
                {
                    std::vector<Slot> slots;
                    for (int i = 0; i < 12; i++)
                    {
                        u8 level = entry[2 + (i * 3)];
                        u16 specie = *reinterpret_cast<const u16 *>(entry + 3 + (i * 3));
                        slots.emplace_back(specie, level, info[specie]);
                    }
                    modifySwarmDPPt(slots, entry, info, profile.getSwarm());
                    modifyTime(slots, entry, info, time);
                    modifyRadar(slots, entry, info, profile.getRadar());
                    modifyDual(slots, entry, info, profile.getDualSlot());
                    encounters.emplace_back(location, grass, Encounter::Grass, slots);
                }

                u8 surf = entry[79];
                if (surf != 0 && encounter == Encounter::Surfing)
                {
                    std::vector<Slot> slots;
                    for (int i = 0; i < 5; i++)
                    {
                        u8 max = entry[80 + (i * 4)];
                        u8 min = entry[81 + (i * 4)];
                        u16 specie = *reinterpret_cast<const u16 *>(entry + 82 + (i * 4));
                        slots.emplace_back(specie, min, max, info[specie]);
                    }
                    encounters.emplace_back(location, surf, Encounter::Surfing, slots);
                }

                u8 old = entry[100];
                if (old != 0 && encounter == Encounter::OldRod)
                {
                    std::vector<Slot> slots;
                    for (int i = 0; i < 5; i++)
                    {
                        u8 max = entry[101 + (i * 4)];
                        u8 min = entry[102 + (i * 4)];
                        u16 specie = *reinterpret_cast<const u16 *>(entry + 103 + (i * 4));
                        slots.emplace_back(specie, min, max, info[specie]);
                    }
                    encounters.emplace_back(location, old, Encounter::OldRod, slots);
                }

                u8 good = entry[121];
                if (good != 0 && encounter == Encounter::GoodRod)
                {
                    std::vector<Slot> slots;
                    for (int i = 0; i < 5; i++)
                    {
                        u8 max = entry[122 + (i * 4)];
                        u8 min = entry[123 + (i * 4)];
                        u16 specie = *reinterpret_cast<const u16 *>(entry + 124 + (i * 4));
                        slots.emplace_back(specie, min, max, info[specie]);
                    }
                    encounters.emplace_back(location, good, Encounter::GoodRod, slots);
                }

                u8 super = entry[142];
                if (super != 0 && encounter == Encounter::SuperRod)
                {
                    std::vector<Slot> slots;
                    for (int i = 0; i < 5; i++)
                    {
                        u8 max = entry[143 + (i * 4)];
                        u8 min = entry[144 + (i * 4)];
                        u16 specie = *reinterpret_cast<const u16 *>(entry + 145 + (i * 4));
                        slots.emplace_back(specie, min, max, info[specie]);
                    }
                    encounters.emplace_back(location, super, Encounter::SuperRod, slots);
                }
            }
            return encounters;
        }
    }

    std::vector<EncounterArea4> getEncounters(Encounter encounter, int modifier, const Profile4 &profile, Encounter safariEncounter,
                                              std::array<u8, 5> blocks)
    {
        Game version = profile.getVersion();
        auto *info = PersonalLoader::getPersonal(version);
        if ((version & Game::DPPt) != Game::None)
        {
            return getDPPt(version, encounter, profile, info, modifier);
        }
        else
        {
            return getHGSS(version, encounter, profile, info, modifier, safariEncounter, blocks);
        }
    }

    bool getHeadbuttSpecialFlag(Game game, int location)
    {
        u8 specialTreesFlag;
        size_t size = hg_headbutt.size();

        for (size_t offset = 0; offset < size; )
        {
            const u8 *entry = (game == Game::HeartGold ? hg_headbutt.data() : ss_headbutt.data()) + offset;
            specialTreesFlag = entry[1];

            if (location == entry[0])
            {
                break;
            }

            offset += specialTreesFlag == 0 ? 50 : 74;
        }

        return specialTreesFlag == 1;
    }

    bool getSafariWaterFlag(int location)
    {
        u8 waterFlag;
        size_t size = hgss_safari.size();

        for (size_t offset = 0; offset < size; )
        {
            const u8 *entry = hgss_safari.data() + offset;
            waterFlag = entry[1];

            if (location == entry[0])
            {
                break;
            }

            offset += waterFlag == 0 ? 222 : 699;
        }

        return waterFlag == 1;
    }
}
