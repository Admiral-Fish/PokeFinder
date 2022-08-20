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
 * @tparam Filter Filter class that is used by the searcher
 */
template <class Filter>
class StaticSearcher : public Searcher<Filter>
{
public:
    /**
     * @brief Construct a new StaticGenerator object
     *
     * @param tid Trainer ID
     * @param sid Secret ID
     * @param version Game version
     * @param method Encounter method
     * @param filter State filter
     */
    StaticSearcher(u16 tid, u16 sid, Game version, Method method, const Filter &filter) :
        Searcher<Filter>(tid, sid, version, method, filter)
    {
    }

    /**
     * @brief Sets the lead type used by the generator
     *
     * @param lead Lead type
     */
    void setLead(Lead lead)
    {
        this->lead = lead;
    }

protected:
    Lead lead;
};

#endif // STATICSEARCHER_HPP
