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

#ifndef ADJACENTSEEDSCALCULATOR_HPP
#define ADJACENTSEEDSCALCULATOR_HPP

#include <Core/Gen5/Profile5.hpp>
#include <Core/Gen5/States/AdjacentSeedsState.hpp>
#include <Core/Global.hpp>
#include <Core/Util/DateTime.hpp>
#include <vector>

enum class Buttons : u16;

namespace AdjacentSeedsCalculator
{
    /**
     * @brief Computes "nearby" seeds for calibration purposes
     * 
     * @param minIVAdvance Minimum IV advances
     * @param maxIVAdvance Maximum IV advances
     * @param seconds Second search range
     * @param roamer Whether encounter is roamer or not
     * @param buttons Current button presses
     * @param dateTime Starting date/time
     * @param profile Profile information
     */
    std::vector<AdjacentSeedsState> generate(u32 minIVAdvance, u32 maxIVAdvance, int seconds, bool roamer, Buttons buttons,
                                             const DateTime &dateTime, const Profile5 &profile);

    /**
     * @brief Computes chatot/needle string for seed verification purposes
     * 
     * @param seed PRNG state
     * @param advances PRNG advances
     * @param count Number of chatot/needle calls to output
     * @param chatot Whether to output chatot/needle
     */
    std::string previewPRNG(u64 seed, u32 advances, u32 count, bool chatot);
}

#endif // ADJACENTSEEDSCALCULATOR_HPP
