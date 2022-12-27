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

#include "IDSearcher5.hpp"
#include <Core/Gen5/Generators/IDGenerator5.hpp>
#include <Core/Gen5/Keypresses.hpp>
#include <Core/Gen5/States/SearcherState5.hpp>
#include <Core/Parents/States/IDState.hpp>
#include <Core/RNG/SHA1.hpp>
#include <Core/Util/DateTime.hpp>
#include <Core/Util/Utilities.hpp>
#include <thread>

IDSearcher5::IDSearcher5(const Profile5 &profile, u32 pid, bool checkPID, bool checkXOR) :
    profile(profile), progress(0), pid(pid), checkPID(checkPID), checkXOR(checkXOR), searching(false)
{
}

void IDSearcher5::startSearch(const IDGenerator5 &generator, int threads, const Date &start, const Date &end)
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

std::vector<SearcherState5<IDState>> IDSearcher5::startSearch(const IDGenerator5 &generator, const Date &date, u8 hour, u8 minute,
                                                              u8 minSecond, u8 maxSecond)
{
    SHA1 sha(profile);
    auto buttons = Keypresses::getKeyPresses(profile);
    auto values = Keypresses::getValues(buttons);

    // IDs only uses minimum Timer0
    sha.setTimer0(profile.getTimer0Min(), profile.getVCount());

    sha.setDate(date);
    auto alpha = sha.precompute();
    for (size_t i = 0; i < values.size(); i++)
    {
        sha.setButton(values[i]);
        for (u8 second = minSecond; second < maxSecond; second++)
        {
            sha.setTime(hour, minute, second, profile.getDSType());
            u64 seed = sha.hashSeed(alpha);

            auto states = generator.generate(seed, pid, checkPID, checkXOR);
            if (!states.empty())
            {
                std::vector<SearcherState5<IDState>> displayStates;
                displayStates.reserve(states.size());

                DateTime dt(date, Time(hour, minute, second));
                for (auto &state : states)
                {
                    displayStates.emplace_back(dt, seed, buttons[i], profile.getTimer0Min(), state);
                }

                results.insert(results.end(), displayStates.begin(), displayStates.end());
            }
        }
    }
    return results;
}

void IDSearcher5::cancelSearch()
{
    searching = false;
}

std::vector<SearcherState5<IDState>> IDSearcher5::getResults()
{
    std::lock_guard<std::mutex> lock(mutex);
    auto data = std::move(results);
    return data;
}

int IDSearcher5::getProgress() const
{
    return progress;
}

void IDSearcher5::search(const IDGenerator5 &generator, const Date &start, const Date &end)
{
    SHA1 sha(profile);
    auto buttons = Keypresses::getKeyPresses(profile);
    auto values = Keypresses::getValues(buttons);

    // IDs only uses minimum Timer0
    sha.setTimer0(profile.getTimer0Min(), profile.getVCount());

    for (Date date = start; date <= end; date = date.addDays(1))
    {
        sha.setDate(date);
        auto alpha = sha.precompute();
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
                        u64 seed = sha.hashSeed(alpha);

                        auto states = generator.generate(seed, pid, checkPID, checkXOR);
                        if (!states.empty())
                        {
                            std::vector<SearcherState5<IDState>> displayStates;
                            displayStates.reserve(states.size());

                            DateTime dt(date, Time(hour, minute, second));
                            for (auto &state : states)
                            {
                                displayStates.emplace_back(dt, seed, buttons[i], profile.getTimer0Min(), state);
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
