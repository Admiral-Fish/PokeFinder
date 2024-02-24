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

#include "GameCubeSearcherTest.hpp"
#include <Core/Enum/Game.hpp>
#include <Core/Enum/Method.hpp>
#include <Core/Gen3/Encounters3.hpp>
#include <Core/Gen3/Generators/GameCubeGenerator.hpp>
#include <Core/Gen3/Searchers/GameCubeSearcher.hpp>
#include <Core/Gen3/ShadowTemplate.hpp>
#include <Core/Parents/States/State.hpp>
#include <QTest>
#include <Test/Data.hpp>
#include <Test/Enum.hpp>

using IVs = std::array<u8, 6>;

template <bool compareAbility = true>
static bool operator==(const SearcherState &left, const GeneratorState &right)
{
    if constexpr (compareAbility)
    {
        return left.getPID() == right.getPID() && left.getStats() == right.getStats() && left.getAbilityIndex() == right.getAbilityIndex()
            && left.getIVs() == right.getIVs() && left.getAbility() == right.getAbility() && left.getGender() == right.getGender()
            && left.getHiddenPower() == right.getHiddenPower() && left.getHiddenPowerStrength() == right.getHiddenPowerStrength()
            && left.getLevel() == right.getLevel() && left.getNature() == right.getNature() && left.getShiny() == right.getShiny();
    }
    else
    {
        return left.getPID() == right.getPID() && left.getStats() == right.getStats() && left.getIVs() == right.getIVs()
            && left.getGender() == right.getGender() && left.getHiddenPower() == right.getHiddenPower()
            && left.getHiddenPowerStrength() == right.getHiddenPowerStrength() && left.getLevel() == right.getLevel()
            && left.getNature() == right.getNature() && left.getShiny() == right.getShiny();
    }
}

void GameCubeSearcherTest::searchChannel_data()
{
    QTest::addColumn<IVs>("min");
    QTest::addColumn<IVs>("max");
    QTest::addColumn<int>("results");

    json data = readData("gamecube", "gamecubesearcher", "searchChannel");
    for (const auto &d : data)
    {
        QTest::newRow(d["name"].get<std::string>().data()) << d["min"].get<IVs>() << d["max"].get<IVs>() << d["results"].get<int>();
    }
}

void GameCubeSearcherTest::searchChannel()
{
    QFETCH(IVs, min);
    QFETCH(IVs, max);
    QFETCH(int, results);

    std::array<bool, 25> natures;
    natures.fill(true);

    std::array<bool, 16> powers;
    powers.fill(true);

    Profile3 profile("-", Game::GC, 12345, 54321, false);

    const StaticTemplate *staticTemplate = Encounters3::getStaticEncounter(8, 0);
    StateFilter filter(255, 255, 255, false, min, max, natures, powers);
    GameCubeSearcher searcher(Method::Channel, false, profile, filter);

    searcher.startSearch(min, max, staticTemplate);
    auto states = searcher.getResults();
    QCOMPARE(states.size(), results);

    for (const auto &state : states)
    {
        // Ensure generator agrees
        GameCubeGenerator generator(0, 0, 0, Method::Channel, false, profile, filter);
        auto generatorStates = generator.generate(state.getSeed(), staticTemplate);

        QCOMPARE(generatorStates.size(), 1);
        QVERIFY(state == generatorStates[0]);
    }
}

void GameCubeSearcherTest::searchColoShadow_data()
{
    QTest::addColumn<IVs>("min");
    QTest::addColumn<IVs>("max");
    QTest::addColumn<int>("pokemon");
    QTest::addColumn<int>("results");

    json data = readData("gamecube", "gamecubesearcher", "searchColoShadow");
    for (const auto &d : data)
    {
        QTest::newRow(d["name"].get<std::string>().data())
            << d["min"].get<IVs>() << d["max"].get<IVs>() << d["pokemon"].get<int>() << d["results"].get<int>();
    }
}

void GameCubeSearcherTest::searchColoShadow()
{
    QFETCH(IVs, min);
    QFETCH(IVs, max);
    QFETCH(int, pokemon);
    QFETCH(int, results);

    std::array<bool, 25> natures;
    natures.fill(true);

    std::array<bool, 16> powers;
    powers.fill(true);

    constexpr std::array<u8, 6> zero = { 0, 0, 0, 0, 0, 0 };

    Profile3 profile("-", Game::Colosseum, 12345, 54321, false);

    const ShadowTemplate *shadowTemplate = Encounters3::getShadowTeam(pokemon);
    StateFilter filter(255, 255, 255, false, shadowTemplate->getType() == ShadowType::EReader ? zero : min,
                       shadowTemplate->getType() == ShadowType::EReader ? zero : max, natures, powers);
    GameCubeSearcher searcher(Method::None, false, profile, filter);

    searcher.startSearch(min, max, shadowTemplate);
    auto states = searcher.getResults();
    QCOMPARE(states.size(), results);

    for (const auto &state : states)
    {
        // Ensure generator agrees
        GameCubeGenerator generator(0, 0, 0, Method::None, false, profile, filter);
        auto generatorStates = generator.generate(state.getSeed(), shadowTemplate);

        QCOMPARE(generatorStates.size(), 1);
        if (shadowTemplate->getType() == ShadowType::EReader)
        {
            QVERIFY(operator==<false>(state, generatorStates[0]));
        }
        else
        {
            QVERIFY(state == generatorStates[0]);
        }
    }
}

void GameCubeSearcherTest::searchGalesShadow_data()
{
    QTest::addColumn<IVs>("min");
    QTest::addColumn<IVs>("max");
    QTest::addColumn<bool>("unset");
    QTest::addColumn<int>("pokemon");
    QTest::addColumn<int>("results");

    json data = readData("gamecube", "gamecubesearcher", "searchGalesShadow");
    for (const auto &d : data)
    {
        QTest::newRow(d["name"].get<std::string>().data())
            << d["min"].get<IVs>() << d["max"].get<IVs>() << d["unset"].get<bool>() << d["pokemon"].get<int>() << d["results"].get<int>();
    }
}

void GameCubeSearcherTest::searchGalesShadow()
{
    QFETCH(IVs, min);
    QFETCH(IVs, max);
    QFETCH(bool, unset);
    QFETCH(int, pokemon);
    QFETCH(int, results);

    std::array<bool, 25> natures;
    natures.fill(true);

    std::array<bool, 16> powers;
    powers.fill(true);

    Profile3 profile("-", Game::Gales, 12345, 54321, false);

    const ShadowTemplate *shadowTemplate = Encounters3::getShadowTeam(pokemon);
    StateFilter filter(255, 255, 255, false, min, max, natures, powers);
    GameCubeSearcher searcher(Method::None, unset, profile, filter);

    searcher.startSearch(min, max, shadowTemplate);
    auto states = searcher.getResults();
    QCOMPARE(states.size(), results);

    for (const auto &state : states)
    {
        // Ensure generator agrees
        GameCubeGenerator generator(0, 0, 0, Method::None, unset, profile, filter);
        auto generatorStates = generator.generate(state.getSeed(), shadowTemplate);

        QCOMPARE(generatorStates.size(), 1);
        QVERIFY(state == generatorStates[0]);
    }
}

void GameCubeSearcherTest::searchNonLock_data()
{
    QTest::addColumn<IVs>("min");
    QTest::addColumn<IVs>("max");
    QTest::addColumn<Game>("version");
    QTest::addColumn<int>("pokemon");
    QTest::addColumn<int>("results");

    json data = readData("gamecube", "gamecubesearcher", "searchNonLock");
    for (const auto &d : data)
    {
        QTest::newRow(d["name"].get<std::string>().data())
            << d["min"].get<IVs>() << d["max"].get<IVs>() << d["version"].get<Game>() << d["pokemon"].get<int>() << d["results"].get<int>();
    }
}

void GameCubeSearcherTest::searchNonLock()
{
    QFETCH(IVs, min);
    QFETCH(IVs, max);
    QFETCH(Game, version);
    QFETCH(int, pokemon);
    QFETCH(int, results);

    std::array<bool, 25> natures;
    natures.fill(true);

    std::array<bool, 16> powers;
    powers.fill(true);

    Profile3 profile("-", version, 12345, 54321, false);

    const StaticTemplate *staticTemplate = Encounters3::getStaticEncounter(7, pokemon);
    StateFilter filter(255, 255, 255, false, min, max, natures, powers);
    GameCubeSearcher searcher(Method::None, false, profile, filter);

    searcher.startSearch(min, max, staticTemplate);
    auto states = searcher.getResults();
    QCOMPARE(states.size(), results);

    for (const auto &state : states)
    {
        // Ensure generator agrees
        GameCubeGenerator generator(0, 0, 0, Method::None, false, profile, filter);
        auto generatorStates = generator.generate(state.getSeed(), staticTemplate);

        QCOMPARE(generatorStates.size(), 1);
        QVERIFY(state == generatorStates[0]);
    }
}
