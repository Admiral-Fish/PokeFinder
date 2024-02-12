/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2023 by Admiral_Fish, bumba, and EzPzStreamz
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

#include "DateTime.hpp"
#include <array>
#include <cstring>

consteval std::array<char[2], 100> computeNumbers()
{
    std::array<char[2], 100> strings;

    for (char i = 0; i < strings.size(); i++)
    {
        if (i < 10)
        {
            strings[i][0] = '0';
            strings[i][1] = i + '0';
        }
        else
        {
            strings[i][0] = (i / 10) + '0';
            strings[i][1] = (i % 10) + '0';
        }
    }

    return strings;
}

constexpr std::array<char[2], 100> numbers = computeNumbers();
constexpr u8 monthDays[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

/**
 * @brief Determines if the year is a leap year.
 *
 * @param year Year to check
 *
 * @return true Year is a leap year
 * @return false Year is not a leap year
 */
static inline bool isLeapYear(u16 year)
{
    // Since we know the years are bound between 2000 and 2099, we can simplify this check
    // return ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0));
    return (year % 4) == 0;
}

Date::Date(u16 year, u8 month, u8 day)
{
    u32 a = month < 3 ? 1 : 0;
    u32 y = year + 4800 - a;
    u32 m = month + 12 * a - 3;
    jd = day + ((153 * m + 2) / 5) - 32045 + 365 * y + (y / 4) - (y / 100) + (y / 400);
}

u8 Date::day() const
{
    return getParts().day;
}

u8 Date::daysInMonth(u16 year, u8 month)
{
    if (month == 2 && isLeapYear(year))
    {
        return 29;
    }
    return monthDays[month - 1];
}

u8 Date::dayOfWeek() const
{
    return (jd + 1) % 7;
}

u32 Date::daysTo(const Date &other) const
{
    return other.jd - jd;
}

DateParts Date::getParts() const
{
    u32 a = jd + 32044;
    u32 b = (4 * a + 3) / 146097;
    u32 c = a - (146097 * b) / 4;

    u32 d = (4 * c + 3) / 1461;
    u32 e = c - (1461 * d) / 4;
    u32 m = (5 * e + 2) / 153;

    u16 year = 100 * b + d - 4800 + (m / 10);
    u8 month = m + 3 - 12 * (m / 10);
    u8 day = e - ((153 * m + 2) / 5) + 1;

    return { year, month, day };
}

u8 Date::month() const
{
    return getParts().month;
}

std::string Date::toString() const
{
    char buf[11] = "20  -  -  ";
    auto parts = getParts();

    std::memcpy(buf + 2, numbers[parts.year - 2000], 2);
    std::memcpy(buf + 5, numbers[parts.month], 2);
    std::memcpy(buf + 8, numbers[parts.day], 2);

    return std::string(buf, sizeof(buf) - 1);
}

u16 Date::year() const
{
    return getParts().year;
}

u32 Time::addSeconds(u32 seconds)
{
    md += seconds;

    u32 days = 0;
    while (md >= 86400)
    {
        md -= 86400;
        days++;
    }
    return days;
}

u8 Time::hour() const
{
    return md / 3600;
}

u8 Time::minute() const
{
    return (md % 3600) / 60;
}

u8 Time::second() const
{
    return md % 60;
}

std::string Time::toString() const
{
    char buf[9] = "  :  :  ";

    std::memcpy(buf, numbers[hour()], 2);
    std::memcpy(buf + 3, numbers[minute()], 2);
    std::memcpy(buf + 6, numbers[second()], 2);

    return std::string(buf, sizeof(buf) - 1);
}

DateTime::DateTime(u16 year, u8 month, u8 day, u8 hour, u8 minute, u8 second) : date(year, month, day), time(hour, minute, second)
{
}

DateTime DateTime::addSeconds(u32 seconds) const
{
    DateTime dt(*this);

    u32 days = dt.time.addSeconds(seconds);
    dt.date += days;

    return dt;
}

Date DateTime::getDate() const
{
    return date;
}

Time DateTime::getTime() const
{
    return time;
}

std::string DateTime::toString() const
{
    char buf[20] = "20  -  -     :  :  ";
    auto parts = date.getParts();

    std::memcpy(buf + 2, numbers[parts.year - 2000], 2);
    std::memcpy(buf + 5, numbers[parts.month], 2);
    std::memcpy(buf + 8, numbers[parts.day], 2);
    std::memcpy(buf + 11, numbers[time.hour()], 2);
    std::memcpy(buf + 14, numbers[time.minute()], 2);
    std::memcpy(buf + 17, numbers[time.second()], 2);

    return std::string(buf, sizeof(buf) - 1);
}
