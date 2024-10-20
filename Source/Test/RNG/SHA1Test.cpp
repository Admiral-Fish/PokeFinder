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

#include "SHA1Test.hpp"
#include <Core/Enum/DSType.hpp>
#include <Core/Enum/Game.hpp>
#include <Core/Gen5/Keypresses.hpp>
#include <Core/Gen5/Profile5.hpp>
#include <Core/RNG/SHA1.hpp>
#include <Core/Util/DateTime.hpp>
#include <QTest>
#include <Test/Data.hpp>
#include <Test/Enum.hpp>

using KeyPresses = std::array<bool, 9>;
using Seeds = std::array<u64, 4>;

void SHA1Test::hash_data()
{
    QTest::addColumn<KeyPresses>("keypresses");
    QTest::addColumn<bool>("skipLR");
    QTest::addColumn<Game>("version");
    QTest::addColumn<Language>("language");
    QTest::addColumn<u64>("mac");
    QTest::addColumn<bool>("softReset");
    QTest::addColumn<u8>("vFrame");
    QTest::addColumn<u8>("gxStat");
    QTest::addColumn<u32>("timer0");
    QTest::addColumn<u8>("vCount");
    QTest::addColumn<DSType>("dsType");
    QTest::addColumn<u64>("seed");

    json data = readData("sha1", "hash");
    for (const auto &d : data)
    {
        QTest::newRow(d["name"].get<std::string>().data())
            << d["keypresses"].get<KeyPresses>() << d["skipLR"].get<bool>() << d["version"].get<Game>() << d["language"].get<Language>()
            << d["mac"].get<u64>() << d["softReset"].get<bool>() << d["vFrame"].get<u8>() << d["gxStat"].get<u8>() << d["timer0"].get<u32>()
            << d["vCount"].get<u8>() << d["dsType"].get<DSType>() << d["seed"].get<u64>();
    }
}

void SHA1Test::hash()
{
    QFETCH(KeyPresses, keypresses);
    QFETCH(bool, skipLR);
    QFETCH(Game, version);
    QFETCH(Language, language);
    QFETCH(u64, mac);
    QFETCH(bool, softReset);
    QFETCH(u8, vFrame);
    QFETCH(u8, gxStat);
    QFETCH(u32, timer0);
    QFETCH(u8, vCount);
    QFETCH(DSType, dsType);
    QFETCH(u64, seed);

    Profile5 profile("-", version, 0, 0, mac, keypresses, vCount, gxStat, vFrame, skipLR, timer0, timer0, false, false, false, dsType,
                     language);

    auto buttons = Keypresses::getKeypresses(profile);

    DateTime dateTime;
    const Date &date = dateTime.getDate();
    const Time &time = dateTime.getTime();

    SHA1 sha(profile);
    sha.setButton(buttons.front().value);
    sha.setDate(date);
    sha.setTime(time.hour(), time.minute(), time.second(), profile.getDSType());
    sha.setTimer0(profile.getTimer0Min(), profile.getVCount());

    auto alpha = sha.precompute();
    QCOMPARE(sha.hashSeed(alpha), seed);
}

void SHA1Test::hashTime_data()
{
    QTest::addColumn<KeyPresses>("keypresses");
    QTest::addColumn<bool>("skipLR");
    QTest::addColumn<Game>("version");
    QTest::addColumn<Language>("language");
    QTest::addColumn<u64>("mac");
    QTest::addColumn<bool>("softReset");
    QTest::addColumn<u8>("vFrame");
    QTest::addColumn<u8>("gxStat");
    QTest::addColumn<u32>("timer0");
    QTest::addColumn<u8>("vCount");
    QTest::addColumn<DSType>("dsType");
    QTest::addColumn<u64>("seed");

    json data = readData("sha1", "hashTime");
    for (const auto &d : data)
    {
        QTest::newRow(d["name"].get<std::string>().data())
            << d["keypresses"].get<KeyPresses>() << d["skipLR"].get<bool>() << d["version"].get<Game>() << d["language"].get<Language>()
            << d["mac"].get<u64>() << d["softReset"].get<bool>() << d["vFrame"].get<u8>() << d["gxStat"].get<u8>() << d["timer0"].get<u32>()
            << d["vCount"].get<u8>() << d["dsType"].get<DSType>() << d["seed"].get<u64>();
    }
}

void SHA1Test::hashTime()
{
    QFETCH(KeyPresses, keypresses);
    QFETCH(bool, skipLR);
    QFETCH(Game, version);
    QFETCH(Language, language);
    QFETCH(u64, mac);
    QFETCH(bool, softReset);
    QFETCH(u8, vFrame);
    QFETCH(u8, gxStat);
    QFETCH(u32, timer0);
    QFETCH(u8, vCount);
    QFETCH(DSType, dsType);
    QFETCH(u64, seed);

    Profile5 profile("-", version, 0, 0, mac, keypresses, vCount, gxStat, vFrame, skipLR, timer0, timer0, false, false, false, dsType,
                     language);

    auto buttons = Keypresses::getKeypresses(profile);

    Date date;
    Time time(12, 0, 0);

    SHA1 sha(profile);
    sha.setButton(buttons.front().value);
    sha.setDate(date);
    sha.setTime(time.hour(), time.minute(), time.second(), profile.getDSType());
    sha.setTimer0(profile.getTimer0Min(), profile.getVCount());

    auto alpha = sha.precompute();
    QCOMPARE(sha.hashSeed(alpha), seed);
}

void MultiSHA1Test::hash_data()
{
    QTest::addColumn<KeyPresses>("keypresses");
    QTest::addColumn<bool>("skipLR");
    QTest::addColumn<Game>("version");
    QTest::addColumn<Language>("language");
    QTest::addColumn<u64>("mac");
    QTest::addColumn<bool>("softReset");
    QTest::addColumn<u8>("vFrame");
    QTest::addColumn<u8>("gxStat");
    QTest::addColumn<u32>("timer0");
    QTest::addColumn<u8>("vCount");
    QTest::addColumn<DSType>("dsType");
    QTest::addColumn<Seeds>("seed");

    json data = readData("multisha1", "hash");
    for (const auto &d : data)
    {
        QTest::newRow(d["name"].get<std::string>().data())
            << d["keypresses"].get<KeyPresses>() << d["skipLR"].get<bool>() << d["version"].get<Game>() << d["language"].get<Language>()
            << d["mac"].get<u64>() << d["softReset"].get<bool>() << d["vFrame"].get<u8>() << d["gxStat"].get<u8>() << d["timer0"].get<u32>()
            << d["vCount"].get<u8>() << d["dsType"].get<DSType>() << d["seed"].get<Seeds>();
    }
}

void MultiSHA1Test::hash()
{
    QFETCH(KeyPresses, keypresses);
    QFETCH(bool, skipLR);
    QFETCH(Game, version);
    QFETCH(Language, language);
    QFETCH(u64, mac);
    QFETCH(bool, softReset);
    QFETCH(u8, vFrame);
    QFETCH(u8, gxStat);
    QFETCH(u32, timer0);
    QFETCH(u8, vCount);
    QFETCH(DSType, dsType);
    QFETCH(Seeds, seed);

    Profile5 profile("-", version, 0, 0, mac, keypresses, vCount, gxStat, vFrame, skipLR, timer0, timer0, false, false, false, dsType,
                     language);

    auto buttons = Keypresses::getKeypresses(profile);

    DateTime dateTime;
    const Date &date = dateTime.getDate();
    const Time &time = dateTime.getTime();

    MultiSHA1 sha(profile);
    sha.setButton(buttons.front().value);
    sha.setDate(date);
    sha.setTime(time.hour(), time.minute(), time.second(), profile.getDSType());
    sha.setTimer0(profile.getTimer0Min(), profile.getVCount());

    auto alpha = sha.precompute();
    QCOMPARE(sha.hashSeed(alpha), seed);
}

void MultiSHA1Test::hashTime_data()
{
    QTest::addColumn<KeyPresses>("keypresses");
    QTest::addColumn<bool>("skipLR");
    QTest::addColumn<Game>("version");
    QTest::addColumn<Language>("language");
    QTest::addColumn<u64>("mac");
    QTest::addColumn<bool>("softReset");
    QTest::addColumn<u8>("vFrame");
    QTest::addColumn<u8>("gxStat");
    QTest::addColumn<u32>("timer0");
    QTest::addColumn<u8>("vCount");
    QTest::addColumn<DSType>("dsType");
    QTest::addColumn<Seeds>("seed");

    json data = readData("multisha1", "hashTime");
    for (const auto &d : data)
    {
        QTest::newRow(d["name"].get<std::string>().data())
            << d["keypresses"].get<KeyPresses>() << d["skipLR"].get<bool>() << d["version"].get<Game>() << d["language"].get<Language>()
            << d["mac"].get<u64>() << d["softReset"].get<bool>() << d["vFrame"].get<u8>() << d["gxStat"].get<u8>() << d["timer0"].get<u32>()
            << d["vCount"].get<u8>() << d["dsType"].get<DSType>() << d["seed"].get<Seeds>();
    }
}

void MultiSHA1Test::hashTime()
{
    QFETCH(KeyPresses, keypresses);
    QFETCH(bool, skipLR);
    QFETCH(Game, version);
    QFETCH(Language, language);
    QFETCH(u64, mac);
    QFETCH(bool, softReset);
    QFETCH(u8, vFrame);
    QFETCH(u8, gxStat);
    QFETCH(u32, timer0);
    QFETCH(u8, vCount);
    QFETCH(DSType, dsType);
    QFETCH(Seeds, seed);

    Profile5 profile("-", version, 0, 0, mac, keypresses, vCount, gxStat, vFrame, skipLR, timer0, timer0, false, false, false, dsType,
                     language);

    auto buttons = Keypresses::getKeypresses(profile);

    Date date;
    Time time(12, 0, 0);

    MultiSHA1 sha(profile);
    sha.setButton(buttons.front().value);
    sha.setDate(date);
    sha.setTime(time.hour(), time.minute(), time.second(), profile.getDSType());
    sha.setTimer0(profile.getTimer0Min(), profile.getVCount());

    auto alpha = sha.precompute();
    QCOMPARE(sha.hashSeed(alpha), seed);
}