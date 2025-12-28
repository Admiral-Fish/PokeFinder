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
#include <fstream>

SHA1Cache::SHA1Cache(const std::string &file) : valid(false)
{
    std::ifstream stream(file.data(), std::ios_base::in | std::ios_base::binary);
    if (stream.is_open())
    {
        u32 magic;

        // Expected magic word is CRC32 of "SHA1Cache"
        stream.read(reinterpret_cast<char *>(&magic), sizeof(magic));
        if (magic != 0x3c50a97e)
        {
            return;
        }

        stream.read(reinterpret_cast<char *>(&initialAdvances), sizeof(initialAdvances));
        stream.read(reinterpret_cast<char *>(&maxAdvances), sizeof(maxAdvances));
        stream.read(reinterpret_cast<char *>(&mac), sizeof(mac));
        stream.read(reinterpret_cast<char *>(&end), sizeof(end));
        stream.read(reinterpret_cast<char *>(&start), sizeof(start));
        stream.read(reinterpret_cast<char *>(&version), sizeof(version));
        stream.read(reinterpret_cast<char *>(&timer0max), sizeof(timer0max));
        stream.read(reinterpret_cast<char *>(&timer0min), sizeof(timer0min));
        stream.read(reinterpret_cast<char *>(&softReset), sizeof(softReset));
        stream.read(reinterpret_cast<char *>(&type), sizeof(type));
        stream.read(reinterpret_cast<char *>(&language), sizeof(language));
        stream.read(reinterpret_cast<char *>(&gxstat), sizeof(gxstat));
        stream.read(reinterpret_cast<char *>(&vcount), sizeof(vcount));
        stream.read(reinterpret_cast<char *>(&vframe), sizeof(vframe));

        u32 countEntralink, countNormal, countRoamer;
        stream.read(reinterpret_cast<char *>(&countEntralink), sizeof(countEntralink));
        stream.read(reinterpret_cast<char *>(&countNormal), sizeof(countNormal));
        stream.read(reinterpret_cast<char *>(&countRoamer), sizeof(countRoamer));

        entralink.resize(countEntralink);
        normal.resize(countNormal);
        roamer.resize(countRoamer);

        stream.read(reinterpret_cast<char *>(entralink.data()), countEntralink * sizeof(SHA1Seed));
        stream.read(reinterpret_cast<char *>(normal.data()), countNormal * sizeof(SHA1Seed));
        stream.read(reinterpret_cast<char *>(roamer.data()), countRoamer * sizeof(SHA1Seed));

        valid = true;
    }
}

fph::MetaFphMap<u64, u64> SHA1Cache::getCache(u32 initialAdvance, u32 maxAdvance, const Date &start, const Date &end,
                                              const fph::MetaFphMap<u64, std::array<u8, 6>> &ivCache, CacheType type,
                                              const Profile5 &profile)
{
    fph::MetaFphMap<u64, u64> cache;
    auto keypresses = Keypresses::getKeypresses(profile);

    SHA1Seed *data;
    u32 count;

    if (type == CacheType::Entralink)
    {
        data = entralink.data();
        count = entralink.size();
    }
    else if (type == CacheType::Normal)
    {
        data = normal.data();
        count = normal.size();
    }
    else
    {
        data = roamer.data();
        count = roamer.size();
    }

    for (u32 i = 0; i < count; i++)
    {
        const auto &entry = &data[i];
        if (entry->key.date >= (start.getJD() - Date().getJD()) && entry->key.date <= (end.getJD() - Date().getJD())
            && std::find_if(keypresses.begin(), keypresses.end(),
                            [&entry](const Keypress keypress) { return entry->key.button == toInt(keypress.button); })
                != keypresses.end())
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

bool SHA1Cache::isValid() const
{
    return valid;
}

bool SHA1Cache::isValid(const Profile5 &profile) const
{
    return valid && mac == profile.getMac() && version == profile.getVersion() && timer0max == profile.getTimer0Max()
        && timer0min == profile.getTimer0Min() && softReset == profile.getSoftReset() && type == profile.getDSType()
        && language == profile.getLanguage() && gxstat == profile.getGxStat() && vcount == profile.getVCount()
        && vframe == profile.getVFrame();
}

bool SHA1Cache::isValid(const std::string &file, const std::string &ivFile)
{
    std::ifstream stream(file.data(), std::ios_base::in | std::ios_base::binary);
    std::ifstream ivStream(ivFile.data(), std::ios_base::in | std::ios_base::binary);

    if (stream.is_open() && ivStream.is_open())
    {
        u32 magic;
        u32 initialAdvance, initialAdvanceIV, maxAdvance, maxAdvanceIV;

        // Expected magic word is CRC32 of "IVCache"
        ivStream.read(reinterpret_cast<char *>(&magic), sizeof(magic));
        if (magic != 0xd08cb7c0)
        {
            return false;
        }

        ivStream.read(reinterpret_cast<char *>(&initialAdvanceIV), sizeof(initialAdvanceIV));
        ivStream.read(reinterpret_cast<char *>(&maxAdvanceIV), sizeof(maxAdvanceIV));

        // Expected magic word is CRC32 of "SHA1Cache"
        stream.read(reinterpret_cast<char *>(&magic), sizeof(magic));
        if (magic != 0x3c50a97e)
        {
            return false;
        }

        stream.read(reinterpret_cast<char *>(&initialAdvance), sizeof(initialAdvance));
        stream.read(reinterpret_cast<char *>(&maxAdvance), sizeof(maxAdvance));

        return initialAdvance == initialAdvanceIV && maxAdvance == maxAdvanceIV;
    }

    return false;
}
