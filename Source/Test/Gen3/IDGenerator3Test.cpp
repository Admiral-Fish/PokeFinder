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
#include <nlohmann/json.hpp>

bool operator==(const IDState &left, const IDState &right)
{
    return left.getAdvances() == right.getAdvances() && left.getTID() == right.getTID() && left.getSID() == right.getSID()
        && left.getTSV() == right.getTSV();
}

void IDGenerator3Test::xdcolo_data()
{
    QTest::addColumn<u32>("seed");
    QTest::addColumn<std::vector<IDState>>("results");

    nlohmann::json data = readData("gen3", "idgenerator3", "xdcolo");
    for (const auto &d : data)
    {
        std::vector<IDState> results;
        for (const auto &r : d["results"])
        {
            results.emplace_back(r["advances"].get<u64>(), r["tid"].get<u16>(), r["sid"].get<u16>(), r["tsv"].get<u16>());
        }

        QTest::newRow(d["name"].get<std::string>().data()) << d["seed"].get<u32>() << results;
    }
}

void IDGenerator3Test::xdcolo()
{
    QFETCH(u32, seed);
    QFETCH(std::vector<IDState>, results);

    IDFilter filter({}, {}, {}, {});
    IDGenerator3 generator(0, 9, filter);

    QCOMPARE(generator.generateXDColo(seed), results);
}

void IDGenerator3Test::frgle_data()
{
    QTest::addColumn<u16>("tid");
    QTest::addColumn<std::vector<IDState>>("results");

    nlohmann::json data = readData("gen3", "idgenerator3", "frlge");
    for (const auto &d : data)
    {
        std::vector<IDState> results;
        for (const auto &r : d["results"])
        {
            results.emplace_back(r["advances"].get<u64>(), r["tid"].get<u16>(), r["sid"].get<u16>(), r["tsv"].get<u16>());
        }

        QTest::newRow(d["name"].get<std::string>().data()) << d["tid"].get<u16>() << results;
    }
}

void IDGenerator3Test::frgle()
{
    QFETCH(u16, tid);
    QFETCH(std::vector<IDState>, results);

    IDFilter filter({}, {}, {}, {});
    IDGenerator3 generator(0, 9, filter);

    QCOMPARE(generator.generateFRLGE(tid), results);
}

void IDGenerator3Test::rs_data()
{
    QTest::addColumn<u16>("seed");
    QTest::addColumn<std::vector<IDState>>("results");

    nlohmann::json data = readData("gen3", "idgenerator3", "rs");
    for (const auto &d : data)
    {
        std::vector<IDState> results;
        for (const auto &r : d["results"])
        {
            results.emplace_back(r["advances"].get<u64>(), r["tid"].get<u16>(), r["sid"].get<u16>(), r["tsv"].get<u16>());
        }

        QTest::newRow(d["name"].get<std::string>().data()) << d["seed"].get<u16>() << results;
    }
}

void IDGenerator3Test::rs()
{
    QFETCH(u16, seed);
    QFETCH(std::vector<IDState>, results);

    IDFilter filter({}, {}, {}, {});
    IDGenerator3 generator(0, 9, filter);

    QCOMPARE(generator.generateRS(seed), results);
}
