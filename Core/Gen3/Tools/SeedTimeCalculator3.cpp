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

#include "SeedTimeCalculator3.hpp"

namespace SeedTimeCalculator3
{
    QVector<SeedTimeFrame3> calculateTimes(u32 seed, u32 frame, u16 year)
    {
        QVector<SeedTimeFrame3> frames;

        u32 minDay = 0;
        u32 maxDay = 0;

        // For whatever reason the start date is different if the year is greater then 2000
        QDateTime start(QDate(year == 2000 ? 2000 : 2001, 1, 1), QTime(0, 0));

        // Game decides to ignore a year of counting days
        for (u16 x = 2001; x < year; x++)
        {
            QDate temp(x, 1, 1);
            minDay += static_cast<u32>(temp.daysInYear());
            maxDay += static_cast<u32>(temp.daysInYear());
        }

        // Loop through the year generating seeds to check against user input
        for (u8 month = 1; month < 13; month++)
        {
            QDate temp(2000, month, 1);
            maxDay += static_cast<u32>(temp.daysInMonth());
            for (u32 day = minDay; day < maxDay; day++)
            {
                for (u32 hour = 0; hour < 24; hour++)
                {
                    for (u32 minute = 0; minute < 60; minute++)
                    {
                        // Formula to generate intial seed
                        u32 v = 1440 * day + 960 * (hour / 10) + 60 * (hour % 10) + 16 * (minute / 10) + (minute % 10) + 0x5A0;
                        v = (v >> 16) ^ (v & 0xFFFF);

                        if (v == seed)
                        {
                            QDateTime finalTime = start.addDays(day).addSecs((hour * 60 * 60) + (minute * 60));

                            SeedTimeFrame3 result(finalTime, frame);
                            frames.append(result);
                        }
                    }
                }
            }
            minDay += static_cast<u32>(temp.daysInMonth());
        }

        return frames;
    }
}
