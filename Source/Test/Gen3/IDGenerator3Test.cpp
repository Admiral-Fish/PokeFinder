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

struct IDResult
{
    u32 advances;
    u16 tid;
    u16 sid;
    u16 tsv;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(IDResult, advances, tid, sid, tsv);
static_assert(sizeof(IDResult) == sizeof(IDState));

bool operator==(const IDState &state, const IDResult &result)
{
    return state.getAdvances() == result.advances && state.getTID() == result.tid && state.getSID() == result.sid
        && state.getTSV() == result.tsv;
}

void IDGenerator3Test::xdcolo_data()
{
    QTest::addColumn<u32>("seed");
    QTest::addColumn<std::vector<IDResult>>("results");

    json data = readData("gen3", "idgenerator3", "xdcolo");
    for (const auto &d : data)
    {
        QTest::newRow(d["name"].get<std::string>().data()) << d["seed"].get<u32>() << d["results"].get<std::vector<IDResult>>();
    }
}

void IDGenerator3Test::xdcolo()
{
    QFETCH(u32, seed);
    QFETCH(std::vector<IDResult>, results);

    IDFilter filter({}, {}, {}, {});
    IDGenerator3 generator(0, 9, filter);

    auto states = generator.generateXDColo(seed);
    QCOMPARE(states.size(), results.size());

    for (size_t i = 0; i < states.size(); i++)
    {
        const auto &state = states[i];
        const auto &result = results[i];

        QCOMPARE(state, result);
    }
}

void IDGenerator3Test::frgle_data()
{
    QTest::addColumn<u16>("tid");
    QTest::addColumn<std::vector<IDResult>>("results");

    json data = readData("gen3", "idgenerator3", "frlge");
    for (const auto &d : data)
    {
        QTest::newRow(d["name"].get<std::string>().data()) << d["tid"].get<u16>() << d["results"].get<std::vector<IDResult>>();
    }
}

void IDGenerator3Test::frgle()
{
    QFETCH(u16, tid);
    QFETCH(std::vector<IDResult>, results);

    IDFilter filter({}, {}, {}, {});
    IDGenerator3 generator(0, 9, filter);

    auto states = generator.generateFRLGE(tid);
    QCOMPARE(states.size(), results.size());

    for (size_t i = 0; i < states.size(); i++)
    {
        const auto &state = states[i];
        const auto &result = results[i];

        QCOMPARE(state, result);
    }
}

void IDGenerator3Test::rs_data()
{
    QTest::addColumn<u16>("seed");
    QTest::addColumn<std::vector<IDResult>>("results");

    json data = readData("gen3", "idgenerator3", "rs");
    for (const auto &d : data)
    {
        QTest::newRow(d["name"].get<std::string>().data()) << d["seed"].get<u16>() << d["results"].get<std::vector<IDResult>>();
    }
}

void IDGenerator3Test::rs()
{
    QFETCH(u16, seed);
    QFETCH(std::vector<IDResult>, results);

    IDFilter filter({}, {}, {}, {});
    IDGenerator3 generator(0, 9, filter);

    auto states = generator.generateRS(seed);
    QCOMPARE(states.size(), results.size());

    for (size_t i = 0; i < states.size(); i++)
    {
        const auto &state = states[i];
        const auto &result = results[i];

        QCOMPARE(state, result);
    }
}
