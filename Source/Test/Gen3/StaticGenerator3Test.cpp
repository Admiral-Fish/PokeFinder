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

#include "StaticGenerator3Test.hpp"
#include <Core/Enum/Lead.hpp>
#include <Core/Gen3/Encounters3.hpp>
#include <Core/Gen3/Generators/StaticGenerator3.hpp>
#include <Core/Gen3/States/State3.hpp>
#include <Core/Parents/StaticTemplate.hpp>
#include <QTest>
#include <Test/Data.hpp>

bool operator==(const GeneratorState3 &left, const GeneratorState3 &right)
{
    return left.getPID() == right.getPID() && left.getStats() == right.getStats() && left.getAbilityIndex() == right.getAbilityIndex()
        && left.getIVs() == right.getIVs() && left.getAbility() == right.getAbility() && left.getGender() == right.getGender()
        && left.getHiddenPower() == right.getHiddenPower() && left.getNature() == right.getNature() && left.getLevel() == right.getLevel()
        && left.getShiny() == right.getShiny() && left.getAdvances() == right.getAdvances()
        && left.getHiddenPowerStrength() == right.getHiddenPowerStrength();
}

void StaticGenerator3Test::generate_data()
{
    QTest::addColumn<u32>("seed");
    QTest::addColumn<Game>("version");
    QTest::addColumn<Method>("method");
    QTest::addColumn<int>("category");
    QTest::addColumn<int>("pokemon");
    QTest::addColumn<std::vector<GeneratorState3>>("results");

    json data = readData("gen3", "staticgenerator3", "generate");
    for (const auto &d : data)
    {
        QTest::newRow(d["name"].get<std::string>().data())
            << d["seed"].get<u32>() << d["version"].get<Game>() << d["method"].get<Method>() << d["category"].get<int>()
            << d["pokemon"].get<int>() << d["results"].get<std::vector<GeneratorState3>>();
    }
}

void StaticGenerator3Test::generate()
{
    QFETCH(u32, seed);
    QFETCH(Game, version);
    QFETCH(Method, method);
    QFETCH(int, category);
    QFETCH(int, pokemon);
    QFETCH(std::vector<GeneratorState3>, results);

    std::array<u8, 6> min;
    min.fill(0);

    std::array<u8, 6> max;
    max.fill(31);

    std::array<bool, 25> natures;
    natures.fill(true);

    std::array<bool, 16> powers;
    powers.fill(true);

    const StaticTemplate *staticTemplate = Encounters3::getStaticEncounter(category, pokemon);
    StateFilter3 filter(255, 255, 255, false, min, max, natures, powers);
    StaticGenerator3 generator(0, 9, 0, 12345, 54321, version, method, Lead::None, filter);

    auto states = generator.generate(seed, staticTemplate);
    QCOMPARE(states.size(), results.size());

    for (size_t i = 0; i < states.size(); i++)
    {
        const auto &state = states[i];
        const auto &result = results[i];
        QVERIFY(state == result);
    }
}
