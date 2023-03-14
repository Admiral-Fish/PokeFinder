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

#ifndef SEEDSEARCHER_HPP
#define SEEDSEARCHER_HPP

#include <Core/Global.hpp>
#include <atomic>
#include <mutex>
#include <vector>

template <class Criteria>
class SeedSearcher
{
public:
    /**
     * @brief Construct a new SeedSearcher object
     *
     * @param criteria Filtering data
     */
    SeedSearcher(const Criteria &criteria) : progress(0), criteria(criteria), searching(false)
    {
    }

    /**
     * @brief Cancels the running search
     */
    void cancelSearch()
    {
        searching = false;
    }

    /**
     * @brief Returns the progress of the running search
     *
     * @return Progress
     */
    virtual int getProgress() const
    {
        return progress;
    }

    /**
     * @brief Returns the states of the running search
     *
     * @return Vector of computed states
     */
    std::vector<u32> getResults() const
    {
        return results;
    }

protected:
    std::mutex mutex;
    std::vector<u32> results;
    std::atomic<u32> progress;
    Criteria criteria;
    bool searching;
};

#endif // SEEDSEARCHER_HPP
