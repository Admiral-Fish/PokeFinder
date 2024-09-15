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

#include "DateTimeTest.hpp"
#include <Core/Util/DateTime.hpp>
#include <QTest>
#include <Test/Data.hpp>

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(DateParts, year, month, day);

static bool operator==(const DateParts &left, const DateParts &right)
{
    return left.year == right.year && left.month == right.month && left.day == right.day;
}

void DateTimeTest::addSecs_data()
{
    QTest::addColumn<int>("seconds");
    QTest::addColumn<DateTime>("result");

    nlohmann::json data = readData("datetime", "datetime", "addSecs");
    for (const auto &d : data)
    {
        QTest::newRow(d["name"].get<std::string>().data())
            << d["seconds"].get<int>()
            << DateTime(Date(d["year"].get<int>(), d["month"].get<int>(), d["day"].get<int>()),
                        Time(d["hour"].get<int>(), d["minute"].get<int>(), d["second"].get<int>()));
    }
}

void DateTimeTest::addSecs()
{
    QFETCH(int, seconds);
    QFETCH(DateTime, result);

    DateTime dt = DateTime().addSeconds(seconds);
    QCOMPARE(dt, result);
}

void DateTest::getParts_data()
{
    QTest::addColumn<int>("jd");
    QTest::addColumn<DateParts>("results");

    nlohmann::json data = readData("datetime", "date", "getParts");
    for (const auto &d : data)
    {
        QTest::newRow(d["name"].get<std::string>().data()) << d["jd"].get<int>() << d["results"].get<DateParts>();
    }
}

void DateTest::getParts()
{
    QFETCH(int, jd);
    QFETCH(DateParts, results);

    Date date(jd);
    QCOMPARE(date.getParts(), results);
}

void DateTest::dayOfWeek_data()
{
    QTest::addColumn<Date>("day");
    QTest::addColumn<int>("result");

    nlohmann::json data = readData("datetime", "date", "dayOfWeek");
    for (const auto &d : data)
    {
        QTest::newRow(d["name"].get<std::string>().data())
            << Date(d["year"].get<int>(), d["month"].get<int>(), d["day"].get<int>()) << d["result"].get<int>();
    }
}

void DateTest::dayOfWeek()
{
    QFETCH(Date, day);
    QFETCH(int, result);

    QCOMPARE(day.dayOfWeek(), result);
}

void TimeTest::addSeconds_data()
{
    QTest::addColumn<int>("seconds");
    QTest::addColumn<int>("expectedDays");
    QTest::addColumn<Time>("result");

    nlohmann::json data = readData("datetime", "time", "addSeconds");
    for (const auto &d : data)
    {
        QTest::newRow(d["name"].get<std::string>().data()) << d["seconds"].get<int>() << d["expectedDays"].get<int>()
                                                           << Time(d["hour"].get<int>(), d["minute"].get<int>(), d["second"].get<int>());
    }
}

void TimeTest::addSeconds()
{
    QFETCH(int, seconds);
    QFETCH(int, expectedDays);
    QFETCH(Time, result);

    Time t;
    int days = t.addSeconds(seconds);

    QCOMPARE(days, expectedDays);
    QCOMPARE(t, result);
}
