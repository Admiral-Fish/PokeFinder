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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301,
 * USA.
 */

#include "EggGenerator3Test.hpp"
#include <Core/Enum/Game.hpp>
#include <Core/Enum/Method.hpp>
#include <Core/Gen3/Generators/EggGenerator3.hpp>
#include <Core/Gen3/States/EggState3.hpp>
#include <QTest>
#include <Test/Data.hpp>
#include <Test/Enum.hpp>

using IVs = std::array<std::array<u8, 6>, 2>;
using Attribute = std::array<u8, 2>;

static bool operator==(const EggState3 &left, const json &right)
{
    return left.getPID() == right["pid"].get<u32>() && left.getStats() == right["stats"].get<std::array<u16, 6>>()
        && left.getAbilityIndex() == right["abilityIndex"].get<u16>() && left.getIVs() == right["ivs"].get<std::array<u8, 6>>()
        && left.getAbility() == right["ability"].get<u8>() && left.getGender() == right["gender"].get<u8>()
        && left.getHiddenPower() == right["hiddenPower"].get<u8>()
        && left.getHiddenPowerStrength() == right["hiddenPowerStrength"].get<u8>() && left.getLevel() == right["level"].get<u8>()
        && left.getNature() == right["nature"].get<u8>() && left.getShiny() == right["shiny"].get<u8>()
        && left.getInheritance() == right["inheritance"].get<std::array<u8, 6>>() && left.getAdvances() == right["advances"].get<u32>()
        && left.getPickupAdvances() == right["pickupAdvances"].get<u32>() && left.getRedraws() == right["redraws"].get<u8>();
}

void EggGenerator3Test::generate_data()
{
    QTest::addColumn<u32>("seed");
    QTest::addColumn<u32>("seedPickup");
    QTest::addColumn<u8>("calibration");
    QTest::addColumn<u8>("minRedraw");
    QTest::addColumn<u8>("maxRedraw");
    QTest::addColumn<Method>("method");
    QTest::addColumn<u8>("compatability");
    QTest::addColumn<Game>("version");
    QTest::addColumn<u16>("pokemon");
    QTest::addColumn<IVs>("parentIVs");
    QTest::addColumn<Attribute>("parentAbility");
    QTest::addColumn<Attribute>("parentGender");
    QTest::addColumn<Attribute>("parentItem");
    QTest::addColumn<Attribute>("parentNature");
    QTest::addColumn<std::string>("results");

    json data = readData("egg3", "generate");
    for (const auto &d : data)
    {
        QTest::newRow(d["name"].get<std::string>().data())
            << d["seed"].get<u32>() << d["seedPickup"].get<u32>() << d["calibration"].get<u8>() << d["minRedraw"].get<u8>()
            << d["maxRedraw"].get<u8>() << d["method"].get<Method>() << d["compatability"].get<u8>() << d["version"].get<Game>()
            << d["pokemon"].get<u16>() << d["parentIVs"].get<IVs>() << d["parentAbility"].get<Attribute>()
            << d["parentGender"].get<Attribute>() << d["parentItem"].get<Attribute>() << d["parentNature"].get<Attribute>()
            << d["results"].get<json>().dump();
    }
}

void EggGenerator3Test::generate()
{
    QFETCH(u32, seed);
    QFETCH(u32, seedPickup);
    QFETCH(u8, calibration);
    QFETCH(u8, minRedraw);
    QFETCH(u8, maxRedraw);
    QFETCH(Method, method);
    QFETCH(u8, compatability);
    QFETCH(Game, version);
    QFETCH(u16, pokemon);
    QFETCH(IVs, parentIVs);
    QFETCH(Attribute, parentAbility);
    QFETCH(Attribute, parentGender);
    QFETCH(Attribute, parentItem);
    QFETCH(Attribute, parentNature);
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

    Daycare daycare(parentIVs, parentAbility, parentGender, parentItem, parentNature, pokemon, false);
    StateFilter3 filter(255, 255, 255, false, min, max, natures, powers);
    EggGenerator3 generator(0, 9, 0, 0, 9, 0, calibration, minRedraw, maxRedraw, method, compatability, daycare, profile, filter);

    auto states = generator.generate(seed, seedPickup);
    QCOMPARE(states.size(), j.size());

    for (size_t i = 0; i < states.size(); i++)
    {
        const auto &state = states[i];
        QVERIFY(state == j[i]);
    }
}
