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

#include "GameCubeGeneratorTest.hpp"
#include <Core/Enum/Game.hpp>
#include <Core/Enum/Method.hpp>
#include <Core/Gen3/Encounters3.hpp>
#include <Core/Gen3/Generators/GameCubeGenerator.hpp>
#include <Core/Parents/States/State.hpp>
#include <QTest>
#include <Test/Data.hpp>
#include <Test/Enum.hpp>

static bool operator==(const GeneratorState &left, const json &right)
{
    return left.getPID() == right["pid"].get<u32>() && left.getStats() == right["stats"].get<std::array<u16, 6>>()
        && left.getAbilityIndex() == right["abilityIndex"].get<u16>() && left.getIVs() == right["ivs"].get<std::array<u8, 6>>()
        && left.getAbility() == right["ability"].get<u8>() && left.getGender() == right["gender"].get<u8>()
        && left.getHiddenPower() == right["hiddenPower"].get<u8>()
        && left.getHiddenPowerStrength() == right["hiddenPowerStrength"].get<u8>() && left.getLevel() == right["level"].get<u8>()
        && left.getNature() == right["nature"].get<u8>() && left.getShiny() == right["shiny"].get<u8>()
        && left.getAdvances() == right["advances"].get<u32>();
}

void GameCubeGeneratorTest::generateChannel_data()
{
    QTest::addColumn<u32>("seed");
    QTest::addColumn<std::string>("results");

    json data = readData("gamecube", "gamecubegenerator", "generateChannel");
    for (const auto &d : data)
    {
        QTest::newRow(d["name"].get<std::string>().data()) << d["seed"].get<u32>() << d["results"].get<json>().dump();
    }
}

void GameCubeGeneratorTest::generateChannel()
{
    QFETCH(u32, seed);
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

    Profile3 profile("-", Game::GC, 12345, 54321, false);

    const StaticTemplate3 *staticTemplate = Encounters3::getStaticEncounter(9, 0);
    StateFilter filter(255, 255, 255, false, min, max, natures, powers);
    GameCubeGenerator generator(0, 9, 0, Method::Channel, false, profile, filter);

    auto states = generator.generate(seed, staticTemplate);
    QCOMPARE(states.size(), j.size());

    for (size_t i = 0; i < states.size(); i++)
    {
        const auto &state = states[i];
        QVERIFY(state == j[i]);
    }
}

void GameCubeGeneratorTest::generateColoShadow_data()
{
    QTest::addColumn<u32>("seed");
    QTest::addColumn<int>("pokemon");
    QTest::addColumn<std::string>("results");

    json data = readData("gamecube", "gamecubegenerator", "generateColoShadow");
    for (const auto &d : data)
    {
        QTest::newRow(d["name"].get<std::string>().data())
            << d["seed"].get<u32>() << d["pokemon"].get<int>() << d["results"].get<json>().dump();
    }
}

void GameCubeGeneratorTest::generateColoShadow()
{
    QFETCH(u32, seed);
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

    Profile3 profile("-", Game::Colosseum, 12345, 54321, false);

    const ShadowTemplate *shadowTemplate = Encounters3::getShadowTeam(pokemon);
    StateFilter filter(255, 255, 255, false, min, max, natures, powers);
    GameCubeGenerator generator(0, 9, 0, Method::None, false, profile, filter);

    auto states = generator.generate(seed, shadowTemplate);
    QCOMPARE(states.size(), j.size());

    for (size_t i = 0; i < states.size(); i++)
    {
        const auto &state = states[i];
        QVERIFY(state == j[i]);
    }
}

void GameCubeGeneratorTest::generateGalesShadow_data()
{
    QTest::addColumn<u32>("seed");
    QTest::addColumn<bool>("unset");
    QTest::addColumn<int>("pokemon");
    QTest::addColumn<std::string>("results");

    json data = readData("gamecube", "gamecubegenerator", "generateGalesShadow");
    for (const auto &d : data)
    {
        QTest::newRow(d["name"].get<std::string>().data())
            << d["seed"].get<u32>() << d["unset"].get<bool>() << d["pokemon"].get<int>() << d["results"].get<json>().dump();
    }
}

void GameCubeGeneratorTest::generateGalesShadow()
{
    QFETCH(u32, seed);
    QFETCH(bool, unset);
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

    Profile3 profile("-", Game::Gales, 12345, 54321, false);

    const ShadowTemplate *shadowTemplate = Encounters3::getShadowTeam(pokemon);
    StateFilter filter(255, 255, 255, false, min, max, natures, powers);
    GameCubeGenerator generator(0, 9, 0, Method::None, unset, profile, filter);

    auto states = generator.generate(seed, shadowTemplate);
    QCOMPARE(states.size(), j.size());

    for (size_t i = 0; i < states.size(); i++)
    {
        const auto &state = states[i];
        QVERIFY(state == j[i]);
    }
}

void GameCubeGeneratorTest::generateNonLock_data()
{
    QTest::addColumn<u32>("seed");
    QTest::addColumn<Game>("version");
    QTest::addColumn<int>("pokemon");
    QTest::addColumn<std::string>("results");

    json data = readData("gamecube", "gamecubegenerator", "generateNonLock");
    for (const auto &d : data)
    {
        QTest::newRow(d["name"].get<std::string>().data())
            << d["seed"].get<u32>() << d["version"].get<Game>() << d["pokemon"].get<int>() << d["results"].get<json>().dump();
    }
}

void GameCubeGeneratorTest::generateNonLock()
{
    QFETCH(u32, seed);
    QFETCH(Game, version);
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

    Profile3 profile("-", version, 12345, 54321, false);

    const StaticTemplate3 *staticTemplate = Encounters3::getStaticEncounter(8, pokemon);
    StateFilter filter(255, 255, 255, false, min, max, natures, powers);
    GameCubeGenerator generator(0, 9, 0, Method::None, false, profile, filter);

    auto states = generator.generate(seed, staticTemplate);
    QCOMPARE(states.size(), j.size());

    for (size_t i = 0; i < states.size(); i++)
    {
        const auto &state = states[i];
        QVERIFY(state == j[i]);
    }
}
