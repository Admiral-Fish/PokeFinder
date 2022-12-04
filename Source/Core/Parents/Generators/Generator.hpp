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

#ifndef GENERATOR_HPP
#define GENERATOR_HPP

#include <Core/Global.hpp>
#include <vector>

enum class Encounter : u8;
enum class Method : u8;

/**
 * @brief Parent generator class that stores common attributes
 *
 * @tparam Profile Profile class that is used by the generator
 * @tparam Filter Filter class that is used by the generator
 */
template <class Profile, class Filter>
class Generator
{
public:
    /**
     * @brief Construct a new Generator object
     *
     * @param initialAdvances Initial number of advances
     * @param maxAdvances Maximum number of advances
     * @param delay Number of advances to offset
     * @param method Encounter method
     * @param profile Profile Information
     * @param filter State filter
     */
    Generator(u32 initialAdvances, u32 maxAdvances, u32 delay, Method method, const Profile &profile, const Filter &filter) :
        profile(profile),
        initialAdvances(initialAdvances),
        maxAdvances(maxAdvances),
        delay(delay),
        tsv(profile.getTID() ^ profile.getSID()),
        method(method),
        filter(filter)
    {
    }

protected:
    Profile profile;
    u32 initialAdvances;
    u32 maxAdvances;
    u32 delay;
    u16 tsv;
    Filter filter;
    Method method;
};

#endif // GENERATOR_HPP
