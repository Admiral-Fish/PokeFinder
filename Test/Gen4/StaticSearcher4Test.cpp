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

#include "StaticSearcher4Test.hpp"
#include <Core/Enum/Game.hpp>
#include <Core/Enum/Lead.hpp>
#include <Core/Enum/Method.hpp>
#include <Core/Gen4/Encounters4.hpp>
#include <Core/Gen4/Generators/StaticGenerator4.hpp>
#include <Core/Gen4/Profile4.hpp>
#include <Core/Gen4/Searchers/StaticSearcher4.hpp>
#include <Core/Gen4/States/State4.hpp>
#include <QTest>
#include <Test/Data.hpp>
#include <Test/Enum.hpp>

using IVs = std::array<u8, 6>;

static bool operator==(const SearcherState4 &left, const GeneratorState4 &right)
{
    return left.getPID() == right.getPID() && left.getStats() == right.getStats() && left.getAbilityIndex() == right.getAbilityIndex()
        && left.getIVs() == right.getIVs() && left.getAbility() == right.getAbility()
        && left.getCharacteristic() == right.getCharacteristic() && left.getGender() == right.getGender()
        && left.getHiddenPower() == right.getHiddenPower() && left.getHiddenPowerStrength() == right.getHiddenPowerStrength()
        && left.getLevel() == right.getLevel() && left.getNature() == right.getNature() && left.getShiny() == right.getShiny();
}

static constexpr Lead operator+(Lead lead, u8 val)
{
    return static_cast<Lead>(toInt(lead) + val);
}

void StaticSearcher4Test::searchMethod1_data()
{
    QTest::addColumn<IVs>("min");
    QTest::addColumn<IVs>("max");
    QTest::addColumn<u32>("minAdvance");
    QTest::addColumn<u32>("maxAdvance");
    QTest::addColumn<u32>("minDelay");
    QTest::addColumn<u32>("maxDelay");
    QTest::addColumn<Game>("version");
    QTest::addColumn<int>("category");
    QTest::addColumn<int>("pokemon");
    QTest::addColumn<int>("results");

    json data = readData("static4", "staticsearcher4", "searchMethod1");
    for (const auto &d : data)
    {
        QTest::newRow(d["name"].get<std::string>().data())
            << d["min"].get<IVs>() << d["max"].get<IVs>() << d["minAdvance"].get<u32>() << d["maxAdvance"].get<u32>()
            << d["minDelay"].get<u32>() << d["maxDelay"].get<u32>() << d["version"].get<Game>() << d["category"].get<int>()
            << d["pokemon"].get<int>() << d["results"].get<int>();
    }
}

void StaticSearcher4Test::searchMethod1()
{
    QFETCH(IVs, min);
    QFETCH(IVs, max);
    QFETCH(u32, minAdvance);
    QFETCH(u32, maxAdvance);
    QFETCH(u32, minDelay);
    QFETCH(u32, maxDelay);
    QFETCH(Game, version);
    QFETCH(int, category);
    QFETCH(int, pokemon);
    QFETCH(int, results);

    std::array<bool, 25> natures;
    natures.fill(true);

    std::array<bool, 16> powers;
    powers.fill(true);

    Profile4 profile("-", version, 12345, 54321, false);

    const StaticTemplate4 *staticTemplate = Encounters4::getStaticEncounter(category, pokemon);
    StateFilter filter(255, 255, 255, 0, 255, 0, 255, false, min, max, natures, powers);
    StaticSearcher4 searcher(minAdvance, maxAdvance, minDelay, maxDelay, Method::Method1, Lead::None, profile, filter);

    searcher.startSearch(min, max, staticTemplate);
    auto states = searcher.getResults();
    QCOMPARE(states.size(), results);

    for (const auto &state : states)
    {
        // Ensure generator agrees
        StaticGenerator4 generator(state.getAdvances(), 0, 0, Method::Method1, Lead::None, *staticTemplate, profile, filter);
        auto generatorStates = generator.generate(state.getSeed());

        QCOMPARE(generatorStates.size(), 1);
        QVERIFY(state == generatorStates[0]);
    }
}

void StaticSearcher4Test::searchMethodJ_data()
{
    QTest::addColumn<IVs>("min");
    QTest::addColumn<IVs>("max");
    QTest::addColumn<u32>("minAdvance");
    QTest::addColumn<u32>("maxAdvance");
    QTest::addColumn<u32>("minDelay");
    QTest::addColumn<u32>("maxDelay");
    QTest::addColumn<Game>("version");
    QTest::addColumn<Lead>("lead");
    QTest::addColumn<int>("category");
    QTest::addColumn<int>("pokemon");
    QTest::addColumn<int>("results");

    json data = readData("static4", "staticsearcher4", "searchMethodJ");
    for (const auto &d : data)
    {
        QTest::newRow(d["name"].get<std::string>().data())
            << d["min"].get<IVs>() << d["max"].get<IVs>() << d["minAdvance"].get<u32>() << d["maxAdvance"].get<u32>()
            << d["minDelay"].get<u32>() << d["maxDelay"].get<u32>() << d["version"].get<Game>() << d["lead"].get<Lead>()
            << d["category"].get<int>() << d["pokemon"].get<int>() << d["results"].get<int>();
    }
}

void StaticSearcher4Test::searchMethodJ()
{
    QFETCH(IVs, min);
    QFETCH(IVs, max);
    QFETCH(u32, minAdvance);
    QFETCH(u32, maxAdvance);
    QFETCH(u32, minDelay);
    QFETCH(u32, maxDelay);
    QFETCH(Game, version);
    QFETCH(Lead, lead);
    QFETCH(int, category);
    QFETCH(int, pokemon);
    QFETCH(int, results);

    std::array<bool, 25> natures;
    natures.fill(true);

    std::array<bool, 16> powers;
    powers.fill(true);

    Profile4 profile("-", version, 12345, 54321, false);

    const StaticTemplate4 *staticTemplate = Encounters4::getStaticEncounter(category, pokemon);
    StateFilter filter(255, 255, 255, 0, 255, 0, 255, false, min, max, natures, powers);
    StaticSearcher4 searcher(minAdvance, maxAdvance, minDelay, maxDelay, Method::MethodJ, lead, profile, filter);

    searcher.startSearch(min, max, staticTemplate);
    auto states = searcher.getResults();
    QCOMPARE(states.size(), results);

    for (const auto &state : states)
    {
        // Ensure generator agrees
        StaticGenerator4 generator(state.getAdvances(), 0, 0, Method::MethodJ, lead != Lead::Synchronize ? lead : lead + state.getNature(),
                                   *staticTemplate, profile, filter);
        auto generatorStates = generator.generate(state.getSeed());

        QCOMPARE(generatorStates.size(), 1);
        QVERIFY(state == generatorStates[0]);
    }
}

void StaticSearcher4Test::searchMethodK_data()
{
    QTest::addColumn<IVs>("min");
    QTest::addColumn<IVs>("max");
    QTest::addColumn<u32>("minAdvance");
    QTest::addColumn<u32>("maxAdvance");
    QTest::addColumn<u32>("minDelay");
    QTest::addColumn<u32>("maxDelay");
    QTest::addColumn<Game>("version");
    QTest::addColumn<Lead>("lead");
    QTest::addColumn<int>("category");
    QTest::addColumn<int>("pokemon");
    QTest::addColumn<int>("results");

    json data = readData("static4", "staticsearcher4", "searchMethodK");
    for (const auto &d : data)
    {
        QTest::newRow(d["name"].get<std::string>().data())
            << d["min"].get<IVs>() << d["max"].get<IVs>() << d["minAdvance"].get<u32>() << d["maxAdvance"].get<u32>()
            << d["minDelay"].get<u32>() << d["maxDelay"].get<u32>() << d["version"].get<Game>() << d["lead"].get<Lead>()
            << d["category"].get<int>() << d["pokemon"].get<int>() << d["results"].get<int>();
    }
}

void StaticSearcher4Test::searchMethodK()
{
    QFETCH(IVs, min);
    QFETCH(IVs, max);
    QFETCH(u32, minAdvance);
    QFETCH(u32, maxAdvance);
    QFETCH(u32, minDelay);
    QFETCH(u32, maxDelay);
    QFETCH(Game, version);
    QFETCH(Lead, lead);
    QFETCH(int, category);
    QFETCH(int, pokemon);
    QFETCH(int, results);

    std::array<bool, 25> natures;
    natures.fill(true);

    std::array<bool, 16> powers;
    powers.fill(true);

    Profile4 profile("-", version, 12345, 54321, false);

    const StaticTemplate4 *staticTemplate = Encounters4::getStaticEncounter(category, pokemon);
    StateFilter filter(255, 255, 255, 0, 255, 0, 255, false, min, max, natures, powers);
    StaticSearcher4 searcher(minAdvance, maxAdvance, minDelay, maxDelay, Method::MethodK, lead, profile, filter);

    searcher.startSearch(min, max, staticTemplate);
    auto states = searcher.getResults();
    QCOMPARE(states.size(), results);

    for (const auto &state : states)
    {
        // Ensure generator agrees
        StaticGenerator4 generator(state.getAdvances(), 0, 0, Method::MethodK, lead != Lead::Synchronize ? lead : lead + state.getNature(),
                                   *staticTemplate, profile, filter);
        auto generatorStates = generator.generate(state.getSeed());

        QCOMPARE(generatorStates.size(), 1);
        QVERIFY(state == generatorStates[0]);
    }
}
