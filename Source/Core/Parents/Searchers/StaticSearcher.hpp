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

#ifndef STATICSEARCHER_HPP
#define STATICSEARCHER_HPP

#include <Core/Enum/Lead.hpp>
#include <Core/Parents/Searchers/Searcher.hpp>

/**
 * @brief Parent searcher class for static encounters
 *
 * @tparam Profile Profile class that is used by the generator
 * @tparam Filter Filter class that is used by the searcher
 */
template <class Profile, class Filter>
class StaticSearcher : public Searcher<Profile, Filter>
{
public:
    /**
     * @brief Construct a new StaticSearcher object
     *
     * @param method Encounter method
     * @param lead Encounter lead
     * @param profile Profile Information
     * @param filter State filter
     */
    StaticSearcher(Method method, Lead lead, const Profile &profile, const Filter &filter) :
        Searcher<Profile, Filter>(method, profile, filter), lead(lead)
    {
    }

protected:
    Lead lead;
};

#endif // STATICSEARCHER_HPP
