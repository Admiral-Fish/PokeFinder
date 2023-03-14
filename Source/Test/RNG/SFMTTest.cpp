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

#include "SFMTTest.hpp"
#include <Core/RNG/SFMT.hpp>
#include <QTest>
#include <Test/Data.hpp>

void SFMTTest::advance_data()
{
    QTest::addColumn<u32>("seed");
    QTest::addColumn<u32>("advances");
    QTest::addColumn<u64>("result");

    json data = readData("sfmt", "advance");
    for (const auto &d : data)
    {
        QTest::newRow(d["name"].get<std::string>().data()) << d["seed"].get<u32>() << d["advances"].get<u32>() << d["result"].get<u64>();
    }
}

void SFMTTest::advance()
{
    QFETCH(u32, seed);
    QFETCH(u32, advances);
    QFETCH(u64, result);

    SFMT rng(seed);
    rng.advance(advances - 1);
    QCOMPARE(rng.next(), result);
}

void SFMTTest::next_data()
{
    QTest::addColumn<u32>("seed");
    QTest::addColumn<u64>("result");

    json data = readData("sfmt", "next");
    for (const auto &d : data)
    {
        QTest::newRow(d["name"].get<std::string>().data()) << d["seed"].get<u32>() << d["result"].get<u64>();
    }
}

void SFMTTest::next()
{
    QFETCH(u32, seed);
    QFETCH(u64, result);

    SFMT rng(seed);
    QCOMPARE(rng.next(), result);
}

void SFMTTest::nextUInt_data()
{
    QTest::addColumn<u32>("seed");
    QTest::addColumn<u32>("result");

    json data = readData("sfmt", "next");
    for (const auto &d : data)
    {
        QTest::newRow(d["name"].get<std::string>().data()) << d["seed"].get<u32>() << d["result"].get<u32>();
    }
}

void SFMTTest::nextUInt()
{
    QFETCH(u32, seed);
    QFETCH(u32, result);

    SFMT rng(seed);
    QCOMPARE(rng.nextUInt(), result);
}
