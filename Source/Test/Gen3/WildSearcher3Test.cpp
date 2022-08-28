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

#include "WildSearcher3Test.hpp"
#include <Core/Gen3/EncounterArea3.hpp>
#include <Core/Gen3/Encounters3.hpp>
#include <Core/Gen3/Generators/WildGenerator3.hpp>
#include <Core/Gen3/Searchers/WildSearcher3.hpp>
#include <Core/Gen3/States/WildState3.hpp>
#include <Core/Parents/Slot.hpp>
#include <QTest>
#include <Test/Data.hpp>

using IVs = std::array<u8, 6>;

struct SearcherWildResult3
{
    u32 pid;
    std::array<u16, 6> stats;
    u16 abilityIndex;
    std::array<u8, 6> ivs;
    u8 ability;
    u8 gender;
    u8 hiddenPower;
    u8 nature;
    u8 level;
    u8 shiny;
    u16 specie;
    u8 encounterSlot;
    u8 item;
    u32 seed;
    u8 hiddenPowerStrength;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SearcherWildResult3, pid, stats, abilityIndex, ivs, ability, gender, hiddenPower, nature, level, shiny,
                                   specie, encounterSlot, seed, hiddenPowerStrength);
static_assert(sizeof(SearcherWildResult3) == sizeof(WildSearcherState3));

bool operator==(const SearcherWildResult3 &result, const WildSearcherState3 &state)
{
    return result.pid == state.getPID() && result.stats == state.getStats() && result.abilityIndex == state.getAbilityIndex()
        && result.ivs == state.getIVs() && result.ability == state.getAbility() && result.gender == state.getGender()
        && result.hiddenPower == state.getHiddenPower() && result.nature == state.getNature() && result.level == state.getLevel()
        && result.shiny == state.getShiny() && result.seed == state.getSeed()
        && result.hiddenPowerStrength == state.getHiddenPowerStrength();
}

bool operator==(const WildSearcherState3 &result, const WildGeneratorState3 &state)
{
    return result.getPID() == state.getPID() && result.getStats() == state.getStats() && result.getAbilityIndex() == state.getAbilityIndex()
        && result.getIVs() == state.getIVs() && result.getAbility() == state.getAbility() && result.getGender() == state.getGender()
        && result.getHiddenPower() == state.getHiddenPower() && result.getNature() == state.getNature()
        && result.getLevel() == state.getLevel() && result.getShiny() == state.getShiny() && result.getSpecie() == state.getSpecie()
        && result.getEncounterSlot() == state.getEncounterSlot() && result.getHiddenPowerStrength() == state.getHiddenPowerStrength();
}

void WildSearcher3Test::search_data()
{
    QTest::addColumn<IVs>("min");
    QTest::addColumn<IVs>("max");
    QTest::addColumn<Game>("version");
    QTest::addColumn<Method>("method");
    QTest::addColumn<Encounter>("encounter");
    QTest::addColumn<Lead>("lead");
    QTest::addColumn<int>("location");
    QTest::addColumn<std::vector<SearcherWildResult3>>("results");

    json data = readData("gen3", "wildsearcher3", "search");
    for (const auto &d : data)
    {
        QTest::newRow(d["name"].get<std::string>().data())
            << d["min"].get<IVs>() << d["max"].get<IVs>() << d["version"].get<Game>() << d["method"].get<Method>()
            << d["encounter"].get<Encounter>() << d["lead"].get<Lead>() << d["location"].get<int>()
            << d["results"].get<std::vector<SearcherWildResult3>>();
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
    QFETCH(int, location);
    QFETCH(std::vector<SearcherWildResult3>, results);

    std::array<bool, 25> natures;
    natures.fill(true);

    std::array<bool, 16> powers;
    powers.fill(true);

    std::vector<bool> encounterSlots(12, true);

    std::vector<EncounterArea3> encounterAreas = Encounters3::getEncounters(encounter, version);
    auto encounterArea = std::find_if(encounterAreas.begin(), encounterAreas.end(),
                                      [location](const EncounterArea3 &encounterArea) { return encounterArea.getLocation() == location; });

    WildStateFilter3 filter(255, 255, 255, false, min, max, natures, powers, encounterSlots);
    WildSearcher3 searcher(12345, 54321, version, method, encounter, lead, *encounterArea, filter);

    searcher.startSearch(min, max);
    auto states = searcher.getResults();
    QCOMPARE(states.size(), results.size());

    for (size_t i = 0; i < states.size(); i++)
    {
        const auto &state = states[i];
        const auto &result = results[i];
        QCOMPARE(state, result);

        // Ensure generator agrees
        WildGenerator3 generator(0, 0, 0, 12345, 54321, version, method, encounter, lead, filter);
        if (lead == Lead::Synchronize)
        {
            generator.setSynchNature(state.getNature());
        }
        auto generatorStates = generator.generate(state.getSeed(), *encounterArea);

        QCOMPARE(generatorStates.size(), 1);
        QCOMPARE(state, generatorStates[0]);
    }
}
