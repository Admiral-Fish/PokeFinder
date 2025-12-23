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

#ifndef SHA1CACHE_HPP
#define SHA1CACHE_HPP

#include <Core/Gen5/IVCache.hpp>
#include <Core/Gen5/Profile5.hpp>
#include <Core/Util/DateTime.hpp>
#include <fph/meta_fph_table.h>

enum class DSType : u8;
enum class Game : u32;
enum class Language : u8;

union SHA1Key {
    u64 key;
    struct
    {
        u32 button : 12;
        u32 time : 20;
        u16 date;
        u16 timer0;
    };

    SHA1Key() = default;

    SHA1Key(u16 button, u32 time, u16 date, u16 timer0) : button(button), time(time), date(date), timer0(timer0)
    {
    }
};

struct SHA1Seed
{
    SHA1Key key;
    u64 seed;

    SHA1Seed() = default;

    SHA1Seed(u16 button, u32 time, u16 date, u16 timer0, u64 seed) : key(button, time, date, timer0), seed(seed)
    {
    }
};

/**
 * @brief Cache for SHA1 seeds
 */
class SHA1Cache
{
public:
    /**
     * @brief Construct a new SHA1Cache object
     *
     * @param file Path to file to read the cache from
     */
    SHA1Cache(const std::string &file);

    /**
     * @brief Returns the SHA1 cache for the \p type
     *
     * @param initialAdvance Initial IV advances
     * @param maxAdvance Maximum IV advances
     * @param start Start date
     * @param end End date
     * @param ivCache Fast search IV cache
     * @param type What cache type to get
     * @param profile Profile information to filter keypresses by
     *
     * @return IV caches
     */
    fph::MetaFphMap<u64, u64> getCache(u32 initialAdvance, u32 maxAdvance, const Date &start, const Date &end,
                                       const fph::MetaFphMap<u64, std::array<u8, 6>> &ivCache, CacheType type, const Profile5 &profile);

    /**
     * @brief Returns the end date
     *
     * @return End date
     */
    Date getEndDate() const
    {
        return end;
    }

    /**
     * @brief Returns the initial advance supported by the cache
     *
     * @return Initial supported advance
     */
    u32 getInitialAdvances() const
    {
        return initialAdvances;
    }

    /**
     * @brief Returns the max advance supported by the cache
     *
     * @return Max supported advance
     */
    u32 getMaxAdvances() const
    {
        return maxAdvances;
    }

    /**
     * @brief Returns the start date
     *
     * @return start date
     */
    Date getStartDate() const
    {
        return start;
    }

    /**
     * @brief Determines if cache is valid
     */
    bool isValid() const;

    /**
     * @brief Determines cache was created from the given \p profile
     *
     * @param profile Profile information
     */
    bool isValid(const Profile5 &profile) const;

    /**
     * @brief Determines if cache is valid
     *
     * @param file Path to file to validate the cache from
     */
    static bool isValid(const std::string &file, const std::string &ivFile);

private:
    std::vector<SHA1Seed> entralink;
    std::vector<SHA1Seed> normal;
    std::vector<SHA1Seed> roamer;
    u64 mac;
    Date end;
    Date start;
    Game version;
    u32 initialAdvances;
    u32 maxAdvances;
    u16 timer0max;
    u16 timer0min;
    bool softReset;
    DSType type;
    Language language;
    bool valid;
    u8 gxstat;
    u8 vcount;
    u8 vframe;
};

#endif // SHA1CACHE_HPP
