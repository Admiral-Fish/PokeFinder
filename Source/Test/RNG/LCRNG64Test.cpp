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

#include "LCRNG64Test.hpp"
#include <Core/RNG/LCRNG64.hpp>
#include <QTest>
#include <Test/Data.hpp>

void LCRNG64Test::advance_data()
{
    QTest::addColumn<u64>("seed");
    QTest::addColumn<u32>("advances");
    QTest::addColumn<std::vector<u64>>("results");

    json data = readData("lcrng64", "advance");
    for (const auto &d : data)
    {
        QTest::newRow(d["name"].get<std::string>().data())
            << d["seed"].get<u64>() << d["advances"].get<u32>() << d["results"].get<std::vector<u64>>();
    }
}

void LCRNG64Test::advance()
{
    QFETCH(u64, seed);
    QFETCH(u32, advances);
    QFETCH(std::vector<u64>, results);

    BWRNG bwrng(seed);
    QCOMPARE(bwrng.advance(advances), results[0]);

    BWRNGR bwrngr(seed);
    QCOMPARE(bwrngr.advance(advances), results[1]);
}

void LCRNG64Test::jump_data()
{
    QTest::addColumn<u64>("seed");
    QTest::addColumn<u32>("advances");
    QTest::addColumn<std::vector<u64>>("results");

    json data = readData("lcrng64", "jump");
    for (const auto &d : data)
    {
        QTest::newRow(d["name"].get<std::string>().data())
            << d["seed"].get<u64>() << d["advances"].get<u32>() << d["results"].get<std::vector<u64>>();
    }
}

void LCRNG64Test::jump()
{
    QFETCH(u64, seed);
    QFETCH(u32, advances);
    QFETCH(std::vector<u64>, results);

    BWRNG bwrng(seed);
    QCOMPARE(bwrng.jump(advances), results[0]);

    BWRNGR bwrngr(seed);
    QCOMPARE(bwrngr.jump(advances), results[1]);
}

void LCRNG64Test::next_data()
{
    QTest::addColumn<u64>("seed");
    QTest::addColumn<std::vector<u64>>("results");

    json data = readData("lcrng64", "next");
    for (const auto &d : data)
    {
        QTest::newRow(d["name"].get<std::string>().data()) << d["seed"].get<u64>() << d["results"].get<std::vector<u64>>();
    }
}

void LCRNG64Test::next()
{
    QFETCH(u64, seed);
    QFETCH(std::vector<u64>, results);

    BWRNG bwrng(seed);
    QCOMPARE(bwrng.next(), results[0]);

    BWRNGR bwrngr(seed);
    QCOMPARE(bwrngr.next(), results[1]);
}
