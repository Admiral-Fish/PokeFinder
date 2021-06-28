/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2021 by Admiral_Fish, bumba, and EzPzStreamz
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

void RTCSearcher::startSearch(u32 initialSeed, u32 targetSeed, u32 initialAdvances, u32 maxAdvances, const Date &end, bool box, bool ageto)
{
    searching = true;
    if (box)
    {
        for (Date date; date < end; date = date.addDays(1))
        {
            for (u8 hour = 0; hour < 24; hour++)
            {
                for (u8 minute = 0; minute < 60; minute++)
                {
                    for (u8 second = 0; second < 60; second++, initialSeed += 0x269FB20)
                    {
                        if (initialSeed == targetSeed)
                        {
                            std::lock_guard<std::mutex> guard(mutex);
                            results.emplace_back(DateTime(date, Time(hour, minute, second)), initialSeed, 0);
                        }
                    }
                }
            }
        }
    }
    else if (ageto)
    {
        XDRNGR back(targetSeed);
        back.advance(29278 + 1018);

        targetSeed = back.getSeed();

        for (Date date; date < end; date = date.addDays(1))
        {
            for (u8 hour = 0; hour < 24; hour++)
            {
                for (u8 minute = 0; minute < 60; minute++)
                {
                    for (u8 second = 0; second < 60; second++, initialSeed += 40500000)
                    {
                        XDRNG rng(initialSeed);

                        for (u32 x = 0; x < maxAdvances; x += 2792, rng.advance(2792, true))
                        {
                            if (!searching)
                            {
                                return;
                            }

                            if (rng.getSeed() == targetSeed)
                            {
                                std::lock_guard<std::mutex> guard(mutex);
                                results.emplace_back(DateTime(date, Time(hour, minute, second)), initialSeed, x + 29278 + 1018);
                            }
                        }
                    }
                }
            }
        }
    }
    else
    {
        XDRNGR back(targetSeed);
        back.advance(initialAdvances);

        targetSeed = back.getSeed();

        for (Date date; date < end; date = date.addDays(1))
        {
            for (u8 hour = 0; hour < 24; hour++)
            {
                for (u8 minute = 0; minute < 60; minute++)
                {
                    for (u8 second = 0; second < 60; second++, initialSeed += 40500000)
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
                                std::lock_guard<std::mutex> guard(mutex);
                                results.emplace_back(DateTime(date, Time(hour, minute, second)), initialSeed, x + 1 + initialAdvances);
                            }
                        }
                    }
                }
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
    auto data = std::move(results);
    return data;
}
