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

#ifndef EVENTGENERATOR5_HPP
#define EVENTGENERATOR5_HPP

#include <Core/Gen5/PGF.hpp>
#include <Core/Gen5/Profile5.hpp>
#include <Core/Parents/Filters/StateFilter.hpp>
#include <Core/Parents/Generators/Generator.hpp>

class State5;

/**
 * @brief Event generator for Gen 5
 */
class EventGenerator5 : public Generator<Profile5, StateFilter>
{
public:
    /**
     * @brief Construct a new EventGenerator5 object
     *
     * @param initialAdvances Initial number of advances
     * @param maxAdvances Maximum number of advances
     * @param delay Number of advances to offset
     * @param pgf Pokemon template
     * @param profile Profile Information
     * @param filter State filter
     */
    EventGenerator5(u32 initialAdvances, u32 maxAdvances, u32 delay, const PGF &pgf, const Profile5 &profile, const StateFilter &filter);

    /**
     * @brief Generates states
     *
     * @param seed Starting PRNG state
     *
     * @return Vector of computed states
     */
    std::vector<State5> generate(u64 seed) const;

private:
    PGF pgf;
    u8 wondercardAdvances;
};

#endif // EVENTGENERATOR5_HPP
