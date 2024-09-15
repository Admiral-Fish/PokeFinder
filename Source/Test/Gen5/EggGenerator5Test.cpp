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

#include "EggGenerator5Test.hpp"
#include <Core/Enum/Game.hpp>
#include <Core/Gen5/Generators/EggGenerator5.hpp>
#include <Core/Gen5/States/EggState5.hpp>
#include <QTest>
#include <Test/Data.hpp>
#include <Test/Enum.hpp>

using IVs = std::array<std::array<u8, 6>, 2>;
using Attribute = std::array<u8, 2>;

static bool operator==(const EggState5 &left, const json &right)
{
    return left.getPID() == right["pid"].get<u32>() && left.getStats() == right["stats"].get<std::array<u16, 6>>()
        && left.getAbilityIndex() == right["abilityIndex"].get<u16>() && left.getIVs() == right["ivs"].get<std::array<u8, 6>>()
        && left.getAbility() == right["ability"].get<u8>() && left.getCharacteristic() == right["characteristic"].get<u8>()
        && left.getGender() == right["gender"].get<u8>() && left.getHiddenPower() == right["hiddenPower"].get<u8>()
        && left.getHiddenPowerStrength() == right["hiddenPowerStrength"].get<u8>() && left.getLevel() == right["level"].get<u8>()
        && left.getNature() == right["nature"].get<u8>() && left.getShiny() == right["shiny"].get<u8>()
        && left.getInheritance() == right["inheritance"].get<std::array<u8, 6>>() && left.getAdvances() == right["advances"].get<u32>()
        && left.getChatot() == right["chatot"].get<u8>();
}

void EggGenerator5Test::generate_data()
{
    QTest::addColumn<u64>("seed");
    QTest::addColumn<Game>("version");
    QTest::addColumn<u16>("pokemon");
    QTest::addColumn<IVs>("parentIVs");
    QTest::addColumn<Attribute>("parentAbility");
    QTest::addColumn<Attribute>("parentGender");
    QTest::addColumn<Attribute>("parentItem");
    QTest::addColumn<Attribute>("parentNature");
    QTest::addColumn<std::string>("results");

    json data = readData("egg5", "generate");
    for (const auto &d : data)
    {
        QTest::newRow(d["name"].get<std::string>().data())
            << d["seed"].get<u64>() << d["version"].get<Game>() << d["pokemon"].get<u16>() << d["parentIVs"].get<IVs>()
            << d["parentAbility"].get<Attribute>() << d["parentGender"].get<Attribute>() << d["parentItem"].get<Attribute>()
            << d["parentNature"].get<Attribute>() << d["results"].get<json>().dump();
    }
}

void EggGenerator5Test::generate()
{
    QFETCH(u64, seed);
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

    Profile5 profile("-", version, 12345, 54321, 0, { false, false, false, false, false, false, false, false }, 0, 0, 0, false, 0, 0, false,
                     false, false, DSType::DS, Language::English);

    Daycare daycare(parentIVs, parentAbility, parentGender, parentItem, parentNature, pokemon, true);
    StateFilter filter(255, 255, 255, false, min, max, natures, powers);
    EggGenerator5 generator(0, 9, 0, daycare, profile, filter);

    auto states = generator.generate(seed);
    QCOMPARE(states.size(), j.size());

    for (size_t i = 0; i < states.size(); i++)
    {
        const auto &state = states[i];
        QVERIFY(state == j[i]);
    }
}
