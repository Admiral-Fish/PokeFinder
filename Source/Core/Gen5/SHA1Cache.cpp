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

#include "SHA1Cache.hpp"
#include <Core/Enum/Buttons.hpp>
#include <Core/Enum/Game.hpp>
#include <Core/Util/DateTime.hpp>
#include <fstream>

SHA1Cache::SHA1Cache(const std::string &file, bool readData) : data(nullptr)
{
    std::ifstream stream(file.data(), std::ios_base::in | std::ios_base::binary);
    if (stream.is_open())
    {
        stream.read(reinterpret_cast<char *>(&mac), sizeof(mac));
        stream.read(reinterpret_cast<char *>(&end), sizeof(end));
        stream.read(reinterpret_cast<char *>(&start), sizeof(start));
        stream.read(reinterpret_cast<char *>(&version), sizeof(version));
        stream.read(reinterpret_cast<char *>(&countEntralink), sizeof(countEntralink));
        stream.read(reinterpret_cast<char *>(&countNormal), sizeof(countNormal));
        stream.read(reinterpret_cast<char *>(&countRoamer), sizeof(countRoamer));
        stream.read(reinterpret_cast<char *>(&timer0max), sizeof(timer0max));
        stream.read(reinterpret_cast<char *>(&timer0min), sizeof(timer0min));
        stream.read(reinterpret_cast<char *>(&softReset), sizeof(softReset));
        stream.read(reinterpret_cast<char *>(&type), sizeof(type));
        stream.read(reinterpret_cast<char *>(&language), sizeof(language));
        stream.read(reinterpret_cast<char *>(&keypresses), sizeof(keypresses));
        stream.read(reinterpret_cast<char *>(&gxstat), sizeof(gxstat));
        stream.read(reinterpret_cast<char *>(&vcount), sizeof(vcount));
        stream.read(reinterpret_cast<char *>(&vframe), sizeof(vframe));

        if (readData)
        {
            data = new SHA1Seed[countEntralink + countNormal + countRoamer];
            stream.read(reinterpret_cast<char *>(data), (countEntralink + countNormal + countRoamer) * sizeof(SHA1Seed));
        }
    }
}

SHA1Cache::~SHA1Cache()
{
    if (data)
    {
        delete[] data;
    }
}

fph::MetaFphMap<u64, u64> SHA1Cache::getCache(u32 initialAdvance, u32 maxAdvance, const Date &start, const Date &end,
                                              const fph::MetaFphMap<u64, std::array<u8, 6>> &ivCache, CacheType type)
{
    fph::MetaFphMap<u64, u64> cache;

    u32 index = 0;
    u32 count;

    if (type == CacheType::Entralink)
    {
        count = countEntralink;
    }
    else if (type == CacheType::Normal)
    {
        index = countEntralink;
        count = countNormal;
    }
    else
    {
        index = countEntralink + countNormal;
        count = countRoamer;
    }

    for (u32 i = 0; i < count; i++)
    {
        const auto &entry = &data[index + i];
        if (entry->key.date >= (start.getJD() - Date().getJD()) && entry->key.date <= (end.getJD() - Date().getJD()))
        {
            for (u64 j = initialAdvance; j <= (initialAdvance + maxAdvance) && j < 6; j++)
            {
                if (ivCache.contains((j << 32) | (entry->seed >> 32)))
                {
                    cache.emplace(entry->key.key, entry->seed);
                }
            }
        }
    }

    cache.max_load_factor(cache.max_load_factor_upper_limit());
    cache.rehash(cache.size());

    return cache;
}

bool SHA1Cache::valid(const Profile5 &profile)
{
    return mac == profile.getMac() && version == profile.getVersion() && timer0max == profile.getTimer0Max()
        && timer0min == profile.getTimer0Min() && softReset == profile.getSoftReset() && type == profile.getDSType()
        && language == profile.getLanguage() && keypresses == profile.getKeypresses() && gxstat == profile.getGxStat()
        && vcount == profile.getVCount() && vframe == profile.getVFrame();
}