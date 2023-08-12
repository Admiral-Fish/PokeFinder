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

#ifndef EVENTGENERATOR4_HPP
#define EVENTGENERATOR4_HPP

#include <Core/Gen4/Profile4.hpp>
#include <Core/Parents/Filters/StateFilter.hpp>
#include <Core/Parents/Generators/Generator.hpp>

class GeneratorState4;

/**
 * @brief Event encounter generator for Gen4
 */
class EventGenerator4 : public Generator<Profile4, StateFilter>
{
public:
    /**
     * @brief Construct a new StaticGenerator4 object
     *
     * @param initialAdvances Initial number of advances
     * @param maxAdvances Maximum number of advances
     * @param delay Number of advances to offset
     * @param species Pokemon specie
     * @param nature Pokemon nature
     * @param level Pokemon level
     * @param profile Profile Information
     * @param filter State filter
     */
    EventGenerator4(u32 initialAdvances, u32 maxAdvances, u32 delay, u16 species, u8 nature, u8 level, const Profile4 &profile,
                    const StateFilter &filter);

    /**
     * @brief Generates states
     *
     * @param seed Starting PRNG state
     *
     * @return Vector of computed states
     */
    std::vector<GeneratorState4> generate(u32 seed) const;

private:
    u16 species;
    u8 nature;
    u8 level;
};

#endif // EVENTGENERATOR4_HPP
