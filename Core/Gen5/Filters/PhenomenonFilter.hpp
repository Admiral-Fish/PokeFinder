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

#ifndef PHENOMENONFILTER_HPP
#define PHENOMENONFILTER_HPP

#include <Core/Global.hpp>
#include <vector>

class PhenomenonState;

class PhenomenonFilter
{
public:
    /**
     * @brief Construct a new PhenomenonFilter object
     *
     * @param encounter Encounter to filter by
     * @param items Items to filter by
     */
    PhenomenonFilter(bool encounter, const std::vector<u16> &items);

    /**
     * @brief Determines if the \p state meets the filter criteria
     *
     * @param state State to compare
     *
     * @return true State passes the filter
     * @return false State does not pass the filter
     */
    bool compareState(const PhenomenonState &state) const;

private:
    std::vector<u16> items;
    bool encounter;
};

#endif // PHENOMENONFILTER_HPP
