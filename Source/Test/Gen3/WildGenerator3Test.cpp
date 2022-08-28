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

#include "WildGenerator3Test.hpp"
#include <Core/Enum/Encounter.hpp>
#include <Core/Enum/Lead.hpp>
#include <Core/Gen3/EncounterArea3.hpp>
#include <Core/Gen3/Encounters3.hpp>
#include <Core/Gen3/Generators/WildGenerator3.hpp>
#include <Core/Gen3/States/WildState3.hpp>
#include <Core/Parents/Slot.hpp>
#include <QTest>
#include <Test/Data.hpp>

struct WildStaticResult3
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
    u32 advances;
    u8 hiddenPowerStrength;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(WildStaticResult3, pid, stats, abilityIndex, ivs, ability, gender, hiddenPower, nature, level, shiny,
                                   specie, encounterSlot, advances, hiddenPowerStrength);
static_assert(sizeof(WildStaticResult3) == sizeof(WildGeneratorState3));

bool operator==(const WildStaticResult3 &result, const WildGeneratorState3 &state)
{
    // Intentionally not comparing item
    return result.pid == state.getPID() && result.stats == state.getStats() && result.abilityIndex == state.getAbilityIndex()
        && result.ivs == state.getIVs() && result.ability == state.getAbility() && result.gender == state.getGender()
        && result.hiddenPower == state.getHiddenPower() && result.nature == state.getNature() && result.level == state.getLevel()
        && state.getShiny() == result.shiny && result.specie == state.getSpecie() && result.encounterSlot == state.getEncounterSlot()
        && state.getAdvances() == result.advances && result.hiddenPowerStrength == state.getHiddenPowerStrength();
}

void WildGenerator3Test::generate_data()
{
    QTest::addColumn<u32>("seed");
    QTest::addColumn<Game>("version");
    QTest::addColumn<Method>("method");
    QTest::addColumn<Encounter>("encounter");
    QTest::addColumn<Lead>("lead");
    QTest::addColumn<int>("location");
    QTest::addColumn<std::vector<WildStaticResult3>>("results");

    json data = readData("gen3", "wildgenerator3", "generate");
    for (const auto &d : data)
    {
        QTest::newRow(d["name"].get<std::string>().data())
            << d["seed"].get<u32>() << d["version"].get<Game>() << d["method"].get<Method>() << d["encounter"].get<Encounter>()
            << d["lead"].get<Lead>() << d["location"].get<int>() << d["results"].get<std::vector<WildStaticResult3>>();
    }
}

void WildGenerator3Test::generate()
{
    QFETCH(u32, seed);
    QFETCH(Game, version);
    QFETCH(Method, method);
    QFETCH(Encounter, encounter);
    QFETCH(Lead, lead);
    QFETCH(int, location);
    QFETCH(std::vector<WildStaticResult3>, results);

    std::array<u8, 6> min;
    min.fill(0);

    std::array<u8, 6> max;
    max.fill(31);

    std::array<bool, 25> natures;
    natures.fill(true);

    std::array<bool, 16> powers;
    powers.fill(true);

    std::vector<bool> encounterSlots(12, true);

    std::vector<EncounterArea3> encounterAreas = Encounters3::getEncounters(encounter, version);
    auto encounterArea = std::find_if(encounterAreas.begin(), encounterAreas.end(),
                                      [location](const EncounterArea3 &encounterArea) { return encounterArea.getLocation() == location; });

    WildStateFilter3 filter(255, 255, 255, false, min, max, natures, powers, encounterSlots);
    WildGenerator3 generator(0, 9, 0, 12345, 54321, version, method, encounter, lead, filter);
    if (lead == Lead::Synchronize)
    {
        generator.setSynchNature(0);
    }

    auto states = generator.generate(seed, *encounterArea);
    QCOMPARE(states.size(), results.size());

    for (size_t i = 0; i < states.size(); i++)
    {
        const auto &state = states[i];
        const auto &result = results[i];
        QVERIFY(state == result);
    }
}
