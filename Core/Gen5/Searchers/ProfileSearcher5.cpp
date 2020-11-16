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

#include "ProfileSearcher5.hpp"
#include <Core/Enum/Game.hpp>
#include <Core/Gen5/Keypresses.hpp>
#include <Core/RNG/LCRNG64.hpp>
#include <Core/RNG/MT.hpp>
#include <Core/RNG/SHA1.hpp>
#include <Core/Util/Utilities.hpp>
#include <QtConcurrent>

ProfileSearcher5::ProfileSearcher5(const QDate &date, const QTime &time, int minSeconds, int maxSeconds, u8 minVCount, u8 maxVCount,
                                   u16 minTimer0, u16 maxTimer0, u8 minGxStat, u8 maxGxStat, bool softReset, Game version,
                                   Language language, DSType dsType, u64 mac, Buttons keypress) :
    date(date),
    time(time),
    minSeconds(minSeconds),
    maxSeconds(maxSeconds),
    minVCount(minVCount),
    maxVCount(maxVCount),
    minTimer0(minTimer0),
    maxTimer0(maxTimer0),
    minGxStat(minGxStat),
    maxGxStat(maxGxStat),
    softReset(softReset),
    version(version),
    language(language),
    dsType(dsType),
    mac(mac),
    keypress(keypress),
    searching(false),
    progress(0)
{
}

void ProfileSearcher5::startSearch(int threads, u8 minVFrame, u8 maxVFrame)
{
    searching = true;
    QThreadPool pool;

    u8 diff = maxVFrame - minVFrame + 1;
    if (diff < threads)
    {
        threads = diff;
    }

    pool.setMaxThreadCount(threads);
    std::vector<QFuture<void>> threadContainer;

    auto split = (diff / threads);
    for (int i = 0; i < threads; i++)
    {
        if (i == threads - 1)
        {
            threadContainer.emplace_back(QtConcurrent::run(&pool, [=] { search(minVFrame, maxVFrame + 1); }));
        }
        else
        {
            threadContainer.emplace_back(QtConcurrent::run(&pool, [=] { search(minVFrame, minVFrame + split); }));
        }
        minVFrame += split;
    }

    for (int i = 0; i < threads; i++)
    {
        threadContainer[i].waitForFinished();
    }
}

void ProfileSearcher5::cancelSearch()
{
    searching = false;
}

std::vector<QList<QStandardItem *>> ProfileSearcher5::getResults()
{
    std::lock_guard<std::mutex> lock(resultMutex);

    auto data(results);
    results.clear();

    return data;
}

int ProfileSearcher5::getProgress() const
{
    return progress;
}

void ProfileSearcher5::search(u8 vframeStart, u8 vframeEnd)
{
    u32 button = Keypresses::getValues({ keypress }).front();
    for (u8 vframe = vframeStart; vframe < vframeEnd; vframe++)
    {
        for (u8 gxStat = minGxStat; gxStat <= maxGxStat; gxStat++)
        {
            SHA1 sha(version, language, dsType, mac, softReset, vframe, gxStat);
            sha.setDate(static_cast<u8>(date.year() - 2000), static_cast<u8>(date.month()), static_cast<u8>(date.day()),
                        static_cast<u8>(date.dayOfWeek()));
            sha.setButton(button);
            for (u16 timer0 = minTimer0; timer0 <= maxTimer0; timer0++)
            {
                for (u8 vcount = minVCount; vcount <= maxVCount; vcount++)
                {
                    sha.setTimer0(timer0, vcount);
                    sha.precompute();

                    for (u8 second = minSeconds; second <= maxSeconds; second++)
                    {
                        sha.setTime(time.hour(), time.minute(), second, dsType);

                        u64 seed = sha.hashSeed();

                        if (valid(seed))
                        {
                            QList<QStandardItem *> items;
                            items.append(new QStandardItem(QString::number(second)));
                            items.append(new QStandardItem(QString::number(vcount, 16)));
                            items.append(new QStandardItem(QString::number(timer0, 16)));
                            items.append(new QStandardItem(QString::number(gxStat, 16)));
                            items.append(new QStandardItem(QString::number(vframe, 16)));
                            items.append(new QStandardItem(QString::number(seed, 16)));

                            std::lock_guard<std::mutex> lock(resultMutex);
                            results.emplace_back(items);
                        }

                        std::lock_guard<std::mutex> lock(progressMutex);
                        progress++;
                    }
                }
            }
        }
    }
}

ProfileIVSearcher5::ProfileIVSearcher5(const std::array<u8, 6> &minIVs, const std::array<u8, 6> &maxIVs, const QDate &date,
                                       const QTime &time, int minSeconds, int maxSeconds, u8 minVCount, u8 maxVCount, u16 minTimer0,
                                       u16 maxTimer0, u8 minGxStat, u8 maxGxStat, bool softReset, Game version, Language language,
                                       DSType dsType, u64 mac, Buttons keypress) :
    ProfileSearcher5(date, time, minSeconds, maxSeconds, minVCount, maxVCount, minTimer0, maxTimer0, minGxStat, maxGxStat, softReset,
                     version, language, dsType, mac, keypress),
    minIVs(minIVs),
    maxIVs(maxIVs),
    offset(version & Game::BW2 ? 2 : 0)
{
}

bool ProfileIVSearcher5::valid(u64 seed)
{
    MTFast<8, true> rng(seed >> 32, offset);

    for (u8 i = 0; i < 6; i++)
    {
        u8 iv = rng.next() >> 27;
        if (iv < minIVs.at(i) || iv > maxIVs.at(i))
        {
            return false;
        }
    }

    return true;
}

ProfileNeedleSearcher5::ProfileNeedleSearcher5(const std::vector<u8> &needles, bool unovaLink, bool memoryLink, const QDate &date,
                                               const QTime &time, int minSeconds, int maxSeconds, u8 minVCount, u8 maxVCount, u16 minTimer0,
                                               u16 maxTimer0, u8 minGxStat, u8 maxGxStat, bool softReset, Game version, Language language,
                                               DSType dsType, u64 mac, Buttons keypress) :
    ProfileSearcher5(date, time, minSeconds, maxSeconds, minVCount, maxVCount, minTimer0, maxTimer0, minGxStat, maxGxStat, softReset,
                     version, language, dsType, mac, keypress),
    needles(needles),
    unovaLink(unovaLink),
    memoryLink(memoryLink),
    game(version & Game::BW)
{
}

bool ProfileNeedleSearcher5::valid(u64 seed)
{
    BWRNG rng(seed);

    u8 advances = game ? Utilities::initialAdvancesBW(seed) : Utilities::initialAdvancesBW2(seed, memoryLink);

    // Opening the menu advances 1
    if (unovaLink)
    {
        advances++;
    }

    rng.advance(advances);

    for (u8 needle : needles)
    {
        u8 rand = rng.nextUInt(8);
        if (rand != needle)
        {
            return false;
        }

        if (unovaLink)
        {
            rng.next();
        }
    }

    return true;
}

ProfileSeedSearcher5::ProfileSeedSearcher5(u64 seed, const QDate &date, const QTime &time, int minSeconds, int maxSeconds, u8 minVCount,
                                           u8 maxVCount, u16 minTimer0, u16 maxTimer0, u8 minGxStat, u8 maxGxStat, bool softReset,
                                           Game version, Language language, DSType dsType, u64 mac, Buttons keypress) :
    ProfileSearcher5(date, time, minSeconds, maxSeconds, minVCount, maxVCount, minTimer0, maxTimer0, minGxStat, maxGxStat, softReset,
                     version, language, dsType, mac, keypress),
    seed(seed)
{
}

bool ProfileSeedSearcher5::valid(u64 seed)
{
    return this->seed == seed;
}
