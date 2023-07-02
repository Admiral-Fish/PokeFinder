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

#include "IDGenerator5Test.hpp"
#include <Core/Enum/Game.hpp>
#include <Core/Gen5/Generators/IDGenerator5.hpp>
#include <Core/Gen5/Profile5.hpp>
#include <Core/Parents/States/IDState.hpp>
#include <QTest>
#include <Test/Data.hpp>
#include <Test/Enum.hpp>

static bool operator==(const IDState &left, const json &right)
{
    return left.getAdvances() == right["advances"].get<u32>() && left.getSID() == right["sid"].get<u16>()
        && left.getTID() == right["tid"].get<u16>() && left.getTSV() == right["tsv"].get<u16>();
}

void IDGenerator5Test::generate_data()
{
    QTest::addColumn<u64>("seed");
    QTest::addColumn<Game>("version");
    QTest::addColumn<std::string>("results");

    json data = readData("id5", "generate");
    for (const auto &d : data)
    {
        QTest::newRow(d["name"].get<std::string>().data())
            << d["seed"].get<u64>() << d["version"].get<Game>() << d["results"].get<json>().dump();
    }
}

void IDGenerator5Test::generate()
{
    QFETCH(u64, seed);
    QFETCH(Game, version);
    QFETCH(std::string, results);

    json j = json::parse(results);

    Profile5 profile("-", version, 12345, 54321, 0, { false, false, false, false }, 0, 0, 0, false, 0, 0, false, false, false, DSType::DS,
                     Language::English);

    IDFilter filter({}, {}, {}, {});
    IDGenerator5 generator(0, 9, profile, filter);

    auto states = generator.generate(seed);
    QCOMPARE(states.size(), j.size());

    for (size_t i = 0; i < states.size(); i++)
    {
        const auto &state = states[i];
        QVERIFY(state == j[i]);
    }
}
