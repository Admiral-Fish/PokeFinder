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

#ifndef IDFILTER_HPP
#define IDFILTER_HPP

#include <Core/Global.hpp>
#include <vector>

class IDState;
class IDState8;

/**
 * @brief Provides ways to determine if the given \ref IDState meets the given criteria
 */
class IDFilter
{
public:
    /**
     * @brief IDFilter
     *
     * @param tidFilter TID filter list
     * @param sidFilter SID filter list
     * @param tsvFilter TSV filter list
     * @param displayFilter Display ID filter list
     */
    IDFilter(const std::vector<u16> &tidFilter, const std::vector<u16> &sidFilter, const std::vector<u16> &tsvFilter,
             const std::vector<u32> &displayFilter);

    /**
     * @brief Determines if the \p state meets the filter criteria
     *
     * @param state State to compare
     *
     * @return true State passes the filter
     * @return false State does not pass the filter
     */
    bool compareState(const IDState &state) const;

    /**
     * @brief Determines if the \p state meets the filter criteria
     *
     * @param state State to compare
     *
     * @return true State passes the filter
     * @return false State does not pass the filter
     */
    bool compareState(const IDState8 &state) const;

protected:
    std::vector<u32> displayFilter;
    std::vector<u16> sidFilter;
    std::vector<u16> tidFilter;
    std::vector<u16> tsvFilter;
};

#endif // IDFILTER_HPP
