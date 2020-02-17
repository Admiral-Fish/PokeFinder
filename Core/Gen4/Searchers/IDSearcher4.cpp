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

#include "IDSearcher4.hpp"
#include <Core/RNG/MTRNG.hpp>

IDSearcher4::IDSearcher4(const IDFilter &filter) : filter(filter), searching(false), progress(0)
{
}

void IDSearcher4::startSearch(bool infinite, u16 year, u32 minDelay, u32 maxDelay)
{
    searching = true;
    maxDelay = infinite ? 0xe8ffff : maxDelay;

    for (u32 efgh = minDelay; efgh <= maxDelay; efgh++)
    {
        for (u16 ab = 0; ab < 256; ab++)
        {
            for (u16 cd = 0; cd < 24; cd++)
            {
                if (!searching)
                {
                    return;
                }

                u32 seed = static_cast<u32>((ab << 24) | (cd << 16)) + efgh;
                MersenneTwister mt(seed);
                mt.advanceFrames(1);

                u32 sidtid = mt.nextUInt();

                u16 tid = sidtid & 0xffff;
                u16 sid = sidtid >> 16;

                IDFrame4 frame(seed, tid, sid);
                if (filter.compare(frame))
                {
                    frame.setDelay(efgh + 2000 - year);

                    std::lock_guard<std::mutex> guard(mutex);
                    results.append(frame);
                }

                progress++;
            }
        }
    }
}

void IDSearcher4::cancelSearch()
{
    searching = false;
}

QVector<IDFrame4> IDSearcher4::getResults()
{
    std::lock_guard<std::mutex> guard(mutex);
    auto data(results);
    results.clear();
    return data;
}

int IDSearcher4::getProgress() const
{
    return progress;
}
