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

inline u16 dateToDays(u8 year, u8 month, u8 day)
{
    u16 days = 0;
    for (int i = year - 1; i > 0; i--)
    {
        days += 365;

        if (Date::isLeapYear(i))
        {
            days++;
        }
    }

    for (int i = 1; i < month; i++)
    {
        days += Date::daysInMonth(i, year);
    }

    return days + day;
}

namespace SeedTimeCalculator3
{
    std::vector<SeedTimeState3> calculateTimes(u32 seed, u32 advance, u16 year)
    {
        std::vector<SeedTimeState3> states;

        DateTime start(year, 1, 1);
        DateTime end(year, 12, 31, 23, 59, 59);

        for (; start <= end; start.addSeconds(60))
        {
            auto parts = start.getDate().getParts();
            u16 days = dateToDays(parts.at(0) - 2000, parts.at(1), parts.at(2));

            auto time = start.getTime();
            u8 hour = time.hour();
            u8 minute = time.minute();

            u32 v = 1440 * days + 960 * (hour / 10) + 60 * (hour % 10) + 16 * (minute / 10) + (minute % 10);
            v = (v >> 16) ^ (v & 0xffff);
            if (v == seed)
            {
                states.emplace_back(start, advance);
            }
        }

        return states;
    }
}
