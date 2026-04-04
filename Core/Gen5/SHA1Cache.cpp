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
#include <Core/Gen5/Keypresses.hpp>
#include <Core/Util/DateTime.hpp>

constexpr u32 DATA_OFFSET = 42;

SHA1Cache::SHA1Cache(const std::string &path) : file(path.data(), std::ios_base::in | std::ios_base::binary), valid(false)
{
    if (file.is_open())
    {
        u32 magic;

        // Expected magic word is CRC32 of "SHA1Cache"
        file.read(reinterpret_cast<char *>(&magic), sizeof(magic));
        if (magic != 0x3c50a97e)
        {
            return;
        }

        file.read(reinterpret_cast<char *>(&initialAdvances), sizeof(initialAdvances));
        file.read(reinterpret_cast<char *>(&maxAdvances), sizeof(maxAdvances));
        file.read(reinterpret_cast<char *>(&mac), sizeof(mac));
        file.read(reinterpret_cast<char *>(&end), sizeof(end));
        file.read(reinterpret_cast<char *>(&start), sizeof(start));
        file.read(reinterpret_cast<char *>(&version), sizeof(version));
        file.read(reinterpret_cast<char *>(&timer0max), sizeof(timer0max));
        file.read(reinterpret_cast<char *>(&timer0min), sizeof(timer0min));
        file.seekg(1, std::ios_base::cur);
        file.read(reinterpret_cast<char *>(&type), sizeof(type));
        file.read(reinterpret_cast<char *>(&language), sizeof(language));
        file.read(reinterpret_cast<char *>(&gxstat), sizeof(gxstat));
        file.read(reinterpret_cast<char *>(&vcount), sizeof(vcount));
        file.read(reinterpret_cast<char *>(&vframe), sizeof(vframe));

        valid = true;
    }
}

SHA1Cache::~SHA1Cache()
{
    file.close();
}

fph::MetaFphMap<u64, u64> SHA1Cache::getCache(u32 initialAdvance, u32 maxAdvance, const Date &start, const Date &end,
                                              const fph::MetaFphMap<u64, std::array<u8, 6>> &ivCache, CacheType type,
                                              const Profile5 &profile)
{
    fph::MetaFphMap<u64, u64> cache;
    auto keypresses = Keypresses::getKeypresses(profile);

    u32 countEntralink;
    u32 countNormal;
    u32 countRoamer;

    file.seekg(DATA_OFFSET);
    file.read(reinterpret_cast<char *>(&countEntralink), sizeof(countEntralink));
    file.read(reinterpret_cast<char *>(&countNormal), sizeof(countNormal));
    file.read(reinterpret_cast<char *>(&countRoamer), sizeof(countRoamer));

    u32 count;
    if (type == CacheType::Entralink)
    {
        count = countEntralink;
    }
    else if (type == CacheType::Normal)
    {
        file.seekg(countEntralink * sizeof(SHA1Seed), std::ios_base::cur);
        count = countNormal;
    }
    else
    {
        file.seekg((countEntralink + countNormal) * sizeof(SHA1Seed), std::ios_base::cur);
        count = countRoamer;
    }

    for (u32 i = 0; i < count; i++)
    {
        SHA1Seed entry;
        file.read(reinterpret_cast<char *>(&entry), sizeof(entry));

        if (entry.key.date >= (start.getJD() - Date().getJD()) && entry.key.date <= (end.getJD() - Date().getJD())
            && std::find_if(keypresses.begin(), keypresses.end(),
                            [&entry](const Keypress keypress) { return entry.key.button == toInt(keypress.button); })
                != keypresses.end())
        {
            for (u64 j = initialAdvance; j <= (initialAdvance + maxAdvance) && j < 6; j++)
            {
                if (ivCache.contains((j << 32) | (entry.seed >> 32)))
                {
                    cache.emplace(entry.key.key, entry.seed);
                }
            }
        }
    }

    cache.max_load_factor(0.9);
    cache.rehash(cache.size());

    return cache;
}

bool SHA1Cache::isValid() const
{
    return valid;
}

bool SHA1Cache::isValid(const Profile5 &profile) const
{
    std::ifstream stream(profile.getIVCache().data(), std::ios_base::in | std::ios_base::binary);
    if (stream.is_open())
    {
        // Expected magic word is CRC32 of "IVCache"
        u32 magic;
        stream.read(reinterpret_cast<char *>(&magic), sizeof(magic));
        if (magic != 0xd08cb7c0)
        {
            return false;
        }

        u32 initialAdvanceIV, maxAdvanceIV;
        stream.read(reinterpret_cast<char *>(&initialAdvanceIV), sizeof(initialAdvanceIV));
        stream.read(reinterpret_cast<char *>(&maxAdvanceIV), sizeof(maxAdvanceIV));

        return valid && initialAdvances == initialAdvanceIV && maxAdvances == maxAdvanceIV && mac == profile.getMac()
            && version == profile.getVersion() && timer0max == profile.getTimer0Max() && timer0min == profile.getTimer0Min()
            && type == profile.getDSType() && language == profile.getLanguage() && gxstat == profile.getGxStat()
            && vcount == profile.getVCount() && vframe == profile.getVFrame();
    }

    return false;
}
