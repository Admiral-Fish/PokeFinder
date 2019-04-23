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
#include "IVSearcher4.hpp"

IVSearcher4::IVSearcher4(const Searcher4 &searcher, const QVector<u8> &min, const QVector<u8> &max)
{
    this->searcher = searcher;
    this->min = min;
    this->max = max;
    searching = false;
    cancel = false;
    progress = 0;

    connect(this, &IVSearcher4::finished, this, &IVSearcher4::deleteLater);
    connect(this, &IVSearcher4::finished, this, [ = ] { emit updateProgress(getResults(), progress); });
}

void IVSearcher4::startSearch()
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

void IVSearcher4::cancelSearch()
{
    cancel = true;
}

void IVSearcher4::search()
{
    for (u8 a = min.at(0); a <= max.at(0); a++)
    {
        for (u8 b = min.at(1); b <= max.at(1); b++)
        {
            for (u8 c = min.at(2); c <= max.at(2); c++)
            {
                for (u8 d = min.at(3); d <= max.at(3); d++)
                {
                    for (u8 e = min.at(4); e <= max.at(4); e++)
                    {
                        for (u8 f = min.at(5); f <= max.at(5); f++)
                        {
                            if (cancel)
                            {
                                searching = false;
                                emit finished();
                                return;
                            }

                            auto frames = searcher.search(a, b, c, d, e, f);

                            QMutexLocker locker(&mutex);
                            results.append(frames);
                            progress++;
                        }
                    }
                }
            }
        }
    }
    searching = false;
    emit finished();
}

void IVSearcher4::update()
{
    do
    {
        emit updateProgress(getResults(), progress);
        QThread::sleep(1);
    }
    while (searching);
}

QVector<Frame4> IVSearcher4::getResults()
{
    QMutexLocker locker(&mutex);
    auto data(results);
    results.clear();
    return data;
}
