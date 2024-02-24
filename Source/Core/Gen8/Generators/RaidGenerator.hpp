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

#ifndef RAIDGENERATOR_HPP
#define RAIDGENERATOR_HPP

#include <Core/Gen8/Profile8.hpp>
#include <Core/Parents/Filters/StateFilter.hpp>
#include <Core/Parents/Generators/Generator.hpp>

class Raid;
class GeneratorState;

/**
 * @brief Raid encounter generator for Gen8
 */
class RaidGenerator : Generator<Profile8, StateFilter>
{
public:
    /**
     * @brief Construct a new RaidGenerator object
     *
     * @param initialAdvances Initial number of advances
     * @param maxAdvances Maximum number of advances
     * @param delay Number of advances to offset
     * @param profile Profile Information
     * @param filter State filter
     */
    RaidGenerator(u32 initialAdvances, u32 maxAdvances, u32 delay, const Profile8 &profile, const StateFilter &filter);

    /**
     * @brief Generates states for the \p raid
     *
     * @param seed0 PRNG state
     * @param raid Pokemon template
     *
     * @return Vector of computed states
     */
    std::vector<GeneratorState> generate(u64 seed, u8 level, const Raid &raid) const;
};

#endif // RAIDGENERATOR_HPP
