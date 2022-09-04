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

using IVs = std::array<u8, 6>;

void RNGEuclideanTest::recoverChannelIV_data()
{
    QTest::addColumn<IVs>("ivs");
    QTest::addColumn<std::vector<u32>>("results");

    json data = readData("rng", "rngeuclidean", "recoverChannelIV");
    for (const auto &d : data)
    {
        QTest::newRow(d["name"].get<std::string>().data()) << d["ivs"].get<IVs>() << d["results"].get<std::vector<u32>>();
    }
}

void RNGEuclideanTest::recoverChannelIV()
{
    QFETCH(IVs, ivs);
    QFETCH(std::vector<u32>, results);

    u32 seeds[12];
    int size = RNGEuclidean::recoverChannelIV(ivs[0], ivs[1], ivs[2], ivs[3], ivs[4], ivs[5], seeds);

    QCOMPARE(size, results.size());
    for (int i = 0; i < size; i++)
    {
        QCOMPARE(seeds[i], results[i]);
    }
}

void RNGEuclideanTest::recoverXDRNGIV_data()
{
    QTest::addColumn<IVs>("ivs");
    QTest::addColumn<std::vector<u32>>("results");

    json data = readData("rng", "rngeuclidean", "recoverXDRNGIV");
    for (const auto &d : data)
    {
        QTest::newRow(d["name"].get<std::string>().data()) << d["ivs"].get<IVs>() << d["results"].get<std::vector<u32>>();
    }
}

void RNGEuclideanTest::recoverXDRNGIV()
{
    QFETCH(IVs, ivs);
    QFETCH(std::vector<u32>, results);

    u32 seeds[6];
    int size = RNGEuclidean::recoverXDRNGIV(ivs[0], ivs[1], ivs[2], ivs[3], ivs[4], ivs[5], seeds);

    QCOMPARE(size, results.size());
    for (int i = 0; i < size; i++)
    {
        QCOMPARE(seeds[i], results[i]);
    }
}

void RNGEuclideanTest::recoverXDRNGPID_data()
{
    QTest::addColumn<u32>("pid");
    QTest::addColumn<std::vector<u32>>("results");

    json data = readData("rng", "rngeuclidean", "recoverXDRNGPID");
    for (const auto &d : data)
    {
        QTest::newRow(d["name"].get<std::string>().data()) << d["pid"].get<u32>() << d["results"].get<std::vector<u32>>();
    }
}

void RNGEuclideanTest::recoverXDRNGPID()
{
    QFETCH(u32, pid);
    QFETCH(std::vector<u32>, results);

    u32 seeds[2];
    int size = RNGEuclidean::recoverXDRNGPID(pid, seeds);

    QCOMPARE(size, results.size());
    for (int i = 0; i < size; i++)
    {
        QCOMPARE(seeds[i], results[i]);
    }
}
