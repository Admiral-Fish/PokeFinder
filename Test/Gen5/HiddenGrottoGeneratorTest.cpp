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

#include "HiddenGrottoGeneratorTest.hpp"
#include <Core/Enum/Game.hpp>
#include <Core/Gen5/Encounters5.hpp>
#include <Core/Gen5/Generators/HiddenGrottoGenerator.hpp>
#include <Core/Gen5/HiddenGrottoArea.hpp>
#include <Core/Gen5/States/HiddenGrottoState.hpp>
#include <Core/Gen5/States/State5.hpp>
#include <QTest>
#include <Test/Data.hpp>
#include <Test/Enum.hpp>

static bool operator==(const HiddenGrottoState &left, const json &right)
{
    return left.getAdvances() == right["advances"].get<u32>() && left.getData() == right["data"].get<u16>()
        && left.getItem() == right["item"].get<bool>() && left.getChatot() == right["chatot"].get<u8>()
        && left.getGender() == right["gender"].get<u8>() && left.getGroup() == right["group"].get<u8>()
        && left.getSlot() == right["slot"].get<u8>();
}

static bool operator==(const State5 &left, const json &right)
{
    return left.getPID() == right["pid"].get<u32>() && left.getStats() == right["stats"].get<std::array<u16, 6>>()
        && left.getAbilityIndex() == right["abilityIndex"].get<u16>() && left.getIVs() == right["ivs"].get<std::array<u8, 6>>()
        && left.getAbility() == right["ability"].get<u8>() && left.getCharacteristic() == right["characteristic"].get<u8>()
        && left.getGender() == right["gender"].get<u8>() && left.getHiddenPower() == right["hiddenPower"].get<u8>()
        && left.getHiddenPowerStrength() == right["hiddenPowerStrength"].get<u8>() && left.getLevel() == right["level"].get<u8>()
        && left.getNature() == right["nature"].get<u8>() && left.getShiny() == right["shiny"].get<u8>()
        && left.getAdvances() == right["advances"].get<u32>() && left.getChatot() == right["chatot"].get<u8>();
}

void HiddenGrottoGeneratorTest::pokemon_data()
{
    QTest::addColumn<u64>("seed");
    QTest::addColumn<Lead>("lead");
    QTest::addColumn<Game>("version");
    QTest::addColumn<u8>("gender");
    QTest::addColumn<u8>("group");
    QTest::addColumn<u8>("index");
    QTest::addColumn<int>("location");
    QTest::addColumn<std::string>("results");

    json data = readData("hiddengrotto", "pokemon");
    for (const auto &d : data)
    {
        QTest::newRow(d["name"].get<std::string>().data())
            << d["seed"].get<u64>() << d["lead"].get<Lead>() << d["version"].get<Game>() << d["gender"].get<u8>() << d["group"].get<u8>()
            << d["index"].get<u8>() << d["location"].get<int>() << d["results"].get<json>().dump();
    }
}

void HiddenGrottoGeneratorTest::pokemon()
{
    QFETCH(u64, seed);
    QFETCH(Lead, lead);
    QFETCH(Game, version);
    QFETCH(u8, gender);
    QFETCH(u8, group);
    QFETCH(u8, index);
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

    Profile5 profile("-", version, 12345, 54321, "", "", 0, { false, false, false, false, false, false, false, false, false }, 0, 0, 0,
                     false, 0, 0, false, false, false, DSType::DS, Language::English);

    std::vector<HiddenGrottoArea> encounterAreas = Encounters5::getHiddenGrottoEncounters();
    auto encounterArea = std::find_if(encounterAreas.begin(), encounterAreas.end(), [location](const HiddenGrottoArea &encounterArea) {
        return encounterArea.getLocation() == location;
    });

    StateFilter filter(255, 255, 255, 0, 255, 0, 255, false, min, max, natures, powers);
    HiddenGrottoGenerator generator(0, 9, 0, lead, gender, encounterArea->getPokemon(group, index), profile, filter);

    auto states = generator.generate(seed, 0, 0);
    QCOMPARE(states.size(), j.size());

    for (size_t i = 0; i < states.size(); i++)
    {
        const auto &state = states[i];
        QVERIFY(state == j[i]);
    }
}

void HiddenGrottoGeneratorTest::slot_data()
{
    QTest::addColumn<u64>("seed");
    QTest::addColumn<int>("location");
    QTest::addColumn<std::string>("results");

    json data = readData("hiddengrotto", "slot");
    for (const auto &d : data)
    {
        QTest::newRow(d["name"].get<std::string>().data())
            << d["seed"].get<u64>() << d["location"].get<int>() << d["results"].get<json>().dump();
    }
}

void HiddenGrottoGeneratorTest::slot()
{
    QFETCH(u64, seed);
    QFETCH(int, location);
    QFETCH(std::string, results);

    json j = json::parse(results);

    std::array<bool, 11> encounterSlots;
    encounterSlots.fill(true);

    std::array<bool, 2> genders;
    genders.fill(true);

    std::array<bool, 4> groups;
    groups.fill(true);

    Profile5 profile("-", Game::BW2, 12345, 54321, "", "", 0, { false, false, false, false, false, false, false, false, false }, 0, 0, 0,
                     false, 0, 0, false, false, false, DSType::DS, Language::English);

    std::vector<HiddenGrottoArea> encounterAreas = Encounters5::getHiddenGrottoEncounters();
    auto encounterArea = std::find_if(encounterAreas.begin(), encounterAreas.end(), [location](const HiddenGrottoArea &encounterArea) {
        return encounterArea.getLocation() == location;
    });

    HiddenGrottoFilter filter(encounterSlots, genders, groups);
    HiddenGrottoSlotGenerator generator(0, 99, 0, 55, *encounterArea, profile, filter);

    auto states = generator.generate(seed);
    QCOMPARE(states.size(), j.size());

    for (size_t i = 0; i < states.size(); i++)
    {
        const auto &state = states[i];
        QVERIFY(state == j[i]);
    }
}
