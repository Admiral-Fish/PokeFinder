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

#ifndef SEARCHERBASE5_HPP
#define SEARCHERBASE5_HPP

#include <Core/Gen5/Keypresses.hpp>
#include <Core/Gen5/Profile5.hpp>
#include <Core/Global.hpp>
#include <Core/Parents/Searchers/SearcherBase.hpp>
#include <Core/RNG/SHA1.hpp>
#include <Core/Util/DateTime.hpp>
#include <Core/Util/Utilities.hpp>
#include <thread>

class Date;
class Profile5;
template <class StateType>
class SearcherState5;

/**
 * @brief Parent searcher class for all Gen 5 generators
 *
 * @tparam Generator Generator class to use
 * @tparam State State class to use
 */
template <class Generator, class State>
class SearcherBase5 : public SearcherBase<SearcherState5<State>>
{
public:
    /**
     * @brief Construct a new SearcherBase5 object
     *
     * @param generator State generator
     * @param profile Profile information
     */
    SearcherBase5(const Generator &generator, const Profile5 &profile) :
        SearcherBase<SearcherState5<State>>(), generator(generator), profile(profile), keypresses(Keypresses::getKeypresses(profile))
    {
    }

    /**
     * @brief Starts the search
     *
     * @param threads Numbers of threads to search with
     * @param start Start date
     * @param end End date
     */
    void startSearch(int threads, const Date &start, const Date &end)
    {
        this->searching = true;

        auto days = start.daysTo(end) + 1;
        if (days < threads)
        {
            threads = days;
        }

        auto *threadContainer = new std::thread[threads];

        auto daysSplit = days / threads;
        Date day = start;
        for (int i = 0; i < threads; i++, day += daysSplit)
        {
            if (i == threads - 1)
            {
                threadContainer[i] = std::thread([=] { search(day, end); });
            }
            else
            {
                Date mid = day + (daysSplit - 1);
                threadContainer[i] = std::thread([=] { search(day, mid); });
            }
        }

        for (int i = 0; i < threads; i++)
        {
            threadContainer[i].join();
        }

        delete[] threadContainer;
    }

protected:
    Generator generator;
    Profile5 profile;
    std::vector<Keypress> keypresses;

    /**
     * @brief Searches between the \p start and \p end dates
     *
     * @param start Start date
     * @param end End date
     */
    virtual void search(const Date &start, const Date &end) = 0;
};

#endif // SEARCHERBASE5_HPP
