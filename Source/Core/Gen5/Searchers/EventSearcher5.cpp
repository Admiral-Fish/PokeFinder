/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2020 by Admiral_Fish, bumba, and EzPzStreamz
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

#include "EventSearcher5.hpp"
#include <Core/Enum/Game.hpp>
#include <Core/Gen5/Keypresses.hpp>
#include <Core/RNG/SHA1.hpp>
#include <Core/Util/Utilities.hpp>
#include <future>

EventSearcher5::EventSearcher5(const Profile5 &profile) : profile(profile), searching(false), progress(0)
{
}

void EventSearcher5::startSearch(const EventGenerator5 &generator, int threads, Date start, const Date &end)
{
    searching = true;

    auto days = start.daysTo(end) + 1;
    if (days < threads)
    {
        threads = days;
    }

    if (threads <= 0)
    {
        threads = 1;
    }

    std::vector<std::future<void>> threadContainer;

    auto daysSplit = days / threads;
    for (int i = 0; i < threads; i++)
    {
        if (i == threads - 1)
        {
            threadContainer.emplace_back(std::async([=] { search(generator, start, end); }));
        }
        else
        {
            Date mid = start.addDays(daysSplit - 1);
            threadContainer.emplace_back(std::async([=] { search(generator, start, mid); }));
        }
        start = start.addDays(daysSplit);
    }

    for (int i = 0; i < threads; i++)
    {
        threadContainer[i].wait();
    }
}

void EventSearcher5::cancelSearch()
{
    searching = false;
}

std::vector<SearcherState5<State>> EventSearcher5::getResults()
{
    std::lock_guard<std::mutex> lock(resultMutex);

    auto data(results);
    results.clear();

    return data;
}

int EventSearcher5::getProgress() const
{
    return progress;
}

void EventSearcher5::search(EventGenerator5 generator, const Date &start, const Date &end)
{
    bool flag = profile.getVersion() & Game::BW;

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

                            generator.setInitialAdvances(flag ? Utilities::initialAdvancesBW(seed)
                                                              : Utilities::initialAdvancesBW2(seed, profile.getMemoryLink()));
                            auto states = generator.generate(seed);

                            if (!states.empty())
                            {
                                std::vector<SearcherState5<State>> displayStates;
                                displayStates.reserve(states.size());

                                DateTime dt(date, Time(hour, minute, second));
                                for (const auto &state : states)
                                {
                                    displayStates.emplace_back(dt, seed, buttons[i], timer0, state);
                                }

                                std::lock_guard<std::mutex> lock(resultMutex);
                                results.insert(results.end(), displayStates.begin(), displayStates.end());
                            }
                        }
                    }
                }

                std::lock_guard<std::mutex> lock(progressMutex);
                progress++;
            }
        }
    }
}
