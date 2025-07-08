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

#ifndef UNDERGROUNDFILTER_HPP
#define UNDERGROUNDFILTER_HPP

#include <Core/Parents/Filters/StateFilter.hpp>
#include <vector>

class UndergroundState;

/**
 * @brief Checks if wild encounters match the filter criteria
 */
class UndergroundStateFilter : public StateFilter
{
public:
    /**
     * @brief Construct a new UndergroundStateFilter object
     *
     * @param gender Gender value to filter by
     * @param ability Ability value to filter by
     * @param shiny Shiny value to filter by
     * @param heightMin Minimum height threshold
     * @param heightMax Maximum height threshold
     * @param weightMin Minimum weight threshold
     * @param weightMax Maximum weight threshold
     * @param skip If filters should be skipped
     * @param ivMin Minimum IV thresholds
     * @param ivMax Maximum IV thresholds
     * @param natures Natures to filter by
     * @param powers Hidden powers to filter by
     * @param species Pokemon species to filter by
     */
    UndergroundStateFilter(u8 gender, u8 ability, u8 shiny, u8 heightMin, u8 heightMax, u8 weightMin, u8 weightMax, bool skip, const std::array<u8, 6> &ivMin, const std::array<u8, 6> &ivMax,
                           const std::array<bool, 25> &natures, const std::array<bool, 16> &powers, const std::vector<u16> &species);

    /**
     * @brief Determines if the \p state meets the filter criteria
     *
     * @param state State to compare
     *
     * @return true State passes the filter
     * @return false State does not pass the filter
     */
    bool compareState(const UndergroundState &state) const;

private:
    std::vector<u16> species;
};

#endif // UNDERGROUNDFILTER_HPP
