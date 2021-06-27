/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2021 by Admiral_Fish, bumba, and EzPzStreamz
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

#include "StationarySearcher5.hpp"
#include <Core/Gen5/Keypresses.hpp>
#include <Core/RNG/SHA1.hpp>
#include <future>

StationarySearcher5::StationarySearcher5(const StationaryGenerator5 &ivGenerator, const StationaryGenerator5 &pidGenerator,
                                         const Profile5 &profile, const std::vector<std::unordered_map<u32, u32>> &ivMap, bool includePID) :
    ivGenerator(ivGenerator),
    pidGenerator(pidGenerator),
    profile(profile),
    ivMap(ivMap),
    includePID(includePID),
    fastSearch(!ivMap.empty()),
    searching(false),
    progress(0)
{
}

void StationarySearcher5::startSearch(int threads, Date start, const Date &end)
{
    searching = true;

    auto days = start.daysTo(end) + 1;
    if (days < threads)
    {
        threads = days;
    }

    std::vector<std::future<void>> threadContainer;

    auto daysSplit = days / threads;
    for (int i = 0; i < threads; i++)
    {
        if (i == threads - 1)
        {
            threadContainer.emplace_back(std::async(std::launch::async, [=] { search(start, end); }));
        }
        else
        {
            Date mid = start.addDays(daysSplit - 1);
            threadContainer.emplace_back(std::async(std::launch::async, [=] { search(start, mid); }));
        }
        start = start.addDays(daysSplit);
    }

    for (int i = 0; i < threads; i++)
    {
        threadContainer[i].wait();
    }
}

void StationarySearcher5::cancelSearch()
{
    searching = false;
}

std::vector<StationaryState5> StationarySearcher5::getResults()
{
    std::lock_guard<std::mutex> lock(mutex);
    auto data = std::move(results);
    return data;
}

int StationarySearcher5::getProgress() const
{
    return progress;
}

void StationarySearcher5::search(const Date &start, const Date &end)
{
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

                            std::vector<StationaryState5> states;
                            if (fastSearch)
                            {
                                for (size_t j = 0; j < ivMap.size(); j++)
                                {
                                    auto it = ivMap[j].find(seed >> 32);
                                    if (it != ivMap[j].end())
                                    {
                                        StationaryState5 state;
                                        state.setIVs(it->second);
                                        state.calculateHiddenPower();

                                        // Filter here
                                        if (ivGenerator.getFilter().compareIVs(state))
                                        {
                                            state.setIVState(j + 1);
                                            states.emplace_back(state);
                                        }
                                    }
                                }
                            }
                            else
                            {
                                // states = ivGenerator.generate(seed);
                            }

                            for (auto ivState : states)
                            {
                                // ivState.setDateTime(DateTime(date, Time(hour, minute, second)));
                                // ivState.setInitialSeed(seed);
                                // ivState.setButtons(buttons[i]);
                                // ivState.setTimer0(timer0);

                                if (includePID)
                                {
                                    auto pidStates = pidGenerator.generate(seed);

                                    for (const auto &pidState : pidStates)
                                    {
                                        ivState.setPID(pidState.getPID());
                                        ivState.setAbility(pidState.getAbility());
                                        ivState.setAdvances(pidState.getAdvances());
                                        ivState.setGender(pidState.getGender());
                                        ivState.setNature(pidState.getNature());
                                        ivState.setShiny(pidState.getShiny());
                                        ivState.setSeed(pidState.getSeed());

                                        std::lock_guard<std::mutex> lock(mutex);
                                        results.emplace_back(ivState);
                                    }
                                }
                                else
                                {
                                    std::lock_guard<std::mutex> lock(mutex);
                                    results.emplace_back(ivState);
                                }
                            }

                            progress++;
                        }
                    }
                }
            }
        }
    }
}
