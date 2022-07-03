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

#include "IDSearcher4Test.hpp"
#include <Core/Gen4/Searchers/IDSearcher4.hpp>
#include <Core/Gen4/States/IDState4.hpp>
#include <QTest>
#include <Test/Data.hpp>

static bool operator==(const IDState4 &left, const json &right)
{
    return left.getSID() == right["sid"].get<u16>() && left.getTID() == right["tid"].get<u16>() && left.getTSV() == right["tsv"].get<u16>()
        && left.getDelay() == right["delay"].get<u32>() && left.getSeed() == right["seed"].get<u32>();
}

void IDSearcher4Test::search_data()
{
    QTest::addColumn<u16>("tid");
    QTest::addColumn<u32>("maxDelay");
    QTest::addColumn<u32>("minDelay");
    QTest::addColumn<u16>("year");
    QTest::addColumn<std::string>("results");

    json data = readData("id4", "idsearcher4", "search");
    for (const auto &d : data)
    {
        QTest::newRow(d["name"].get<std::string>().data()) << d["tid"].get<u16>() << d["maxDelay"].get<u32>() << d["minDelay"].get<u32>()
                                                           << d["year"].get<u16>() << d["results"].get<json>().dump();
    }
}

void IDSearcher4Test::search()
{
    QFETCH(u16, tid);
    QFETCH(u32, maxDelay);
    QFETCH(u32, minDelay);
    QFETCH(u16, year);
    QFETCH(std::string, results);

    json j = json::parse(results);

    IDFilter filter({ tid }, {}, {}, {});
    IDSearcher4 searcher(filter);

    searcher.startSearch(false, year, minDelay, maxDelay);
    auto states = searcher.getResults();
    QCOMPARE(states.size(), j.size());

    for (size_t i = 0; i < states.size(); i++)
    {
        const auto &state = states[i];
        QVERIFY(state == j[i]);
    }
}
