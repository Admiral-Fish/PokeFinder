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

#include "IVToPIDCalculatorTest.hpp"
#include <Core/Parents/States/IVToPIDState.hpp>
#include <Core/Util/IVToPIDCalculator.hpp>
#include <QTest>
#include <Test/Data.hpp>
#include <Test/Enum.hpp>

using IVs = std::array<u8, 6>;

static bool operator==(const IVToPIDState &left, const json &right)
{
    return left.getPID() == right["pid"].get<u32>() && left.getSeed() == right["seed"].get<u32>()
        && left.getSID() == right["sid"].get<u16>() && left.getMethod() == right["method"].get<Method>();
}

void IVToPIDCalculatorTest::calculatePIDs_data()
{
    QTest::addColumn<IVs>("ivs");
    QTest::addColumn<u8>("nature");
    QTest::addColumn<u16>("tid");
    QTest::addColumn<std::string>("results");

    json data = readData("ivtopidcalculator", "calculatePIDs");
    for (const auto &d : data)
    {
        QTest::newRow(d["name"].get<std::string>().data())
            << d["ivs"].get<IVs>() << d["nature"].get<u8>() << d["tid"].get<u16>() << d["results"].get<json>().dump();
    }
}

void IVToPIDCalculatorTest::calculatePIDs()
{
    QFETCH(IVs, ivs);
    QFETCH(u8, nature);
    QFETCH(u16, tid);
    QFETCH(std::string, results);

    json j = json::parse(results);

    auto states = IVToPIDCalculator::calculatePIDs(ivs[0], ivs[1], ivs[2], ivs[3], ivs[4], ivs[5], nature, tid);
    QCOMPARE(states.size(), j.size());

    for (size_t i = 0; i < states.size(); i++)
    {
        const auto &state = states[i];
        QVERIFY(state == j[i]);
    }
}
