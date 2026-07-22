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
        && left.getEncounterSlot() == right["encounterSlot"].get<u8>() && left.getForm() == right["form"].get<u8>()
        && left.getAdvances() == right["advances"].get<u32>() && left.getBattleAdvances() == right["battleAdvances"].get<u32>()
        && left.getCall() == right["call"].get<u8>() && left.getChatot() == right["chatot"].get<u8>();
}

static bool isRockSmashItemState(const WildGeneratorState4 &state)
{
    constexpr std::array<u16, 8> cianwoodItems = { 39, 28, 93, 72, 73, 75, 74, 91 };

    return state.getSpecie() == 0 && state.getPID() == 0 && state.getAbility() == 0 && state.getGender() == 0 && state.getLevel() == 0
        && state.getNature() == 0 && state.getShiny() == 0 && state.getEncounterSlot() == 0 && state.getForm() == 0
        && state.getIVs() == std::array<u8, 6> {}
        && std::ranges::find(cianwoodItems, state.getItem()) != cianwoodItems.end();
}

void WildGenerator4Test::generateMethodJ_data()
{
    QTest::addColumn<u32>("seed");
    QTest::addColumn<Game>("version");
    QTest::addColumn<Encounter>("encounter");
    QTest::addColumn<Lead>("lead");
    QTest::addColumn<bool>("feebasTile");
    QTest::addColumn<int>("location");
    QTest::addColumn<std::string>("results");

    json data = readData("wild4", "wildgenerator4", "generateMethodJ");
    for (const auto &d : data)
    {
        QTest::newRow(d["name"].get<std::string>().data())
            << d["seed"].get<u32>() << d["version"].get<Game>() << d["encounter"].get<Encounter>() << d["lead"].get<Lead>()
            << d.value("feebasTile", false) << d["location"].get<int>() << d["results"].get<json>().dump();
    }
}

void WildGenerator4Test::generateMethodJ()
{
    QFETCH(u32, seed);
    QFETCH(Game, version);
    QFETCH(Encounter, encounter);
    QFETCH(Lead, lead);
    QFETCH(bool, feebasTile);
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

    Profile4 profile("", version, 12345, 54321, false);
    EncounterSettings4 settings = { };

    settings.dppt.feebasTile = feebasTile;

    std::vector<EncounterArea4> encounterAreas = Encounters4::getEncounters(encounter, settings, &profile);
    auto encounterArea = std::ranges::find_if(
        encounterAreas, [location](const EncounterArea4 &encounterArea) { return encounterArea.getLocation() == location; });

    WildStateFilter filter(255, 255, 255, 1, 100, 0, 255, 0, 255, false, min, max, natures, powers, encounterSlots);
    WildGenerator4 generator(0, 9, 0, Method::MethodJ, lead, settings.dppt.feebasTile, false, false, 50, false, 0, *encounterArea, profile,
                             filter);

    auto states = generator.generate(seed, 0);
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
            << d["seed"].get<u32>() << d["version"].get<Game>() << d["encounter"].get<Encounter>() << d["lead"].get<Lead>()
            << d["location"].get<int>() << d["results"].get<json>().dump();
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

    std::array<bool, 26> unownDiscovered;
    unownDiscovered.fill(true);

    std::array<bool, 4> unownPuzzles;
    unownPuzzles.fill(true);

    Profile4 profile("", version, 12345, 54321, false, unownDiscovered, unownPuzzles);
    EncounterSettings4 settings = { };

    std::vector<EncounterArea4> encounterAreas = Encounters4::getEncounters(encounter, settings, &profile);
    auto encounterArea = std::ranges::find_if(
        encounterAreas, [location](const EncounterArea4 &encounterArea) { return encounterArea.getLocation() == location; });

    WildStateFilter filter(255, 255, 255, 1, 100, 0, 255, 0, 255, false, min, max, natures, powers, encounterSlots);
    WildGenerator4 generator(0, 9, 0, Method::MethodK, lead, false, false, false, 50, false, 0, *encounterArea, profile, filter);

    auto states = generator.generate(seed, 0);
    std::vector<WildGeneratorState4> pokemonStates;
    std::vector<WildGeneratorState4> itemStates;
    for (const auto &state : states)
    {
        if (state.getSpecie() == 0)
        {
            itemStates.emplace_back(state);
        }
        else
        {
            pokemonStates.emplace_back(state);
        }
    }

    size_t expectedItemStates
        = seed == 3537031890 && version == Game::HeartGold && encounter == Encounter::RockSmash && lead == Lead::None && location == 51 ? 1
                                                                                                                                       : 0;
    QCOMPARE(pokemonStates.size(), j.size());
    QCOMPARE(itemStates.size(), expectedItemStates);

    for (const auto &state : itemStates)
    {
        QVERIFY(isRockSmashItemState(state));
    }

    for (size_t i = 0; i < pokemonStates.size(); i++)
    {
        const auto &state = pokemonStates[i];
        QVERIFY(state == j[i]);
    }
}

void WildGenerator4Test::generateHoneyTree_data()
{
    QTest::addColumn<u32>("seed");
    QTest::addColumn<Game>("version");
    QTest::addColumn<Encounter>("encounter");
    QTest::addColumn<Lead>("lead");
    QTest::addColumn<int>("location");
    QTest::addColumn<u8>("index");
    QTest::addColumn<std::string>("results");

    json data = readData("wild4", "wildgenerator4", "generateHoneyTree");
    for (const auto &d : data)
    {
        QTest::newRow(d["name"].get<std::string>().data())
            << d["seed"].get<u32>() << d["version"].get<Game>() << d["encounter"].get<Encounter>() << d["lead"].get<Lead>()
            << d["location"].get<int>() << d["index"].get<u8>() << d["results"].get<json>().dump();
    }
}

void WildGenerator4Test::generateHoneyTree()
{
    QFETCH(u32, seed);
    QFETCH(Game, version);
    QFETCH(Encounter, encounter);
    QFETCH(Lead, lead);
    QFETCH(int, location);
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

    Profile4 profile("", version, 12345, 54321, false);
    EncounterSettings4 settings = { };

    std::vector<EncounterArea4> encounterAreas = Encounters4::getEncounters(encounter, settings, &profile);
    auto encounterArea = std::ranges::find_if(
        encounterAreas, [location](const EncounterArea4 &encounterArea) { return encounterArea.getLocation() == location; });

    WildStateFilter filter(255, 255, 255, 1, 100, 0, 255, 0, 255, false, min, max, natures, powers, encounterSlots);
    WildGenerator4 generator(0, 9, 0, Method::HoneyTree, lead, false, false, false, 50, false, 0, *encounterArea, profile, filter);

    auto states = generator.generate(seed, index);
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
            << d["seed"].get<u32>() << d["version"].get<Game>() << d["encounter"].get<Encounter>() << d["lead"].get<Lead>()
            << d["location"].get<int>() << d["shiny"].get<bool>() << d["index"].get<u8>() << d["results"].get<json>().dump();
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

    Profile4 profile("", version, 12345, 54321, false);
    EncounterSettings4 settings = { };

    settings.dppt.radar = true;

    std::vector<EncounterArea4> encounterAreas = Encounters4::getEncounters(encounter, settings, &profile);
    auto encounterArea = std::ranges::find_if(
        encounterAreas, [location](const EncounterArea4 &encounterArea) { return encounterArea.getLocation() == location; });

    WildStateFilter filter(255, 255, 255, 1, 100, 0, 255, 0, 255, false, min, max, natures, powers, encounterSlots);
    WildGenerator4 generator(0, 9, 0, Method::PokeRadar, lead, false, shiny, false, 50, false, 0, *encounterArea, profile, filter);

    auto states = generator.generate(seed, index);
    QCOMPARE(states.size(), j.size());

    for (size_t i = 0; i < states.size(); i++)
    {
        const auto &state = states[i];
        QVERIFY(state == j[i]);
    }
}
