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

#include "IDGenerator4Test.hpp"
#include <Core/Gen4/Generators/IDGenerator4.hpp>
#include <Core/Gen4/States/IDState4.hpp>
#include <QTest>
#include <Test/Data.hpp>

struct IDResult4
{
    u32 advances;
    u16 sid;
    u16 tid;
    u16 tsv;
    u32 delay;
    u32 seed;
    u8 seconds;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(IDResult4, sid, tid, tsv, delay, seed, seconds);
static_assert(sizeof(IDResult4) == sizeof(IDState4));

static bool operator==(const IDState4 &left, const IDResult4 &right)
{
    return left.getSID() == right.sid && left.getTID() == right.tid && left.getTSV() == right.tsv && left.getDelay() == right.delay
        && left.getSeed() == right.seed && left.getSeconds() == right.seconds;
}

static bool operator==(const IDResult4 &left, const IDState4 &right)
{
    return operator==(right, left);
}

void IDGenerator4Test::generate_data()
{
    QTest::addColumn<u16>("tid");
    QTest::addColumn<u32>("maxDelay");
    QTest::addColumn<u32>("minDelay");
    QTest::addColumn<u16>("year");
    QTest::addColumn<u8>("month");
    QTest::addColumn<u8>("day");
    QTest::addColumn<u8>("hour");
    QTest::addColumn<u8>("minute");
    QTest::addColumn<std::vector<IDResult4>>("results");

    json data = readData("gen4", "idgenerator4", "generate");
    for (const auto &d : data)
    {
        QTest::newRow(d["name"].get<std::string>().data())
            << d["tid"].get<u16>() << d["maxDelay"].get<u32>() << d["minDelay"].get<u32>() << d["year"].get<u16>() << d["month"].get<u8>()
            << d["day"].get<u8>() << d["hour"].get<u8>() << d["minute"].get<u8>() << d["results"].get<std::vector<IDResult4>>();
    }
}

void IDGenerator4Test::generate()
{
    QFETCH(u16, tid);
    QFETCH(u32, maxDelay);
    QFETCH(u32, minDelay);
    QFETCH(u16, year);
    QFETCH(u8, month);
    QFETCH(u8, day);
    QFETCH(u8, hour);
    QFETCH(u8, minute);
    QFETCH(std::vector<IDResult4>, results);

    IDFilter filter({ tid }, {}, {}, {});
    IDGenerator4 generator(minDelay, maxDelay, year, month, day, hour, minute, filter);

    auto states = generator.generate();
    QCOMPARE(states.size(), results.size());

    for (size_t i = 0; i < states.size(); i++)
    {
        const auto &state = states[i];
        const auto &result = results[i];
        QVERIFY(state == result);
    }
}
