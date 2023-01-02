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

#include "EventGenerator8Test.hpp"
#include <Core/Enum/Game.hpp>
#include <Core/Gen8/Generators/EventGenerator8.hpp>
#include <Core/Gen8/WB8.hpp>
#include <Core/Parents/States/State.hpp>
#include <QTest>
#include <Test/Data.hpp>

static bool operator==(const GeneratorState &left, const json &right)
{
    // Intentionally ignoring hidden power
    return left.getPID() == right["pid"].get<u32>() && left.getStats() == right["stats"].get<std::array<u16, 6>>()
        && left.getAbilityIndex() == right["abilityIndex"].get<u16>() && left.getIVs() == right["ivs"].get<std::array<u8, 6>>()
        && left.getAbility() == right["ability"].get<u8>() && left.getGender() == right["gender"].get<u8>()
        && left.getLevel() == right["level"].get<u8>() && left.getNature() == right["nature"].get<u8>()
        && left.getShiny() == right["shiny"].get<u8>() && left.getAdvances() == right["advances"].get<u32>();
}

void EventGenerator8Test::generate_data()
{
    QTest::addColumn<u64>("seed0");
    QTest::addColumn<u64>("seed1");
    QTest::addColumn<u16>("tid");
    QTest::addColumn<u16>("sid");
    QTest::addColumn<u32>("ec");
    QTest::addColumn<u32>("pid");
    QTest::addColumn<u16>("specie");
    QTest::addColumn<u8>("form");
    QTest::addColumn<u8>("gender");
    QTest::addColumn<bool>("egg");
    QTest::addColumn<u8>("nature");
    QTest::addColumn<u8>("ability");
    QTest::addColumn<u8>("shiny");
    QTest::addColumn<u8>("ivCount");
    QTest::addColumn<u8>("level");
    QTest::addColumn<std::string>("results");

    json data = readData("event8", "generate");
    for (const auto &d : data)
    {
        QTest::newRow(d["name"].get<std::string>().data())
            << d["seed0"].get<u64>() << d["seed1"].get<u64>() << d["tid"].get<u16>() << d["sid"].get<u16>() << d["ec"].get<u32>()
            << d["pid"].get<u32>() << d["specie"].get<u16>() << d["form"].get<u8>() << d["gender"].get<u8>() << d["egg"].get<bool>()
            << d["nature"].get<u8>() << d["ability"].get<u8>() << d["shiny"].get<u8>() << d["ivCount"].get<u8>() << d["level"].get<u8>()
            << d["results"].get<json>().dump();
    }
}

void EventGenerator8Test::generate()
{
    QFETCH(u64, seed0);
    QFETCH(u64, seed1);
    QFETCH(u16, tid);
    QFETCH(u16, sid);
    QFETCH(u32, ec);
    QFETCH(u32, pid);
    QFETCH(u16, specie);
    QFETCH(u8, form);
    QFETCH(u8, gender);
    QFETCH(bool, egg);
    QFETCH(u8, nature);
    QFETCH(u8, ability);
    QFETCH(u8, shiny);
    QFETCH(u8, ivCount);
    QFETCH(u8, level);
    QFETCH(std::string, results);

    json j = json::parse(results);

    std::array<u8, 6> min;
    min.fill(0);

    std::array<u8, 6> max;
    max.fill(31);

    std::array<bool, 25> natures;
    natures.fill(true);

    std::array<bool, 16> powers;
    powers.fill(true);

    Profile8 profile("-", Game::BD, 12345, 54321, false, false, false);

    WB8 wb8(tid, sid, ec, pid, specie, form, gender, egg, nature, ability, shiny, ivCount, level);
    StateFilter8 filter(255, 255, 255, false, min, max, natures, powers);
    EventGenerator8 generator(0, 9, 0, wb8, profile, filter);

    auto states = generator.generate(seed0, seed1);
    QCOMPARE(states.size(), j.size());

    for (size_t i = 0; i < states.size(); i++)
    {
        const auto &state = states[i];
        QVERIFY(state == j[i]);
    }
}
