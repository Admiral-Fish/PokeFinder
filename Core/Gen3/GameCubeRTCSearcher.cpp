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
#include "GameCubeRTCSearcher.hpp"
#include <Core/RNG/LCRNG.hpp>

namespace PokeFinderCore
{

    GameCubeRTCSearcher::GameCubeRTCSearcher(u32 initialSeed, u32 targetSeed, u32 minFrame, u32 maxFrame)
    {
        this->initialSeed = initialSeed;
        this->targetSeed = targetSeed;
        this->minFrame = minFrame;
        this->maxFrame = maxFrame;
        searching = false;
        cancel = false;

        connect(this, &GameCubeRTCSearcher::finished, this, [ = ]
        {
            searching = false;
            deleteLater();
        });
    }

    void GameCubeRTCSearcher::startSearch()
    {
        if (!searching)
        {
            searching = true;
            cancel = false;

            QtConcurrent::run([ = ] { search(); });
        }
    }

    void GameCubeRTCSearcher::cancelSearch()
    {
        cancel = true;
    }

    void GameCubeRTCSearcher::search()
    {
        XDRNGR back(targetSeed, minFrame);
        targetSeed = back.getSeed();

        u32 seconds = 0;
        u32 minutes = 0;
        u8 secoundCount = 0;

        while (!cancel)
        {
            XDRNG rng(initialSeed);

            for (u32 x = 0; x < maxFrame; x++)
            {
                if (rng.nextUInt() == targetSeed)
                {
                    QDateTime finalTime = date.addSecs(seconds);
                    QList<QStandardItem *> row;
                    QString time = finalTime.toString(Qt::SystemLocaleShortDate);
                    row << (time.contains("M") ? new QStandardItem(time.insert((time.indexOf('M') - 2), ":" + QString::number(finalTime.time().second()))) : new QStandardItem(time.append(":" + QString::number(finalTime.time().second()))))
                        << new QStandardItem(QString::number(x + 2 + minFrame)) << new QStandardItem(QString::number(initialSeed, 16).toUpper());

                    emit result(row);
                    emit finished();
                    return;
                }
            }

            initialSeed += 40500000;
            seconds++;
            secoundCount++;

            if (secoundCount == 60)
            {
                minutes++;
                secoundCount = 0;
            }
        }
        emit finished();
    }

}
