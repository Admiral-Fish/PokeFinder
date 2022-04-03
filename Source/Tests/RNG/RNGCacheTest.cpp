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
#include <QVector>

Q_DECLARE_METATYPE(Method)

void RNGCacheTest::ivs_data()
{
    QTest::addColumn<Method>("method");
    QTest::addColumn<QVector<u8>>("ivs");
    QTest::addColumn<QVector<u32>>("results");

    QTest::newRow("Method 1/2 6IV") << Method::Method1 << QVector<u8>({ 31, 31, 31, 31, 31, 31 })
                                    << QVector<u32>({ 0x7FFF305A, 0x7FFF982D, 0x7FFFF961 });
    QTest::newRow("Method 1/2 Atk") << Method::Method1 << QVector<u8>({ 31, 31, 31, 0, 31, 31 })
                                    << QVector<u32>({ 0x7FFF753B, 0x7FFFDD0E });
    QTest::newRow("Method 1/2 SpA") << Method::Method1 << QVector<u8>({ 31, 0, 31, 31, 31, 31 })
                                    << QVector<u32>({ 0x7C1F0DEC, 0x7C1F75BF });
    QTest::newRow("Method 4 6IV") << Method::Method4 << QVector<u8>({ 31, 31, 31, 31, 31, 31 }) << QVector<u32>({ 0x7FFF52E5, 0x7FFF8D6E });
    QTest::newRow("Method 4 Atk") << Method::Method4 << QVector<u8>({ 31, 31, 31, 0, 31, 31 }) << QVector<u32>({ 0x7FFFA85E, 0x7FFFE2E7 });
    // Idk why this other vector constructor is necessary, initializer list of length 1 was segfaulting
    QTest::newRow("Method 4 SpA") << Method::Method4 << QVector<u8>({ 31, 0, 31, 31, 31, 31 }) << QVector<u32>(1, 0x7C1FF9B6);
}

void RNGCacheTest::ivs()
{
    QFETCH(Method, method);
    QFETCH(QVector<u8>, ivs);
    QFETCH(QVector<u32>, results);

    RNGCache cache(method);
    QCOMPARE(cache.recoverLower16BitsIV(ivs[0], ivs[1], ivs[2], ivs[3], ivs[4], ivs[5]), std::vector<u32>(results.begin(), results.end()));
}

void RNGCacheTest::pid_data()
{
    QTest::addColumn<Method>("method");
    QTest::addColumn<u32>("pid");
    QTest::addColumn<QVector<u32>>("results");

    QTest::newRow("Method 1/2") << Method::Method1 << 0x00000000U << QVector<u32>({ 0x00000000, 0x000067D3 });
    // Idk why this other vector constructor is necessary, initializer list of length 1 given 50k 0 elements
    QTest::newRow("Method 1/2") << Method::Method1 << 0x80000000U << QVector<u32>(1, 0x0000C907);
    QTest::newRow("Method 4") << Method::Method4 << 0x00000000U << QVector<u32>({});
    QTest::newRow("Method 4") << Method::Method4 << 0x80000000U << QVector<u32>({ 0x00000280, 0x00003D09 });
}

void RNGCacheTest::pid()
{
    QFETCH(Method, method);
    QFETCH(u32, pid);
    QFETCH(QVector<u32>, results);

    RNGCache cache(method);
    QCOMPARE(cache.recoverLower16BitsPID(pid), std::vector<u32>(results.begin(), results.end()));
}
