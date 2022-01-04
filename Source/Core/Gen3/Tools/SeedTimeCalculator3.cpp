/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2022 by Admiral_Fish, bumba, and EzPzStreamz
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
    std::vector<SeedTimeState3> calculateTimes(u32 seed, u32 advance, u16 year)
    {
        std::vector<SeedTimeState3> states;

        constexpr Date start;
        Date date(year, 1, 1);
        Date end(year, 12, 31);

        for (; date <= end; date = date.addDays(1))
        {
            // The if statement considering the year is to handle a bug the game has
            // If the year is greater then 2000 it does not count the days in that year
            u16 days = start.daysTo(date) - (year > 2000 ? 366 : 0) + 1;

            for (u8 hour = 0; hour < 24; hour++)
            {
                for (u8 minute = 0; minute < 60; minute++)
                {
                    u32 v = 1440 * days + 960 * (hour / 10) + 60 * (hour % 10) + 16 * (minute / 10) + (minute % 10);
                    v = (v >> 16) ^ (v & 0xffff);
                    if (v == seed)
                    {
                        states.emplace_back(DateTime(date, Time(hour, minute, 0)), advance);
                    }
                }
            }
        }

        return states;
    }
}
