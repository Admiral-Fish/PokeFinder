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

#include "EventGenerator5Test.hpp"
#include <Core/Enum/Game.hpp>
#include <Core/Gen5/Generators/EventGenerator5.hpp>
#include <Core/Gen5/PGF.hpp>
#include <Core/Gen5/Profile5.hpp>
#include <Core/Gen5/States/State5.hpp>
#include <QTest>
#include <Test/Data.hpp>
#include <Test/Enum.hpp>

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

void EventGenerator5Test::generate_data()
{
    QTest::addColumn<u64>("seed");
    QTest::addColumn<Game>("version");
    QTest::addColumn<u16>("tid");
    QTest::addColumn<u16>("sid");
    QTest::addColumn<u16>("specie");
    QTest::addColumn<u8>("nature");
    QTest::addColumn<u8>("gender");
    QTest::addColumn<u8>("ability");
    QTest::addColumn<u8>("shiny");
    QTest::addColumn<u8>("level");
    QTest::addColumn<bool>("egg");
    QTest::addColumn<u8>("hp");
    QTest::addColumn<u8>("atk");
    QTest::addColumn<u8>("def");
    QTest::addColumn<u8>("spa");
    QTest::addColumn<u8>("spd");
    QTest::addColumn<u8>("spe");
    QTest::addColumn<std::string>("results");

    json data = readData("event5", "generate");
    for (const auto &d : data)
    {
        QTest::newRow(d["name"].get<std::string>().data())
            << d["seed"].get<u64>() << getGame(d["version"].get<std::string>()) << d["tid"].get<u16>() << d["sid"].get<u16>()
            << d["specie"].get<u16>() << d["nature"].get<u8>() << d["gender"].get<u8>() << d["ability"].get<u8>() << d["shiny"].get<u8>()
            << d["level"].get<u8>() << d["egg"].get<bool>() << d["hp"].get<u8>() << d["atk"].get<u8>() << d["def"].get<u8>()
            << d["spa"].get<u8>() << d["spd"].get<u8>() << d["spe"].get<u8>() << d["results"].get<json>().dump();
    }
}

void EventGenerator5Test::generate()
{
    QFETCH(u64, seed);
    QFETCH(Game, version);
    QFETCH(u16, tid);
    QFETCH(u16, sid);
    QFETCH(u16, specie);
    QFETCH(u8, nature);
    QFETCH(u8, gender);
    QFETCH(u8, ability);
    QFETCH(u8, shiny);
    QFETCH(u8, level);
    QFETCH(bool, egg);
    QFETCH(u8, hp);
    QFETCH(u8, atk);
    QFETCH(u8, def);
    QFETCH(u8, spa);
    QFETCH(u8, spd);
    QFETCH(u8, spe);
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

    Profile5 profile("-", version, 12345, 54321, 0, { false, false, false, false }, 0, 0, 0, false, 0, 0, false, false, false, DSType::DS,
                     Language::English);

    PGF pgf(tid, sid, specie, nature, gender, ability, shiny, level, hp, atk, def, spa, spd, spe, egg);

    StateFilter5 filter(255, 255, 255, false, min, max, natures, powers);
    EventGenerator5 generator(0, 9, 0, pgf, profile, filter);

    auto states = generator.generate(seed);
    QCOMPARE(states.size(), j.size());

    for (size_t i = 0; i < states.size(); i++)
    {
        const auto &state = states[i];
        QVERIFY(state == j[i]);
    }
}
