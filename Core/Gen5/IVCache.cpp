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

#include "IVCache.hpp"
#include <Core/Enum/Game.hpp>
#include <Core/Parents/Filters/StateFilter.hpp>
#include <Core/RNG/MT.hpp>
#include <Core/Util/Utilities.hpp>
#include <fstream>
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

    MT mt(seed, advance);

    ivs[0] = mt.next() >> 27;
    ivs[1] = mt.next() >> 27;
    ivs[2] = mt.next() >> 27;

    if (type == CacheType::Roamer)
    {
        ivs[4] = mt.next() >> 27;
        ivs[5] = mt.next() >> 27;
        ivs[3] = mt.next() >> 27;
    }
    else
    {
        ivs[3] = mt.next() >> 27;
        ivs[4] = mt.next() >> 27;
        ivs[5] = mt.next() >> 27;
    }

    return ivs;
}

IVCache::IVCache(const std::string &file, bool read) : valid(false)
{
    std::ifstream stream(file.data(), std::ios_base::in | std::ios_base::binary);
    if (stream.is_open())
    {
        u32 magic;

        // Expected magic word is CRC32 of "IVCache"
        stream.read(reinterpret_cast<char *>(&magic), sizeof(magic));
        if (magic != 0xd08cb7c0)
        {
            return;
        }

        stream.read(reinterpret_cast<char *>(&initialAdvances), sizeof(initialAdvances));
        stream.read(reinterpret_cast<char *>(&maxAdvances), sizeof(maxAdvances));

        if (read)
        {
            std::vector<u32> entralinkCount(maxAdvances + 5);
            std::vector<u32> normalCount(maxAdvances + 3);
            std::vector<u32> roamerCount(maxAdvances + 1);

            stream.read(reinterpret_cast<char *>(entralinkCount.data()), entralinkCount.size() * sizeof(u32));
            stream.read(reinterpret_cast<char *>(normalCount.data()), normalCount.size() * sizeof(u32));
            stream.read(reinterpret_cast<char *>(roamerCount.data()), roamerCount.size() * sizeof(u32));

            entralinkSeeds.resize(entralinkCount.size());
            for (int i = 0; i < entralinkCount.size(); i++)
            {
                entralinkSeeds[i].resize(entralinkCount[i]);
                stream.read(reinterpret_cast<char *>(entralinkSeeds[i].data()), entralinkCount[i] * sizeof(u32));
            }

            normalSeeds.resize(normalCount.size());
            for (int i = 0; i < normalCount.size(); i++)
            {
                normalSeeds[i].resize(normalCount[i]);
                stream.read(reinterpret_cast<char *>(normalSeeds[i].data()), normalCount[i] * sizeof(u32));
            }

            roamerSeeds.resize(roamerCount.size());
            for (int i = 0; i < roamerCount.size(); i++)
            {
                roamerSeeds[i].resize(roamerCount[i]);
                stream.read(reinterpret_cast<char *>(roamerSeeds[i].data()), roamerCount[i] * sizeof(u32));
            }
        }

        valid = true;
    }
}

fph::MetaFphMap<u64, std::array<u8, 6>> IVCache::getCache(u32 initialAdvances, u32 maxAdvances, Game version, CacheType type,
                                                          const StateFilter &filter) const
{
    if (type == CacheType::Entralink)
    {
        return getEntralinkCache(initialAdvances, maxAdvances, filter);
    }
    else if (type == CacheType::Normal)
    {
        return getNormalCache(initialAdvances, maxAdvances, version, filter);
    }
    else
    {
        return getRoamerCache(initialAdvances, maxAdvances, filter);
    }
}

std::vector<u32> IVCache::getSeeds(Game version, CacheType type) const
{
    std::vector<u32> seeds;
    if (type == CacheType::Entralink)
    {
        for (int i = 0; i < entralinkSeeds.size(); i++)
        {
            seeds.insert(seeds.end(), entralinkSeeds[i].begin(), entralinkSeeds[i].end());
        }
    }
    else if (type == CacheType::Normal)
    {
        int start = (version & Game::BW) != Game::None ? 0 : 2;
        int end = (version & Game::BW) != Game::None ? normalSeeds.size() - 2 : normalSeeds.size();
        for (int i = start; i < end; i++)
        {
            seeds.insert(seeds.end(), normalSeeds[i].begin(), normalSeeds[i].end());
        }
    }
    else
    {
        for (int i = 0; i < roamerSeeds.size(); i++)
        {
            seeds.insert(seeds.end(), roamerSeeds[i].begin(), roamerSeeds[i].end());
        }
    }

    std::sort(seeds.begin(), seeds.end());
    seeds.erase(std::unique(seeds.begin(), seeds.end()), seeds.end());

    return seeds;
}

bool IVCache::isValid() const
{
    return valid;
}

fph::MetaFphMap<u64, std::array<u8, 6>> IVCache::getEntralinkCache(u32 initialAdvances, u32 maxAdvances, const StateFilter &filter) const
{
    fph::MetaFphMap<u64, std::array<u8, 6>> cache;

    for (u64 i = initialAdvances; i <= (initialAdvances + maxAdvances); i++)
    {
        for (u32 seed : entralinkSeeds[i])
        {
            auto ivs = computeIVs(seed, i, CacheType::Entralink);
            if (compareIVs(ivs, filter))
            {
                cache.emplace((i << 32) | seed, ivs);
            }
        }
    }

    cache.max_load_factor(cache.max_load_factor_upper_limit());
    cache.rehash(cache.size());

    return cache;
}

fph::MetaFphMap<u64, std::array<u8, 6>> IVCache::getNormalCache(u32 initialAdvances, u32 maxAdvances, Game version,
                                                                const StateFilter &filter) const
{
    fph::MetaFphMap<u64, std::array<u8, 6>> cache;

    bool bw = (version & Game::BW) != Game::None;
    for (u64 i = initialAdvances; i <= (initialAdvances + maxAdvances); i++)
    {
        for (u32 seed : normalSeeds[i + (bw ? 0 : 2)])
        {
            auto ivs = computeIVs(seed, i + (bw ? 0 : 2), CacheType::Normal);
            if (compareIVs(ivs, filter))
            {
                cache.emplace((i << 32) | seed, ivs);
            }
        }
    }

    cache.max_load_factor(cache.max_load_factor_upper_limit());
    cache.rehash(cache.size());

    return cache;
}

fph::MetaFphMap<u64, std::array<u8, 6>> IVCache::getRoamerCache(u32 initialAdvances, u32 maxAdvances, const StateFilter &filter) const
{
    fph::MetaFphMap<u64, std::array<u8, 6>> cache;

    for (u64 i = initialAdvances; i <= (initialAdvances + maxAdvances) && i < 6; i++)
    {
        for (u32 seed : roamerSeeds[i])
        {
            auto ivs = computeIVs(seed, i, CacheType::Roamer);
            if (compareIVs(ivs, filter))
            {
                cache.emplace((i << 32) | seed, ivs);
            }
        }
    }

    cache.max_load_factor(cache.max_load_factor_upper_limit());
    cache.rehash(cache.size());

    return cache;
}
