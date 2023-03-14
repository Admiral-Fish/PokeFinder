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

#ifndef STATICGENERATOR_HPP
#define STATICGENERATOR_HPP

#include <Core/Parents/Generators/Generator.hpp>

enum class Lead : u8;

/**
 * @brief Parent generator class for static encounters
 *
 * @tparam Profile Profile class that is used by the generator
 * @tparam Filter Filter class that is used by the generator
 */
template <class Profile, class Filter>
class StaticGenerator : public Generator<Profile, Filter>
{
public:
    /**
     * @brief Construct a new StaticGenerator object
     *
     * @param initialAdvances Initial number of advances
     * @param maxAdvances Maximum number of advances
     * @param delay Number of advances to offset
     * @param method Encounter method
     * @param lead Encounter lead
     * @param profile Profile Information
     * @param filter State filter
     */
    StaticGenerator(u32 initialAdvances, u32 maxAdvances, u32 delay, Method method, Lead lead, const Profile &profile,
                    const Filter &filter) :
        Generator<Profile, Filter>(initialAdvances, maxAdvances, delay, method, profile, filter), lead(lead)
    {
    }

protected:
    Lead lead;
};

#endif // STATICGENERATOR_HPP
