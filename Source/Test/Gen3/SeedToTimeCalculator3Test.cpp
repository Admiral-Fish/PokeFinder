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

#include "SeedToTimeCalculator3Test.hpp"
#include <Core/Gen3/Tools/SeedToTimeCalculator3.hpp>
#include <Core/Util/DateTime.hpp>
#include <QTest>
#include <Test/Data.hpp>

struct Result
{
    u16 origin;
    u32 advances;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Result, origin, advances)
};

bool operator==(const DateTime &left, const json &right)
{
    auto date = left.getDate();
    auto parts = date.getParts();

    auto time = left.getTime();

    return parts.year == right["year"].get<u16>() && parts.month == right["month"].get<u8>() && parts.day == right["day"].get<u8>()
        && time.hour() == right["hour"].get<u8>() && time.minute() == right["minute"].get<u8>();
}

void SeedToTimeCalculator3Test::calculateOriginSeed_data()
{
    QTest::addColumn<u32>("seed");
    QTest::addColumn<Result>("result");

    json data = readData("seedtotime3", "calculateOriginSeed");
    for (const auto &d : data)
    {
        QTest::newRow(d["name"].get<std::string>().data()) << d["seed"].get<u32>() << d["result"].get<Result>();
    }
}

void SeedToTimeCalculator3Test::calculateOriginSeed()
{
    QFETCH(u32, seed);
    QFETCH(Result, result);

    u32 advances = 0;
    u16 origin = SeedToTimeCalculator3::calculateOriginSeed(seed, advances);

    QVERIFY(result.origin == origin);
    QVERIFY(result.advances == advances);
}

void SeedToTimeCalculator3Test::calculateTimes_data()
{
    QTest::addColumn<u32>("seed");
    QTest::addColumn<u16>("year");
    QTest::addColumn<std::string>("results");

    json data = readData("seedtotime3", "calculateTimes");
    for (const auto &d : data)
    {
        QTest::newRow(d["name"].get<std::string>().data())
            << d["seed"].get<u32>() << d["year"].get<u16>() << d["results"].get<json>().dump();
    }
}

void SeedToTimeCalculator3Test::calculateTimes()
{
    QFETCH(u32, seed);
    QFETCH(u16, year);
    QFETCH(std::string, results);

    json j = json::parse(results);

    auto states = SeedToTimeCalculator3::calculateTimes(seed, year);
    QCOMPARE(states.size(), j.size());

    for (size_t i = 0; i < states.size(); i++)
    {
        const auto &state = states[i];
        QVERIFY(state == j[i]);
    }
}
