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

#include "RNGEuclideanTest.hpp"
#include <Core/RNG/RNGEuclidean.hpp>
#include <QTest>
#include <Test/Data.hpp>

void RNGEuclideanTest::ivs_data()
{
    QTest::addColumn<std::vector<u8>>("ivs");
    QTest::addColumn<std::vector<std::pair<u32, u32>>>("results");

    nlohmann::json data = readData("rng", "rngeuclidean", "ivs");
    for (const auto &d : data)
    {
        QTest::newRow(d["name"].get<std::string>().data())
            << d["ivs"].get<std::vector<u8>>() << d["results"].get<std::vector<std::pair<u32, u32>>>();
    }
}

void RNGEuclideanTest::ivs()
{
    using Results = std::vector<std::pair<u32, u32>>;

    QFETCH(std::vector<u8>, ivs);
    QFETCH(Results, results);

    QCOMPARE(RNGEuclidean::recoverLower16BitsIV(ivs[0], ivs[1], ivs[2], ivs[3], ivs[4], ivs[5]), results);
}

void RNGEuclideanTest::pid_data()
{
    QTest::addColumn<u32>("pid");
    QTest::addColumn<std::vector<std::pair<u32, u32>>>("results");

    nlohmann::json data = readData("rng", "rngeuclidean", "pid");
    for (const auto &d : data)
    {
        QTest::newRow(d["name"].get<std::string>().data()) << d["pid"].get<u32>() << d["results"].get<std::vector<std::pair<u32, u32>>>();
    }
}

void RNGEuclideanTest::pid()
{
    using Results = std::vector<std::pair<u32, u32>>;

    QFETCH(u32, pid);
    QFETCH(Results, results);

    QCOMPARE(RNGEuclidean::recoverLower16BitsPID(pid), results);
}

void RNGEuclideanTest::ivsChannel_data()
{
    QTest::addColumn<std::vector<u8>>("ivs");
    QTest::addColumn<std::vector<u32>>("results");

    nlohmann::json data = readData("rng", "rngeuclidean", "ivsChannel");
    for (const auto &d : data)
    {
        QTest::newRow(d["name"].get<std::string>().data()) << d["ivs"].get<std::vector<u8>>() << d["results"].get<std::vector<u32>>();
    }
}

void RNGEuclideanTest::ivsChannel()
{
    QFETCH(std::vector<u8>, ivs);
    QFETCH(std::vector<u32>, results);

    QCOMPARE(RNGEuclidean::recoverLower27BitsChannel(ivs[0], ivs[1], ivs[2], ivs[3], ivs[4], ivs[5]), results);
}
