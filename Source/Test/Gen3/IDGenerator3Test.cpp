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

#include "IDGenerator3Test.hpp"
#include <Core/Gen3/Generators/IDGenerator3.hpp>
#include <Core/Parents/States/IDState.hpp>
#include <QTest>
#include <Test/Data.hpp>

static bool operator==(const IDState &left, const json &right)
{
    return left.getAdvances() == right["advances"].get<u32>() && left.getSID() == right["sid"].get<u16>()
        && left.getTID() == right["tid"].get<u16>() && left.getTSV() == right["tsv"].get<u16>();
}

void IDGenerator3Test::xdcolo_data()
{
    QTest::addColumn<u32>("seed");
    QTest::addColumn<std::string>("results");

    json data = readData("gen3", "idgenerator3", "xdcolo");
    for (const auto &d : data)
    {
        QTest::newRow(d["name"].get<std::string>().data()) << d["seed"].get<u32>() << d["results"].get<json>().dump();
    }
}

void IDGenerator3Test::xdcolo()
{
    QFETCH(u32, seed);
    QFETCH(std::string, results);

    json j = json::parse(results);

    IDFilter filter({}, {}, {}, {});
    IDGenerator3 generator(0, 9, filter);

    auto states = generator.generateXDColo(seed);
    QCOMPARE(states.size(), j.size());

    for (size_t i = 0; i < states.size(); i++)
    {
        const auto &state = states[i];
        QVERIFY(state == j[i]);
    }
}

void IDGenerator3Test::frgle_data()
{
    QTest::addColumn<u16>("tid");
    QTest::addColumn<std::string>("results");

    json data = readData("gen3", "idgenerator3", "frlge");
    for (const auto &d : data)
    {
        QTest::newRow(d["name"].get<std::string>().data()) << d["tid"].get<u16>() << d["results"].get<json>().dump();
    }
}

void IDGenerator3Test::frgle()
{
    QFETCH(u16, tid);
    QFETCH(std::string, results);

    json j = json::parse(results);

    IDFilter filter({}, {}, {}, {});
    IDGenerator3 generator(0, 9, filter);

    auto states = generator.generateFRLGE(tid);
    QCOMPARE(states.size(), j.size());

    for (size_t i = 0; i < states.size(); i++)
    {
        const auto &state = states[i];
        QVERIFY(state == j[i]);
    }
}

void IDGenerator3Test::rs_data()
{
    QTest::addColumn<u16>("seed");
    QTest::addColumn<std::string>("results");

    json data = readData("gen3", "idgenerator3", "rs");
    for (const auto &d : data)
    {
        QTest::newRow(d["name"].get<std::string>().data()) << d["seed"].get<u16>() << d["results"].get<json>().dump();
    }
}

void IDGenerator3Test::rs()
{
    QFETCH(u16, seed);
    QFETCH(std::string, results);

    json j = json::parse(results);

    IDFilter filter({}, {}, {}, {});
    IDGenerator3 generator(0, 9, filter);

    auto states = generator.generateRS(seed);
    QCOMPARE(states.size(), j.size());

    for (size_t i = 0; i < states.size(); i++)
    {
        const auto &state = states[i];
        QVERIFY(state == j[i]);
    }
}
