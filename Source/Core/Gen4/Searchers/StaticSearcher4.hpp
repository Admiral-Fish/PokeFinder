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

#ifndef STATICSEARCHER4_HPP
#define STATICSEARCHER4_HPP

#include <Core/Gen4/Filters/StateFilter4.hpp>
#include <Core/Gen4/Profile4.hpp>
#include <Core/Parents/Searchers/StaticSearcher.hpp>
#include <mutex>

class StaticTemplate4;

/**
 * @brief Static encounter searcher for Gen4
 */
class StaticSearcher4 : public StaticSearcher<Profile4, StateFilter4>
{
public:
    /**
     * @brief Construct a new StaticSearcher4 object
     *
     * @param minAdvance Minimum advances
     * @param maxAdvance Maximum advances
     * @param minDelay Minimum delay
     * @param maxDelay Maximum delay
     * @param method Encounter method
     * @param lead Encounter lead
     * @param profile Profile Information
     * @param filter State filter
     */
    StaticSearcher4(u32 minAdvance, u32 maxAdvance, u32 minDelay, u32 maxDelay, Method method, Lead lead, const Profile4 &profile,
                    const StateFilter4 &filter);

    /**
     * @brief Cancels the running search
     */
    void cancelSearch();

    /**
     * @brief Returns the progress of the running search
     *
     * @return Progress
     */
    int getProgress() const;

    /**
     * @brief Returns the states of the running search
     *
     * @return Vector of computed states
     */
    std::vector<SearcherState4> getResults();

    /**
     * @brief Starts the search
     *
     * @param min Minimum IVs
     * @param max Maximum IVs
     * @param staticTemplate Pokemon template
     */
    void startSearch(const std::array<u8, 6> &min, const std::array<u8, 6> &max, const StaticTemplate4 *staticTemplate);

private:
    std::mutex mutex;
    std::vector<SearcherState4> results;
    int progress;
    u32 maxAdvance;
    u32 minAdvance;
    u32 maxDelay;
    u32 minDelay;
    bool searching;
    u8 buffer;

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
    std::vector<SearcherState4> search(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe, const StaticTemplate4 *staticTemplate) const;

    /**
     * @brief Searches for initial seeds within min/max advances and min/max delay
     *
     * @param results Original results
     *
     * @return Computed results
     */
    std::vector<SearcherState4> searchInitialSeeds(const std::vector<SearcherState4> &results) const;

    /**
     * @brief Searches for matching states from provided IVs via Method 1
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
    std::vector<SearcherState4> searchMethod1(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe, const StaticTemplate4 *staticTemplate) const;

    /**
     * @brief Searches for matching states from provided IVs via Method J
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
    std::vector<SearcherState4> searchMethodJ(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe, const StaticTemplate4 *staticTemplate) const;

    /**
     * @brief Searches for matching states from provided IVs via Method K
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
    std::vector<SearcherState4> searchMethodK(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe, const StaticTemplate4 *staticTemplate) const;
};

#endif // STATICSEARCHER4_HPP
