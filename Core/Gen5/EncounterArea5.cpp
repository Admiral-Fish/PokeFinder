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

#include "EncounterArea5.hpp"
#include <Core/Util/Translator.hpp>
#include <algorithm>

constexpr std::array<u16, 17> GEMS = { 548, 549, 550, 551, 552, 553, 554, 555, 556, 557, 558, 559, 560, 561, 562, 563, 564 };
constexpr std::array<u16, 7> FEATHERS = { 565, 566, 567, 568, 569, 570, 571 };
constexpr std::array<u16, 4> SHARDS = { 72, 73, 74, 75 };
constexpr std::array<u16, 10> STONES = { 80, 81, 82, 83, 84, 85, 107, 108, 109, 110 };
constexpr u16 EVERSTONE = 229;

constexpr std::array<u16, 7> SHARD_LOCATIONS = { 48, 49, 50, 52, 81, 82, 83 };

static u16 getBridgeItem(BWRNG &rng)
{
    u16 prng = rng.nextUInt(1000);
    if (prng < 900)
    {
        return FEATHERS[rng.nextUInt((FEATHERS.size() - 1) * 100) / 100];
    }

    return FEATHERS.back();
}

static u16 getCaveItem(BWRNG &rng, bool bw, u16 location)
{
    u16 prng = rng.nextUInt(1000);
    if (!bw && std::ranges::find(SHARD_LOCATIONS, location) != SHARD_LOCATIONS.end())
    {
        return SHARDS[prng / (1000 / SHARDS.size())];
    }
    else
    {
        if (prng < 100)
        {
            return STONES[rng.nextUInt(STONES.size() * 100) / 100];
        }
        else if (prng < 950)
        {
            return GEMS[rng.nextUInt(GEMS.size() * 100) / 100];
        }
        return EVERSTONE;
    }
}

EncounterArea5::EncounterArea5(u8 location, u8 rate, bool season, PhenomenonType phenomenon, Encounter encounter,
                               const std::array<Slot, 13> &pokemon) :
    EncounterArea(location, rate, encounter, pokemon), season(season), phenomenon(phenomenon)
{
}

u16 EncounterArea5::getItem(BWRNG &rng, bool bw) const
{
    switch (phenomenon)
    {
    case PhenomenonType::Bridge:
        return getBridgeItem(rng);
        break;

    case PhenomenonType::Cave:
        return getCaveItem(rng, bw, location);
        break;

    default:
        return 0;
    }
}

std::vector<std::string> EncounterArea5::getItemNames(bool bw) const
{
    return Translator::getItems(getUniqueItems(bw));
}

u8 EncounterArea5::getPhenomenonRate() const
{
    switch (phenomenon)
    {
    case PhenomenonType::Bridge:
        return 150;
    default:
        return 100;
    }
}

u16 EncounterArea5::getPhenomenonRatio() const
{
    switch (phenomenon)
    {
    case PhenomenonType::Bridge:
        return 200;
    case PhenomenonType::Cave:
        return 400;
    default:
        return 1000;
    }
}

PhenomenonType EncounterArea5::getPhenomenonType() const
{
    return phenomenon;
}

std::vector<u16> EncounterArea5::getUniqueItems(bool bw) const
{
    std::vector<u16> nums;

    switch (phenomenon)
    {
    case PhenomenonType::Bridge:
        nums.insert(nums.end(), FEATHERS.begin(), FEATHERS.end());
        break;
    case PhenomenonType::Cave:
        if (!bw && std::ranges::find(SHARD_LOCATIONS, location) != SHARD_LOCATIONS.end())
        {
            nums.insert(nums.end(), SHARDS.begin(), SHARDS.end());
        }
        else
        {
            nums.insert(nums.end(), STONES.begin(), STONES.end());
            nums.insert(nums.end(), GEMS.begin(), GEMS.end());
            nums.emplace_back(EVERSTONE);
        }
        break;
    default:
        break;
    }

    return nums;
}
