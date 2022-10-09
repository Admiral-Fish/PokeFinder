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

#ifndef IVCHECKER_HPP
#define IVCHECKER_HPP

#include <Core/Global.hpp>
#include <array>
#include <vector>

/**
 * @brief Computes IV range given a number of entry of stats
 */
namespace IVChecker
{
    /**
     * @brief Calculates the IV range for the given \p stats. Uses additional information when available to narrow down the range.
     *
     * @param baseStats Pokemon base stats
     * @param stats Pokemon stats
     * @param level Pokemon level
     * @param nature Pokemon nature
     * @param characteristic Pokemon characteristic
     * @param hiddenPower Pokemon hidden power
     *
     * @return Array of IV ranges for each stat
     */
    std::array<std::vector<u8>, 6> calculateIVRange(const std::array<u8, 6> &baseStats, const std::vector<std::array<u16, 6>> &stats,
                                                    const std::vector<u8> &level, u8 nature, u8 characteristic, u8 hiddenPower);

    /**
     * @brief nextLevel
     *
     * @param baseStats Pokemon base stats
     * @param ivs Pokemon candidate IVs
     * @param level Pokemon level
     * @param nature Pokemon nature
     *
     * @return Next level that will narrow down \p ivs more
     */
    std::array<u8, 6> nextLevel(const std::array<u8, 6> &baseStats, const std::array<std::vector<u8>, 6> &ivs, u8 level, u8 nature);
};

#endif // IVCHECKER_HPP
