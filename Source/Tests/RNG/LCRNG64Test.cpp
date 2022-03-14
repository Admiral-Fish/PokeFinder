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

#include "LCRNG64Test.hpp"
#include <Core/RNG/LCRNG64.hpp>
#include <QTest>
#include <QVector>

void LCRNG64Test::advance_data()
{
    QTest::addColumn<u64>("seed");
    QTest::addColumn<u32>("advances");
    QTest::addColumn<QVector<u64>>("results");

    QTest::newRow("Advance 1") << 0x0000000000000000ULL << 5U << QVector<u64>({ 0xC83FB970153A9227, 0xADC2D56AF375BEB5 });
    QTest::newRow("Advance 2") << 0x0000000000000000ULL << 10U << QVector<u64>({ 0x67795501267F125A, 0x44B99BE460CCF9D6 });
    QTest::newRow("Advance 3") << 0x8000000000000000ULL << 5U << QVector<u64>({ 0x483FB970153A9227, 0x2DC2D56AF375BEB5 });
    QTest::newRow("Advance 4") << 0x8000000000000000ULL << 10U << QVector<u64>({ 0xE7795501267F125A, 0xC4B99BE460CCF9D6 });
}

void LCRNG64Test::advance()
{
    QFETCH(u64, seed);
    QFETCH(u32, advances);
    QFETCH(QVector<u64>, results);

    BWRNG bwrng(seed);
    bwrng.advance(advances);
    QCOMPARE(bwrng.getSeed(), results[0]);

    BWRNGR bwrngr(seed);
    bwrngr.advance(advances);
    QCOMPARE(bwrngr.getSeed(), results[1]);
}

void LCRNG64Test::next_data()
{
    QTest::addColumn<u64>("seed");
    QTest::addColumn<QVector<u64>>("results");

    QTest::newRow("Next 1") << 0x0000000000000000ULL << QVector<u64>({ 0x0000000000269EC3, 0x9B1AE6E9A384E6F9 });
    QTest::newRow("Next 2") << 0x4000000000000000ULL << QVector<u64>({ 0x4000000000269EC3, 0xDB1AE6E9A384E6F9 });
    QTest::newRow("Next 3") << 0x8000000000000000ULL << QVector<u64>({ 0x8000000000269EC3, 0x1B1AE6E9A384E6F9 });
    QTest::newRow("Next 4") << 0xC000000000000000ULL << QVector<u64>({ 0xC000000000269EC3, 0x5B1AE6E9A384E6F9 });
}

void LCRNG64Test::next()
{
    QFETCH(u64, seed);
    QFETCH(QVector<u64>, results);

    BWRNG bwrng(seed);
    QCOMPARE(bwrng.next(), results[0]);

    BWRNGR bwrngr(seed);
    QCOMPARE(bwrngr.next(), results[1]);
}
