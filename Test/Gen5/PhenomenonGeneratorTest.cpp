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

#include "PhenomenonGeneratorTest.hpp"
#include <Core/Enum/Game.hpp>
#include <Core/Gen5/EncounterArea5.hpp>
#include <Core/Gen5/Encounters5.hpp>
#include <Core/Gen5/Generators/PhenomenonGenerator.hpp>
#include <Core/Gen5/Profile5.hpp>
#include <Core/Gen5/States/PhenomenonState.hpp>
#include <QTest>
#include <Test/Data.hpp>
#include <Test/Enum.hpp>

static bool operator==(const PhenomenonState &left, const json &right)
{
    return left.getAdvances() == right["advances"].get<u32>() && left.getItem() == right["item"].get<u16>()
        && left.getChatot() == right["chatot"].get<u8>() && left.isValid() == right["valid"].get<bool>();
}

void PhenomenonGeneratorTest::generate_data()
{
    QTest::addColumn<u64>("seed");
    QTest::addColumn<Game>("version");
    QTest::addColumn<int>("location");
    QTest::addColumn<std::string>("results");

    json data = readData("phenomenon", "generate");
    for (const auto &d : data)
    {
        QTest::newRow(d["name"].get<std::string>().data())
            << d["seed"].get<u64>() << d["version"].get<Game>() << d["location"].get<int>() << d["results"].get<json>().dump();
    }
}

void PhenomenonGeneratorTest::generate()
{
    QFETCH(u64, seed);
    QFETCH(Game, version);
    QFETCH(int, location);
    QFETCH(std::string, results);

    json j = json::parse(results);

    Profile5 profile("-", version, 12345, 54321, "", "", 0, { false, false, false, false, false, false, false, false, false }, 0, 0, 0,
                     false, 0, 0, false, false, false, DSType::DS, Language::English);

    auto areas = Encounters5::getPhenomenonEncounters(&profile);
    auto area = std::ranges::find_if(areas, [location](const auto &area) { return area.getLocation() == location; });

    auto items = area->getUniqueItems((version & Game::BW) != Game::None);

    PhenomenonFilter filter(items);
    PhenomenonGenerator generator(0, 9, 0, *area, profile, filter);

    auto states = generator.generate(seed);
    QCOMPARE(states.size(), j.size());

    for (size_t i = 0; i < states.size(); i++)
    {
        const auto &state = states[i];
        QVERIFY(state == j[i]);
    }
}
