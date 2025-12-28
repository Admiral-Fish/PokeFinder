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

#ifndef IVCACHE_HPP
#define IVCACHE_HPP

#include <Core/Global.hpp>
#include <array>
#include <fph/meta_fph_table.h>
#include <vector>

class StateFilter;
enum class Game : u32;

enum class CacheType : u8
{
    Entralink,
    Normal,
    Roamer
};

/**
 * @brief Cache for IV seeds
 */
class IVCache
{
public:
    /**
     * @brief Construct a new IVCache object
     *
     * @param file Path to file to read the cache from
     * @param readData Whether or not to read the seed cache data
     */
    IVCache(const std::string &file);

    /**
     * @brief Returns the IV caches for the \p type
     *
     * @param initialAdvance Initial IV advances
     * @param maxAdvance Maximum IV advances
     * @param version Game version
     * @param type What cache type to get
     * @param filter IV filter
     *
     * @return IV caches
     */
    fph::MetaFphMap<u64, std::array<u8, 6>> getCache(u32 initialAdvances, u32 maxAdvances, Game version, CacheType type,
                                                     const StateFilter &filter) const;

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
     * @brief Returns the IV cache seeds for the \p type
     *
     * @param version Game version
     * @param type What cache type to get
     *
     * @return Vector of IV cache seeds
     */
    std::vector<u32> getSeeds(Game version, CacheType type) const;

    /**
     * @brief Determines if cache is valid
     */
    bool isValid() const;

    /**
     * @brief Determines if cache is valid
     * 
     * @param file Path to file to validate the cache from
     */
    static bool isValid(const std::string &file);


private:
    std::vector<std::vector<u32>> entralinkSeeds;
    std::vector<std::vector<u32>> normalSeeds;
    std::vector<std::vector<u32>> roamerSeeds;
    u32 initialAdvances;
    u32 maxAdvances;
    bool valid;

    /**
     * @brief Returns the IV caches for entralink
     *
     * @param initialAdvance Initial IV advances
     * @param maxAdvance Maximum IV advances
     * @param filter IV filter
     *
     * @return IV caches
     */
    fph::MetaFphMap<u64, std::array<u8, 6>> getEntralinkCache(u32 initialAdvances, u32 maxAdvances, const StateFilter &filter) const;

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
    fph::MetaFphMap<u64, std::array<u8, 6>> getNormalCache(u32 initialAdvances, u32 maxAdvances, Game version,
                                                           const StateFilter &filter) const;

    /**
     * @brief Returns the IV caches for roamers
     *
     * @param initialAdvance Initial IV advances
     * @param maxAdvance Maximum IV advances
     * @param filter IV filter
     *
     * @return IV caches
     */
    fph::MetaFphMap<u64, std::array<u8, 6>> getRoamerCache(u32 initialAdvances, u32 maxAdvances, const StateFilter &filter) const;
};

#endif // IVCACHE_HPP
