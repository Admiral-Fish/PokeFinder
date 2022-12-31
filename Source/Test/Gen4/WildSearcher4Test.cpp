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

#include "WildSearcher4Test.hpp"
#include <Core/Enum/Encounter.hpp>
#include <Core/Enum/Game.hpp>
#include <Core/Enum/Lead.hpp>
#include <Core/Enum/Method.hpp>
#include <Core/Gen4/EncounterArea4.hpp>
#include <Core/Gen4/Encounters4.hpp>
#include <Core/Gen4/Generators/WildGenerator4.hpp>
#include <Core/Gen4/Profile4.hpp>
#include <Core/Gen4/Searchers/WildSearcher4.hpp>
#include <Core/Gen4/States/WildState4.hpp>
#include <Core/Parents/Slot.hpp>
#include <QTest>
#include <Test/Data.hpp>
#include <Test/Enum.hpp>

using IVs = std::array<u8, 6>;

static bool operator==(const WildSearcherState4 &left, const WildGeneratorState4 &right)
{
    return left.getPID() == right.getPID() && left.getStats() == right.getStats() && left.getAbilityIndex() == right.getAbilityIndex()
        && left.getIVs() == right.getIVs() && left.getAbility() == right.getAbility() && left.getGender() == right.getGender()
        && left.getHiddenPower() == right.getHiddenPower() && left.getHiddenPowerStrength() == right.getHiddenPowerStrength()
        && left.getLevel() == right.getLevel() && left.getNature() == right.getNature() && left.getShiny() == right.getShiny()
        && left.getItem() == right.getItem() && left.getSpecie() == right.getSpecie()
        && left.getEncounterSlot() == right.getEncounterSlot();
}

static constexpr Lead operator+(Lead lead, u8 val)
{
    return static_cast<Lead>(toInt(lead) + val);
}

void WildSearcher4Test::searchMethodJ_data()
{
    QTest::addColumn<IVs>("min");
    QTest::addColumn<IVs>("max");
    QTest::addColumn<u32>("minAdvance");
    QTest::addColumn<u32>("maxAdvance");
    QTest::addColumn<u32>("minDelay");
    QTest::addColumn<u32>("maxDelay");
    QTest::addColumn<Game>("version");
    QTest::addColumn<Encounter>("encounter");
    QTest::addColumn<Lead>("lead");
    QTest::addColumn<int>("location");
    QTest::addColumn<int>("results");

    json data = readData("wild4", "wildsearcher4", "searchMethodJ");
    for (const auto &d : data)
    {
        QTest::newRow(d["name"].get<std::string>().data())
            << d["min"].get<IVs>() << d["max"].get<IVs>() << d["minAdvance"].get<u32>() << d["maxAdvance"].get<u32>()
            << d["minDelay"].get<u32>() << d["maxDelay"].get<u32>() << getGame(d["version"].get<std::string>())
            << getEncounter(d["encounter"].get<std::string>()) << getLead(d["lead"].get<std::string>()) << d["location"].get<int>()
            << d["results"].get<int>();
    }
}

void WildSearcher4Test::searchMethodJ()
{
    QFETCH(IVs, min);
    QFETCH(IVs, max);
    QFETCH(u32, minAdvance);
    QFETCH(u32, maxAdvance);
    QFETCH(u32, minDelay);
    QFETCH(u32, maxDelay);
    QFETCH(Game, version);
    QFETCH(Encounter, encounter);
    QFETCH(Lead, lead);
    QFETCH(int, location);
    QFETCH(int, results);

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
    WildSearcher4 searcher(minAdvance, maxAdvance, minDelay, maxDelay, Method::MethodJ, encounter, lead, false, *encounterArea, profile,
                           filter);

    searcher.startSearch(min, max, 0);
    auto states = searcher.getResults();
    QCOMPARE(states.size(), results);

    for (const auto &state : states)
    {
        // Ensure generator agrees
        WildGenerator4 generator(state.getAdvances(), 0, 0, Method::MethodJ, encounter,
                                 lead != Lead::Synchronize ? lead : lead + state.getNature(), false, profile, filter);
        auto generatorStates = generator.generate(state.getSeed(), *encounterArea, 0);

        QCOMPARE(generatorStates.size(), 1);
        QVERIFY(state == generatorStates[0]);
    }
}

void WildSearcher4Test::searchMethodK_data()
{
    QTest::addColumn<IVs>("min");
    QTest::addColumn<IVs>("max");
    QTest::addColumn<u32>("minAdvance");
    QTest::addColumn<u32>("maxAdvance");
    QTest::addColumn<u32>("minDelay");
    QTest::addColumn<u32>("maxDelay");
    QTest::addColumn<Game>("version");
    QTest::addColumn<Encounter>("encounter");
    QTest::addColumn<Lead>("lead");
    QTest::addColumn<int>("location");
    QTest::addColumn<int>("results");

    json data = readData("wild4", "wildsearcher4", "searchMethodK");
    for (const auto &d : data)
    {
        QTest::newRow(d["name"].get<std::string>().data())
            << d["min"].get<IVs>() << d["max"].get<IVs>() << d["minAdvance"].get<u32>() << d["maxAdvance"].get<u32>()
            << d["minDelay"].get<u32>() << d["maxDelay"].get<u32>() << getGame(d["version"].get<std::string>())
            << getEncounter(d["encounter"].get<std::string>()) << getLead(d["lead"].get<std::string>()) << d["location"].get<int>()
            << d["results"].get<int>();
    }
}

void WildSearcher4Test::searchMethodK()
{
    QFETCH(IVs, min);
    QFETCH(IVs, max);
    QFETCH(u32, minAdvance);
    QFETCH(u32, maxAdvance);
    QFETCH(u32, minDelay);
    QFETCH(u32, maxDelay);
    QFETCH(Game, version);
    QFETCH(Encounter, encounter);
    QFETCH(Lead, lead);
    QFETCH(int, location);
    QFETCH(int, results);

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
    WildSearcher4 searcher(minAdvance, maxAdvance, minDelay, maxDelay, Method::MethodK, encounter, lead, false, *encounterArea, profile,
                           filter);

    searcher.startSearch(min, max, 0);
    auto states = searcher.getResults();
    QCOMPARE(states.size(), results);

    for (const auto &state : states)
    {
        // Ensure generator agrees
        WildGenerator4 generator(state.getAdvances(), 0, 0, Method::MethodK, encounter,
                                 lead != Lead::Synchronize ? lead : lead + state.getNature(), false, profile, filter);
        auto generatorStates = generator.generate(state.getSeed(), *encounterArea, 0);

        QCOMPARE(generatorStates.size(), 1);
        QVERIFY(state == generatorStates[0]);
    }
}

void WildSearcher4Test::searchPokeRadar_data()
{
    QTest::addColumn<IVs>("min");
    QTest::addColumn<IVs>("max");
    QTest::addColumn<u32>("minAdvance");
    QTest::addColumn<u32>("maxAdvance");
    QTest::addColumn<u32>("minDelay");
    QTest::addColumn<u32>("maxDelay");
    QTest::addColumn<Game>("version");
    QTest::addColumn<Encounter>("encounter");
    QTest::addColumn<Lead>("lead");
    QTest::addColumn<int>("location");
    QTest::addColumn<bool>("shiny");
    QTest::addColumn<u8>("index");
    QTest::addColumn<int>("results");

    json data = readData("wild4", "wildsearcher4", "searchPokeRadar");
    for (const auto &d : data)
    {
        QTest::newRow(d["name"].get<std::string>().data())
            << d["min"].get<IVs>() << d["max"].get<IVs>() << d["minAdvance"].get<u32>() << d["maxAdvance"].get<u32>()
            << d["minDelay"].get<u32>() << d["maxDelay"].get<u32>() << getGame(d["version"].get<std::string>())
            << getEncounter(d["encounter"].get<std::string>()) << getLead(d["lead"].get<std::string>()) << d["location"].get<int>()
            << d["shiny"].get<bool>() << d["index"].get<u8>() << d["results"].get<int>();
    }
}

void WildSearcher4Test::searchPokeRadar()
{
    QFETCH(IVs, min);
    QFETCH(IVs, max);
    QFETCH(u32, minAdvance);
    QFETCH(u32, maxAdvance);
    QFETCH(u32, minDelay);
    QFETCH(u32, maxDelay);
    QFETCH(Game, version);
    QFETCH(Encounter, encounter);
    QFETCH(Lead, lead);
    QFETCH(int, location);
    QFETCH(bool, shiny);
    QFETCH(u8, index);
    QFETCH(int, results);

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
    WildSearcher4 searcher(minAdvance, maxAdvance, minDelay, maxDelay, Method::PokeRadar, encounter, lead, shiny, *encounterArea, profile,
                           filter);

    searcher.startSearch(min, max, index);
    auto states = searcher.getResults();
    QCOMPARE(states.size(), results);

    for (const auto &state : states)
    {
        // Ensure generator agrees
        WildGenerator4 generator(state.getAdvances(), 0, 0, Method::PokeRadar, encounter,
                                 lead != Lead::Synchronize ? lead : lead + state.getNature(), shiny, profile, filter);
        auto generatorStates = generator.generate(state.getSeed(), *encounterArea, index);

        QCOMPARE(generatorStates.size(), 1);
        QVERIFY(state == generatorStates[0]);
    }
}
