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

#ifndef EGGGENERATOR7_HPP
#define EGGGENERATOR7_HPP

#include <Core/Gen7/Profile7.hpp>
#include <Core/Parents/Filters/StateFilter.hpp>
#include <Core/Parents/Generators/EggGenerator.hpp>

class EggGeneratorState;

/**
 * @brief Egg generator for Gen7
 */
class EggGenerator7 : public EggGenerator<Profile7, StateFilter>
{
public:
    /**
     * @brief Construct a new EggGenerator7 object
     *
     * @param initialAdvances Initial number of advances
     * @param maxAdvances Maximum number of advances
     * @param offset Number of advances to offset
     * @param daycare Daycare parent information
     * @param profile Profile Information
     * @param filter State filter
     */
    EggGenerator7(u32 initialAdvances, u32 maxAdvances, u32 offset, const Daycare &daycare, const Profile7 &profile,
                  const StateFilter &filter);

    /**
     * @brief Generates states
     *
     * @param seed0 PRNG value 0
     * @param seed1 PRNG value 1
     * @param seed2 PRNG value 2
     * @param seed3 PRNG value 3
     *
     * @return Vector of computed states
     */
    std::vector<EggGeneratorState> generate(u32 seed0, u32 seed1, u32 seed2, u32 seed3) const;

private:
    bool shinyCharm;
};

#endif // EGGGENERATOR7_HPP
