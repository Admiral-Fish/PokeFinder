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

#ifndef PIDTOIVCALCULATOR_HPP
#define PIDTOIVCALCULATOR_HPP

#include <Core/Global.hpp>
#include <vector>

class PIDToIVState;

/**
 * @brief Computes IV from PID for Method 1/2/4, XD/Colo, and Channel
 */
namespace PIDToIVCalculator
{
    /**
     * @brief Computes IVs from the \p pid for Method 1/2/4, XD/Colo, and Channel
     *
     * @param pid PID value
     *
     * @return Vector of computed IVs
     */
    std::vector<PIDToIVState> calculateIVs(u32 pid);
};

#endif // PIDTOIVCALCULATOR_HPP
