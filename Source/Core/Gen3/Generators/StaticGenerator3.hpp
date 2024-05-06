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

#ifndef STATICGENERATOR3_HPP
#define STATICGENERATOR3_HPP

#include <Core/Gen3/Profile3.hpp>
#include <Core/Parents/Filters/StateFilter.hpp>
#include <Core/Parents/Generators/StaticGenerator.hpp>
#include <Core/Parents/StaticTemplate.hpp>

class GeneratorState;

/**
 * @brief Static encounter generator for Gen3
 */
class StaticGenerator3 : public StaticGenerator<StaticTemplate, Profile3, StateFilter>
{
public:
    /**
     * @brief Construct a new StaticGenerator3 object
     *
     * @param initialAdvances Initial number of advances
     * @param maxAdvances Maximum number of advances
     * @param delay Number of advances to offset
     * @param method Encounter method
     * @param staticTemplate Pokemon template
     * @param profile Profile Information
     * @param filter State filter
     */
    StaticGenerator3(u32 initialAdvances, u32 maxAdvances, u32 delay, Method method, const StaticTemplate &staticTemplate,
                     const Profile3 &profile, const StateFilter &filter);

    /**
     * @brief Generates states
     *
     * @param seed Starting PRNG state
     *
     * @return Vector of computed states
     */
    std::vector<GeneratorState> generate(u32 seed, const StaticTemplate &staticTemplate) const;
};

#endif // STATICGENERATOR3_HPP
