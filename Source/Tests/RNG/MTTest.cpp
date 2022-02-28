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

#include "MTTest.hpp"
#include <Core/RNG/MT.hpp>
#include <QTest>

void MTTest::advance_data()
{
    QTest::addColumn<u32>("seed");
    QTest::addColumn<u32>("advances");
    QTest::addColumn<u32>("result");

    QTest::newRow("Advance 1") << 0x00000000U << 624U << 0xE2031CE4U;
    QTest::newRow("Advance 2") << 0x00000000U << 1248U << 0x44463F17U;
    QTest::newRow("Advance 3") << 0x80000000U << 624U << 0x881A2E77U;
    QTest::newRow("Advance 4") << 0x80000000U << 1248U << 0x64C4EF44U;
}

void MTTest::advance()
{
    QFETCH(u32, seed);
    QFETCH(u32, advances);
    QFETCH(u32, result);

    MT rng(seed);
    rng.advance(advances - 1);
    QCOMPARE(rng.next(), result);
}

void MTTest::next_data()
{
    QTest::addColumn<u32>("seed");
    QTest::addColumn<u32>("result");

    QTest::newRow("Next 1") << 0x00000000U << 0x8C7F0AACU;
    QTest::newRow("Next 2") << 0x40000000U << 0x355A22A8U;
    QTest::newRow("Next 3") << 0x80000000U << 0x26E9A91AU;
    QTest::newRow("Next 4") << 0xC0000000U << 0x4CD3ADB9U;
}

void MTTest::next()
{
    QFETCH(u32, seed);
    QFETCH(u32, result);

    MT rng(seed);
    QCOMPARE(rng.next(), result);
}
