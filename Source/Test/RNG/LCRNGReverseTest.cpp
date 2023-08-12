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

#include "LCRNGReverseTest.hpp"
#include <Core/RNG/LCRNGReverse.hpp>
#include <QTest>
#include <Test/Data.hpp>
#include <Test/Enum.hpp>

using IVs = std::array<u8, 6>;

void LCRNGReverseTest::recoverChannelIV_data()
{
    QTest::addColumn<IVs>("ivs");
    QTest::addColumn<std::vector<u32>>("results");

    json data = readData("lcrngreverse", "recoverChannelIV");
    for (const auto &d : data)
    {
        QTest::newRow(d["name"].get<std::string>().data()) << d["ivs"].get<IVs>() << d["results"].get<std::vector<u32>>();
    }
}

void LCRNGReverseTest::recoverChannelIV()
{
    QFETCH(IVs, ivs);
    QFETCH(std::vector<u32>, results);

    u32 seeds[12];
    int size = LCRNGReverse::recoverChannelIV(ivs[0], ivs[1], ivs[2], ivs[3], ivs[4], ivs[5], seeds);

    QCOMPARE(size, results.size());
    for (int i = 0; i < size; i++)
    {
        QCOMPARE(seeds[i], results[i]);
    }
}

void LCRNGReverseTest::recoverPokeRNGIV_data()
{
    QTest::addColumn<Method>("method");
    QTest::addColumn<std::vector<u8>>("ivs");
    QTest::addColumn<std::vector<u32>>("results");

    json data = readData("lcrngreverse", "recoverPokeRNGIV");
    for (const auto &d : data)
    {
        QTest::newRow(d["name"].get<std::string>().data())
            << d["method"].get<Method>() << d["ivs"].get<std::vector<u8>>() << d["results"].get<std::vector<u32>>();
    }
}

void LCRNGReverseTest::recoverPokeRNGIV()
{
    QFETCH(Method, method);
    QFETCH(std::vector<u8>, ivs);
    QFETCH(std::vector<u32>, results);

    u32 seeds[6];
    int size = LCRNGReverse::recoverPokeRNGIV(ivs[0], ivs[1], ivs[2], ivs[3], ivs[4], ivs[5], seeds, method);

    QCOMPARE(size, results.size());
    for (int i = 0; i < size; i++)
    {
        QCOMPARE(seeds[i], results[i]);
    }
}

void LCRNGReverseTest::recoverPokeRNGPID_data()
{
    QTest::addColumn<Method>("method");
    QTest::addColumn<u32>("pid");
    QTest::addColumn<std::vector<u32>>("results");

    json data = readData("lcrngreverse", "recoverPokeRNGPID_data");
    for (const auto &d : data)
    {
        QTest::newRow(d["name"].get<std::string>().data())
            << d["method"].get<Method>() << d["pid"].get<u32>() << d["results"].get<std::vector<u32>>();
    }
}

void LCRNGReverseTest::recoverPokeRNGPID()
{
    QFETCH(Method, method);
    QFETCH(u32, pid);
    QFETCH(std::vector<u32>, results);

    u32 seeds[3];
    int size = LCRNGReverse::recoverPokeRNGPID(pid, seeds);

    QCOMPARE(size, results.size());
    for (int i = 0; i < size; i++)
    {
        QCOMPARE(seeds[i], results[i]);
    }
}

void LCRNGReverseTest::recoverXDRNGIV_data()
{
    QTest::addColumn<IVs>("ivs");
    QTest::addColumn<std::vector<u32>>("results");

    json data = readData("lcrngreverse", "recoverXDRNGIV");
    for (const auto &d : data)
    {
        QTest::newRow(d["name"].get<std::string>().data()) << d["ivs"].get<IVs>() << d["results"].get<std::vector<u32>>();
    }
}

void LCRNGReverseTest::recoverXDRNGIV()
{
    QFETCH(IVs, ivs);
    QFETCH(std::vector<u32>, results);

    u32 seeds[6];
    int size = LCRNGReverse::recoverXDRNGIV(ivs[0], ivs[1], ivs[2], ivs[3], ivs[4], ivs[5], seeds);

    QCOMPARE(size, results.size());
    for (int i = 0; i < size; i++)
    {
        QCOMPARE(seeds[i], results[i]);
    }
}

void LCRNGReverseTest::recoverXDRNGPID_data()
{
    QTest::addColumn<u32>("pid");
    QTest::addColumn<std::vector<u32>>("results");

    json data = readData("lcrngreverse", "recoverXDRNGPID");
    for (const auto &d : data)
    {
        QTest::newRow(d["name"].get<std::string>().data()) << d["pid"].get<u32>() << d["results"].get<std::vector<u32>>();
    }
}

void LCRNGReverseTest::recoverXDRNGPID()
{
    QFETCH(u32, pid);
    QFETCH(std::vector<u32>, results);

    u32 seeds[2];
    int size = LCRNGReverse::recoverXDRNGPID(pid, seeds);

    QCOMPARE(size, results.size());
    for (int i = 0; i < size; i++)
    {
        QCOMPARE(seeds[i], results[i]);
    }
}
