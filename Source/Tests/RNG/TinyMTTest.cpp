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

#include "TinyMTTest.hpp"
#include <Core/RNG/TinyMT.hpp>
#include <QTest>

void TinyMTTest::advance_data()
{
    QTest::addColumn<u32>("seed");
    QTest::addColumn<u32>("advances");
    QTest::addColumn<u32>("result");

    QTest::newRow("Advance 1") << 0x00000000U << 5U << 0x8D5F56B0U;
    QTest::newRow("Advance 2") << 0x00000000U << 10U << 0x4315A2A3U;
    QTest::newRow("Advance 3") << 0x80000000U << 5U << 0x4C4DDD7CU;
    QTest::newRow("Advance 4") << 0x80000000U << 10U << 0x527971DAU;
}

void TinyMTTest::advance()
{
    QFETCH(u32, seed);
    QFETCH(u32, advances);
    QFETCH(u32, result);

    TinyMT rng(seed);
    rng.advance(advances - 1);
    QCOMPARE(rng.next(), result);
}

void TinyMTTest::next_data()
{
    QTest::addColumn<u32>("seed");
    QTest::addColumn<u32>("result");

    QTest::newRow("Next 1") << 0x00000000U << 0x7C159927U;
    QTest::newRow("Next 2") << 0x40000000U << 0x28EF00ACU;
    QTest::newRow("Next 3") << 0x80000000U << 0xCBD0CB62U;
    QTest::newRow("Next 4") << 0xC0000000U << 0xBD61B252U;
}

void TinyMTTest::next()
{
    QFETCH(u32, seed);
    QFETCH(u32, result);

    TinyMT rng(seed);
    QCOMPARE(rng.next(), result);
}
