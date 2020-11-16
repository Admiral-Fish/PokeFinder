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

#include "RTCSearcher.hpp"
#include <Core/RNG/LCRNG.hpp>

RTCSearcher::RTCSearcher() : searching(false)
{
}

void RTCSearcher::startSearch(u32 initialSeed, u32 targetSeed, u32 initialAdvances, u32 maxAdvances, const QDateTime &end)
{
    searching = true;

    XDRNGR back(targetSeed);
    back.advance(initialAdvances);

    targetSeed = back.getSeed();

    QDateTime time(QDate(2000, 1, 1), QTime(0, 0));
    for (; time < end; time = time.addSecs(1), initialSeed += 40500000)
    {
        XDRNG rng(initialSeed);

        for (u32 x = 0; x < maxAdvances; x++)
        {
            if (!searching)
            {
                return;
            }

            if (rng.next() == targetSeed)
            {
                GameCubeRTCState result(time, initialSeed, x + 1 + initialAdvances);

                std::lock_guard<std::mutex> guard(mutex);
                results.push_back(result);
            }
        }
    }
}

void RTCSearcher::cancelSearch()
{
    searching = false;
}

std::vector<GameCubeRTCState> RTCSearcher::getResults()
{
    std::lock_guard<std::mutex> guard(mutex);
    auto data(results);
    results.clear();
    return data;
}
