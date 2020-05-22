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
#include <QtConcurrent>

EventSearcher5::EventSearcher5(const EventGenerator5 &generator, const Profile5 &profile) :
    generator(generator),
    profile(profile),
    searching(false),
    progress(0)
{
}

void EventSearcher5::startSearch(int threads, QDate start, const QDate &end)
{
    searching = true;
    QThreadPool pool;

    auto days = start.daysTo(end) + 1;
    if (days < threads)
    {
        threads = days;
    }

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

void EventSearcher5::cancelSearch()
{
    searching = false;
}

QVector<EventFrame5> EventSearcher5::getResults()
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

void EventSearcher5::search(const QDate &start, const QDate &end)
{
    bool flag = profile.getVersion() & Game::BW;

    SHA1 sha(profile);
    auto buttons = Keypresses::getKeyPresses(profile.getKeypresses(), profile.getSkipLR());
    auto values = Keypresses::getValues(buttons);

    for (u16 timer0 = profile.getTimer0Min(); timer0 <= profile.getTimer0Max(); timer0++)
    {
        sha.setTimer0(timer0, profile.getVCount());
        for (int i = 0; i < values.size(); i++)
        {
            sha.setButton(values.at(i));

            for (QDate date = start; date <= end; date = date.addDays(1))
            {
                sha.setDate(static_cast<u8>(date.year() - 2000), static_cast<u8>(date.month()), static_cast<u8>(date.day()),
                            static_cast<u8>(date.dayOfWeek()));
                sha.precompute();

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

                            generator.setInitialFrame(flag ? Utilities::initialFrameBW(seed)
                                                           : Utilities::initialFrameBW2(seed, profile.getMemoryLink()));
                            auto frames = generator.generate(seed);

                            QVector<EventFrame5> displayFrames;
                            displayFrames.reserve(frames.size());

                            QDateTime dt(date, QTime(hour, minute, second));
                            for (const auto &frame : frames)
                            {
                                EventFrame5 display(dt, seed, buttons.at(i), timer0, frame);
                                displayFrames.append(display);
                            }

                            {
                                std::lock_guard<std::mutex> lock(resultMutex);
                                results.append(displayFrames);
                            }

                            {
                                std::lock_guard<std::mutex> lock(progressMutex);
                                progress++;
                            }
                        }
                    }
                }
            }
        }
    }
}
