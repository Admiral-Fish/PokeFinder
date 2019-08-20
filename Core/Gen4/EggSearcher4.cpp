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
#include "EggSearcher4.hpp"

EggSearcher4::EggSearcher4(const Egg4 &generatorIV, const Egg4 &generatorPID, const FrameCompare &compare, u32 minDelay, u32 maxDelay, int type)
{
    this->generatorIV = generatorIV;
    this->generatorPID = generatorPID;
    this->compare = compare;
    this->minDelay = minDelay;
    this->maxDelay = maxDelay;
    this->type = type;
    searching = false;
    cancel = false;
    progress = 0;

    connect(this, &EggSearcher4::finished, this, [ = ]
    {
        searching = false;
        emit updateProgress(getResults(), progress);
        QTimer::singleShot(1000, this, &EggSearcher4::deleteLater);
    });
}

void EggSearcher4::startSearch()
{
    if (!searching)
    {
        progress = 0;
        searching = true;
        cancel = false;

        auto *timer = new QTimer(this);
        connect(this, &EggSearcher4::finished, timer, &QTimer::stop);
        connect(timer, &QTimer::timeout, this, [ = ] { emit updateProgress(getResults(), progress); });
        timer->start(1000);

        QtConcurrent::run([ = ] { search(); });
    }
}

void EggSearcher4::cancelSearch()
{
    cancel = true;
}

void EggSearcher4::search()
{
    u16 total = 0;

    for (u16 ab = 0; ab < 256; ab++)
    {
        for (u8 cd = 0; cd < 24; cd++)
        {
            for (u32 efgh = minDelay; efgh <= maxDelay; efgh++)
            {
                if (cancel)
                {
                    emit finished();
                    return;
                }

                if (total > 10000)
                {
                    progress = static_cast<int>(256 * 24 * (maxDelay - minDelay + 1));
                    emit finished();
                    return;
                }

                QVector<Frame4> frames;
                u32 seed = ((ab << 24) | (cd << 16)) + efgh;

                if (type == 0)
                {
                    generatorIV.setSeed(seed);
                    frames = generatorIV.generate(compare);
                }
                else if (type == 1)
                {
                    generatorPID.setSeed(seed);
                    frames = generatorPID.generate(compare);
                }
                else
                {
                    generatorIV.setSeed(seed);
                    generatorPID.setSeed(seed);

                    auto framesIV = generatorIV.generate(compare);
                    auto framesPID = generatorPID.generate(compare);

                    if (!framesIV.isEmpty() && !framesPID.isEmpty())
                    {
                        for (auto framePID : framesPID)
                        {
                            for (const auto &frameIV : framesIV)
                            {
                                framePID.setIVs(frameIV.getIV(0), frameIV.getIV(1), frameIV.getIV(2),
                                                frameIV.getIV(3), frameIV.getIV(4), frameIV.getIV(5));
                                for (u8 i = 0; i < 6; i++)
                                {
                                    framePID.setInheritance(i, frameIV.getInheritance(i));
                                }
                                framePID.setEggFrame(frameIV.getFrame());

                                frames.append(framePID);
                            }
                        }
                    }
                }

                total += frames.size();

                QMutexLocker locker(&mutex);
                results.append(frames);
                progress++;
            }
        }
    }
    searching = false;
    emit finished();
}

QVector<Frame4> EggSearcher4::getResults()
{
    QMutexLocker lock(&mutex);
    auto data(results);
    results.clear();
    return data;
}
