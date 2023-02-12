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
static inline bool isLeapYear(int year)
{
    // Since we know the years are bound between 2000 and 2099, we can simplify this check
    // return ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0));
    return (year % 4) == 0;
}

Date::Date(int year, int month, int day)
{
    int a = month < 3 ? 1 : 0;
    int y = year + 4800 - a;
    int m = month + 12 * a - 3;
    jd = day + ((153 * m + 2) / 5) - 32045 + 365 * y + (y / 4) - (y / 100) + (y / 400);
}

Date Date::addDays(int days) const
{
    return Date(jd + days);
}

int Date::day() const
{
    return getParts().day;
}

int Date::daysInMonth(int month, int year)
{
    if (month == 2 && isLeapYear(year))
    {
        return 29;
    }
    return monthDays[month - 1];
}

int Date::dayOfWeek() const
{
    return (jd + 1) % 7;
}

int Date::daysTo(const Date &other) const
{
    return other.jd - jd;
}

DateParts Date::getParts() const
{
    int a = jd + 32044;
    int b = (4 * a + 3) / 146097;
    int c = a - (146097 * b) / 4;

    int d = (4 * c + 3) / 1461;
    int e = c - (1461 * d) / 4;
    int m = (5 * e + 2) / 153;

    int year = 100 * b + d - 4800 + (m / 10);
    int month = m + 3 - 12 * (m / 10);
    int day = e - ((153 * m + 2) / 5) + 1;

    return { year, month, day };
}

int Date::month() const
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

    return std::string(buf, sizeof(buf));
}

int Date::year() const
{
    return getParts().year;
}

int Time::addSeconds(int seconds)
{
    md += seconds;

    int days = 0;
    while (md >= 86400)
    {
        md -= 86400;
        days++;
    }
    return days;
}

int Time::hour() const
{
    return md / 3600;
}

int Time::minute() const
{
    return (md % 3600) / 60;
}

int Time::second() const
{
    return md % 60;
}

std::string Time::toString() const
{
    char buf[9] = "  :  :  ";

    std::memcpy(buf, numbers[hour()], 2);
    std::memcpy(buf + 3, numbers[minute()], 2);
    std::memcpy(buf + 6, numbers[second()], 2);

    return std::string(buf, sizeof(buf));
}

DateTime::DateTime(int year, int month, int day, int hour, int minute, int second) : date(year, month, day), time(hour, minute, second)
{
}

void DateTime::addSeconds(int seconds)
{
    int days = time.addSeconds(seconds);
    date = date.addDays(days);
}

DateTime DateTime::addSecs(int seconds)
{
    DateTime dt(*this);
    dt.addSeconds(seconds);
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

    return std::string(buf, sizeof(buf));
}
