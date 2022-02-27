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

#include "DateTimeTest.hpp"
#include <Core/Util/DateTime.hpp>
#include <QTest>
#include <QVector>

void DateTimeTest::addSecs_data()
{
    QTest::addColumn<int>("seconds");
    QTest::addColumn<DateTime>("result");

    QTest::newRow("1 second") << 1 << DateTime(Date(2000, 1, 1), Time(0, 0, 1));
    QTest::newRow("1 minute") << 60 << DateTime(Date(2000, 1, 1), Time(0, 1, 0));
    QTest::newRow("1 hour") << 3600 << DateTime(Date(2000, 1, 1), Time(1, 0, 0));
    QTest::newRow("1 day") << 86400 << DateTime(Date(2000, 1, 2), Time(0, 0, 0));
}

void DateTimeTest::addSecs()
{
    QFETCH(int, seconds);
    QFETCH(DateTime, result);

    DateTime dt = DateTime().addSecs(seconds);
    QCOMPARE(dt, result);
}

void DateTest::getParts_data()
{
    QTest::addColumn<int>("jd");
    QTest::addColumn<std::array<int, 3>>("results");

    QTest::newRow("Jan 1, 2000") << 2451545 << std::array<int, 3> { 2000, 1, 1 };
    QTest::newRow("Dec 31, 2099") << 2488069 << std::array<int, 3> { 2099, 12, 31 };
}

void DateTest::getParts()
{
    using Results = std::array<int, 3>;

    QFETCH(int, jd);
    QFETCH(Results, results);

    Date date(jd);
    QCOMPARE(date.getParts(), results);
}

void DateTest::dayOfWeek_data()
{
    QTest::addColumn<Date>("day");
    QTest::addColumn<int>("result");

    QTest::newRow("Monday") << Date(2000, 1, 3) << 1;
    QTest::newRow("Tuesday") << Date(2000, 1, 4) << 2;
    QTest::newRow("Wednesday") << Date(2000, 1, 5) << 3;
    QTest::newRow("Thursday") << Date(2000, 1, 6) << 4;
    QTest::newRow("Friday") << Date(2000, 1, 7) << 5;
    QTest::newRow("Saturday") << Date(2000, 1, 8) << 6;
    QTest::newRow("Sunday") << Date(2000, 1, 9) << 0;
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

    QTest::newRow("1 second") << 1 << 0 << Time(0, 0, 1);
    QTest::newRow("1 minute") << 60 << 0 << Time(0, 1, 0);
    QTest::newRow("1 hour") << 3600 << 0 << Time(1, 0, 0);
    QTest::newRow("1 day") << 86400 << 1 << Time(0, 0, 0);
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
