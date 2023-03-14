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

#ifndef DATETIME_HPP
#define DATETIME_HPP

#include <Core/Global.hpp>
#include <string>

struct DateParts
{
    u16 year;
    u8 month;
    u8 day;
};

/**
 * @brief Provides a date library based on the Julian calendar
 * Bound between January 1, 2000 and Dec 31, 2099 as those are the only valid dates on the DS family of consoles.
 */
class Date
{
public:
    /**
     * @brief Construct a new Date object
     * Defaults to January 1, 2000
     *
     * @param jd Initial julian date
     */
    constexpr Date(u32 jd = 2451545) : jd(jd)
    {
    }

    /**
     * @brief Construct a new Date object
     *
     * @param year Initial year
     * @param month Initial month
     * @param day Initial day
     */
    Date(u16 year, u8 month, u8 day);

    /**
     * @brief Calculates a new date by adding a number of days
     *
     * @param days Days to add
     *
     * @return New calculated date
     */
    Date addDays(u32 days) const;

    /**
     * @brief Converts the Julian date to a Gregorian date and returns the day
     *
     * @return Day of the date
     */
    u8 day() const;

    /**
     * @brief Calculates how many days are in the month for a given year
     *
     * @param month Month to calculate for
     * @param year Year to calculate for
     *
     * @return How many days are in the month/year
     */
    static u8 daysInMonth(u16 year, u8 month);

    /**
     * @brief Calculates the Gregorian day of the week
     *
     * @return Day of week
     * 0: Sunday
     * 1: Monday
     * 2: Tuesday
     * 3: Wednesday
     * 4: Thursday
     * 5: Friday
     * 6: Saturday
     */
    u8 dayOfWeek() const;

    /**
     * @brief Computes the number of days between two dates
     *
     * @param other Date to compare too
     *
     * @return Day difference
     */
    u32 daysTo(const Date &other) const;

    /**
     * @brief Converts the Julian date to a Gregorian date
     *
     * @return Year, month, and day
     */
    DateParts getParts() const;

    /**
     * @brief Converts the Julian date to a Gregorian date and returns the month
     *
     * @return Month of the date
     */
    u8 month() const;

    /**
     * @brief Converts the date to a ISO 8601 string
     *
     * @return Date string
     */
    std::string toString() const;

    /**
     * @brief Converts the Julian date to a Gregorian date and returns the year
     *
     * @return Year of the date
     */
    u16 year() const;

    /**
     * @brief Checks if two dates are equal
     *
     * @param other Date to compare
     *
     * @return true Dates are equal
     * @return false Dates are not equal
     */
    constexpr bool operator==(const Date &other) const
    {
        return jd == other.jd;
    }

    /**
     * @brief Checks if this date is less than or equal to the other date
     *
     * @param other Date to compare
     *
     * @return true This date is less than or equal to \p other
     * @return false This date is not less than or equal to \p other
     */
    constexpr bool operator<=(const Date &other) const
    {
        return jd <= other.jd;
    }

    /**
     * @brief Checks if this date is less than to the other date
     *
     * @param other Date to compare
     *
     * @return true This date is less than \p other
     * @return false This date is not less than \p other
     */
    constexpr bool operator<(const Date &other) const
    {
        return jd < other.jd;
    }

    /**
     * @brief Checks if this date is greater than to the other date
     *
     * @param other Date to compare
     *
     * @return true This date is greater than \p other
     * @return false This date is not greater than \p other
     */
    constexpr bool operator>(const Date &other) const
    {
        return jd > other.jd;
    }

    /**
     * @brief Postfix operator that increments the day by 1
     *
     * @return Day value before increment
     */
    constexpr Date operator++(int)
    {
        Date old = *this;
        jd++;
        return old;
    }

private:
    u32 jd;

    friend class DateTime;
};

/**
 * @brief Provides a time library to track hour/minute/second
 */
class Time
{
public:
    /**
     * @brief Construct a new Time object
     *
     * @param seconds Initial seconds
     */
    constexpr Time(u32 seconds = 0) : md(seconds)
    {
    }

    /**
     * @brief Construct a new Time object
     *
     * @param hour Initialize hour
     * @param minute Initial minute
     * @param second Initial second
     */
    constexpr Time(u8 hour, u8 minute, u8 second) : md(hour * 3600 + minute * 60 + second)
    {
    }

    /**
     * @brief Adds a number of seconds to the Time
     *
     * @param seconds Seconds to add
     *
     * @return How many days passed
     */
    u32 addSeconds(u32 seconds);

    /**
     * @brief Computes hour field
     *
     * @return Hour of the time
     */
    u8 hour() const;

    /**
     * @brief Computes minute field
     *
     * @return Minute of the time
     */
    u8 minute() const;

    /**
     * @brief Computes second field
     *
     * @return Second of the time
     */
    u8 second() const;

    /**
     * @brief Converts the time to a string
     *
     * @return Time string
     */
    std::string toString() const;

    /**
     * @brief Checks if two times are equal
     *
     * @param other Time to compare
     *
     * @return true Times are equal
     * @return false Times are not equal
     */
    constexpr bool operator==(const Time &other) const
    {
        return md == other.md;
    }

    /**
     * @brief Checks if this time is less than or equal to the other time
     *
     * @param other Time to compare
     *
     * @return true Time is less than or equal to \p other
     * @return false Time is not less than or equal to \p other
     */
    constexpr bool operator<=(const Time &other) const
    {
        return md <= other.md;
    }

    /**
     * @brief Checks if this time is less than to the other time
     *
     * @param other Time to compare
     *
     * @return true This time is less than \p other
     * @return false This time is not less than \p other
     */
    constexpr bool operator<(const Time &other) const
    {
        return md < other.md;
    }

    /**
     * @brief Checks if this time is greater than to the other time
     *
     * @param other Time to compare
     *
     * @return true This time is greater than \p other
     * @return false This time is not greater than \p other
     */
    constexpr bool operator>(const Time &other) const
    {
        return md > other.md;
    }

private:
    u32 md;

    friend class DateTime;
};

/**
 * @brief Provides a date/time library based on the Julian calendar
 */
class DateTime
{
public:
    /**
     * @brief Construct a new DateTime object
     */
    constexpr DateTime() = default;

    /**
     * @brief Construct a new DateTime object
     *
     * @param date Initial Date
     * @param time Initial Time
     */
    constexpr DateTime(const Date &date, const Time &time) : date(date), time(time)
    {
    }

    /**
     * @brief Construct a new DateTime object
     *
     * @param jd Initial julian day
     * @param seconds Initial seconds for time
     */
    constexpr DateTime(u32 jd, u32 seconds) : date(Date(jd)), time(Time(seconds))
    {
    }

    /**
     * @brief Construct a new DateTime object
     *
     * @param year Initial year
     * @param month Initial month
     * @param day Initial day
     * @param hour Initial hour
     * @param minute Initial minute
     * @param second Initial second
     */
    DateTime(u16 year, u8 month, u8 day, u8 hour = 0, u8 minute = 0, u8 second = 0);

    /**
     * @brief Adds a number of seconds to the DateTime
     * This also internally handles updating the \ref Date if the \ref Time goes over 86400
     *
     * @param seconds Seconds to add
     */
    void addSeconds(u32 seconds);

    /**
     * @brief Adds a number of seconds to the DateTime
     *
     * @param seconds Seconds to add
     *
     * @return Computed DateTime
     */
    DateTime addSecs(u32 seconds) const;

    /**
     * @brief Returns the \ref Date component
     *
     * @return Date
     */
    Date getDate() const;

    /**
     * @brief Returns the \ref Time component
     * @return Time
     */
    Time getTime() const;

    /**
     * @brief Converts the date/time to a ISO 8601 string
     *
     * @return DateTime string
     */
    std::string toString() const;

    /**
     * @brief Checks if two date/times are equal
     *
     * @param other DateTime to compare
     *
     * @return true DateTimes are equal
     * @return false DateTimes are not equal
     */
    constexpr bool operator==(const DateTime &other) const
    {
        return date == other.date && time == other.time;
    }

    /**
     * @brief Checks if this date/time is less than or equal to the other date/time
     *
     * @param other DateTime to compare
     *
     * @return true This DateTime is less than or equal to \p other
     * @return false This DateTime is not less than or equal to \p other
     */
    constexpr bool operator<=(const DateTime &other) const
    {
        return date < other.date || (date == other.date && time <= other.time);
    }

    /**
     * @brief Checks if this date/time is less than to the other date/time
     *
     * @param other DateTime to compare
     *
     * @return true This DateTime is less than \p other
     * @return false This DateTime is not less than \p other
     */
    constexpr bool operator<(const DateTime &other) const
    {
        return date < other.date || (date == other.date && time < other.time);
    }

    /**
     * @brief Checks if this date/time is greater than to the other date/time
     *
     * @param other DateTime to compare
     *
     * @return true This DateTime is greater than \p other
     * @return false This DateTime is not greater than \p other
     */
    constexpr bool operator>(const DateTime &other) const
    {
        return date > other.date || (date == other.date && time > other.time);
    }

private:
    Date date;
    Time time;
};

#endif // DATETIME_HPP
