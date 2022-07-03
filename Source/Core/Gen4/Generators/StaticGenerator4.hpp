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

#ifndef STATICGENERATOR4_HPP
#define STATICGENERATOR4_HPP

#include <Core/Gen4/Filters/StateFilter4.hpp>
#include <Core/Gen4/Profile4.hpp>
#include <Core/Parents/Generators/StaticGenerator.hpp>

class StaticTemplate4;

/**
 * @brief Static encounter generator for Gen4
 */
class StaticGenerator4 : public StaticGenerator<Profile4, StateFilter4>
{
public:
    /**
     * @brief Construct a new StaticGenerator4 object
     *
     * @param initialAdvances Initial number of advances
     * @param maxAdvances Maximum number of advances
     * @param delay Number of advances to offset
     * @param method Encounter method
     * @param lead Encounter lead
     * @param profile Profile Information
     * @param filter State filter
     */
    StaticGenerator4(u32 initialAdvances, u32 maxAdvances, u32 delay, Method method, Lead lead, const Profile4 &profile,
                     const StateFilter4 &filter);

    /**
     * @brief Generates states for the \p staticTemplate
     *
     * @param seed Starting PRNG state
     * @param staticTemplate Pokemon template
     *
     * @return Vector of computed states
     */
    std::vector<GeneratorState4> generate(u32 seed, const StaticTemplate4 *staticTemplate) const;

private:
    /**
     * @brief Generates states for the \p staticTemplate via Method 1
     *
     * @param seed Starting PRNG state
     * @param staticTemplate Pokemon template
     *
     * @return Vector of computed states
     */
    std::vector<GeneratorState4> generateMethod1(u32 seed, const StaticTemplate4 *staticTemplate) const;

    /**
     * @brief Generates states for the \p staticTemplate via Method J
     *
     * @param seed Starting PRNG state
     * @param staticTemplate Pokemon template
     *
     * @return Vector of computed states
     */
    std::vector<GeneratorState4> generateMethodJ(u32 seed, const StaticTemplate4 *staticTemplate) const;

    /**
     * @brief Generates states for the \p staticTemplate via Method K
     *
     * @param seed Starting PRNG state
     * @param staticTemplate Pokemon template
     *
     * @return Vector of computed states
     */
    std::vector<GeneratorState4> generateMethodK(u32 seed, const StaticTemplate4 *staticTemplate) const;
};

#endif // STATICGENERATOR4_HPP
