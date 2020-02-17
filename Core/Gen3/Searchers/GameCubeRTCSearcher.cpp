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

#include "GameCubeRTCSearcher.hpp"
#include <Core/RNG/LCRNG.hpp>
#include <QDateTime>

static const QDateTime date(QDate(2000, 1, 1), QTime(0, 0));

GameCubeRTCSearcher::GameCubeRTCSearcher() : searching(false)
{
}

void GameCubeRTCSearcher::startSearch(u32 initialSeed, u32 targetSeed, u32 initialFrame, u32 maxResults)
{
    searching = true;

    XDRNGR back(targetSeed);
    back.advanceFrames(initialFrame);

    targetSeed = back.getSeed();

    u32 seconds = 0;
    u32 minutes = 0;
    u8 secoundCount = 0;

    while (searching)
    {
        XDRNG rng(initialSeed);

        for (u32 x = 0; x < maxResults; x++)
        {
            if (rng.nextUInt() == targetSeed)
            {
                QDateTime finalTime = date.addSecs(seconds);

                GameCubeRTCFrame result(finalTime, initialSeed, x + 2 + initialFrame);
                results.append(result);

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
}

void GameCubeRTCSearcher::cancelSearch()
{
    searching = false;
}

QVector<GameCubeRTCFrame> GameCubeRTCSearcher::getResults()
{
    return results;
}
