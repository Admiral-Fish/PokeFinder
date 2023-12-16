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

#ifndef IDSEARCHER4_HPP
#define IDSEARCHER4_HPP

#include <Core/Parents/Filters/IDFilter.hpp>
#include <Core/Parents/Searchers/Searcher.hpp>

class IDState4;

/**
 * @brief TID/SID searcher for Gen4
 */
class IDSearcher4 : public SearcherBase<IDState4>
{
public:
    /**
     * @brief Construct a new IDSearcher4 object
     *
     * @param filter State filter
     */
    IDSearcher4(const IDFilter &filter);

    /**
     * @brief Starts the search
     *
     * @param infinite Whether to search all delays or not
     * @param year Search year
     * @param minDelay Minimum delay
     * @param maxDelay Maximum delay
     */
    void startSearch(bool infinite, u16 year, u32 minDelay, u32 maxDelay);

private:
    IDFilter filter;
};

#endif // IDSEARCHER4_HPP
