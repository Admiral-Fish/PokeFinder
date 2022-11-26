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

#ifndef SEARCHER5_HPP
#define SEARCHER5_HPP

#include <Core/Enum/Game.hpp>
#include <Core/Gen5/Keypresses.hpp>
#include <Core/Gen5/Profile5.hpp>
#include <Core/Global.hpp>
#include <Core/RNG/SHA1.hpp>
#include <Core/Util/DateTime.hpp>
#include <Core/Util/Utilities.hpp>
#include <atomic>
#include <mutex>
#include <thread>
#include <vector>

class Date;
template <class StateType>
class SearcherState5;

/**
 * @brief Parent searcher class for most Gen 5 generators
 *
 * @tparam Generator Generator class to use
 * @tparam State State class to use
 */
template <class Generator, class State>
class Searcher5
{
public:
    /**
     * @brief Construct a new DreamRadarSearcher object
     *
     * @param profile Profile information
     */
    Searcher5(const Profile5 &profile) : profile(profile), progress(0), searching(false)
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
    int getProgress() const
    {
        return progress;
    }

    /**
     * @brief Returns the states of the running search
     *
     * @return Vector of computed states
     */
    std::vector<SearcherState5<State>> getResults()
    {
        std::lock_guard<std::mutex> lock(mutex);
        auto data = std::move(results);
        return data;
    }

    /**
     * @brief Starts the search
     *
     * @param generator State generator
     * @param threads Numbers of threads to search with
     * @param start Start date
     * @param end End date
     */
    void startSearch(const Generator &generator, int threads, const Date &start, const Date &end)
    {
        searching = true;

        auto days = start.daysTo(end) + 1;
        if (days < threads)
        {
            threads = days;
        }

        std::vector<std::thread> threadContainer;

        auto daysSplit = days / threads;
        Date day = start;
        for (int i = 0; i < threads; i++)
        {
            if (i == threads - 1)
            {
                threadContainer.emplace_back([=] { search(generator, day, end); });
            }
            else
            {
                Date mid = day.addDays(daysSplit - 1);
                threadContainer.emplace_back([=] { search(generator, day, mid); });
            }
            day = day.addDays(daysSplit);
        }

        for (int i = 0; i < threads; i++)
        {
            threadContainer[i].join();
        }
    }

private:
    Profile5 profile;
    std::mutex mutex;
    std::vector<SearcherState5<State>> results;
    std::atomic<int> progress;
    bool searching;

    /**
     * @brief Searches between the \p start and \p end dates
     *
     * @param generator State generator
     * @param start Start date
     * @param end End date
     */
    void search(Generator generator, const Date &start, const Date &end)
    {
        bool flag = (profile.getVersion() & Game::BW) != Game::None;

        SHA1 sha(profile);
        auto buttons = Keypresses::getKeyPresses(profile.getKeypresses(), profile.getSkipLR());
        auto values = Keypresses::getValues(buttons);

        for (u16 timer0 = profile.getTimer0Min(); timer0 <= profile.getTimer0Max(); timer0++)
        {
            sha.setTimer0(timer0, profile.getVCount());
            for (Date date = start; date <= end; date = date.addDays(1))
            {
                sha.setDate(date);
                sha.precompute();
                for (size_t i = 0; i < values.size(); i++)
                {
                    sha.setButton(values[i]);

                    for (u8 hour = 0; hour < 24; hour++)
                    {
                        for (u8 minute = 0; minute < 60; minute++)
                        {
                            for (u8 second = 0; second < 60; second++)
                            {
                                if (!searching)
                                {
                                    return;
                                }

                                sha.setTime(hour, minute, second, profile.getDSType());
                                u64 seed = sha.hashSeed();

                                u32 advances = flag ? Utilities5::initialAdvancesBW(seed)
                                                    : Utilities5::initialAdvancesBW2(seed, profile.getMemoryLink());
                                auto states = generator.generate(seed);

                                if (!states.empty())
                                {
                                    std::vector<SearcherState5<State>> displayStates;
                                    displayStates.reserve(states.size());

                                    DateTime dt(date, Time(hour, minute, second));
                                    for (const auto &state : states)
                                    {
                                        displayStates.emplace_back(dt, seed, advances, buttons[i], timer0, state);
                                    }

                                    std::lock_guard<std::mutex> lock(mutex);
                                    results.insert(results.end(), displayStates.begin(), displayStates.end());
                                }
                            }
                        }
                    }
                    progress++;
                }
            }
        }
    }
};

#endif // SEARCHER5_HPP
