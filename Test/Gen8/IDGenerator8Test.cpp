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

#include "IDGenerator8Test.hpp"
#include <Core/Gen8/Generators/IDGenerator8.hpp>
#include <Core/Gen8/States/IDState8.hpp>
#include <QTest>
#include <Test/Data.hpp>

static bool operator==(const IDState8 &left, const json &right)
{
    return left.getAdvances() == right["advances"].get<u32>() && left.getSID() == right["sid"].get<u16>()
        && left.getTID() == right["tid"].get<u16>() && left.getTSV() == right["tsv"].get<u16>()
        && left.getDisplayTID() == right["displayTID"].get<u32>();
}

void IDGenerator8Test::generate_data()
{
    QTest::addColumn<u64>("seed0");
    QTest::addColumn<u64>("seed1");
    QTest::addColumn<std::string>("results");

    json data = readData("id8", "generate");
    for (const auto &d : data)
    {
        QTest::newRow(d["name"].get<std::string>().data())
            << d["seed0"].get<u64>() << d["seed1"].get<u64>() << d["results"].get<json>().dump();
    }
}

void IDGenerator8Test::generate()
{
    QFETCH(u64, seed0);
    QFETCH(u64, seed1);
    QFETCH(std::string, results);

    json j = json::parse(results);

    IDFilter filter({}, {}, {}, {});
    IDGenerator8 generator(0, 9, filter);

    auto states = generator.generate(seed0, seed1);
    QCOMPARE(states.size(), j.size());

    for (size_t i = 0; i < states.size(); i++)
    {
        const auto &state = states[i];
        QVERIFY(state == j[i]);
    }
}
