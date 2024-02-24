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

#ifndef EGGGENERATOR_HPP
#define EGGGENERATOR_HPP

#include <Core/Parents/Daycare.hpp>
#include <Core/Parents/Generators/Generator.hpp>

/**
 * @brief Parent generator class for egg encounters
 *
 * @tparam Profile Profile class that is used by the generator
 * @tparam Filter Filter class that is used by the generator
 */
template <class Profile, class Filter>
class EggGenerator : public Generator<Profile, Filter>
{
public:
    /**
     * @brief Construct a new EggGenerator object
     *
     * @param initialAdvances Initial number of advances
     * @param maxAdvances Maximum number of advances
     * @param delay Number of advances to offset
     * @param method Encounter method
     * @param compatability Parent compatability
     * @param daycare Daycare parent information
     * @param profile Profile Information
     * @param filter State filter
     */
    EggGenerator(u32 initialAdvances, u32 maxAdvances, u32 delay, Method method, u8 compatability, const Daycare &daycare,
                 const Profile &profile, const Filter &filter) :
        Generator<Profile, Filter>(initialAdvances, maxAdvances, delay, method, profile, filter),
        daycare(daycare),
        compatability(compatability)
    {
    }

protected:
    Daycare daycare;
    u8 compatability;
};

#endif // EGGGENERATOR_HPP
