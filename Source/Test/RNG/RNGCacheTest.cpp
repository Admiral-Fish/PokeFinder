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

#include "RNGCacheTest.hpp"
#include <Core/Enum/Method.hpp>
#include <Core/RNG/RNGCache.hpp>
#include <QTest>
#include <Test/Data.hpp>

Q_DECLARE_METATYPE(Method)

void RNGCacheTest::ivs_data()
{
    QTest::addColumn<Method>("method");
    QTest::addColumn<std::vector<u8>>("ivs");
    QTest::addColumn<std::vector<u32>>("results");

    nlohmann::json data = readData("rng", "rngcache", "ivs");
    for (const auto &d : data)
    {
        QTest::newRow(d["name"].get<std::string>().data())
            << d["method"].get<Method>() << d["ivs"].get<std::vector<u8>>() << d["results"].get<std::vector<u32>>();
    }
}

void RNGCacheTest::ivs()
{
    QFETCH(Method, method);
    QFETCH(std::vector<u8>, ivs);
    QFETCH(std::vector<u32>, results);

    RNGCache cache(method);
    QCOMPARE(cache.recoverLower16BitsIV(ivs[0], ivs[1], ivs[2], ivs[3], ivs[4], ivs[5]), results);
}

void RNGCacheTest::pid_data()
{
    QTest::addColumn<Method>("method");
    QTest::addColumn<u32>("pid");
    QTest::addColumn<std::vector<u32>>("results");

    nlohmann::json data = readData("rng", "rngcache", "pid");
    for (const auto &d : data)
    {
        QTest::newRow(d["name"].get<std::string>().data())
            << d["method"].get<Method>() << d["pid"].get<u32>() << d["results"].get<std::vector<u32>>();
    }
}

void RNGCacheTest::pid()
{
    QFETCH(Method, method);
    QFETCH(u32, pid);
    QFETCH(std::vector<u32>, results);

    RNGCache cache(method);
    QCOMPARE(cache.recoverLower16BitsPID(pid), std::vector<u32>(results.begin(), results.end()));
}
