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

#ifndef IVTOPIDCALCULATOR_HPP
#define IVTOPIDCALCULATOR_HPP

#include <Core/Global.hpp>
#include <vector>

class IVToPIDState;

/**
 * @brief Computes PID from IVs
 */
namespace IVToPIDCalculator
{
    /**
     * @brief Computes PID from the IVs for Method 1/2/4, XD/Colo, and Channel
     *
     * @param hp HP IV
     * @param atk Atk IV
     * @param def Def IV
     * @param spa SpA IV
     * @param spd SpD IV
     * @param spe Spe IV
     * @param nature Nature value
     * @param tid Trainer ID
     *
     * @return Vector of computed PIDs
     */
    std::vector<IVToPIDState> calculatePIDs(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe, u8 nature, u16 tid);
};

#endif // IVTOPIDCALCULATOR_HPP
