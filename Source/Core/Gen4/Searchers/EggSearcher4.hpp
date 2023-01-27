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

#ifndef EGGSEARCHER4_HPP
#define EGGSEARCHER4_HPP

#include <Core/Gen4/Filters/StateFilter4.hpp>
#include <Core/Gen4/Profile4.hpp>
#include <Core/Parents/Searchers/Searcher.hpp>
#include <mutex>

class EggGenerator4;
class EggSearcherState4;

/**
 * @brief Egg encounter searcher for Gen4
 */
class EggSearcher4 : public Searcher<Profile4, StateFilter4>
{
public:
    /**
     * @brief Construct a new EggSearcher4 object
     *
     * @param minDelay Minimum delay
     * @param maxDelay Maximum delay
     * @param profile Profile Information
     * @param filter State filter
     */
    EggSearcher4(u32 minDelay, u32 maxDelay, const Profile4 &profile, const StateFilter4 &filter);

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
    std::vector<EggSearcherState4> getResults();

    /**
     * @brief Starts the search
     *
     * @param generator Egg generator
     */
    void startSearch(const EggGenerator4 &generator);

private:
    std::mutex mutex;
    std::vector<EggSearcherState4> results;
    int progress;
    u32 maxDelay;
    u32 minDelay;
    bool searching;
};

#endif // EGGSEARCHER4_HPP
