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

#ifndef DATETIME_HPP
#define DATETIME_HPP

#include <Core/Util/Global.hpp>
#include <array>
#include <string>

// Julian date bound between Jan 1, 2000 (2451545) and Dec 31, 2099 (2488069)
class Date
{
public:
    // Default to Jan 1, 2000
    constexpr Date(int jd = 2451545) : jd(jd)
    {
    }
    Date(int year, int month, int day);
    Date addDays(int days) const;
    int daysTo(const Date &other) const;
    int year() const;
    int month() const;
    int day() const;
    int dayOfWeek() const;
    std::array<int, 3> getParts() const;
    static int daysInMonth(int month, int year);
    std::string toString() const;

    constexpr bool operator==(const Date &other) const
    {
        return jd == other.jd;
    }

    constexpr bool operator<=(const Date &other) const
    {
        return jd <= other.jd;
    }

    constexpr bool operator<(const Date &other) const
    {
        return jd < other.jd;
    }

    constexpr bool operator>(const Date &other) const
    {
        return jd > other.jd;
    }

private:
    int jd;

    friend class DateTime;
};

class Time
{
public:
    constexpr Time(int seconds = 0) : md(seconds)
    {
    }
    constexpr Time(int hour, int minute, int second) : md(hour * 3600 + minute * 60 + second)
    {
    }
    int addSeconds(int seconds);
    int hour() const;
    int minute() const;
    int second() const;
    std::string toString() const;

    constexpr bool operator==(const Time &other) const
    {
        return md == other.md;
    }

    constexpr bool operator<(const Time &other) const
    {
        return md < other.md;
    }

    constexpr bool operator<=(const Time &other) const
    {
        return md <= other.md;
    }

    constexpr bool operator>(const Time &other) const
    {
        return md > other.md;
    }

private:
    int md;

    friend class DateTime;
};

class DateTime
{
public:
    constexpr DateTime() : date(Date()), time(Time())
    {
    }
    constexpr DateTime(const Date &date, const Time &time) : date(date), time(time)
    {
    }
    constexpr DateTime(int jd, int seconds) : date(Date(jd)), time(Time(seconds))
    {
    }
    DateTime(int year, int month, int day, int hour = 0, int minute = 0, int second = 0);
    void addSeconds(int seconds);
    DateTime addSecs(int seconds);
    Date getDate() const;
    Time getTime() const;
    std::string toString() const;

    constexpr bool operator==(const DateTime &other) const
    {
        return date == other.date && time == other.time;
    }

    constexpr bool operator<=(const DateTime &other) const
    {
        return date < other.date || (date == other.date && time <= other.time);
    }

    constexpr bool operator<(const DateTime &other) const
    {
        return date < other.date || (date == other.date && time < other.time);
    }

    constexpr bool operator>(const DateTime &other) const
    {
        return date > other.date || (date == other.date && time > other.time);
    }

private:
    Date date;
    Time time;
};

#endif // DATETIME_HPP
