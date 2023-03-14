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

#ifndef IDGENERATOR5_HPP
#define IDGENERATOR5_HPP

#include <Core/Gen5/Profile5.hpp>
#include <Core/Parents/Generators/IDGenerator.hpp>

class IDState;

/**
 * @brief TID/SID generator for Gen5
 */
class IDGenerator5 : public IDGenerator
{
public:
    /**
     * @brief Construct a new IDGenerator5 object
     *
     * @param minDelay Minimum delay
     * @param maxDelay Maximum delay
     * @param year Search year
     * @param month Search month
     * @param day Search day
     * @param hour Seach hour
     * @param minute Search minute
     * @param filter State filter
     */
    IDGenerator5(u32 initialAdvances, u32 maxAdvances, const Profile5 &profile, const IDFilter &filter);

    /**
     * @brief Generates states
     *
     * @param seed Starting PRNG state
     * @param pid PID to search for
     * @param checkPID Whether to check for PID
     * @param checkXOR Whether to check for PID XORed
     *
     * @return Vector of computed states
     */
    std::vector<IDState> generate(u64 seed, u32 pid = 0, bool checkPID = false, bool checkXOR = false) const;

private:
    Profile5 profile;
};

#endif // IDGENERATOR5_HPP
