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

#ifndef IDSEARCHER5_HPP
#define IDSEARCHER5_HPP

#include <Core/Gen5/Profile5.hpp>
#include <Core/Global.hpp>
#include <atomic>
#include <mutex>
#include <vector>

class Date;
class IDGenerator5;
class IDState;
template <class StateType>
class SearcherState5;

/**
 * @brief TID/SID searcher for Gen5
 */
class IDSearcher5
{
public:
    /**
     * @brief Construct a new IDSearcher5 object
     *
     * @param profile Profile information
     * @param pid PID to search for
     * @param checkPID Whether to check for PID
     * @param checkXOR Whether to check for PID XORed
     */
    IDSearcher5(const Profile5 &profile, u32 pid, bool checkPID, bool checkXOR);

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
    std::vector<SearcherState5<IDState>> getResults();

    /**
     * @brief Starts the search
     *
     * @param generator State generator
     * @param threads Threads to search with
     * @param start Start date
     * @param end End date
     */
    void startSearch(const IDGenerator5 &generator, int threads, const Date &start, const Date &end);

    /**
     * @brief Starts the search
     *
     * @param generator State generator
     * @param date Date to search
     * @param hour Hour to search
     * @param minute Minute to search
     * @param minSecond Minimum second
     * @param maxSecond Maximum second
     *
     * @return Vector of computed states
     */
    std::vector<SearcherState5<IDState>> startSearch(IDGenerator5 generator, const Date &date, u8 hour, u8 minute, u8 minSecond,
                                                     u8 maxSecond);

private:
    Profile5 profile;
    std::mutex mutex;
    std::vector<SearcherState5<IDState>> results;
    std::atomic<int> progress;
    u32 pid;
    bool checkPID;
    bool checkXOR;
    bool searching;

    /**
     * @brief Searches between \p start and \p end dates
     *
     * @param generator State generator
     * @param start Start date
     * @param end End date
     */
    void search(IDGenerator5 generator, const Date &start, const Date &end);
};

#endif // IDSEARCHER5_HPP
