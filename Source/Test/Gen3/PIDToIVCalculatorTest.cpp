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

#include "PIDToIVCalculatorTest.hpp"
#include <Core/Gen3/PIDToIVCalculator.hpp>
#include <Core/Gen3/States/PIDToIVState.hpp>
#include <QTest>
#include <Test/Data.hpp>
#include <Test/Enum.hpp>

static bool operator==(const PIDToIVState &left, const json &right)
{
    return left.getSeed() == right["seed"].get<u32>() && left.getMethod() == getMethod(right["method"].get<std::string>())
        && left.getIVs() == right["ivs"].get<std::array<u8, 6>>();
}

void PIDToIVCalculatorTest::calculateIVs_data()
{
    QTest::addColumn<u32>("pid");
    QTest::addColumn<std::string>("results");

    json data = readData("pidtoivcalculator", "calculateIVs");
    for (const auto &d : data)
    {
        QTest::newRow(d["name"].get<std::string>().data()) << d["pid"].get<u32>() << d["results"].get<json>().dump();
    }
}

void PIDToIVCalculatorTest::calculateIVs()
{
    QFETCH(u32, pid);
    QFETCH(std::string, results);

    json j = json::parse(results);

    auto states = PIDToIVCalculator::calculateIVs(pid);
    QCOMPARE(states.size(), j.size());

    for (size_t i = 0; i < states.size(); i++)
    {
        const auto &state = states[i];
        QVERIFY(state == j[i]);
    }
}
