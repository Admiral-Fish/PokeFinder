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

#ifndef NATURE_HPP
#define NATURE_HPP

#include <Core/Global.hpp>

extern const float modifiers[25][5];

/**
 * @brief Computes stats that get modified from the nature
 */
namespace Nature
{
    /**
     * @brief Computes the stat based upon the \p nature and stat \p index
     *
     * @param baseStat Pokemon base stat
     * @param iv Pokemon IV
     * @param nature Pokemon nature
     * @param level Pokemon level
     * @param index Pokemon stat index
     *
     * @return Computed modified stat
     */
    inline u16 computeStat(u16 baseStat, u8 iv, u8 nature, u8 level, u8 index)
    {
        u16 stat = ((2 * baseStat + iv) * level) / 100;
        if (index == 0)
        {
            return stat + level + 10;
        }
        else
        {
            return (stat + 5) * modifiers[nature][index - 1];
        }
    }
}

#endif // NATURE_HPP
