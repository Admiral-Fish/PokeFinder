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

#ifndef STATEFILTER3_HPP
#define STATEFILTER3_HPP

#include <Core/Parents/Filters/StateFilter.hpp>

class EggGeneratorState4;
class GeneratorState4;
class SearcherState4;
class WildGeneratorState4;
class WildSearcherState4;

/**
 * @brief Checks if static encounters match the filter criteria
 */
class StateFilter4 : public StateFilter
{
public:
    /**
     * @brief Construct a new StateFilter4 object
     *
     * @param gender Gender value to filter by
     * @param ability Ability value to filter by
     * @param shiny Shiny value to filter by
     * @param skip If filters should be skipped
     * @param min Minimum IV thresholds
     * @param max Maximum IV thresholds
     * @param natures Natures to filter by
     * @param powers Hidden powers to filter by
     */
    StateFilter4(u8 gender, u8 ability, u8 shiny, bool skip, const std::array<u8, 6> &min, const std::array<u8, 6> &max,
                 const std::array<bool, 25> &natures, const std::array<bool, 16> &powers);

    /**
     * @brief Determines if the \p state meets the filter criteria
     *
     * @param state State to compare
     *
     * @return true State passes the filter
     * @return false State does not pass the filter
     */
    bool compareState(const EggGeneratorState4 &state) const;

    /**
     * @brief Determines if the \p state meets the filter criteria
     *
     * @param state State to compare
     *
     * @return true State passes the filter
     * @return false State does not pass the filter
     */
    bool compareState(const GeneratorState4 &state) const;

    /**
     * @brief Determines if the \p state meets the filter criteria
     *
     * @param state State to compare
     *
     * @return true State passes the filter
     * @return false State does not pass the filter
     */
    bool compareState(const SearcherState4 &state) const;
};

/**
 * @brief Checks if wild encounters match the filter criteria
 */
class WildStateFilter4 : public WildStateFilter
{
public:
    /**
     * @brief Construct a new StateFilter4 object
     *
     * @param gender Gender value to filter by
     * @param ability Ability value to filter by
     * @param shiny Shiny value to filter by
     * @param skip If filters should be skipped
     * @param min Minimum IV thresholds
     * @param max Maximum IV thresholds
     * @param natures Natures to filter by
     * @param powers Hidden powers to filter by
     */
    WildStateFilter4(u8 gender, u8 ability, u8 shiny, bool skip, const std::array<u8, 6> &min, const std::array<u8, 6> &max,
                     const std::array<bool, 25> &natures, const std::array<bool, 16> &powers, const std::array<bool, 12> &encounterSlots);

    /**
     * @brief Determines if the \p state meets the filter criteria
     *
     * @param state State to compare
     *
     * @return true State passes the filter
     * @return false State does not pass the filter
     */
    bool compareState(const WildGeneratorState4 &state) const;

    /**
     * @brief Determines if the \p state meets the filter criteria
     *
     * @param state State to compare
     *
     * @return true State passes the filter
     * @return false State does not pass the filter
     */
    bool compareState(const WildSearcherState4 &state) const;
};

#endif // STATEFILTER3_HPP
