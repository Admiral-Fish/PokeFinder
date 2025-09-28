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

#include "XoroshiroTest.hpp"
#include <Core/RNG/Xoroshiro.hpp>
#include <QTest>
#include <Test/Data.hpp>

void XoroshiroTest::advance_data()
{
    QTest::addColumn<u64>("seed");
    QTest::addColumn<u32>("advances");
    QTest::addColumn<u64>("result");

    json data = readData("xoroshiro", "normal", "advance");
    for (const auto &d : data)
    {
        QTest::newRow(d["name"].get<std::string>().data()) << d["seed"].get<u64>() << d["advances"].get<u32>() << d["result"].get<u64>();
    }
}

void XoroshiroTest::advance()
{
    QFETCH(u64, seed);
    QFETCH(u32, advances);
    QFETCH(u64, result);

    Xoroshiro rng(seed);
    rng.advance(advances - 1);
    QCOMPARE(rng.next(), result);
}

void XoroshiroTest::jump_data()
{
    QTest::addColumn<u64>("seed");
    QTest::addColumn<u32>("advances");
    QTest::addColumn<u64>("result");

    json data = readData("xoroshiro", "normal", "jump");
    for (const auto &d : data)
    {
        QTest::newRow(d["name"].get<std::string>().data()) << d["seed"].get<u64>() << d["advances"].get<u32>() << d["result"].get<u64>();
    }
}

void XoroshiroTest::jump()
{
    QFETCH(u64, seed);
    QFETCH(u32, advances);
    QFETCH(u64, result);

    Xoroshiro rng(seed);
    rng.jump(advances - 1);
    QCOMPARE(rng.next(), result);
}

void XoroshiroTest::next_data()
{
    QTest::addColumn<u64>("seed");
    QTest::addColumn<u64>("result");

    json data = readData("xoroshiro", "normal", "next");
    for (const auto &d : data)
    {
        QTest::newRow(d["name"].get<std::string>().data()) << d["seed"].get<u64>() << d["result"].get<u64>();
    }
}

void XoroshiroTest::next()
{
    QFETCH(u64, seed);
    QFETCH(u64, result);

    Xoroshiro rng(seed);
    QCOMPARE(rng.next(), result);
}

void XoroshiroBDSPTest::advance_data()
{
    QTest::addColumn<u64>("seed");
    QTest::addColumn<u32>("advances");
    QTest::addColumn<u64>("result");

    json data = readData("xoroshiro", "bdsp", "advance");
    for (const auto &d : data)
    {
        QTest::newRow(d["name"].get<std::string>().data()) << d["seed"].get<u64>() << d["advances"].get<u32>() << d["result"].get<u64>();
    }
}

void XoroshiroBDSPTest::advance()
{
    QFETCH(u64, seed);
    QFETCH(u32, advances);
    QFETCH(u64, result);

    XoroshiroBDSP rng(seed);
    rng.advance(advances - 1);
    QCOMPARE(rng.next(), result);
}

void XoroshiroBDSPTest::jump_data()
{
    QTest::addColumn<u64>("seed");
    QTest::addColumn<u32>("advances");
    QTest::addColumn<u64>("result");

    json data = readData("xoroshiro", "bdsp", "jump");
    for (const auto &d : data)
    {
        QTest::newRow(d["name"].get<std::string>().data()) << d["seed"].get<u64>() << d["advances"].get<u32>() << d["result"].get<u64>();
    }
}

void XoroshiroBDSPTest::jump()
{
    QFETCH(u64, seed);
    QFETCH(u32, advances);
    QFETCH(u64, result);

    XoroshiroBDSP rng(seed);
    rng.jump(advances - 1);
    QCOMPARE(rng.next(), result);
}

void XoroshiroBDSPTest::next_data()
{
    QTest::addColumn<u64>("seed");
    QTest::addColumn<u64>("result");

    json data = readData("xoroshiro", "bdsp", "next");
    for (const auto &d : data)
    {
        QTest::newRow(d["name"].get<std::string>().data()) << d["seed"].get<u64>() << d["result"].get<u64>();
    }
}

void XoroshiroBDSPTest::next()
{
    QFETCH(u64, seed);
    QFETCH(u64, result);

    XoroshiroBDSP rng(seed);
    QCOMPARE(rng.next(), result);
}
