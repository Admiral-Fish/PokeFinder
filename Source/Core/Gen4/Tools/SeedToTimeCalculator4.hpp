/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2023 by Admiral_Fish, bumba, and EzPzStreamz
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

#ifndef SEEDTOTIMECALCULATOR4_HPP
#define SEEDTOTIMECALCULATOR4_HPP

#include <Core/Gen4/SeedTime4.hpp>
#include <vector>

namespace SeedToTimeCalculator4
{
    /**
     * @brief Generates date/times from a seed. Can filter by a specific second with \p forceSecond and \p forcedSecond
     *
     * @param seed PRNG state
     * @param year Year to search
     * @param forceSecond Whether to filter by second or not
     * @param forcedSecond Second to filter by
     *
     * @return Vector of generated date/times
     */
    std::vector<SeedTime4> calculateTimes(u32 seed, u16 year, bool forceSecond, u8 forcedSecond);

    /**
     * @brief Generates calibration results within \p minusDelay, \p plusDelay, \p minusSecond, and \p plusSecond from the selected \p
     * target.
     *
     * @param minusDelay Minimum delay range
     * @param plusDelay Maximum delay range
     * @param minusSecond Minimum second range
     * @param plusSecond Maximum second range
     * @param target Date/time target
     *
     * @return Vector of calibration date/times
     */
    std::vector<SeedTimeCalibrate4> calibrate(int minusDelay, int plusDelay, int minusSecond, int plusSecond, const SeedTime4 &target);

    /**
     * @brief Generates calibration results within \p minusDelay, \p plusDelay, \p minusSecond, and \p plusSecond from the selected \p
     * target.
     *
     * @param minusDelay Minimum delay range
     * @param plusDelay Maximum delay range
     * @param minusSecond Minimum second range
     * @param plusSecond Maximum second range
     * @param roamers Which roamers are active
     * @param routes Which route active roamers are on
     * @param target Date/time target
     *
     * @return Vector of calibration date/times
     */
    std::vector<SeedTimeCalibrate4> calibrate(int minusDelay, int plusDelay, int minusSecond, int plusSecond,
                                              const std::array<bool, 3> &roamers, std::array<u8, 3> &routes, const SeedTime4 &target);
};

#endif // SEEDTOTIMECALCULATOR4_HPP
