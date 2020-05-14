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
#include <Core/RNG/MTRNG.hpp>
#include <Core/RNG/SHA1.hpp>
#include <QtConcurrent>

ProfileSearcher5::ProfileSearcher5(const QVector<u8> &minIVs, const QVector<u8> &maxIVs, const QDate &date, const QTime &time,
                                   int minSeconds, int maxSeconds, u8 minVCount, u8 maxVCount, u16 minTimer0, u16 maxTimer0, u8 minGxStat,
                                   u8 maxGxStat, bool softReset, Game version, Language language, DSType dsType, u64 mac, u16 keypress) :
    minIVs(minIVs),
    maxIVs(maxIVs),
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
    offset((version & Game::BW2 ? 2 : 0)),
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
    QVector<QFuture<void>> threadContainer;

    auto split = (diff / threads);
    for (int i = 0; i < threads; i++)
    {
        if (i == threads - 1)
        {
            threadContainer.append(QtConcurrent::run(&pool, [=] { search(minVFrame, maxVFrame + 1); }));
        }
        else
        {
            threadContainer.append(QtConcurrent::run(&pool, [=] { search(minVFrame, minVFrame + split); }));
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

QVector<QList<QStandardItem *>> ProfileSearcher5::getResults()
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
    u32 button = Keypresses::getValues({ keypress }).first();
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

                        MersenneTwisterFast rng(6 + offset, seed >> 32);
                        rng.advanceFrames(offset);

                        bool flag = true;
                        for (u8 i = 0; i < 6; i++)
                        {
                            u8 iv = rng.nextUInt() >> 27;
                            if (iv < minIVs.at(i) || iv > maxIVs.at(i))
                            {
                                flag = false;
                                break;
                            }
                        }

                        if (flag)
                        {
                            QList<QStandardItem *> items;
                            items.append(new QStandardItem(QString::number(second)));
                            items.append(new QStandardItem(QString::number(vcount, 16)));
                            items.append(new QStandardItem(QString::number(timer0, 16)));
                            items.append(new QStandardItem(QString::number(gxStat, 16)));
                            items.append(new QStandardItem(QString::number(vframe, 16)));
                            items.append(new QStandardItem(QString::number(seed, 16)));

                            std::lock_guard<std::mutex> lock(resultMutex);
                            results.append(items);
                        }

                        std::lock_guard<std::mutex> lock(progressMutex);
                        progress++;
                    }
                }
            }
        }
    }
}
