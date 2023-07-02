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

#include "ProfileSearcher5Test.hpp"
#include <Core/Enum/Game.hpp>
#include <Core/Gen5/Searchers/ProfileSearcher5.hpp>
#include <Core/Gen5/States/ProfileSearcherState5.hpp>
#include <Core/Util/DateTime.hpp>
#include <QTest>
#include <Test/Data.hpp>
#include <Test/Enum.hpp>

using IVs = std::array<u8, 6>;

void ProfileSearcher5Test::ivs_data()
{
    QTest::addColumn<Date>("date");
    QTest::addColumn<Time>("time");
    QTest::addColumn<u8>("minSeconds");
    QTest::addColumn<u8>("maxSeconds");
    QTest::addColumn<u8>("minVCount");
    QTest::addColumn<u8>("maxVCount");
    QTest::addColumn<u16>("minTimer0");
    QTest::addColumn<u16>("maxTimer0");
    QTest::addColumn<u8>("minGxStat");
    QTest::addColumn<u8>("maxGxStat");
    QTest::addColumn<u8>("minVFrame");
    QTest::addColumn<u8>("maxVFrame");
    QTest::addColumn<bool>("softReset");
    QTest::addColumn<Game>("version");
    QTest::addColumn<Language>("language");
    QTest::addColumn<DSType>("dsType");
    QTest::addColumn<u64>("mac");
    QTest::addColumn<Buttons>("buttons");
    QTest::addColumn<IVs>("minIVs");
    QTest::addColumn<IVs>("maxIVs");
    QTest::addColumn<u64>("result");

    json data = readData("profilesearcher5", "ivs");
    for (const auto &d : data)
    {
        QTest::newRow(d["name"].get<std::string>().data())
            << Date(d["date"].get<int>()) << Time(d["time"].get<int>()) << d["minSeconds"].get<u8>() << d["maxSeconds"].get<u8>()
            << d["minVCount"].get<u8>() << d["maxVCount"].get<u8>() << d["minTimer0"].get<u16>() << d["maxTimer0"].get<u16>()
            << d["minGxStat"].get<u8>() << d["maxGxStat"].get<u8>() << d["minVFrame"].get<u8>() << d["maxVFrame"].get<u8>()
            << d["softReset"].get<bool>() << d["version"].get<Game>() << d["language"].get<Language>() << d["dsType"].get<DSType>()
            << d["mac"].get<u64>() << d["buttons"].get<Buttons>() << d["minIVs"].get<IVs>() << d["maxIVs"].get<IVs>()
            << d["result"].get<u64>();
    }
}

void ProfileSearcher5Test::ivs()
{
    QFETCH(Date, date);
    QFETCH(Time, time);
    QFETCH(u8, minSeconds);
    QFETCH(u8, maxSeconds);
    QFETCH(u8, minVCount);
    QFETCH(u8, maxVCount);
    QFETCH(u16, minTimer0);
    QFETCH(u16, maxTimer0);
    QFETCH(u8, minGxStat);
    QFETCH(u8, maxGxStat);
    QFETCH(u8, minVFrame);
    QFETCH(u8, maxVFrame);
    QFETCH(bool, softReset);
    QFETCH(Game, version);
    QFETCH(Language, language);
    QFETCH(DSType, dsType);
    QFETCH(u64, mac);
    QFETCH(Buttons, buttons);
    QFETCH(IVs, minIVs);
    QFETCH(IVs, maxIVs);
    QFETCH(u64, result);

    ProfileIVSearcher5 searcher(date, time, minSeconds, maxSeconds, minVCount, maxVCount, minTimer0, maxTimer0, minGxStat, maxGxStat,
                                softReset, version, language, dsType, mac, buttons, minIVs, maxIVs);
    searcher.startSearch(1, minVFrame, maxVFrame);

    auto results = searcher.getResults();
    QCOMPARE(results.size(), 1);

    QCOMPARE(results[0].getSeed(), result);
}

void ProfileSearcher5Test::needle_data()
{
    QTest::addColumn<Date>("date");
    QTest::addColumn<Time>("time");
    QTest::addColumn<u8>("minSeconds");
    QTest::addColumn<u8>("maxSeconds");
    QTest::addColumn<u8>("minVCount");
    QTest::addColumn<u8>("maxVCount");
    QTest::addColumn<u16>("minTimer0");
    QTest::addColumn<u16>("maxTimer0");
    QTest::addColumn<u8>("minGxStat");
    QTest::addColumn<u8>("maxGxStat");
    QTest::addColumn<u8>("minVFrame");
    QTest::addColumn<u8>("maxVFrame");
    QTest::addColumn<bool>("softReset");
    QTest::addColumn<Game>("version");
    QTest::addColumn<Language>("language");
    QTest::addColumn<DSType>("dsType");
    QTest::addColumn<u64>("mac");
    QTest::addColumn<Buttons>("buttons");
    QTest::addColumn<std::vector<u8>>("needles");
    QTest::addColumn<bool>("unovaLink");
    QTest::addColumn<bool>("memoryLink");
    QTest::addColumn<u64>("result");

    json data = readData("profilesearcher5", "needle");
    for (const auto &d : data)
    {
        QTest::newRow(d["name"].get<std::string>().data())
            << Date(d["date"].get<int>()) << Time(d["time"].get<int>()) << d["minSeconds"].get<u8>() << d["maxSeconds"].get<u8>()
            << d["minVCount"].get<u8>() << d["maxVCount"].get<u8>() << d["minTimer0"].get<u16>() << d["maxTimer0"].get<u16>()
            << d["minGxStat"].get<u8>() << d["maxGxStat"].get<u8>() << d["minVFrame"].get<u8>() << d["maxVFrame"].get<u8>()
            << d["softReset"].get<bool>() << d["version"].get<Game>() << d["language"].get<Language>() << d["dsType"].get<DSType>()
            << d["mac"].get<u64>() << d["buttons"].get<Buttons>() << d["needles"].get<std::vector<u8>>() << d["unovaLink"].get<bool>()
            << d["memoryLink"].get<bool>() << d["result"].get<u64>();
    }
}

void ProfileSearcher5Test::needle()
{
    QFETCH(Date, date);
    QFETCH(Time, time);
    QFETCH(u8, minSeconds);
    QFETCH(u8, maxSeconds);
    QFETCH(u8, minVCount);
    QFETCH(u8, maxVCount);
    QFETCH(u16, minTimer0);
    QFETCH(u16, maxTimer0);
    QFETCH(u8, minGxStat);
    QFETCH(u8, maxGxStat);
    QFETCH(u8, minVFrame);
    QFETCH(u8, maxVFrame);
    QFETCH(bool, softReset);
    QFETCH(Game, version);
    QFETCH(Language, language);
    QFETCH(DSType, dsType);
    QFETCH(u64, mac);
    QFETCH(Buttons, buttons);
    QFETCH(std::vector<u8>, needles);
    QFETCH(bool, unovaLink);
    QFETCH(bool, memoryLink);
    QFETCH(u64, result);

    ProfileNeedleSearcher5 searcher(date, time, minSeconds, maxSeconds, minVCount, maxVCount, minTimer0, maxTimer0, minGxStat, maxGxStat,
                                    softReset, version, language, dsType, mac, buttons, needles, unovaLink, memoryLink);
    searcher.startSearch(1, minVFrame, maxVFrame);

    auto results = searcher.getResults();
    QCOMPARE(results.size(), 1);

    QCOMPARE(results[0].getSeed(), result);
}

void ProfileSearcher5Test::seed_data()
{
    QTest::addColumn<Date>("date");
    QTest::addColumn<Time>("time");
    QTest::addColumn<u8>("minSeconds");
    QTest::addColumn<u8>("maxSeconds");
    QTest::addColumn<u8>("minVCount");
    QTest::addColumn<u8>("maxVCount");
    QTest::addColumn<u16>("minTimer0");
    QTest::addColumn<u16>("maxTimer0");
    QTest::addColumn<u8>("minGxStat");
    QTest::addColumn<u8>("maxGxStat");
    QTest::addColumn<u8>("minVFrame");
    QTest::addColumn<u8>("maxVFrame");
    QTest::addColumn<bool>("softReset");
    QTest::addColumn<Game>("version");
    QTest::addColumn<Language>("language");
    QTest::addColumn<DSType>("dsType");
    QTest::addColumn<u64>("mac");
    QTest::addColumn<Buttons>("buttons");
    QTest::addColumn<u64>("seed");

    json data = readData("profilesearcher5", "seed");
    for (const auto &d : data)
    {
        QTest::newRow(d["name"].get<std::string>().data())
            << Date(d["date"].get<int>()) << Time(d["time"].get<int>()) << d["minSeconds"].get<u8>() << d["maxSeconds"].get<u8>()
            << d["minVCount"].get<u8>() << d["maxVCount"].get<u8>() << d["minTimer0"].get<u16>() << d["maxTimer0"].get<u16>()
            << d["minGxStat"].get<u8>() << d["maxGxStat"].get<u8>() << d["minVFrame"].get<u8>() << d["maxVFrame"].get<u8>()
            << d["softReset"].get<bool>() << d["version"].get<Game>() << d["language"].get<Language>() << d["dsType"].get<DSType>()
            << d["mac"].get<u64>() << d["buttons"].get<Buttons>() << d["seed"].get<u64>();
    }
}

void ProfileSearcher5Test::seed()
{
    QFETCH(Date, date);
    QFETCH(Time, time);
    QFETCH(u8, minSeconds);
    QFETCH(u8, maxSeconds);
    QFETCH(u8, minVCount);
    QFETCH(u8, maxVCount);
    QFETCH(u16, minTimer0);
    QFETCH(u16, maxTimer0);
    QFETCH(u8, minGxStat);
    QFETCH(u8, maxGxStat);
    QFETCH(u8, minVFrame);
    QFETCH(u8, maxVFrame);
    QFETCH(bool, softReset);
    QFETCH(Game, version);
    QFETCH(Language, language);
    QFETCH(DSType, dsType);
    QFETCH(u64, mac);
    QFETCH(Buttons, buttons);
    QFETCH(u64, seed);

    ProfileSeedSearcher5 searcher(date, time, minSeconds, maxSeconds, minVCount, maxVCount, minTimer0, maxTimer0, minGxStat, maxGxStat,
                                  softReset, version, language, dsType, mac, buttons, seed);
    searcher.startSearch(1, minVFrame, maxVFrame);

    auto results = searcher.getResults();
    QCOMPARE(results.size(), 1);
}
