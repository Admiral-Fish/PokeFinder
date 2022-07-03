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

#include "WildGenerator4Test.hpp"
#include <Core/Enum/Encounter.hpp>
#include <Core/Enum/Game.hpp>
#include <Core/Enum/Lead.hpp>
#include <Core/Enum/Method.hpp>
#include <Core/Gen4/EncounterArea4.hpp>
#include <Core/Gen4/Encounters4.hpp>
#include <Core/Gen4/Generators/WildGenerator4.hpp>
#include <Core/Gen4/Profile4.hpp>
#include <Core/Gen4/States/WildState4.hpp>
#include <Core/Parents/Slot.hpp>
#include <QTest>
#include <Test/Data.hpp>
#include <Test/Enum.hpp>

static bool operator==(const WildGeneratorState4 &left, const json &right)
{
    return left.getPID() == right["pid"].get<u32>() && left.getStats() == right["stats"].get<std::array<u16, 6>>()
        && left.getAbilityIndex() == right["abilityIndex"].get<u16>() && left.getIVs() == right["ivs"].get<std::array<u8, 6>>()
        && left.getAbility() == right["ability"].get<u8>() && left.getCharacteristic() == right["characteristic"].get<u8>()
        && left.getGender() == right["gender"].get<u8>() && left.getHiddenPower() == right["hiddenPower"].get<u8>()
        && left.getHiddenPowerStrength() == right["hiddenPowerStrength"].get<u8>() && left.getLevel() == right["level"].get<u8>()
        && left.getNature() == right["nature"].get<u8>() && left.getShiny() == right["shiny"].get<u8>()
        && left.getItem() == right["item"].get<u16>() && left.getSpecie() == right["specie"].get<u16>()
        && left.getEncounterSlot() == right["encounterSlot"].get<u8>() && left.getAdvances() == right["advances"].get<u32>()
        && left.getBattleAdvances() == right["battleAdvances"].get<u32>() && left.getCall() == right["call"].get<u8>()
        && left.getChatot() == right["chatot"].get<u8>();
}

void WildGenerator4Test::generateMethodJ_data()
{
    QTest::addColumn<u32>("seed");
    QTest::addColumn<Game>("version");
    QTest::addColumn<Encounter>("encounter");
    QTest::addColumn<Lead>("lead");
    QTest::addColumn<int>("location");
    QTest::addColumn<std::string>("results");

    json data = readData("wild4", "wildgenerator4", "generateMethodJ");
    for (const auto &d : data)
    {
        QTest::newRow(d["name"].get<std::string>().data())
            << d["seed"].get<u32>() << getGame(d["version"].get<std::string>()) << getEncounter(d["encounter"].get<std::string>())
            << getLead(d["lead"].get<std::string>()) << d["location"].get<int>() << d["results"].get<json>().dump();
    }
}

void WildGenerator4Test::generateMethodJ()
{
    QFETCH(u32, seed);
    QFETCH(Game, version);
    QFETCH(Encounter, encounter);
    QFETCH(Lead, lead);
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

    std::array<bool, 12> encounterSlots;
    encounterSlots.fill(true);

    std::array<u16, 2> replacement = { 0, 0 };
    std::array<u8, 5> blocks = { 0, 0, 0, 0, 0 };
    Profile4 profile("", version, 12345, 54321, false);

    std::vector<EncounterArea4> encounterAreas
        = Encounters4::getEncounters(encounter, 0, Game::None, false, 0, false, replacement, blocks, &profile);
    auto encounterArea = std::find_if(encounterAreas.begin(), encounterAreas.end(),
                                      [location](const EncounterArea4 &encounterArea) { return encounterArea.getLocation() == location; });

    WildStateFilter4 filter(255, 255, 255, false, min, max, natures, powers, encounterSlots);
    WildGenerator4 generator(0, 9, 0, Method::MethodJ, encounter, lead, false, profile, filter);

    auto states = generator.generate(seed, *encounterArea, 0);
    QCOMPARE(states.size(), j.size());

    for (size_t i = 0; i < states.size(); i++)
    {
        const auto &state = states[i];
        QVERIFY(state == j[i]);
    }
}

void WildGenerator4Test::generateMethodK_data()
{
    QTest::addColumn<u32>("seed");
    QTest::addColumn<Game>("version");
    QTest::addColumn<Encounter>("encounter");
    QTest::addColumn<Lead>("lead");
    QTest::addColumn<int>("location");
    QTest::addColumn<std::string>("results");

    json data = readData("wild4", "wildgenerator4", "generateMethodK");
    for (const auto &d : data)
    {
        QTest::newRow(d["name"].get<std::string>().data())
            << d["seed"].get<u32>() << getGame(d["version"].get<std::string>()) << getEncounter(d["encounter"].get<std::string>())
            << getLead(d["lead"].get<std::string>()) << d["location"].get<int>() << d["results"].get<json>().dump();
    }
}

void WildGenerator4Test::generateMethodK()
{
    QFETCH(u32, seed);
    QFETCH(Game, version);
    QFETCH(Encounter, encounter);
    QFETCH(Lead, lead);
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

    std::array<bool, 12> encounterSlots;
    encounterSlots.fill(true);

    std::array<u16, 2> replacement = { 0, 0 };
    std::array<u8, 5> blocks = { 0, 0, 0, 0, 0 };
    Profile4 profile("", version, 12345, 54321, false);

    std::vector<EncounterArea4> encounterAreas
        = Encounters4::getEncounters(encounter, 0, Game::None, false, 0, false, replacement, blocks, &profile);
    auto encounterArea = std::find_if(encounterAreas.begin(), encounterAreas.end(),
                                      [location](const EncounterArea4 &encounterArea) { return encounterArea.getLocation() == location; });

    WildStateFilter4 filter(255, 255, 255, false, min, max, natures, powers, encounterSlots);
    WildGenerator4 generator(0, 9, 0, Method::MethodK, encounter, lead, false, profile, filter);

    auto states = generator.generate(seed, *encounterArea, 0);
    QCOMPARE(states.size(), j.size());

    for (size_t i = 0; i < states.size(); i++)
    {
        const auto &state = states[i];
        QVERIFY(state == j[i]);
    }
}

void WildGenerator4Test::generatePokeRadar_data()
{
    QTest::addColumn<u32>("seed");
    QTest::addColumn<Game>("version");
    QTest::addColumn<Encounter>("encounter");
    QTest::addColumn<Lead>("lead");
    QTest::addColumn<int>("location");
    QTest::addColumn<bool>("shiny");
    QTest::addColumn<u8>("index");
    QTest::addColumn<std::string>("results");

    json data = readData("wild4", "wildgenerator4", "generatePokeRadar");
    for (const auto &d : data)
    {
        QTest::newRow(d["name"].get<std::string>().data())
            << d["seed"].get<u32>() << getGame(d["version"].get<std::string>()) << getEncounter(d["encounter"].get<std::string>())
            << getLead(d["lead"].get<std::string>()) << d["location"].get<int>() << d["shiny"].get<bool>() << d["index"].get<u8>()
            << d["results"].get<json>().dump();
    }
}

void WildGenerator4Test::generatePokeRadar()
{
    QFETCH(u32, seed);
    QFETCH(Game, version);
    QFETCH(Encounter, encounter);
    QFETCH(Lead, lead);
    QFETCH(int, location);
    QFETCH(bool, shiny);
    QFETCH(u8, index);
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

    std::array<bool, 12> encounterSlots;
    encounterSlots.fill(true);

    std::array<u16, 2> replacement = { 0, 0 };
    std::array<u8, 5> blocks = { 0, 0, 0, 0, 0 };
    Profile4 profile("", version, 12345, 54321, false);

    std::vector<EncounterArea4> encounterAreas
        = Encounters4::getEncounters(encounter, 0, Game::None, true, 0, false, replacement, blocks, &profile);
    auto encounterArea = std::find_if(encounterAreas.begin(), encounterAreas.end(),
                                      [location](const EncounterArea4 &encounterArea) { return encounterArea.getLocation() == location; });

    WildStateFilter4 filter(255, 255, 255, false, min, max, natures, powers, encounterSlots);
    WildGenerator4 generator(0, 9, 0, Method::PokeRadar, encounter, lead, shiny, profile, filter);

    auto states = generator.generate(seed, *encounterArea, index);
    QCOMPARE(states.size(), j.size());

    for (size_t i = 0; i < states.size(); i++)
    {
        const auto &state = states[i];
        QVERIFY(state == j[i]);
    }
}
