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

#ifndef WILDSEARCHER3_HPP
#define WILDSEARCHER3_HPP

#include <Core/Gen3/EncounterArea3.hpp>
#include <Core/Gen3/Filters/StateFilter3.hpp>
#include <Core/Gen3/Profile3.hpp>
#include <Core/Parents/Searchers/WildSearcher.hpp>
#include <mutex>

/**
 * @brief Wild encounter searcher for Gen3
 */
class WildSearcher3 : public WildSearcher<EncounterArea3, Profile3, WildStateFilter3>
{
public:
    /**
     * @brief Construct a new WildSearcher3 object
     *
     * @param method Encounter method
     * @param encounter Encounter type
     * @param lead Encounter lead
     * @param profile Profile Information
     * @param filter State filter
     */
    WildSearcher3(Method method, Encounter encounter, Lead lead, const EncounterArea3 &encounterArea, const Profile3 &profile,
                  const WildStateFilter3 &filter);

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
    std::vector<WildSearcherState3> getResults();

    /**
     * @brief Starts the search
     *
     * @param min Minimum IVs
     * @param max Maximum IVs
     */
    void startSearch(const std::array<u8, 6> &min, const std::array<u8, 6> &max);

private:
    std::mutex mutex;
    std::vector<u8> modifiedSlots;
    std::vector<WildSearcherState3> results;
    int progress;
    bool ivAdvance;
    bool searching;

    /**
     * @brief Searches for matching states from provided IVs
     *
     * @param hp HP IV
     * @param atk Atk IV
     * @param def Def IV
     * @param spa SpA IV
     * @param spd SpD IV
     * @param spe Spe IV
     * @param safari Whether the encounter location is the Safari Zone in RSE
     *
     * @return Vector of computed states
     */
    std::vector<WildSearcherState3> search(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe, bool safari) const;
};

#endif // WILDSEARCHER3_HPP
