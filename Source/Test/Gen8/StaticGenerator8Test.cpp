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

#include "StaticGenerator8Test.hpp"
#include <Core/Enum/Game.hpp>
#include <Core/Enum/Lead.hpp>
#include <Core/Gen8/Encounters8.hpp>
#include <Core/Gen8/Generators/StaticGenerator8.hpp>
#include <Core/Parents/States/State.hpp>
#include <QTest>
#include <Test/Data.hpp>
#include <Test/Enum.hpp>

static bool operator==(const GeneratorState &left, const json &right)
{
    // Intentionally ignoring hidden power
    return left.getEC() == right["ec"].get<u32>() && left.getPID() == right["pid"].get<u32>()
        && left.getStats() == right["stats"].get<std::array<u16, 6>>() && left.getAbilityIndex() == right["abilityIndex"].get<u16>()
        && left.getIVs() == right["ivs"].get<std::array<u8, 6>>() && left.getAbility() == right["ability"].get<u8>()
        && left.getCharacteristic() == right["characteristic"].get<u8>() && left.getGender() == right["gender"].get<u8>()
        && left.getLevel() == right["level"].get<u8>() && left.getNature() == right["nature"].get<u8>()
        && left.getShiny() == right["shiny"].get<u8>() && left.getAdvances() == right["advances"].get<u32>();
}

void StaticGenerator8Test::generate_data()
{
    QTest::addColumn<u64>("seed0");
    QTest::addColumn<u64>("seed1");
    QTest::addColumn<Lead>("lead");
    QTest::addColumn<int>("category");
    QTest::addColumn<int>("pokemon");
    QTest::addColumn<std::string>("results");

    json data = readData("static8", "generate");
    for (const auto &d : data)
    {
        QTest::newRow(d["name"].get<std::string>().data())
            << d["seed0"].get<u64>() << d["seed1"].get<u64>() << getLead(d["lead"].get<std::string>()) << d["category"].get<int>()
            << d["pokemon"].get<int>() << d["results"].get<json>().dump();
    }
}

void StaticGenerator8Test::generate()
{
    QFETCH(u64, seed0);
    QFETCH(u64, seed1);
    QFETCH(Lead, lead);
    QFETCH(int, category);
    QFETCH(int, pokemon);
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

    Profile8 profile("-", Game::BDSP, 12345, 54321, false, false, false);

    const StaticTemplate *staticTemplate = Encounters8::getStaticEncounter(category, pokemon);
    StateFilter8 filter(255, 255, 255, false, min, max, natures, powers);
    StaticGenerator8 generator(0, 9, 0, lead, profile, filter);

    auto states = generator.generate(seed0, seed1, staticTemplate);
    QCOMPARE(states.size(), j.size());

    for (size_t i = 0; i < states.size(); i++)
    {
        const auto &state = states[i];
        QVERIFY(state == j[i]);
    }
}

void StaticGenerator8Test::generateRoamer_data()
{
    QTest::addColumn<u64>("seed0");
    QTest::addColumn<u64>("seed1");
    QTest::addColumn<int>("category");
    QTest::addColumn<int>("pokemon");
    QTest::addColumn<std::string>("results");

    json data = readData("static8", "generateRoamer");
    for (const auto &d : data)
    {
        QTest::newRow(d["name"].get<std::string>().data()) << d["seed0"].get<u64>() << d["seed1"].get<u64>() << d["category"].get<int>()
                                                           << d["pokemon"].get<int>() << d["results"].get<json>().dump();
    }
}

void StaticGenerator8Test::generateRoamer()
{
    QFETCH(u64, seed0);
    QFETCH(u64, seed1);
    QFETCH(int, category);
    QFETCH(int, pokemon);
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

    const StaticTemplate *staticTemplate = Encounters8::getStaticEncounter(category, pokemon);
    StateFilter8 filter(255, 255, 255, false, min, max, natures, powers);
    StaticGenerator8 generator(0, 9, 0, Lead::None, profile, filter);

    auto states = generator.generateRoamer(seed0, seed1, staticTemplate);
    QCOMPARE(states.size(), j.size());

    for (size_t i = 0; i < states.size(); i++)
    {
        const auto &state = states[i];
        QVERIFY(state == j[i]);
    }
}
