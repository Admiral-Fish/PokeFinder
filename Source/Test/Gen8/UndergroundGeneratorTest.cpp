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

#include "UndergroundGeneratorTest.hpp"
#include <Core/Enum/Game.hpp>
#include <Core/Enum/Lead.hpp>
#include <Core/Gen8/Encounters8.hpp>
#include <Core/Gen8/Generators/UndergroundGenerator.hpp>
#include <Core/Gen8/Profile8.hpp>
#include <Core/Gen8/States/UndergroundState.hpp>
#include <Core/Gen8/UndergroundArea.hpp>
#include <QTest>
#include <Test/Data.hpp>
#include <Test/Enum.hpp>

static bool operator==(const UndergroundState &left, const json &right)
{
    // Intentionally ignoring hidden power
    return left.getEC() == right["ec"].get<u32>() && left.getPID() == right["pid"].get<u32>()
        && left.getStats() == right["stats"].get<std::array<u16, 6>>() && left.getAbilityIndex() == right["abilityIndex"].get<u16>()
        && left.getIVs() == right["ivs"].get<std::array<u8, 6>>() && left.getAbility() == right["ability"].get<u8>()
        && left.getCharacteristic() == right["characteristic"].get<u8>() && left.getGender() == right["gender"].get<u8>()
        && left.getLevel() == right["level"].get<u8>() && left.getNature() == right["nature"].get<u8>()
        && left.getShiny() == right["shiny"].get<u8>() && left.getAdvances() == right["advances"].get<u32>()
        && left.getEggMove() == right["eggMove"].get<u16>() && left.getItem() == right["item"].get<u16>()
        && left.getSpecie() == right["specie"].get<u16>();
}

void UndergroundGeneratorTest::generate_data()
{
    QTest::addColumn<u64>("seed0");
    QTest::addColumn<u64>("seed1");
    QTest::addColumn<Lead>("lead");
    QTest::addColumn<bool>("diglett");
    QTest::addColumn<u8>("levelFlag");
    QTest::addColumn<u8>("storyFlag");
    QTest::addColumn<int>("location");
    QTest::addColumn<std::string>("results");

    json data = readData("underground", "generate");
    for (const auto &d : data)
    {
        QTest::newRow(d["name"].get<std::string>().data())
            << d["seed0"].get<u64>() << d["seed1"].get<u64>() << getLead(d["lead"].get<std::string>()) << d["diglett"].get<bool>()
            << d["levelFlag"].get<u8>() << d["storyFlag"].get<u8>() << d["location"].get<int>() << d["results"].get<json>().dump();
    }
}

void UndergroundGeneratorTest::generate()
{
    QFETCH(u64, seed0);
    QFETCH(u64, seed1);
    QFETCH(Lead, lead);
    QFETCH(bool, diglett);
    QFETCH(u8, levelFlag);
    QFETCH(u8, storyFlag);
    QFETCH(int, location);
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

    Profile8 profile("", Game::BD, 12345, 54321, false, false, false);

    std::vector<UndergroundArea> encounterAreas = Encounters8::getUndergroundEncounters(storyFlag, diglett, &profile);
    auto encounterArea = std::find_if(encounterAreas.begin(), encounterAreas.end(),
                                      [location](const UndergroundArea &encounterArea) { return encounterArea.getLocation() == location; });

    UndergroundStateFilter filter(255, 255, 255, false, min, max, natures, powers, encounterArea->getSpecies());
    UndergroundGenerator generator(0, 9, 0, lead, diglett, levelFlag, profile, filter);

    auto states = generator.generate(seed0, seed1, *encounterArea);
    QCOMPARE(states.size(), j.size());

    for (size_t i = 0; i < states.size(); i++)
    {
        const auto &state = states[i];
        QVERIFY(state == j[i]);
    }
}
