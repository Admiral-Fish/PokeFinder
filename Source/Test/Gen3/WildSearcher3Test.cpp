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

#include "WildSearcher3Test.hpp"
#include <Core/Gen3/EncounterArea3.hpp>
#include <Core/Gen3/Encounters3.hpp>
#include <Core/Gen3/Generators/WildGenerator3.hpp>
#include <Core/Gen3/Searchers/WildSearcher3.hpp>
#include <Core/Gen3/States/WildState3.hpp>
#include <Core/Parents/Slot.hpp>
#include <QTest>
#include <Test/Data.hpp>
#include <Test/Enum.hpp>

using IVs = std::array<u8, 6>;

static bool operator==(const WildSearcherState3 &left, const WildGeneratorState3 &right)
{
    return left.getPidRollCount() == right.getPidRollCount() && left.getPID() == right.getPID() && left.getStats() == right.getStats()
        && left.getAbilityIndex() == right.getAbilityIndex() && left.getIVs() == right.getIVs() && left.getAbility() == right.getAbility()
        && left.getGender() == right.getGender() && left.getHiddenPower() == right.getHiddenPower()
        && left.getHiddenPowerStrength() == right.getHiddenPowerStrength() && left.getLevel() == right.getLevel()
        && left.getNature() == right.getNature() && left.getShiny() == right.getShiny() && left.getSpecie() == right.getSpecie()
        && left.getEncounterSlot() == right.getEncounterSlot() && left.getForm() == right.getForm();
}

static constexpr Lead operator+(Lead lead, u8 val)
{
    return static_cast<Lead>(toInt(lead) + val);
}

void WildSearcher3Test::search_data()
{
    QTest::addColumn<IVs>("min");
    QTest::addColumn<IVs>("max");
    QTest::addColumn<Game>("version");
    QTest::addColumn<Method>("method");
    QTest::addColumn<Encounter>("encounter");
    QTest::addColumn<Lead>("lead");
    QTest::addColumn<bool>("feebasTile");
    QTest::addColumn<int>("location");
    QTest::addColumn<int>("results");

    json data = readData("wild3", "wildsearcher3", "search");
    for (const auto &d : data)
    {
        QTest::newRow(d["name"].get<std::string>().data())
            << d["min"].get<IVs>() << d["max"].get<IVs>() << d["version"].get<Game>() << d["method"].get<Method>()
            << d["encounter"].get<Encounter>() << d["lead"].get<Lead>() << d.value("feebasTile", false) << d["location"].get<int>()
            << d["results"].get<int>();
    }
}

void WildSearcher3Test::search()
{
    QFETCH(IVs, min);
    QFETCH(IVs, max);
    QFETCH(Game, version);
    QFETCH(Method, method);
    QFETCH(Encounter, encounter);
    QFETCH(Lead, lead);
    QFETCH(bool, feebasTile);
    QFETCH(int, location);
    QFETCH(int, results);

    std::array<bool, 25> natures;
    natures.fill(true);

    std::array<bool, 16> powers;
    powers.fill(true);

    std::array<bool, 12> encounterSlots;
    encounterSlots.fill(true);

    Profile3 profile("-", version, 12345, 54321, false);

    EncounterSettings3 settings;
    settings.feebasTile = feebasTile;

    std::vector<EncounterArea3> encounterAreas = Encounters3::getEncounters(encounter, settings, version);
    auto encounterArea = std::find_if(encounterAreas.begin(), encounterAreas.end(),
                                      [location](const EncounterArea3 &encounterArea) { return encounterArea.getLocation() == location; });

    WildStateFilter filter(255, 255, 255, 0, 255, 0, 255, false, min, max, natures, powers, encounterSlots);
    WildSearcher3 searcher(method, lead, settings.feebasTile, *encounterArea, profile, filter);

    searcher.startSearch(min, max);
    auto states = searcher.getResults();
    QCOMPARE(states.size(), results);

    for (const auto &state : states)
    {
        // Ensure generator agrees
        WildGenerator3 generator(0, 0, 0, method, lead != Lead::Synchronize ? lead : lead + state.getNature(), settings.feebasTile,
                                 *encounterArea, profile, filter);
        auto generatorStates = generator.generate(state.getSeed());

        QCOMPARE(generatorStates.size(), 1);
        QVERIFY(state == generatorStates[0]);
    }
}
