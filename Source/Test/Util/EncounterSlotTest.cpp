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
#include <Core/Enum/Encounter.hpp>
#include <Core/Util/EncounterSlot.hpp>
#include <QTest>
#include <QVector>

void EncounterSlotTest::hSlot_data()
{
    QTest::addColumn<Encounter>("encounter");
    QTest::addColumn<QVector<u16>>("rand");

    QTest::newRow("Old Rod") << Encounter::OldRod << QVector<u16> { 0xE2CC, 0x67DB };
    QTest::newRow("Good Rod") << Encounter::GoodRod << QVector<u16> { 0x27A6, 0x375D, 0x82D9 };
    QTest::newRow("Super Rod") << Encounter::SuperRod << QVector<u16> { 0x2B0B, 0xCB60, 0x8D5B, 0x112D, 0x16A7 };
    QTest::newRow("Surfing") << Encounter::Surfing << QVector<u16> { 0xF583, 0x0A70, 0xA855, 0xECB4, 0x16A7 };
    QTest::newRow("Rock Smash") << Encounter::RockSmash << QVector<u16> { 0xF583, 0x0A70, 0xA855, 0xECB4, 0x16A7 };
    QTest::newRow("Grass") << Encounter::Grass << QVector<u16> { 0x0966, 0x172E, 0xD8F5, 0x109F, 0xB585, 0xA58B,
                                                                 0xD217, 0xF60B, 0xBE34, 0xECB4, 0x4522, 0x16A7 };
}

void EncounterSlotTest::hSlot()
{
    QFETCH(Encounter, encounter);
    QFETCH(QVector<u16>, rand);

    for (size_t i = 0; i < rand.size(); i++)
    {
        QCOMPARE(EncounterSlot::hSlot(rand[i], encounter), i);
    }
}

void EncounterSlotTest::jSlot_data()
{
    QTest::addColumn<Encounter>("encounter");
    QTest::addColumn<QVector<u16>>("rand");

    QTest::newRow("Old Rod") << Encounter::OldRod << QVector<u16> { 0x5271, 0xE2CC, 0xF009, 0xF9CB, 0xFEE7 };
    QTest::newRow("Good Rod") << Encounter::GoodRod << QVector<u16> { 0x639E, 0x9268, 0xDFA3, 0xFB23, 0xFEE7 };
    QTest::newRow("Super Rod") << Encounter::SuperRod << QVector<u16> { 0x639E, 0x9268, 0xDFA3, 0xFB23, 0xFEE7 };
    QTest::newRow("Surfing") << Encounter::Surfing << QVector<u16> { 0x5271, 0xE2CC, 0xF009, 0xF9CB, 0xFEE7 };
    QTest::newRow("Grass") << Encounter::Grass << QVector<u16> { 0x1D29, 0x6296, 0x6E03, 0x9268, 0xA397, 0xC3EA,
                                                                 0xD3C5, 0xE0C6, 0xF009, 0xF9CB, 0xFC33, 0xFEE7 };
}

void EncounterSlotTest::jSlot()
{
    QFETCH(Encounter, encounter);
    QFETCH(QVector<u16>, rand);

    for (size_t i = 0; i < rand.size(); i++)
    {
        QCOMPARE(EncounterSlot::jSlot(rand[i], encounter), i);
    }
}

void EncounterSlotTest::kSlot_data()
{
    QTest::addColumn<Encounter>("encounter");
    QTest::addColumn<QVector<u16>>("rand");

    QTest::newRow("Old Rod") << Encounter::OldRod << QVector<u16> { 0x3080, 0xFEE7, 0x48FB, 0xFB23, 0x112D };
    QTest::newRow("Good Rod") << Encounter::GoodRod << QVector<u16> { 0x3080, 0xFEE7, 0x48FB, 0xFB23, 0x112D };
    QTest::newRow("Super Rod") << Encounter::SuperRod << QVector<u16> { 0x3080, 0xFEE7, 0x48FB, 0xFB23, 0x112D };
    QTest::newRow("Surfing") << Encounter::Surfing << QVector<u16> { 0xF583, 0x0A70, 0xA855, 0xECB4, 0x16A7 };
    QTest::newRow("Bug Contest") << Encounter::BugCatchingContest
                                 << QVector<u16> { 0x34A9, 0x2D93, 0x7D3A, 0xBE03, 0x44E5, 0x87A9, 0x7BE7, 0xFC00, 0x4B09, 0x8728 };
    QTest::newRow("Head Butt") << Encounter::Headbutt << QVector<u16> { 0x4744, 0x34F8, 0xA58B, 0xE7F9, 0xA341, 0xECB4 };
    QTest::newRow("Rock Smash") << Encounter::RockSmash << QVector<u16> { 0xA1E0, 0x112D };
    QTest::newRow("Grass") << Encounter::Grass << QVector<u16> { 0x0966, 0x172E, 0xD8F5, 0x109F, 0xB585, 0xA58B,
                                                                 0xD217, 0xF60B, 0xBE34, 0xECB4, 0x4522, 0x16A7 };
}

void EncounterSlotTest::kSlot()
{
    QFETCH(Encounter, encounter);
    QFETCH(QVector<u16>, rand);

    for (size_t i = 0; i < rand.size(); i++)
    {
        QCOMPARE(EncounterSlot::kSlot(rand[i], encounter), i);
    }
}

void EncounterSlotTest::bdspSlot_data()
{
    QTest::addColumn<Encounter>("encounter");
    QTest::addColumn<QVector<u8>>("rand");

    QTest::newRow("Old Rod") << Encounter::OldRod << QVector<u8> { 53, 85, 93, 96, 99 };
    QTest::newRow("Good Rod") << Encounter::GoodRod << QVector<u8> { 28, 55, 88, 97, 99 };
    QTest::newRow("Super Rod") << Encounter::SuperRod << QVector<u8> { 28, 55, 88, 97, 99 };
    QTest::newRow("Surfing") << Encounter::Surfing << QVector<u8> { 53, 85, 93, 96, 99 };
    QTest::newRow("Grass") << Encounter::Grass << QVector<u8> { 15, 34, 46, 53, 66, 73, 82, 87, 92, 96, 98, 99 };
}

void EncounterSlotTest::bdspSlot()
{
    QFETCH(Encounter, encounter);
    QFETCH(QVector<u8>, rand);

    for (size_t i = 0; i < rand.size(); i++)
    {
        QCOMPARE(EncounterSlot::bdspSlot(rand[i], encounter), i);
    }
}
