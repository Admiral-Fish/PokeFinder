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

static bool operator==(const SearcherState &left, const GeneratorState &right)
{
    return left.getPID() == right.getPID() && left.getStats() == right.getStats() && left.getAbilityIndex() == right.getAbilityIndex()
        && left.getIVs() == right.getIVs() && left.getAbility() == right.getAbility() && left.getGender() == right.getGender()
        && left.getHiddenPower() == right.getHiddenPower() && left.getHiddenPowerStrength() == right.getHiddenPowerStrength()
        && left.getLevel() == right.getLevel() && left.getNature() == right.getNature() && left.getShiny() == right.getShiny();
}

static bool operator==(const SearcherState &left, const json &right)
{
    return left.getPID() == right["pid"].get<u32>() && left.getStats() == right["stats"].get<std::array<u16, 6>>()
        && left.getAbilityIndex() == right["abilityIndex"].get<u16>() && left.getIVs() == right["ivs"].get<std::array<u8, 6>>()
        && left.getAbility() == right["ability"].get<u8>() && left.getGender() == right["gender"].get<u8>()
        && left.getHiddenPower() == right["hiddenPower"].get<u8>()
        && left.getHiddenPowerStrength() == right["hiddenPowerStrength"].get<u8>() && left.getLevel() == right["level"].get<u8>()
        && left.getNature() == right["nature"].get<u8>() && left.getShiny() == right["shiny"].get<u8>()
        && left.getSeed() == right["seed"].get<u32>();
}

void GameCubeSearcherTest::searchChannel_data()
{
    QTest::addColumn<IVs>("min");
    QTest::addColumn<IVs>("max");
    QTest::addColumn<std::string>("results");

    json data = readData("gamecube", "gamecubesearcher", "searchChannel");
    for (const auto &d : data)
    {
        QTest::newRow(d["name"].get<std::string>().data()) << d["min"].get<IVs>() << d["max"].get<IVs>() << d["results"].get<json>().dump();
    }
}

void GameCubeSearcherTest::searchChannel()
{
    QFETCH(IVs, min);
    QFETCH(IVs, max);
    QFETCH(std::string, results);

    json j = json::parse(results);

    std::array<bool, 25> natures;
    natures.fill(true);

    std::array<bool, 16> powers;
    powers.fill(true);

    Profile3 profile("-", Game::GC, 12345, 54321, false);

    const StaticTemplate *staticTemplate = Encounters3::getStaticEncounter(8, 0);
    StateFilter3 filter(255, 255, 255, false, min, max, natures, powers);
    GameCubeSearcher searcher(Method::Channel, false, profile, filter);

    searcher.startSearch(min, max, staticTemplate);
    auto states = searcher.getResults();
    QCOMPARE(states.size(), j.size());

    for (int i = 0; i < states.size(); i++)
    {
        const auto &state = states[i];
        QVERIFY(state == j[i]);
    }
}

void GameCubeSearcherTest::searchColoShadow_data()
{
    QTest::addColumn<IVs>("min");
    QTest::addColumn<IVs>("max");
    QTest::addColumn<int>("pokemon");
    QTest::addColumn<std::string>("results");

    json data = readData("gamecube", "gamecubesearcher", "searchColoShadow");
    for (const auto &d : data)
    {
        QTest::newRow(d["name"].get<std::string>().data())
            << d["min"].get<IVs>() << d["max"].get<IVs>() << d["pokemon"].get<int>() << d["results"].get<json>().dump();
    }
}

void GameCubeSearcherTest::searchColoShadow()
{
    QFETCH(IVs, min);
    QFETCH(IVs, max);
    QFETCH(int, pokemon);
    QFETCH(std::string, results);

    json j = json::parse(results);

    std::array<bool, 25> natures;
    natures.fill(true);

    std::array<bool, 16> powers;
    powers.fill(true);

    Profile3 profile("-", Game::Colosseum, 12345, 54321, false);

    const ShadowTemplate *shadowTemplate = Encounters3::getShadowTeam(pokemon);
    StateFilter3 filter(255, 255, 255, false, min, max, natures, powers);
    GameCubeSearcher searcher(Method::None, false, profile, filter);

    searcher.startSearch(min, max, shadowTemplate);
    auto states = searcher.getResults();
    QCOMPARE(states.size(), j.size());

    for (int i = 0; i < states.size(); i++)
    {
        const auto &state = states[i];
        QVERIFY(state == j[i]);
    }
}

void GameCubeSearcherTest::searchGalesShadow_data()
{
    QTest::addColumn<IVs>("min");
    QTest::addColumn<IVs>("max");
    QTest::addColumn<bool>("unset");
    QTest::addColumn<int>("pokemon");
    QTest::addColumn<std::string>("results");

    json data = readData("gamecube", "gamecubesearcher", "searchGalesShadow");
    for (const auto &d : data)
    {
        QTest::newRow(d["name"].get<std::string>().data()) << d["min"].get<IVs>() << d["max"].get<IVs>() << d["unset"].get<bool>()
                                                           << d["pokemon"].get<int>() << d["results"].get<json>().dump();
    }
}

void GameCubeSearcherTest::searchGalesShadow()
{
    QFETCH(IVs, min);
    QFETCH(IVs, max);
    QFETCH(bool, unset);
    QFETCH(int, pokemon);
    QFETCH(std::string, results);

    json j = json::parse(results);

    std::array<bool, 25> natures;
    natures.fill(true);

    std::array<bool, 16> powers;
    powers.fill(true);

    Profile3 profile("-", Game::Gales, 12345, 54321, false);

    const ShadowTemplate *shadowTemplate = Encounters3::getShadowTeam(pokemon);
    StateFilter3 filter(255, 255, 255, false, min, max, natures, powers);
    GameCubeSearcher searcher(Method::None, unset, profile, filter);

    searcher.startSearch(min, max, shadowTemplate);
    auto states = searcher.getResults();
    QCOMPARE(states.size(), j.size());

    for (int i = 0; i < states.size(); i++)
    {
        const auto &state = states[i];
        QVERIFY(state == j[i]);
    }
}

void GameCubeSearcherTest::searchNonLock_data()
{
    QTest::addColumn<IVs>("min");
    QTest::addColumn<IVs>("max");
    QTest::addColumn<Game>("version");
    QTest::addColumn<int>("pokemon");
    QTest::addColumn<std::string>("results");

    json data = readData("gamecube", "gamecubesearcher", "searchNonLock");
    for (const auto &d : data)
    {
        QTest::newRow(d["name"].get<std::string>().data()) << d["min"].get<IVs>() << d["max"].get<IVs>() << d["version"].get<Game>()
                                                           << d["pokemon"].get<int>() << d["results"].get<json>().dump();
    }
}

void GameCubeSearcherTest::searchNonLock()
{
    QFETCH(IVs, min);
    QFETCH(IVs, max);
    QFETCH(Game, version);
    QFETCH(int, pokemon);
    QFETCH(std::string, results);

    json j = json::parse(results);

    std::array<bool, 25> natures;
    natures.fill(true);

    std::array<bool, 16> powers;
    powers.fill(true);

    Profile3 profile("-", version, 12345, 54321, false);

    const StaticTemplate *staticTemplate = Encounters3::getStaticEncounter(7, pokemon);
    StateFilter3 filter(255, 255, 255, false, min, max, natures, powers);
    GameCubeSearcher searcher(Method::None, false, profile, filter);

    searcher.startSearch(min, max, staticTemplate);
    auto states = searcher.getResults();
    QCOMPARE(states.size(), j.size());

    for (int i = 0; i < states.size(); i++)
    {
        const auto &state = states[i];
        QVERIFY(state == j[i]);
    }
}
