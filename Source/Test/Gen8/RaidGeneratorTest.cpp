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

#include "RaidGeneratorTest.hpp"
#include <Core/Enum/Game.hpp>
#include <Core/Gen8/Den.hpp>
#include <Core/Gen8/Encounters8.hpp>
#include <Core/Gen8/Generators/RaidGenerator.hpp>
#include <Core/Gen8/States/State8.hpp>
#include <QTest>
#include <Test/Data.hpp>

static bool operator==(const State8 &left, const json &right)
{
    // Intentionally ignoring hidden power
    return left.getEC() == right["ec"].get<u32>() && left.getPID() == right["pid"].get<u32>()
        && left.getStats() == right["stats"].get<std::array<u16, 6>>() && left.getAbilityIndex() == right["abilityIndex"].get<u16>()
        && left.getIVs() == right["ivs"].get<std::array<u8, 6>>() && left.getAbility() == right["ability"].get<u8>()
        && left.getCharacteristic() == right["characteristic"].get<u8>() && left.getGender() == right["gender"].get<u8>()
        && left.getLevel() == right["level"].get<u8>() && left.getNature() == right["nature"].get<u8>()
        && left.getShiny() == right["shiny"].get<u8>() && left.getAdvances() == right["advances"].get<u32>()
        && left.getHeight() == right["height"].get<u8>() && left.getWeight() == right["weight"].get<u8>();
}

void RaidGeneratorTest::generate_data()
{
    QTest::addColumn<u64>("seed");
    QTest::addColumn<Game>("version");
    QTest::addColumn<int>("denIndex");
    QTest::addColumn<int>("rarity");
    QTest::addColumn<int>("raidIndex");
    QTest::addColumn<u8>("level");
    QTest::addColumn<std::string>("results");

    json data = readData("raid", "generate");
    for (const auto &d : data)
    {
        QTest::newRow(d["name"].get<std::string>().data())
            << d["seed"].get<u64>() << d["version"].get<Game>() << d["denIndex"].get<int>() << d["rarity"].get<int>()
            << d["raidIndex"].get<int>() << d["level"].get<u8>() << d["results"].get<json>().dump();
    }
}

void RaidGeneratorTest::generate()
{
    QFETCH(u64, seed);
    QFETCH(Game, version);
    QFETCH(int, denIndex);
    QFETCH(int, rarity);
    QFETCH(int, raidIndex);
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

    Profile8 profile("-", version, 12345, 54321, false, false, false);

    const Den *den = Encounters8::getDen(denIndex, rarity);
    Raid raid = den->getRaid(raidIndex, version);

    StateFilter filter(255, 255, 255, false, min, max, natures, powers);
    RaidGenerator generator(0, 9, 0, profile, filter);

    auto states = generator.generate(seed, level, raid);
    QCOMPARE(states.size(), j.size());

    for (size_t i = 0; i < states.size(); i++)
    {
        const auto &state = states[i];
        QVERIFY(state == j[i]);
    }
}
