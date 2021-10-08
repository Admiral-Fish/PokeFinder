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

#include "EncounterSlotTest.hpp"
#include <Core/Util/EncounterSlot.hpp>
#include <QTest>
#include <QVector>

void EncounterSlotTest::hSlot_data()
{
    QTest::addColumn<Encounter>("encounter");
    QTest::addColumn<QVector<u16>>("rand");
    QTest::addColumn<QVector<u8>>("results");
}

void EncounterSlotTest::hSlot()
{
    QFETCH(Encounter, encounter);
    QFETCH(QVector<u16>, rand);
    QFETCH(QVector<u8>, results);

    for (size_t i = 0; i < rand.size(); i++)
    {
        QCOMPARE(EncounterSlot::hSlot(rand[i], encounter), results[i]);
    }
}

void EncounterSlotTest::jSlot_data()
{
    QTest::addColumn<Encounter>("encounter");
    QTest::addColumn<QVector<u16>>("rand");
    QTest::addColumn<QVector<u8>>("results");
}

void EncounterSlotTest::jSlot()
{
    QFETCH(Encounter, encounter);
    QFETCH(QVector<u16>, rand);
    QFETCH(QVector<u8>, results);

    for (size_t i = 0; i < rand.size(); i++)
    {
        QCOMPARE(EncounterSlot::jSlot(rand[i], encounter), results[i]);
    }
}

void EncounterSlotTest::kSlot_data()
{
    QTest::addColumn<Encounter>("encounter");
    QTest::addColumn<QVector<u16>>("rand");
    QTest::addColumn<QVector<u8>>("results");
}

void EncounterSlotTest::kSlot()
{
    QFETCH(Encounter, encounter);
    QFETCH(QVector<u16>, rand);
    QFETCH(QVector<u8>, results);

    for (size_t i = 0; i < rand.size(); i++)
    {
        QCOMPARE(EncounterSlot::kSlot(rand[i], encounter), results[i]);
    }
}

void EncounterSlotTest::bdspSlot_data()
{
    QTest::addColumn<Encounter>("encounter");
    QTest::addColumn<QVector<u16>>("rand");
    QTest::addColumn<QVector<u8>>("results");
}

void EncounterSlotTest::bdspSlot()
{
    QFETCH(Encounter, encounter);
    QFETCH(QVector<u16>, rand);
    QFETCH(QVector<u8>, results);

    for (size_t i = 0; i < rand.size(); i++)
    {
        QCOMPARE(EncounterSlot::bdspSlot(rand[i], encounter), results[i]);
    }
}
