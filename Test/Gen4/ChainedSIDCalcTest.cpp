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

#include "ChainedSIDCalcTest.hpp"
#include <Core/Enum/Game.hpp>
#include <Core/Gen4/Tools/ChainedSIDCalc.hpp>
#include <Core/Parents/PersonalInfo.hpp>
#include <Core/Parents/PersonalLoader.hpp>
#include <QTest>
#include <Test/Data.hpp>
#include <Test/Enum.hpp>

using IVs = std::vector<std::array<u8, 6>>;

void ChainedSIDCalcTest::addEntry_data()
{
    QTest::addColumn<u16>("tid");
    QTest::addColumn<Game>("version");
    QTest::addColumn<u16>("pokemon");
    QTest::addColumn<std::vector<u8>>("ability");
    QTest::addColumn<IVs>("ivs");
    QTest::addColumn<std::vector<u8>>("gender");
    QTest::addColumn<std::vector<u8>>("nature");
    QTest::addColumn<u16>("result");

    json data = readData("chainedsid", "addentry");
    for (const auto &d : data)
    {
        QTest::newRow(d["name"].get<std::string>().data())
            << d["tid"].get<u16>() << d["version"].get<Game>() << d["pokemon"].get<u16>() << d["ability"].get<std::vector<u8>>()
            << d["ivs"].get<IVs>() << d["gender"].get<std::vector<u8>>() << d["nature"].get<std::vector<u8>>() << d["result"].get<u16>();
    }
}

void ChainedSIDCalcTest::addEntry()
{
    QFETCH(u16, tid);
    QFETCH(Game, version);
    QFETCH(u16, pokemon);
    QFETCH(std::vector<u8>, ability);
    QFETCH(IVs, ivs);
    QFETCH(std::vector<u8>, gender);
    QFETCH(std::vector<u8>, nature);
    QFETCH(u16, result);

    QVERIFY(ability.size() == ivs.size());
    QVERIFY(ivs.size() == gender.size());
    QVERIFY(gender.size() == nature.size());

    const PersonalInfo *info = PersonalLoader::getPersonal(version, pokemon);

    ChainedSIDCalc calc(tid);
    for (int i = 0; i < ability.size(); i++)
    {
        calc.addEntry(ivs[i][0], ivs[i][1], ivs[i][2], ivs[i][3], ivs[i][4], ivs[i][5], info->getAbility(ability[i]), gender[i], nature[i],
                      info);
    }

    auto sids = calc.getSIDs();
    QVERIFY(sids.size() == 1);

    QVERIFY(sids[0] == result);
}
