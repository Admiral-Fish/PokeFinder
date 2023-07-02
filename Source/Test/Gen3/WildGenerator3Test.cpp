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

#include "WildGenerator3Test.hpp"
#include <Core/Enum/Encounter.hpp>
#include <Core/Enum/Lead.hpp>
#include <Core/Gen3/EncounterArea3.hpp>
#include <Core/Gen3/Encounters3.hpp>
#include <Core/Gen3/Generators/WildGenerator3.hpp>
#include <Core/Parents/Slot.hpp>
#include <Core/Parents/States/WildState.hpp>
#include <QTest>
#include <Test/Data.hpp>
#include <Test/Enum.hpp>

static bool operator==(const WildGeneratorState &left, const json &right)
{
    // Intentionally not comparing item
    return left.getPID() == right["pid"].get<u32>() && left.getStats() == right["stats"].get<std::array<u16, 6>>()
        && left.getAbilityIndex() == right["abilityIndex"].get<u16>() && left.getIVs() == right["ivs"].get<std::array<u8, 6>>()
        && left.getAbility() == right["ability"].get<u8>() && left.getGender() == right["gender"].get<u8>()
        && left.getHiddenPower() == right["hiddenPower"].get<u8>()
        && left.getHiddenPowerStrength() == right["hiddenPowerStrength"].get<u8>() && left.getLevel() == right["level"].get<u8>()
        && left.getNature() == right["nature"].get<u8>() && left.getShiny() == right["shiny"].get<u8>()
        && left.getSpecie() == right["specie"].get<u16>() && left.getEncounterSlot() == right["encounterSlot"].get<u8>()
        && left.getForm() == right["form"].get<u8>() && left.getAdvances() == right["advances"].get<u32>();
}

void WildGenerator3Test::generate_data()
{
    QTest::addColumn<u32>("seed");
    QTest::addColumn<Game>("version");
    QTest::addColumn<Method>("method");
    QTest::addColumn<Encounter>("encounter");
    QTest::addColumn<Lead>("lead");
    QTest::addColumn<int>("location");
    QTest::addColumn<std::string>("results");

    json data = readData("wild3", "wildgenerator3", "generate");
    for (const auto &d : data)
    {
        QTest::newRow(d["name"].get<std::string>().data())
            << d["seed"].get<u32>() << d["version"].get<Game>() << d["method"].get<Method>() << d["encounter"].get<Encounter>()
            << d["lead"].get<Lead>() << d["location"].get<int>() << d["results"].get<json>().dump();
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

    Profile3 profile("-", version, 12345, 54321, false);

    std::vector<EncounterArea3> encounterAreas = Encounters3::getEncounters(encounter, version);
    auto encounterArea = std::find_if(encounterAreas.begin(), encounterAreas.end(),
                                      [location](const EncounterArea3 &encounterArea) { return encounterArea.getLocation() == location; });

    WildStateFilter3 filter(255, 255, 255, false, min, max, natures, powers, encounterSlots);
    WildGenerator3 generator(0, 9, 0, method, encounter, lead, profile, filter);

    auto states = generator.generate(seed, *encounterArea);
    QCOMPARE(states.size(), j.size());

    for (size_t i = 0; i < states.size(); i++)
    {
        const auto &state = states[i];
        QVERIFY(state == j[i]);
    }
}
