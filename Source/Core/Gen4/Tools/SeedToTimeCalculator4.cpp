/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2024 by Admiral_Fish, bumba, and EzPzStreamz
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

#include "SeedToTimeCalculator4.hpp"

namespace SeedToTimeCalculator4
{
    std::vector<SeedTime4> calculateTimes(u32 seed, u16 year, bool forceSecond, u8 forcedSecond)
    {
        u8 ab = seed >> 24;
        u8 cd = (seed >> 16) & 0xFF;
        u32 efgh = seed & 0xFFFF;

        // Allow overflow seeds by setting hour to 23 and adjusting for delay
        u8 hour = cd > 23 ? 23 : cd;
        u32 delay = cd > 23 ? (efgh + (2000 - year)) + ((cd - 23) * 0x10000) : efgh + (2000 - year);

        std::vector<SeedTime4> results;
        for (u8 month = 1; month <= 12; month++)
        {
            u8 maxDays = Date::daysInMonth(year, month);
            for (u8 day = 1; day <= maxDays; day++)
            {
                for (u8 minute = 0; minute < 60; minute++)
                {
                    for (u8 second = 0; second < 60; second++)
                    {
                        if (ab == ((month * day + minute + second) & 0xFF))
                        {
                            if (!forceSecond || second == forcedSecond)
                            {
                                results.emplace_back(DateTime(year, month, day, hour, minute, second), delay);
                            }
                        }
                    }
                }
            }
        }
        return results;
    }

    std::vector<SeedTimeCalibrate4> calibrate(int minusDelay, int plusDelay, int minusSecond, int plusSecond, const SeedTime4 &target)
    {
        DateTime time = target.getDateTime();
        u32 delay = target.getDelay();

        std::vector<SeedTimeCalibrate4> results;
        results.reserve((plusDelay - minusDelay + 1) * (plusSecond - minusSecond + 1));
        for (int secondOffset = minusSecond; secondOffset <= plusSecond; secondOffset++)
        {
            DateTime offset = time.addSeconds(secondOffset);
            for (int delayOffset = minusDelay; delayOffset <= plusDelay; delayOffset++)
            {
                results.emplace_back(offset, delay + delayOffset);
            }
        }

        return results;
    }

    std::vector<SeedTimeCalibrate4> calibrate(int minusDelay, int plusDelay, int minusSecond, int plusSecond,
                                              const std::array<bool, 3> &roamers, std::array<u8, 3> &routes, const SeedTime4 &target)
    {
        DateTime time = target.getDateTime();
        u32 delay = target.getDelay();

        std::vector<SeedTimeCalibrate4> results;
        results.reserve((plusDelay - minusDelay + 1) * (plusSecond - minusSecond + 1));
        for (int secondOffset = minusSecond; secondOffset <= plusSecond; secondOffset++)
        {
            DateTime offset = time.addSeconds(secondOffset);
            for (int delayOffset = minusDelay; delayOffset <= plusDelay; delayOffset++)
            {
                results.emplace_back(offset, delay + delayOffset, roamers, routes);
            }
        }

        return results;
    }
}
