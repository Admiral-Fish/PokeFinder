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

#include "StationarySearcher5.hpp"
#include <Core/Gen5/Keypresses.hpp>
#include <Core/RNG/SHA1.hpp>
#include <QtConcurrent>

StationarySearcher5::StationarySearcher5(const StationaryGenerator5 &ivGenerator, const StationaryGenerator5 &pidGenerator,
                                         const Profile5 &profile, const QVector<QHash<u32, u32>> &ivMap, bool includePID) :
    ivGenerator(ivGenerator),
    pidGenerator(pidGenerator),
    profile(profile),
    ivMap(ivMap),
    includePID(includePID),
    fastSearch(!ivMap.isEmpty()),
    searching(false),
    progress(0)
{
}

void StationarySearcher5::startSearch(int threads, QDate start, const QDate &end)
{
    searching = true;
    QThreadPool pool;

    auto days = start.daysTo(end);
    if (days == 0 || days < threads)
    {
        pool.setMaxThreadCount(1);
        auto future = QtConcurrent::run(&pool, [=] { search(start, end); });
        future.waitForFinished();
    }
    else
    {
        pool.setMaxThreadCount(threads);
        QVector<QFuture<void>> threadContainer;

        auto daysSplit = days / threads;
        for (int i = 0; i < threads; i++)
        {
            if (i == threads - 1)
            {
                threadContainer.append(QtConcurrent::run(&pool, [=] { search(start, end); }));
            }
            else
            {
                QDate mid = start.addDays(daysSplit);
                threadContainer.append(QtConcurrent::run(&pool, [=] { search(start, mid); }));
            }
            start = start.addDays(daysSplit);
        }

        for (int i = 0; i < threads; i++)
        {
            threadContainer[i].waitForFinished();
        }
    }
}

void StationarySearcher5::cancelSearch()
{
    searching = false;
}

QVector<StationaryState5> StationarySearcher5::getResults()
{
    std::lock_guard<std::mutex> lock(resultMutex);

    auto data(results);
    results.clear();

    return data;
}

int StationarySearcher5::getProgress() const
{
    return progress;
}

void StationarySearcher5::search(const QDate &start, const QDate &end)
{
    SHA1 sha(profile);
    auto buttons = Keypresses::getKeyPresses(profile.getKeypresses(), profile.getSkipLR());
    auto values = Keypresses::getValues(buttons);

    for (u16 timer0 = profile.getTimer0Min(); timer0 <= profile.getTimer0Max(); timer0++)
    {
        sha.setTimer0(timer0, profile.getVCount());

        for (QDate date = start; date <= end; date = date.addDays(1))
        {
            sha.setDate(static_cast<u8>(date.year() - 2000), static_cast<u8>(date.month()), static_cast<u8>(date.day()),
                        static_cast<u8>(date.dayOfWeek()));
            sha.precompute();

            for (int i = 0; i < values.size(); i++)
            {
                sha.setButton(values.at(i));

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

                            QVector<StationaryState5> states;
                            if (fastSearch)
                            {
                                for (u8 j = 0; j < ivMap.size(); j++)
                                {
                                    auto it = ivMap.at(j).find(seed >> 32);
                                    if (it != ivMap.at(j).end())
                                    {
                                        StationaryState5 currentState;
                                        currentState.setIVs(it.value());
                                        currentState.calculateHiddenPower();

                                        // Filter here
                                        if (ivGenerator.getFilter().compareIVs(currentState))
                                        {
                                            currentState.setIVCurrentState(j + 1);
                                            states.append(currentState);
                                        }
                                    }
                                }
                            }
                            else
                            {
                                // states = ivGenerator.generate(seed);
                            }

                            for (auto ivCurrentState : states)
                            {
                                // ivCurrentState.setDateTime(QDateTime(date, QTime(hour, minute, second)));
                                // ivCurrentState.setInitialSeed(seed);
                                // ivCurrentState.setButtons(buttons.at(i));
                                // ivCurrentState.setTimer0(timer0);

                                if (includePID)
                                {
                                    auto pidStates = pidGenerator.generate(seed);

                                    for (const auto &pidCurrentState : pidStates)
                                    {
                                        ivCurrentState.setPID(pidCurrentState.getPID());
                                        ivCurrentState.setAbility(pidCurrentState.getAbility());
                                        ivCurrentState.setAdvance(pidCurrentState.getAdvance());
                                        ivCurrentState.setGender(pidCurrentState.getGender());
                                        ivCurrentState.setNature(pidCurrentState.getNature());
                                        ivCurrentState.setShiny(pidCurrentState.getShiny());
                                        ivCurrentState.setSeed(pidCurrentState.getSeed());

                                        std::lock_guard<std::mutex> lock(resultMutex);
                                        results.append(ivCurrentState);
                                    }
                                }
                                else
                                {
                                    std::lock_guard<std::mutex> lock(resultMutex);
                                    results.append(ivCurrentState);
                                }
                            }

                            std::lock_guard<std::mutex> lock(progressMutex);
                            progress++;
                        }
                    }
                }
            }
        }
    }
}
