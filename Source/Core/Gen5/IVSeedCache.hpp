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

#ifndef IVSEEDCACHE_HPP
#define IVSEEDCACHE_HPP

#include <Core/Global.hpp>
#include <array>
#include <dynamic_fph_table.h>

class StateFilter;
enum class Game : u32;

/**
 * @brief Grabs IV caches for the various encounter types
 */
namespace IVSeedCache
{
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
    std::array<fph::DynamicFphMap<u32, std::array<u8, 6>>, 6> getNormalCache(u32 initialAdvance, u32 maxAdvance, Game version,
                                                                             const StateFilter &filter);

    /**
     * @brief Returns the IV caches for roamers
     *
     * @param initialAdvance Initial IV advances
     * @param maxAdvance Maximum IV advances
     * @param filter IV filter
     *
     * @return IV caches
     */
    std::array<fph::DynamicFphMap<u32, std::array<u8, 6>>, 6> getRoamerCache(u32 initialAdvance, u32 maxAdvance, const StateFilter &filter);

    /**
     * @brief Returns the IV caches for entralink
     *
     * @param initialAdvance Initial IV advances
     * @param maxAdvance Maximum IV advances
     * @param filter IV filter
     *
     * @return IV caches
     */
    std::array<fph::DynamicFphMap<u32, std::array<u8, 6>>, 6> getEntralinkCache(u32 initialAdvance, u32 maxAdvance,
                                                                                const StateFilter &filter);
};

#endif // IVSEEDCACHE_HPP
