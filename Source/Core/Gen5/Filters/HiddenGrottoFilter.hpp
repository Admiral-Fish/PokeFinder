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

#ifndef HIDDENGROTTOFILTER_HPP
#define HIDDENGROTTOFILTER_HPP

#include <array>

class HiddenGrottoState;

class HiddenGrottoFilter
{
public:
    /**
     * @brief Construct a new HiddenGrottoFilter object
     *
     * @param encounterSlots Encounter slots to filter by
     * @param genders Genders to filter by
     * @param groups Groups to filter by
     */
    HiddenGrottoFilter(const std::array<bool, 11> &encounterSlots, const std::array<bool, 2> &genders, const std::array<bool, 4> &groups);

    /**
     * @brief Determines if the \p state meets the filter criteria
     *
     * @param state State to compare
     *
     * @return true State passes the filter
     * @return false State does not pass the filter
     */
    bool compareState(const HiddenGrottoState &state) const;

private:
    std::array<bool, 11> encounterSlots;
    std::array<bool, 2> genders;
    std::array<bool, 4> groups;
};

#endif // HIDDENGROTTOFILTER_HPP
