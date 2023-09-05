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
#include <Core/Resources/IVs.hpp>
#include <Core/Util/Utilities.hpp>
#include <numeric>

struct IV
{
    u16 hp : 1;
    u16 atk : 5;
    u16 def : 1;
    u16 spa : 5;
    u16 spd : 1;
    u16 spe : 2;
};
static_assert(sizeof(IV) == 2);

#pragma pack(push, 1)
struct IVEntry
{
    u32 seed;
    IV iv;
};
#pragma pack(pop)
static_assert(sizeof(IVEntry) == 6);

struct IVCache
{
    std::array<u16, 10> entralinkCount;
    std::array<u16, 8> normalCount;
    std::array<u16, 6> roamerCount;
    IVEntry data[];
};
static_assert(sizeof(IVCache) == 48);

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

static std::array<u8, 6> computeIVs(IV iv)
{
    // HP maps 0-1 to 30-31
    // Atk maps 0-31 to 0-31
    // Def maps 0-1 to 30-31
    // SpA maps 0-31 to 0-31
    // SpD maps 0-1 to 0-31
    // Spe maps 0-1 to 30-31 and 2-3 to 0-1
    return { static_cast<u8>(30 + iv.hp), static_cast<u8>(iv.atk),      static_cast<u8>(30 + iv.def),
             static_cast<u8>(iv.spa),     static_cast<u8>(30 + iv.spd), static_cast<u8>((30 + iv.spe) & 0x1f) };
}

namespace IVSeedCache
{
    std::array<fph::DynamicFphMap<u32, std::array<u8, 6>>, 6> getNormalCache(u32 initialAdvance, u32 maxAdvance, Game version,
                                                                             const StateFilter &filter)
    {
        std::array<fph::DynamicFphMap<u32, std::array<u8, 6>>, 6> cache;

        bool bw = (version & Game::BW) != Game::None;

        u32 size;
        u8 *data = Utilities::decompress(IVS.data(), IVS.size(), size);
        const auto *ivCache = reinterpret_cast<const IVCache *>(data);

        u32 index = std::accumulate(ivCache->entralinkCount.begin(), ivCache->entralinkCount.end(), 0);

        for (u32 i = initialAdvance; i <= (initialAdvance + maxAdvance) && i < cache.size(); i++)
        {
            u16 count = ivCache->normalCount[i + (bw ? 0 : 2)];

            cache[i].reserve(count);

            const IVEntry *entry = &ivCache->data[index];
            for (u16 j = 0; j < count; j++, entry++)
            {
                auto ivs = computeIVs(entry->iv);
                if (compareIVs(ivs, filter))
                {
                    cache[i].emplace(entry->seed, ivs);
                }
            }
            cache[i].max_load_factor(cache[i].max_load_factor_upper_limit());
            cache[i].rehash(cache[i].size());

            index += count;
        }

        delete[] data;
        return cache;
    }

    std::array<fph::DynamicFphMap<u32, std::array<u8, 6>>, 6> getRoamerCache(u32 initialAdvance, u32 maxAdvance, const StateFilter &filter)
    {
        std::array<fph::DynamicFphMap<u32, std::array<u8, 6>>, 6> cache;

        u32 size;
        u8 *data = Utilities::decompress(IVS.data(), IVS.size(), size);
        const auto *ivCache = reinterpret_cast<const IVCache *>(data);

        u32 index = std::accumulate(ivCache->normalCount.begin(), ivCache->normalCount.end(),
                                    std::accumulate(ivCache->entralinkCount.begin(), ivCache->entralinkCount.end(), 0));

        for (u32 i = initialAdvance; i <= (initialAdvance + maxAdvance) && i < cache.size(); i++)
        {
            u16 count = ivCache->roamerCount[i];

            cache[i].reserve(count);

            const IVEntry *entry = &ivCache->data[index];
            for (u16 j = 0; j < count; j++, entry++)
            {
                auto ivs = computeIVs(entry->iv);
                if (compareIVs(ivs, filter))
                {
                    cache[i].emplace(entry->seed, ivs);
                }
            }
            cache[i].max_load_factor(cache[i].max_load_factor_upper_limit());
            cache[i].rehash(cache[i].size());

            index += count;
        }

        delete[] data;
        return cache;
    }

    std::array<fph::DynamicFphMap<u32, std::array<u8, 6>>, 6> getEntralinkCache(u32 initialAdvance, u32 maxAdvance,
                                                                                const StateFilter &filter)
    {
        std::array<fph::DynamicFphMap<u32, std::array<u8, 6>>, 6> cache;

        u32 size;
        u8 *data = Utilities::decompress(IVS.data(), IVS.size(), size);
        const auto *ivCache = reinterpret_cast<const IVCache *>(data);

        u32 index = 0;

        for (u32 i = initialAdvance; i <= (initialAdvance + maxAdvance) && i < cache.size(); i++)
        {
            u16 count = ivCache->entralinkCount[i];

            cache[i].reserve(count);

            const IVEntry *entry = &ivCache->data[index];
            for (u16 j = 0; j < count; j++, entry++)
            {
                auto ivs = computeIVs(entry->iv);
                if (compareIVs(ivs, filter))
                {
                    cache[i].emplace(entry->seed, ivs);
                }
            }
            cache[i].max_load_factor(cache[i].max_load_factor_upper_limit());
            cache[i].rehash(cache[i].size());

            index += count;
        }

        delete[] data;
        return cache;
    }
}
