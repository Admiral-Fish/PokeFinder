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

#include "IVCheckerTest.hpp"
#include <Core/Enum/Game.hpp>
#include <Core/Parents/PersonalInfo.hpp>
#include <Core/Parents/PersonalLoader.hpp>
#include <Core/Util/IVChecker.hpp>
#include <QTest>
#include <Test/Data.hpp>
#include <Test/Enum.hpp>

using Stats = std::vector<std::array<u16, 6>>;
using Levels = std::vector<u8>;
using Results = std::array<std::vector<u8>, 6>;

void IVCheckerTest::calculateIVRange_data()
{
    QTest::addColumn<u16>("specie");
    QTest::addColumn<u8>("form");
    QTest::addColumn<Game>("version");
    QTest::addColumn<Stats>("stats");
    QTest::addColumn<Levels>("levels");
    QTest::addColumn<u8>("nature");
    QTest::addColumn<u8>("characteristic");
    QTest::addColumn<u8>("hiddenPower");
    QTest::addColumn<Results>("results");

    json data = readData("ivchecker", "calculateIVRange");
    for (const auto &d : data)
    {
        QTest::newRow(d["name"].get<std::string>().data())
            << d["specie"].get<u16>() << d["form"].get<u8>() << getGame(d["version"].get<std::string>()) << d["stats"].get<Stats>()
            << d["levels"].get<Levels>() << d["nature"].get<u8>() << d["characteristic"].get<u8>() << d["hiddenPower"].get<u8>()
            << d["results"].get<Results>();
    }
}

void IVCheckerTest::calculateIVRange()
{
    QFETCH(u16, specie);
    QFETCH(u8, form);
    QFETCH(Game, version);
    QFETCH(Stats, stats);
    QFETCH(Levels, levels);
    QFETCH(u8, nature);
    QFETCH(u8, characteristic);
    QFETCH(u8, hiddenPower);
    QFETCH(Results, results);

    const PersonalInfo *info = PersonalLoader::getPersonal(version, specie, form);
    auto range = IVChecker::calculateIVRange(info->getStats(), stats, levels, nature, characteristic, hiddenPower);

    QCOMPARE(range, results);
}
