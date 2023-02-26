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

#include "SeedToTimeCalculator4Test.hpp"
#include <Core/Gen4/Tools/SeedToTimeCalculator4.hpp>
#include <QTest>
#include <Test/Data.hpp>

static bool operator==(const SeedTime4 &left, const json &right)
{
    auto dt = left.getDateTime();
    auto date = dt.getDate();
    auto time = dt.getTime();

    return date.year() == right["year"].get<u16>() && date.month() == right["month"].get<u8>() && date.day() == right["day"].get<u8>()
        && time.hour() == right["hour"].get<u8>() && time.minute() == right["minute"].get<u8>()
        && time.second() == right["second"].get<u8>() && left.getDelay() == right["delay"].get<u32>();
}

void SeedToTimeCalculator4Test::calculateTimes_data()
{
    QTest::addColumn<u32>("seed");
    QTest::addColumn<u16>("year");
    QTest::addColumn<std::string>("results");

    json data = readData("seedtotime4", "calculateTimes");
    for (const auto &d : data)
    {
        QTest::newRow(d["name"].get<std::string>().data())
            << d["seed"].get<u32>() << d["year"].get<u16>() << d["results"].get<json>().dump();
    }
}

void SeedToTimeCalculator4Test::calculateTimes()
{
    QFETCH(u32, seed);
    QFETCH(u16, year);
    QFETCH(std::string, results);

    json j = json::parse(results);

    auto states = SeedToTimeCalculator4::calculateTimes(seed, year, true, 0);
    QCOMPARE(states.size(), j.size());

    for (size_t i = 0; i < states.size(); i++)
    {
        const auto &state = states[i];
        QVERIFY(state == j[i]);
    }
}
