/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2024 by Admiral_Fish, bumba, and EzPzStreamz
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
#include <Test/Data.hpp>
#include <Test/Enum.hpp>

void EncounterSlotTest::hSlot_data()
{
    QTest::addColumn<Encounter>("encounter");
    QTest::addColumn<std::vector<u16>>("rand");

    json data = readData("encounterslot", "hSlot");
    for (const auto &d : data)
    {
        QTest::newRow(d["name"].get<std::string>().data()) << d["encounter"].get<Encounter>() << d["rand"].get<std::vector<u16>>();
    }
}

void EncounterSlotTest::hSlot()
{
    QFETCH(Encounter, encounter);
    QFETCH(std::vector<u16>, rand);

    for (size_t i = 0; i < rand.size(); i++)
    {
        QCOMPARE(EncounterSlot::hSlot(rand[i] % 100, encounter), i);
    }
}

void EncounterSlotTest::jSlot_data()
{
    QTest::addColumn<Encounter>("encounter");
    QTest::addColumn<std::vector<u16>>("rand");

    json data = readData("encounterslot", "jSlot");
    for (const auto &d : data)
    {
        QTest::newRow(d["name"].get<std::string>().data()) << d["encounter"].get<Encounter>() << d["rand"].get<std::vector<u16>>();
    }
}

void EncounterSlotTest::jSlot()
{
    QFETCH(Encounter, encounter);
    QFETCH(std::vector<u16>, rand);

    for (size_t i = 0; i < rand.size(); i++)
    {
        QCOMPARE(EncounterSlot::jSlot(rand[i] / 656, encounter), i);
    }
}

void EncounterSlotTest::kSlot_data()
{
    QTest::addColumn<Encounter>("encounter");
    QTest::addColumn<std::vector<u16>>("rand");

    json data = readData("encounterslot", "kSlot");
    for (const auto &d : data)
    {
        QTest::newRow(d["name"].get<std::string>().data()) << d["encounter"].get<Encounter>() << d["rand"].get<std::vector<u16>>();
    }
}

void EncounterSlotTest::kSlot()
{
    QFETCH(Encounter, encounter);
    QFETCH(std::vector<u16>, rand);

    for (size_t i = 0; i < rand.size(); i++)
    {
        QCOMPARE(EncounterSlot::kSlot(rand[i] % 100, encounter), i);
    }
}

void EncounterSlotTest::bdspSlot_data()
{
    QTest::addColumn<Encounter>("encounter");
    QTest::addColumn<std::vector<u8>>("rand");

    json data = readData("encounterslot", "bdsp");
    for (const auto &d : data)
    {
        QTest::newRow(d["name"].get<std::string>().data()) << d["encounter"].get<Encounter>() << d["rand"].get<std::vector<u8>>();
    }
}

void EncounterSlotTest::bdspSlot()
{
    QFETCH(Encounter, encounter);
    QFETCH(std::vector<u8>, rand);

    for (size_t i = 0; i < rand.size(); i++)
    {
        QCOMPARE(EncounterSlot::bdspSlot(rand[i], encounter), i);
    }
}
