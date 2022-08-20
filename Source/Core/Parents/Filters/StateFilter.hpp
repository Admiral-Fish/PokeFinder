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

#ifndef STATEFILTER_HPP
#define STATEFILTER_HPP

#include <Core/Global.hpp>
#include <array>

/**
 * @brief Provides ways to determine if the given \ref State meets the given criteria
 */
class StateFilter
{
public:
    /**
     * @brief Construct a new StateFilter object
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
    StateFilter(u8 gender, u8 ability, u8 shiny, bool skip, const std::array<u8, 6> &min, const std::array<u8, 6> &max,
                const std::array<bool, 25> &natures, const std::array<bool, 16> &powers);

    /**
     * @brief Determines if the \p ability meets the filter criteria
     *
     * @param ability Ability to compare
     *
     * @return true Ability passes the filter
     * @return false Ability does not pass the filter
     */
    bool compareAbility(u8 ability) const;

    /**
     * @brief Determines if the \p gender meets the filter criteria
     *
     * @param gender Gender to compare
     *
     * @return true Gender passes the filter
     * @return false Gender does not pass the filter
     */
    bool compareGender(u8 gender) const;

    /**
     * @brief Determines if the \p hiddenPower meets the filter criteria
     *
     * @param hiddenPower Hidden power to compare
     *
     * @return true Hidden power passes the filter
     * @return false Gender does not pass the filter
     */
    bool compareHiddenPower(u8 hiddenPower) const;

    /**
     * @brief Determines if the \p ivs meet the filter criteria
     *
     * @param ivs IVs to compare
     *
     * @return true IVs pass the filter
     * @return false IVs do not pass the filter
     */
    bool compareIV(std::array<u8, 6> ivs) const;

    /**
     * @brief Determines if the \p nature meets the filter criteria
     *
     * @param nature Nature to compare
     *
     * @return true Nature passes the filter
     * @return false Nature does not pass the filter
     */
    bool compareNature(u8 nature) const;

    /**
     * @brief Determines if the \p shiny meets the filter criteria
     *
     * @param shiny Shiny to compare
     *
     * @return true Shiny passes the filter
     * @return false Shiny does not pass the filter
     */
    bool compareShiny(u8 shiny) const;

protected:
    std::array<bool, 25> natures;
    std::array<bool, 16> powers;
    std::array<u8, 6> max;
    std::array<u8, 6> min;
    bool skip;
    u8 ability;
    u8 gender;
    u8 shiny;
};

#endif // STATEFILTER_HPP
