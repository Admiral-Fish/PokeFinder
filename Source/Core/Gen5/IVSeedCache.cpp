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

#include "IVSeedCache.hpp"
#include <Core/Enum/Game.hpp>
#include <Core/Parents/Filters/StateFilter.hpp>
#include <Core/RNG/MTFast.hpp>
#include <Core/Resources/IVs.hpp>
#include <Core/Util/Utilities.hpp>
#include <numeric>

struct SeedCache
{
    std::array<u16, 10> entralinkCount;
    std::array<u16, 8> normalCount;
    std::array<u16, 6> roamerCount;
    u32 seeds[];
};
static_assert(sizeof(SeedCache) == 48);

static bool compareIVs(const std::array<u8, 6> &ivs, const StateFilter &filter)
{
    constexpr u8 order[6] = { 0, 1, 2, 5, 3, 4 };

    u8 hiddenPower = 0;
    for (int i = 0; i < 6; i++)
    {
        hiddenPower |= (ivs[order[i]] & 1) << i;
    }
    hiddenPower = hiddenPower * 15 / 63;

    return filter.compareIV(ivs) && filter.compareHiddenPower(hiddenPower);
}

static std::array<u8, 6> computeIVs(u32 seed, u8 advance, CacheType type)
{
    std::array<u8, 6> ivs;

    if (type == CacheType::Entralink)
    {
        advance += 22;
    }
    else if (type == CacheType::Roamer)
    {
        advance += 1;
    }

    MTFast<37, true> mt(seed, advance);

    ivs[0] = mt.next();
    ivs[1] = mt.next();
    ivs[2] = mt.next();

    if (type == CacheType::Roamer)
    {
        ivs[4] = mt.next();
        ivs[5] = mt.next();
        ivs[3] = mt.next();
    }
    else
    {
        ivs[3] = mt.next();
        ivs[4] = mt.next();
        ivs[5] = mt.next();
    }

    return ivs;
}

/**
 * @brief Returns the IV caches for entralink
 *
 * @param initialAdvance Initial IV advances
 * @param maxAdvance Maximum IV advances
 * @param filter IV filter
 *
 * @return IV caches
 */
static fph::MetaFphMap<u64, std::array<u8, 6>> getEntralinkCache(u32 initialAdvance, u32 maxAdvance, const StateFilter &filter)
{
    fph::MetaFphMap<u64, std::array<u8, 6>> cache;

    u32 size;
    auto *data = Utilities::decompress<SeedCache>(IVS.data(), IVS.size(), size);

    u32 index = 0;

    for (u64 i = initialAdvance; i <= (initialAdvance + maxAdvance) && i < 6; i++)
    {
        u16 count = data->entralinkCount[i];

        for (u16 j = 0; j < count; j++)
        {
            u32 seed = data->seeds[index + j];
            auto ivs = computeIVs(seed, i, CacheType::Entralink);
            if (compareIVs(ivs, filter))
            {
                cache.emplace((i << 32) | seed, ivs);
            }
        }

        index += count;
    }

    cache.max_load_factor(cache.max_load_factor_upper_limit());
    cache.rehash(cache.size());

    delete[] data;
    return cache;
}

/**
 * @brief Returns the IV caches for most encounter types
 *
 * @param initialAdvance Initial IV advances
 * @param maxAdvance Maximum IV advances
 * @param version Game version
 * @param filter IV filter
 *
 * @return IV caches
 */
static fph::MetaFphMap<u64, std::array<u8, 6>> getNormalCache(u32 initialAdvance, u32 maxAdvance, Game version, const StateFilter &filter)
{
    fph::MetaFphMap<u64, std::array<u8, 6>> cache;

    bool bw = (version & Game::BW) != Game::None;

    u32 size;
    auto *data = Utilities::decompress<SeedCache>(IVS.data(), IVS.size(), size);

    u32 index = std::accumulate(data->entralinkCount.begin(), data->entralinkCount.end(), 0);
    if (!bw)
    {
        index += data->normalCount[0] + data->normalCount[1];
    }

    for (u64 i = initialAdvance; i <= (initialAdvance + maxAdvance) && i < 6; i++)
    {
        u16 count = data->normalCount[i + (bw ? 0 : 2)];

        for (u16 j = 0; j < count; j++)
        {
            u32 seed = data->seeds[index + j];
            auto ivs = computeIVs(seed, i + (bw ? 0 : 2), CacheType::Normal);
            if (compareIVs(ivs, filter))
            {
                cache.emplace((i << 32) | seed, ivs);
            }
        }

        index += count;
    }

    cache.max_load_factor(cache.max_load_factor_upper_limit());
    cache.rehash(cache.size());

    delete[] data;
    return cache;
}

/**
 * @brief Returns the IV caches for roamers
 *
 * @param initialAdvance Initial IV advances
 * @param maxAdvance Maximum IV advances
 * @param filter IV filter
 *
 * @return IV caches
 */
static fph::MetaFphMap<u64, std::array<u8, 6>> getRoamerCache(u32 initialAdvance, u32 maxAdvance, const StateFilter &filter)
{
    fph::MetaFphMap<u64, std::array<u8, 6>> cache;

    u32 size;
    auto *data = Utilities::decompress<SeedCache>(IVS.data(), IVS.size(), size);

    u32 index = std::accumulate(data->normalCount.begin(), data->normalCount.end(),
                                std::accumulate(data->entralinkCount.begin(), data->entralinkCount.end(), 0));

    for (u64 i = initialAdvance; i <= (initialAdvance + maxAdvance) && i < 6; i++)
    {
        u16 count = data->roamerCount[i];

        for (u16 j = 0; j < count; j++)
        {
            u32 seed = data->seeds[index + j];
            auto ivs = computeIVs(seed, i, CacheType::Roamer);
            if (compareIVs(ivs, filter))
            {
                cache.emplace((i << 32) | seed, ivs);
            }
        }

        index += count;
    }

    cache.max_load_factor(cache.max_load_factor_upper_limit());
    cache.rehash(cache.size());

    delete[] data;
    return cache;
}

namespace IVSeedCache
{
    fph::MetaFphMap<u64, std::array<u8, 6>> getCache(u32 initialAdvance, u32 maxAdvance, Game version, CacheType type,
                                                     const StateFilter &filter)
    {
        if (type == CacheType::Entralink)
        {
            return getEntralinkCache(initialAdvance, maxAdvance, filter);
        }
        else if (type == CacheType::Normal)
        {
            return getNormalCache(initialAdvance, maxAdvance, version, filter);
        }
        else
        {
            return getRoamerCache(initialAdvance, maxAdvance, filter);
        }
    }

    std::vector<u32> getSeeds(Game version, CacheType type)
    {
        u32 size;
        auto *data = Utilities::decompress<SeedCache>(IVS.data(), IVS.size(), size);

        u32 start = 0;
        u32 count;

        if (type == CacheType::Entralink)
        {
            count = std::accumulate(data->entralinkCount.begin(), data->entralinkCount.end(), 0);
        }
        else if (type == CacheType::Normal)
        {
            start = std::accumulate(data->entralinkCount.begin(), data->entralinkCount.end(), 0);
            if ((version & Game::BW) != Game::None)
            {
                count = std::accumulate(data->normalCount.begin(), data->normalCount.begin() + 6, 0);
            }
            else
            {
                start += std::accumulate(data->normalCount.begin(), data->normalCount.begin() + 2, 0);
                count = std::accumulate(data->normalCount.begin() + 2, data->normalCount.begin() + 8, 0);
            }
        }
        else
        {
            start = std::accumulate(data->entralinkCount.begin(), data->entralinkCount.end(),
                                    std::accumulate(data->normalCount.begin(), data->normalCount.end(), 0));
            count = std::accumulate(data->roamerCount.begin(), data->roamerCount.end(), 0);
        }

        std::vector<u32> seeds(data->seeds + start, data->seeds + start + count);
        std::sort(seeds.begin(), seeds.end());
        seeds.erase(std::unique(seeds.begin(), seeds.end()), seeds.end());

        delete[] data;
        return seeds;
    }
}
