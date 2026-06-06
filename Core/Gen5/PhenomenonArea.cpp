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

#include "PhenomenonArea.hpp"
#include <Core/Util/Translator.hpp>
#include <algorithm>

constexpr std::array<u16, 17> GEMS = { 548, 549, 550, 551, 552, 553, 554, 555, 556, 557, 558, 559, 560, 561, 562, 563, 564 };
constexpr std::array<u16, 7> FEATHERS = { 565, 566, 567, 568, 569, 570, 571 };
constexpr std::array<u16, 4> SHARDS = { 72, 73, 74, 75 };
constexpr std::array<u16, 10> STONES = { 80, 81, 82, 83, 84, 85, 107, 108, 109, 110 };
constexpr u16 EVERSTONE = 229;

constexpr std::array<u16, 15> SHARD_LOCATIONS = { 503, 504, 505, 531, 532, 533, 534, 535, 536, 537, 538, 539, 540, 541, 541 };

static u16 getBridgeItem(BWRNG &rng)
{
    u16 prng = rng.nextUInt(1000);
    if (prng < 900)
    {
        return FEATHERS[rng.nextUInt((FEATHERS.size() - 1) * 100) / 100];
    }

    return FEATHERS.back();
}

static u16 getCaveItem(BWRNG &rng, u16 location)
{
    u16 prng = rng.nextUInt(1000);
    if (std::binary_search(SHARD_LOCATIONS.begin(), SHARD_LOCATIONS.end(), location))
    {
        return SHARDS[prng / (1000 / SHARDS.size())];
    }

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

PhenomenonArea::PhenomenonArea(u8 location, PhenomenonType type) : location(location), type(type), itemCount(0)
{
    switch (type)
    {
    case PhenomenonType::Bridge:
        std::copy(FEATHERS.begin(), FEATHERS.end(), items.begin());
        itemCount = FEATHERS.size();
        break;
    case PhenomenonType::Cave:
        if (std::find(SHARD_LOCATIONS.begin(), SHARD_LOCATIONS.end(), location) != SHARD_LOCATIONS.end())
        {
            std::copy(SHARD_LOCATIONS.begin(), SHARD_LOCATIONS.end(), items.begin());
            itemCount = SHARD_LOCATIONS.size();
        }

        std::copy(STONES.begin(), STONES.end(), items.begin() + itemCount);
        itemCount += STONES.size();

        std::copy(GEMS.begin(), GEMS.end(), items.begin() + itemCount);
        itemCount += GEMS.size();

        items[itemCount++] = EVERSTONE;
        break;
    default:
        break;
    }

    std::sort(items.begin(), items.begin() + itemCount);
}

u16 PhenomenonArea::getItem(BWRNG &rng) const
{
    switch (type)
    {
    case PhenomenonType::Bridge:
        return getBridgeItem(rng);
        break;

    case PhenomenonType::Cave:
        return getCaveItem(rng, location);
        break;

    default:
        return 0;
    }
}

std::vector<std::string> PhenomenonArea::getItemNames() const
{
    return Translator::getItems(getUniqueItems());
}

u8 PhenomenonArea::getLocation() const
{
    return location;
}

u16 PhenomenonArea::getRate() const
{
    switch (type)
    {
    case PhenomenonType::Bridge:
        return 200;
    case PhenomenonType::Cave:
        return 400;
    default:
        return 1000;
    }
}

std::vector<u16> PhenomenonArea::getUniqueItems() const
{
    std::vector<u16> nums;
    for (u8 i = 0; i < itemCount; i++)
    {
        if (std::find(nums.begin(), nums.end(), items[i]) == nums.end())
        {
            nums.emplace_back(items[i]);
        }
    }

    return nums;
}
