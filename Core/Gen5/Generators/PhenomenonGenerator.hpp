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

#ifndef PHENOMENONGENERATOR_HPP
#define PHENOMENONGENERATOR_HPP

#include <Core/Gen5/Filters/PhenomenonFilter.hpp>
#include <Core/Gen5/PhenomenonArea.hpp>
#include <Core/Gen5/Profile5.hpp>
#include <Core/Parents/Generators/Generator.hpp>

class HiddenGrottoState;
class State5;

/**
 * @brief Phenomenon grotto slot generator for Gen 5
 */
class PhenomenonGenerator : public Generator<Profile5, PhenomenonFilter>
{
public:
    /**
     * @brief Construct a new PhenomenonGenerator object
     *
     * @param initialAdvances Initial number of advances
     * @param maxAdvances Maximum number of advances
     * @param offset Number of advances to offset
     * @param encounterArea Hidden grotto information
     * @param profile Profile Information
     * @param filter State filter
     */
    PhenomenonGenerator(u32 initialAdvances, u32 maxAdvances, u32 offset, const PhenomenonArea &encounterArea, const Profile5 &profile,
                        const PhenomenonFilter &filter);

    /**
     * @brief Generates states
     *
     * @param seed Starting PRNG state
     *
     * @return Vector of computed states
     */
    std::vector<PhenomenonState> generate(u64 seed) const;

private:
    PhenomenonArea area;
};

#endif // PHENOMENONGENERATOR_HPP
