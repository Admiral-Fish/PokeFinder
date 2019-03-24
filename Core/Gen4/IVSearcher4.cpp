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

#include "IVSearcher4.hpp"

IVSearcher4::IVSearcher4(const Searcher4 &searcher, const QVector<u8> &min, const QVector<u8> &max)
{
    this->searcher = searcher;
    this->min = min;
    this->max = max;
    cancel = false;
    progress = 0;

    connect(this, &IVSearcher4::finished, this, &IVSearcher4::deleteLater);
}

void IVSearcher4::run()
{
    for (u8 a = min[0]; a <= max[0]; a++)
    {
        for (u8 b = min[1]; b <= max[1]; b++)
        {
            for (u8 c = min[2]; c <= max[2]; c++)
            {
                for (u8 d = min[3]; d <= max[3]; d++)
                {
                    for (u8 e = min[4]; e <= max[4]; e++)
                    {
                        for (u8 f = min[5]; f <= max[5]; f++)
                        {
                            if (cancel)
                            {
                                return;
                            }

                            auto frames = searcher.search(a, b, c, d, e, f);
                            progress++;

                            QMutexLocker locker(&mutex);
                            results.append(frames);
                        }
                    }
                }
            }
        }
    }
}

int IVSearcher4::currentProgress() const
{
    return progress;
}

QVector<Frame4> IVSearcher4::getResults()
{
    QMutexLocker locker(&mutex);
    auto data(results);
    results.clear();
    return data;
}

void IVSearcher4::cancelSearch()
{
    cancel = true;
}
