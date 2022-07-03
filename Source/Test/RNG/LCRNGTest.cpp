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

#include "LCRNGTest.hpp"
#include <Core/RNG/LCRNG.hpp>
#include <QTest>
#include <Test/Data.hpp>

void LCRNGTest::advance_data()
{
    QTest::addColumn<u32>("seed");
    QTest::addColumn<u32>("advances");
    QTest::addColumn<std::vector<u32>>("results");

    nlohmann::json data = readData("rng", "lcrng", "advance");
    for (const auto &d : data)
    {
        QTest::newRow(d["name"].get<std::string>().data())
            << d["seed"].get<u32>() << d["advances"].get<u32>() << d["results"].get<std::vector<u32>>();
    }
}

void LCRNGTest::advance()
{
    QFETCH(u32, seed);
    QFETCH(u32, advances);
    QFETCH(std::vector<u32>, results);

    ARNG arng(seed);
    QCOMPARE(arng.advance(advances), results[0]);

    ARNGR arngr(seed);
    QCOMPARE(arngr.advance(advances), results[1]);

    PokeRNG pokerng(seed);
    QCOMPARE(pokerng.advance(advances), results[2]);

    PokeRNGR pokerngr(seed);
    QCOMPARE(pokerngr.advance(advances), results[3]);

    XDRNG xdrng(seed);
    QCOMPARE(xdrng.advance(advances), results[4]);

    XDRNGR xdrngr(seed);
    QCOMPARE(xdrngr.advance(advances), results[5]);
}

void LCRNGTest::next_data()
{
    QTest::addColumn<u32>("seed");
    QTest::addColumn<std::vector<u32>>("results");

    nlohmann::json data = readData("rng", "lcrng", "next");
    for (const auto &d : data)
    {
        QTest::newRow(d["name"].get<std::string>().data()) << d["seed"].get<u32>() << d["results"].get<std::vector<u32>>();
    }
}

void LCRNGTest::next()
{
    QFETCH(u32, seed);
    QFETCH(std::vector<u32>, results);

    ARNG arng(seed);
    QCOMPARE(arng.next(), results[0]);

    ARNGR arngr(seed);
    QCOMPARE(arngr.next(), results[1]);

    PokeRNG pokerng(seed);
    QCOMPARE(pokerng.next(), results[2]);

    PokeRNGR pokerngr(seed);
    QCOMPARE(pokerngr.next(), results[3]);

    XDRNG xdrng(seed);
    QCOMPARE(xdrng.next(), results[4]);

    XDRNGR xdrngr(seed);
    QCOMPARE(xdrngr.next(), results[5]);
}
