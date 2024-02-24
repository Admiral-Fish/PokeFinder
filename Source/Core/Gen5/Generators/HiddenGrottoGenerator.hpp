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

#ifndef HIDDENGROTTOGENERATOR_HPP
#define HIDDENGROTTOGENERATOR_HPP

#include <Core/Gen5/Filters/HiddenGrottoFilter.hpp>
#include <Core/Gen5/HiddenGrottoArea.hpp>
#include <Core/Gen5/Profile5.hpp>
#include <Core/Parents/Generators/Generator.hpp>

class HiddenGrottoState;

/**
 * @brief Hidden grotto generator for Gen 5
 */
class HiddenGrottoGenerator : public Generator<Profile5, HiddenGrottoFilter>
{
public:
    /**
     * @brief Construct a new HiddenGrottoGenerator object
     *
     * @param initialAdvances Initial number of advances
     * @param maxAdvances Maximum number of advances
     * @param delay Number of advances to offset
     * @param powerLevel Hidden grotto encounter rate
     * @param encounterArea Hidden grotto information
     * @param profile Profile Information
     * @param filter State filter
     */
    HiddenGrottoGenerator(u32 initialAdvances, u32 maxAdvances, u32 delay, u8 powerLevel, const HiddenGrottoArea &encounterArea,
                          const Profile5 &profile, const HiddenGrottoFilter &filter);

    /**
     * @brief Generates states
     *
     * @param seed Starting PRNG state
     *
     * @return Vector of computed states
     */
    std::vector<HiddenGrottoState> generate(u64 seed) const;

private:
    HiddenGrottoArea encounterArea;
    u8 powerLevel;
};

#endif // HIDDENGROTTOGENERATOR_HPP
