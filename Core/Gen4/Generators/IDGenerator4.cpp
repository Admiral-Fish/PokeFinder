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

#include "IDGenerator4.hpp"
#include <Core/Parents/Filters/IDFilter.hpp>
#include <Core/RNG/MTRNG.hpp>

IDGenerator4::IDGenerator4(u32 minDelay, u32 maxDelay, u16 year, u8 month, u8 day, u8 hour, u8 minute) :
    minDelay(minDelay), maxDelay(maxDelay), year(year), month(month), day(day), hour(hour), minute(minute)
{
}

QVector<IDFrame4> IDGenerator4::generate(const IDFilter &filter)
{
    QVector<IDFrame4> frames;

    for (u8 second = 0; second < 60; second++)
    {
        for (u32 efgh = minDelay; efgh <= maxDelay; efgh++)
        {
            u32 seed = static_cast<u32>(((((month * day) + (minute + second)) & 0xFF) << 24) | (hour << 16)) + efgh;

            MersenneTwister mt(seed);
            mt.advanceFrames(1);

            u32 sidtid = mt.nextUInt();

            u16 tid = sidtid & 0xFFFF;
            u16 sid = sidtid >> 16;

            IDFrame4 frame(seed, tid, sid);

            if (filter.compare(frame))
            {
                frame.setDelay(efgh + 2000 - year);
                frame.setSeconds(second);
                frames.append(frame);
            }
        }
    }

    return frames;
}
