/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2022 by Admiral_Fish, bumba, and EzPzStreamz
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

#ifndef RNGCACHE_HPP
#define RNGCACHE_HPP

#include <Core/Global.hpp>
#include <vector>

enum class Method : u8;

/**
 * @brief Provides a way to compute origin seed given IVs or PID for Method 1/2/4.
 * See https://crypto.stackexchange.com/a/10609 for how the following math works
 */
class RNGCache
{
public:
    /**
     * @brief Construct a new RNGCache object
     *
     * @param method Generation method
     */
    RNGCache(Method method);

    /**
     * @brief Recovers origin seeds for two 16 bit calls(15 bits known) with or without gap based on the cache
     *
     * @param hp HP iv
     * @param atk Atk iv
     * @param def Def iv
     * @param spa SpA iv
     * @param spd SpD iv
     * @param spe Spe iv
     *
     * @return Vector of origin seeds
     */
    std::vector<u32> recoverLower16BitsIV(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe) const;

    /**
     * @brief Recovers origin seeds for two 16 bit calls based on the cache
     *
     * @param pid PID value
     *
     * @return Vector of origin seeds
     */
    std::vector<u32> recoverLower16BitsPID(u32 pid) const;

private:
    bool flags[0x10000];
    u8 low[0x10000];
    u32 add;
    u32 k;
    u32 mult;
};

#endif // RNGCACHE_HPP
