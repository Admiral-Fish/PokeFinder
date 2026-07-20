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

#include "PickupGenerator.hpp"
#include <Core/Enum/Encounter.hpp>
#include <Core/Enum/Game.hpp>
#include <Core/Enum/Shiny.hpp>
#include <Core/Gen5/States/PickupState.hpp>
#include <Core/RNG/LCRNG64.hpp>
#include <Core/RNG/MT.hpp>
#include <Core/RNG/RNGList.hpp>
#include <Core/Util/EncounterSlot.hpp>
#include <Core/Util/Utilities.hpp>
#include <algorithm>
#include <optional>

constexpr std::array<u8, 11> itemSlots = { 30, 10, 10, 10, 10, 10, 10, 4, 4, 1, 1 };

constexpr std::array<u16, 18> commonItems = {
    17, // Potion
    18, // Antidote
    26, // Super Potion
    3, // Great Ball
    79, // Repel
    78, // Escape Rope
    27, // Full Heal
    25, // Hyper Potion
    2, // Ultra Ball
    28, // Revive
    50, // Rare Candy
    80, // Sun Stone
    81, // Moon Stone
    93, // Heart Scale
    23, // Full Restore
    29, // Max Revive
    51, // PP Up
    41 // Max Elixir
};

constexpr std::array<u16, 12> rareItems = {
    50, // Rare Candy
    25, // Hyper Potion
    92, // Nugget
    221, // King's Rock
    23, // Full Restore
    38, // Ether
    278, // Iron Ball
    537, // Prism Scale
    40, // Elixir
    537, // Prism Scale
    234, // Leftovers
    537 // Prism Scale
};

static u8 gen(MT &rng)
{
    return rng.next() >> 27;
}

static u8 levelRange(u8 level)
{
    return level == 0 ? 0 : std::min<u8>((level - 1) / 10, 9);
}

static WildStateFilter createDefaultFilter()
{
    std::array<u8, 6> min = {};
    std::array<u8, 6> max = { 31, 31, 31, 31, 31, 31 };
    std::array<bool, 25> natures;
    natures.fill(true);
    std::array<bool, 16> powers;
    powers.fill(true);
    std::array<bool, 12> slots;
    slots.fill(true);

    return WildStateFilter(255, 255, 255, 1, 100, 0, 255, 0, 255, true, min, max, natures, powers, slots);
}

static u8 getPercentRand(BWRNG &rng, bool bw)
{
    if (bw)
    {
        return rng.nextUInt(0xffff) / 656;
    }
    else
    {
        return rng.nextUInt(100);
    }
}

static u16 getWildItem(BWRNG &rng, bool bw, Encounter encounter, const PersonalInfo *info)
{
    constexpr u8 itemTable[3] = { 50, 55, 0 };
    constexpr u8 itemTableRare[3] = { 50, 55, 56 };

    if (info->getItem(0) == info->getItem(1))
    {
        return info->getItem(0);
    }

    const u8 *table = encounter != Encounter::GrassDark ? itemTable : itemTableRare;
    u8 rand = getPercentRand(rng, bw);
    for (int i = 0; i < 3; i++)
    {
        if (rand < table[i])
        {
            return info->getItem(i);
        }
    }

    return 0;
}

static std::optional<WildState5> generateWild(BWRNG &go, u32 advances, u64 seed, u32 ivAdvances, bool bw, u16 tsv,
                                              const EncounterArea5 &area, const Profile5 &profile)
{
    getPercentRand(go, bw);

    bool doubleBattle = false;
    if (area.getEncounter() == Encounter::GrassDark && getPercentRand(go, bw) < 40)
    {
        doubleBattle = true;
    }

    u8 encounterSlot = EncounterSlot::bwSlot(getPercentRand(go, bw), area.getEncounter(), 0);
    u8 level = area.calculateLevel(encounterSlot, getPercentRand(go, bw), false);

    if (doubleBattle)
    {
        go.advance(2);
    }

    const auto &slot = area.getPokemon(encounterSlot);
    const PersonalInfo *info = slot.getInfo();

    u8 shinyRolls = 1;
    if ((profile.getVersion() & Game::BW2) != Game::None && profile.getShinyCharm())
    {
        shinyRolls += 2;
    }

    u32 pid = 0;
    for (u8 i = 0; i < shinyRolls; i++)
    {
        pid = Utilities5::createPID(tsv, 2, 255, Shiny::Random, true, info->getGender(), go);
        if (Utilities::isShiny<true>(pid, tsv))
        {
            break;
        }
    }

    u8 ability = (pid >> 16) & 1;
    u8 gender = Utilities::getGender(pid, info);
    u8 shiny = Utilities::getShiny<true>(pid, tsv);
    u8 nature = go.nextUInt(25);
    u16 item = getWildItem(go, bw, area.getEncounter(), info);

    RNGList<u8, MT, 8, gen> rngList(seed >> 32, ivAdvances + (bw ? 0 : 2));
    std::array<u8, 6> ivs;
    std::ranges::generate(ivs, [&rngList] { return rngList.next(); });

    return WildState5(0, advances, ivAdvances, pid, ivs, ability, gender, level, nature, shiny, encounterSlot, item, slot.getSpecie(),
                      slot.getForm(), info);
}

PickupGenerator::PickupGenerator(u32 initialAdvances, u32 maxAdvances, const std::array<Slot, 6> &pickupSlots,
                                 const std::vector<Request> &requests, const EncounterArea5 &area, const Profile5 &profile,
                                 bool includeInvalid) :
    PickupGenerator(initialAdvances, maxAdvances, pickupSlots, area, profile, createDefaultFilter(), includeInvalid)
{
    this->requests = requests;
}

PickupGenerator::PickupGenerator(u32 initialAdvances, u32 maxAdvances, const std::array<Slot, 6> &pickupSlots, const EncounterArea5 &area,
                                 const Profile5 &profile, const WildStateFilter &filter, bool includeInvalid) :
    initialAdvances(initialAdvances),
    maxAdvances(maxAdvances),
    pickupSlots(pickupSlots),
    active({}),
    area(area),
    profile(profile),
    filter(filter),
    includeInvalid(includeInvalid)
{
    for (size_t i = 0; i < pickupSlots.size(); i++)
    {
        active[i] = pickupSlots[i].active;
    }
}

std::vector<PickupState> PickupGenerator::generate(u64 seed, u32 ivAdvances) const
{
    u32 advances = Utilities5::initialAdvances(seed, profile);
    u16 tsv = profile.getTID() ^ profile.getSID();
    bool bw2 = (profile.getVersion() & Game::BW2) != Game::None;
    u32 consumed = 5 + (profile.getMemoryLink() ? 1 : 0) + (bw2 && profile.getShinyCharm() ? 2 : 0);
    BWRNG chatotRNG(seed, advances + initialAdvances);
    BWRNG itemRNG(seed, advances + initialAdvances + consumed);
    bool bw = (profile.getVersion() & Game::BW) != Game::None;

    std::vector<PickupState> states;
    for (u32 cnt = 0; cnt <= maxAdvances; cnt++)
    {
        BWRNG go(itemRNG);
        std::optional<WildState5> wild;
        if (area)
        {
            u32 displayAdvance = advances + initialAdvances + cnt;
            u32 pickupAdvance = displayAdvance + 2;
            u32 wildAdvance = displayAdvance + 1;
            if (bw2)
            {
                pickupAdvance--;
                if (!profile.getMemoryLink())
                {
                    pickupAdvance--;
                    wildAdvance--;
                }
            }
            else
            {
                pickupAdvance = displayAdvance;
                wildAdvance = displayAdvance;
            }

            BWRNG pickupRNG(seed, pickupAdvance);
            generateWild(pickupRNG, pickupAdvance, seed, ivAdvances, bw, tsv, *area, profile);
            go = pickupRNG;

            BWRNG wildRNG(seed, wildAdvance);
            wild = generateWild(wildRNG, displayAdvance, seed, ivAdvances, bw, tsv, *area, profile);
        }

        std::array<u16, 6> items = {};

        for (size_t i = 0; i < pickupSlots.size(); i++)
        {
            if (!pickupSlots[i].active)
            {
                continue;
            }

            if (go.nextUInt(100) < 10)
            {
                items[i] = getPickupItem(pickupSlots[i].level, go.nextUInt(100));
            }
        }

        itemRNG.nextUInt(0x1fff);
        u16 chatot = chatotRNG.nextUInt(0x1fff);
        bool valid = false;
        for (size_t i = 0; i < pickupSlots.size(); i++)
        {
            valid |= pickupSlots[i].active && items[i] != 0;
        }
        bool wildMatches = !wild || filter.compareState(static_cast<const WildState &>(*wild));
        bool pickupMatches = compare(items);
        if ((valid || includeInvalid) && pickupMatches && wildMatches)
        {
            states.emplace_back(advances + initialAdvances + cnt, chatot, valid, active, items, wild);
        }
    }

    return states;
}

std::vector<u16> PickupGenerator::getLevelItems(u8 level)
{
    if (level == 0)
    {
        return {};
    }

    u8 range = levelRange(level);
    std::vector<u16> items;
    items.reserve(11);

    for (int i = range; i < range + 9; i++)
    {
        items.emplace_back(commonItems[i]);
    }

    for (int i = 2; i > 0; i--)
    {
        items.emplace_back(rareItems[range + i]);
    }

    std::ranges::sort(items);
    items.erase(std::ranges::unique(items).begin(), items.end());
    return items;
}

bool PickupGenerator::compare(const std::array<u16, 6> &items) const
{
    if (!requests.empty())
    {
        for (const auto &request : requests)
        {
            u8 count = 0;
            for (size_t i = 0; i < pickupSlots.size(); i++)
            {
                if (pickupSlots[i].active && items[i] == request.item)
                {
                    count++;
                }
            }

            if (count < request.count)
            {
                return false;
            }
        }

        return true;
    }

    for (size_t i = 0; i < pickupSlots.size(); i++)
    {
        if (!pickupSlots[i].active || pickupSlots[i].filter.empty())
        {
            continue;
        }

        if (std::ranges::find(pickupSlots[i].filter, items[i]) == pickupSlots[i].filter.end())
        {
            return false;
        }
    }

    return true;
}

u16 PickupGenerator::getPickupItem(u8 level, u8 itemRand) const
{
    u8 range = levelRange(level);
    for (size_t i = 0; i < itemSlots.size(); i++)
    {
        if (itemRand < itemSlots[i])
        {
            if (i < 9)
            {
                return commonItems[range + i];
            }

            return rareItems[range + (11 - i)];
        }

        itemRand -= itemSlots[i];
    }

    return 0;
}
