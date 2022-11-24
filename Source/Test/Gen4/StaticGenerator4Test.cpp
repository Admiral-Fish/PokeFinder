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

#include "StaticGenerator4Test.hpp"
#include <Core/Enum/Game.hpp>
#include <Core/Enum/Lead.hpp>
#include <Core/Enum/Method.hpp>
#include <Core/Gen4/Encounters4.hpp>
#include <Core/Gen4/Generators/StaticGenerator4.hpp>
#include <Core/Gen4/Profile4.hpp>
#include <Core/Gen4/States/State4.hpp>
#include <QTest>
#include <Test/Data.hpp>
#include <Test/Enum.hpp>

static bool operator==(const GeneratorState4 &left, const json &right)
{
    return left.getPID() == right["pid"].get<u32>() && left.getStats() == right["stats"].get<std::array<u16, 6>>()
        && left.getAbilityIndex() == right["abilityIndex"].get<u16>() && left.getIVs() == right["ivs"].get<std::array<u8, 6>>()
        && left.getAbility() == right["ability"].get<u8>() && left.getGender() == right["gender"].get<u8>()
        && left.getHiddenPower() == right["hiddenPower"].get<u8>() && left.getNature() == right["nature"].get<u8>()
        && left.getLevel() == right["level"].get<u8>() && left.getShiny() == right["shiny"].get<u8>()
        && left.getAdvances() == right["advances"].get<u32>() && left.getCall() == right["call"].get<u8>()
        && left.getChatot() == right["chatot"].get<u8>() && left.getHiddenPowerStrength() == right["hiddenPowerStrength"].get<u8>();
}

void StaticGenerator4Test::generateMethod1_data()
{
    QTest::addColumn<u32>("seed");
    QTest::addColumn<Game>("version");
    QTest::addColumn<int>("category");
    QTest::addColumn<int>("pokemon");
    QTest::addColumn<std::string>("results");

    json data = readData("static4", "staticgenerator4", "generateMethod1");
    for (const auto &d : data)
    {
        QTest::newRow(d["name"].get<std::string>().data())
            << d["seed"].get<u32>() << getGame(d["version"].get<std::string>()) << d["category"].get<int>() << d["pokemon"].get<int>()
            << d["results"].get<json>().dump();
    }
}

void StaticGenerator4Test::generateMethod1()
{
    QFETCH(u32, seed);
    QFETCH(Game, version);
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

    const StaticTemplate4 *staticTemplate = Encounters4::getStaticEncounter(category, pokemon);
    StateFilter4 filter(255, 255, 255, false, min, max, natures, powers);
    StaticGenerator4 generator(0, 9, 0, 12345, 54321, version, Method::Method1, Lead::None, filter);

    auto states = generator.generate(seed, staticTemplate);
    QCOMPARE(states.size(), j.size());

    for (size_t i = 0; i < states.size(); i++)
    {
        const auto &state = states[i];
        QVERIFY(state == j[i]);
    }
}

void StaticGenerator4Test::generateMethodJ_data()
{
    QTest::addColumn<u32>("seed");
    QTest::addColumn<Game>("version");
    QTest::addColumn<int>("category");
    QTest::addColumn<int>("pokemon");
    QTest::addColumn<Lead>("lead");
    QTest::addColumn<std::string>("results");

    json data = readData("static4", "staticgenerator4", "generateMethodJ");
    for (const auto &d : data)
    {
        QTest::newRow(d["name"].get<std::string>().data())
            << d["seed"].get<u32>() << getGame(d["version"].get<std::string>()) << d["category"].get<int>() << d["pokemon"].get<int>()
            << getLead(d["lead"].get<std::string>()) << d["results"].get<json>().dump();
    }
}

void StaticGenerator4Test::generateMethodJ()
{
    QFETCH(u32, seed);
    QFETCH(Game, version);
    QFETCH(int, category);
    QFETCH(int, pokemon);
    QFETCH(Lead, lead);
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

    const StaticTemplate4 *staticTemplate = Encounters4::getStaticEncounter(category, pokemon);
    StateFilter4 filter(255, 255, 255, false, min, max, natures, powers);
    StaticGenerator4 generator(0, 9, 0, 12345, 54321, version, Method::MethodJ, lead, filter);

    auto states = generator.generate(seed, staticTemplate);
    QCOMPARE(states.size(), j.size());

    for (size_t i = 0; i < states.size(); i++)
    {
        const auto &state = states[i];
        QVERIFY(state == j[i]);
    }
}

void StaticGenerator4Test::generateMethodK_data()
{
    QTest::addColumn<u32>("seed");
    QTest::addColumn<Game>("version");
    QTest::addColumn<int>("category");
    QTest::addColumn<int>("pokemon");
    QTest::addColumn<Lead>("lead");
    QTest::addColumn<std::string>("results");

    json data = readData("static4", "staticgenerator4", "generateMethodK");
    for (const auto &d : data)
    {
        QTest::newRow(d["name"].get<std::string>().data())
            << d["seed"].get<u32>() << getGame(d["version"].get<std::string>()) << d["category"].get<int>() << d["pokemon"].get<int>()
            << getLead(d["lead"].get<std::string>()) << d["results"].get<json>().dump();
    }
}

void StaticGenerator4Test::generateMethodK()
{
    QFETCH(u32, seed);
    QFETCH(Game, version);
    QFETCH(int, category);
    QFETCH(int, pokemon);
    QFETCH(Lead, lead);
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

    const StaticTemplate4 *staticTemplate = Encounters4::getStaticEncounter(category, pokemon);
    StateFilter4 filter(255, 255, 255, false, min, max, natures, powers);
    StaticGenerator4 generator(0, 9, 0, 12345, 54321, version, Method::MethodK, lead, filter);

    auto states = generator.generate(seed, staticTemplate);
    QCOMPARE(states.size(), j.size());

    for (size_t i = 0; i < states.size(); i++)
    {
        const auto &state = states[i];
        QVERIFY(state == j[i]);
    }
}
