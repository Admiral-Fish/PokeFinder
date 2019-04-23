/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2019 by Admiral_Fish, bumba, and EzPzStreamz
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

#include <QtConcurrent>
#include "IDSearcher4.hpp"
#include <Core/RNG/MTRNG.hpp>
#include <Core/Util/Utilities.hpp>

ShinyPIDSearcher::ShinyPIDSearcher(u32 pid, bool useTID, u16 tid, u32 year, u32 minDelay, u32 maxDelay, bool infinite)
{
    this->pid = pid;
    this->useTID = useTID;
    this->tid = tid;
    this->year = year;
    this->minDelay = minDelay;
    this->maxDelay = maxDelay;
    this->infinite = infinite;
    searching = false;
    cancel = false;
    progress = 0;

    connect(this, &ShinyPIDSearcher::finished, this, &ShinyPIDSearcher::deleteLater);
    connect(this, &ShinyPIDSearcher::finished, this, [ = ] { emit updateProgress(getResults(), progress); });
}

void ShinyPIDSearcher::startSearch()
{
    if (!searching)
    {
        progress = 0;
        searching = true;
        cancel = false;

        QtConcurrent::run([ = ] { update(); });
        QtConcurrent::run([ = ] { search(); });
    }
}

void ShinyPIDSearcher::cancelSearch()
{
    cancel = true;
}

void ShinyPIDSearcher::search()
{
    maxDelay = infinite ? 0xE8FFFF : maxDelay;
    for (u32 efgh = minDelay; efgh <= maxDelay; efgh++)
    {
        for (u16 ab = 0; ab < 256; ab++)
        {
            for (u8 cd = 0; cd < 24; cd++)
            {
                if (cancel)
                {
                    searching = false;
                    emit finished();
                    return;
                }

                u32 seed = ((ab << 24) | (cd << 16)) + efgh;
                MersenneTwister mt(seed, 1);

                u32 y = mt.nextUInt();

                u16 id = y & 0xFFFF;
                u16 sid = y >> 16;

                if (Utilities::shiny(pid, id, sid) && (!useTID || id == tid))
                {
                    u32 delay = efgh + 2000 - year;
                    auto frame = QList<QStandardItem *>() << new QStandardItem(QString::number(seed, 16).toUpper().rightJustified(8, '0')) << new QStandardItem(QString::number(id))
                                 << new QStandardItem(QString::number(sid)) << new QStandardItem(QString::number(delay)) << new QStandardItem("0");
                    QMutexLocker locker(&mutex);
                    results.append(frame);
                }

                progress++;
            }
        }
    }
    searching = false;
    emit finished();
}

void ShinyPIDSearcher::update()
{
    do
    {
        emit updateProgress(getResults(), progress);
        QThread::sleep(1);
    }
    while (searching);
}

QVector<QList<QStandardItem *>> ShinyPIDSearcher::getResults()
{
    QMutexLocker locker(&mutex);
    auto data(results);
    results.clear();
    return data;
}


TIDSIDSearcher::TIDSIDSearcher(u16 tid, bool useSID, u16 searchSID, u32 year, u32 minDelay, u32 maxDelay, bool infinite)
{
    this->tid = tid;
    this->useSID = useSID;
    this->searchSID = searchSID;
    this->year = year;
    this->minDelay = minDelay;
    this->maxDelay = maxDelay;
    this->infinite = infinite;
    searching = false;
    cancel = false;
    progress = 0;

    connect(this, &TIDSIDSearcher::finished, this, &TIDSIDSearcher::deleteLater);
    connect(this, &TIDSIDSearcher::finished, this, [ = ] { emit updateProgress(getResults(), progress); });
}

void TIDSIDSearcher::startSearch()
{
    if (!searching)
    {
        progress = 0;
        searching = true;
        cancel = false;

        QtConcurrent::run([ = ] { update(); });
        QtConcurrent::run([ = ] { search(); });
    }
}

void TIDSIDSearcher::cancelSearch()
{
    cancel = true;
}

void TIDSIDSearcher::search()
{
    maxDelay = infinite ? 0xE8FFFF : maxDelay;
    for (u32 efgh = minDelay; efgh <= maxDelay; efgh++)
    {
        for (u16 ab = 0; ab < 256; ab++)
        {
            for (u8 cd = 0; cd < 24; cd++)
            {
                if (cancel)
                {
                    searching = false;
                    emit finished();
                    return;
                }

                u32 seed = ((ab << 24) | (cd << 16)) + efgh;
                MersenneTwister mt(seed, 1);

                u32 y = mt.nextUInt();

                u16 id = y & 0xFFFF;
                u16 sid = y >> 16;

                if (id == tid && (!useSID || sid == searchSID))
                {
                    u32 delay = efgh + 2000 - year;
                    auto frame = QList<QStandardItem *>() << new QStandardItem(QString::number(seed, 16).toUpper().rightJustified(8, '0')) << new QStandardItem(QString::number(id))
                                 << new QStandardItem(QString::number(sid)) << new QStandardItem(QString::number(delay)) << new QStandardItem("0");
                    QMutexLocker locker(&mutex);
                    results.append(frame);
                }

                progress++;
            }
        }
    }
    searching = false;
    emit finished();
}

void TIDSIDSearcher::update()
{
    do
    {
        emit updateProgress(getResults(), progress);
        QThread::sleep(1);
    }
    while (searching);
}

QVector<QList<QStandardItem *>> TIDSIDSearcher::getResults()
{
    QMutexLocker locker(&mutex);
    auto data(results);
    results.clear();
    return data;
}
