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

#include "JirachiPatternTest.hpp"
#include <Core/Gen3/Tools/JirachiPattern.hpp>
#include <Core/RNG/LCRNG.hpp>
#include <QTest>
#include <Test/Data.hpp>

void JirachiPatternTest::calculateActions_data()
{
    QTest::addColumn<u32>("seed");
    QTest::addColumn<u32>("target");
    QTest::addColumn<u32>("bruteForce");
    QTest::addColumn<std::vector<u8>>("results");

    json data = readData("jirachipattern", "calculateActions");
    for (const auto &d : data)
    {
        QTest::newRow(d["name"].get<std::string>().data())
            << d["seed"].get<u32>() << d["target"].get<u32>() << d["bruteForce"].get<u32>() << d["results"].get<std::vector<u8>>();
    }
}

void JirachiPatternTest::calculateActions()
{
    QFETCH(u32, seed);
    QFETCH(u32, target);
    QFETCH(u32, bruteForce);
    QFETCH(std::vector<u8>, results);

    u32 targetAdvance = XDRNG::distance(seed, target);
    QCOMPARE(JirachiPattern::calculateActions(seed, targetAdvance, bruteForce), results);
}

void JirachiPatternTest::computeJirachiSeed_data()
{
    QTest::addColumn<u32>("seed");
    QTest::addColumn<u32>("result");

    json data = readData("jirachipattern", "computeJirachiSeed");
    for (const auto &d : data)
    {
        QTest::newRow(d["name"].get<std::string>().data()) << d["seed"].get<u32>() << d["result"].get<u32>();
    }
}

void JirachiPatternTest::computeJirachiSeed()
{
    QFETCH(u32, seed);
    QFETCH(u32, result);

    QCOMPARE(JirachiPattern::computeJirachiSeed(seed), result);
}
