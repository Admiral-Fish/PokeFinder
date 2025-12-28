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

#ifndef STATICSEARCHER3_HPP
#define STATICSEARCHER3_HPP

#include <Core/Gen3/Profile3.hpp>
#include <Core/Parents/Filters/StateFilter.hpp>
#include <Core/Parents/Searchers/StaticSearcher.hpp>

class StaticTemplate3;

/**
 * @brief Static encounter searcher for Gen3
 */
class StaticSearcher3 : public StaticSearcher<Profile3, StateFilter, SearcherState>
{
public:
    /**
     * @brief Construct a new StaticSearcher3 object
     *
     * @param method Encounter method
     * @param profile Profile Information
     * @param filter State filter
     */
    StaticSearcher3(Method method, const Profile3 &profile, const StateFilter &filter);

    /**
     * @brief Starts the search
     *
     * @param min Minimum IVs
     * @param max Maximum IVs
     * @param staticTemplate Pokemon template
     */
    void startSearch(const std::array<u8, 6> &min, const std::array<u8, 6> &max, const StaticTemplate3 *staticTemplate);

private:
    bool ivAdvance;

    /**
     * @brief Searches for matching states from provided IVs
     *
     * @param hp HP IV
     * @param atk Atk IV
     * @param def Def IV
     * @param spa SpA IV
     * @param spd SpD IV
     * @param spe Spe IV
     * @param staticTemplate Pokemon template
     *
     * @return Vector of computed states
     */
    std::vector<SearcherState> search(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe, const StaticTemplate3 *staticTemplate) const;
};

#endif // STATICSEARCHER3_HPP
