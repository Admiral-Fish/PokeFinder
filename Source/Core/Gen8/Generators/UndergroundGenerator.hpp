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

#ifndef UNDERGROUNDGENERATOR_HPP
#define UNDERGROUNDGENERATOR_HPP

#include <Core/Gen8/Filters/StateFilter8.hpp>
#include <Core/Gen8/Profile8.hpp>
#include <Core/Parents/Generators/StaticGenerator.hpp>

class UndergroundArea;

/**
 * @brief Underground encounter generator for Gen8
 */
class UndergroundGenerator : public StaticGenerator<Profile8, UndergroundStateFilter>
{
public:
    /**
     * @brief Construct a new UndergroundGenerator object
     *
     * @param initialAdvances Initial number of advances
     * @param maxAdvances Maximum number of advances
     * @param delay Number of advances to offset
     * @param lead Encounter lead
     * @param diglett Whether diglett bonus is activiated
     * @param levelFlag Determines which level range to pull from
     * @param profile Profile Information
     * @param filter State filter
     */
    UndergroundGenerator(u32 initialAdvances, u32 maxAdvances, u32 delay, Lead lead, bool diglett, u8 levelFlag, const Profile8 &profile,
                         const UndergroundStateFilter &filter);

    /**
     * @brief Generates states for the \p encounterArea
     *
     * @param seed0 Upper half of PRNG state
     * @param seed1 Lower half of PRNG state
     * @param encounterArea Wild pokemon info
     *
     * @return Vector of computed states
     */
    std::vector<UndergroundState> generate(u64 seed0, u64 seed1, const UndergroundArea &encounterArea) const;

private:
    bool diglett;
    u8 levelFlag;
};

#endif // UNDERGROUNDGENERATOR_HPP
