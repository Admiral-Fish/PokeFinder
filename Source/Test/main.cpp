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

#include <QDebug>
#include <QTest>
#include <Test/Gen3/EggGenerator3Test.hpp>
#include <Test/Gen3/GameCubeGeneratorTest.hpp>
#include <Test/Gen3/GameCubeSearcherTest.hpp>
#include <Test/Gen3/IDGenerator3Test.hpp>
#include <Test/Gen3/JirachiPatternTest.hpp>
#include <Test/Gen3/PIDToIVCalculatorTest.hpp>
#include <Test/Gen3/PokeSpotGeneratorTest.hpp>
#include <Test/Gen3/SeedToTimeCalculator3Test.hpp>
#include <Test/Gen3/StaticGenerator3Test.hpp>
#include <Test/Gen3/StaticSearcher3Test.hpp>
#include <Test/Gen3/WildGenerator3Test.hpp>
#include <Test/Gen3/WildSearcher3Test.hpp>
#include <Test/Gen4/ChainedSIDCalcTest.hpp>
#include <Test/Gen4/EggGenerator4Test.hpp>
#include <Test/Gen4/IDGenerator4Test.hpp>
#include <Test/Gen4/IDSearcher4Test.hpp>
#include <Test/Gen4/SeedToTimeCalculator4Test.hpp>
#include <Test/Gen4/StaticGenerator4Test.hpp>
#include <Test/Gen4/StaticSearcher4Test.hpp>
#include <Test/Gen4/WildGenerator4Test.hpp>
#include <Test/Gen4/WildSearcher4Test.hpp>
#include <Test/Gen5/DreamRadarGeneratorTest.hpp>
#include <Test/Gen5/EggGenerator5Test.hpp>
#include <Test/Gen5/EventGenerator5Test.hpp>
#include <Test/Gen5/HiddenGrottoGeneratorTest.hpp>
#include <Test/Gen5/IDGenerator5Test.hpp>
#include <Test/Gen5/ProfileSearcher5Test.hpp>
#include <Test/Gen8/EggGenerator8Test.hpp>
#include <Test/Gen8/EventGenerator8Test.hpp>
#include <Test/Gen8/IDGenerator8Test.hpp>
#include <Test/Gen8/RaidGeneratorTest.hpp>
#include <Test/Gen8/StaticGenerator8Test.hpp>
#include <Test/Gen8/UndergroundGeneratorTest.hpp>
#include <Test/Gen8/WildGenerator8Test.hpp>
#include <Test/RNG/LCRNG64Test.hpp>
#include <Test/RNG/LCRNGReverseTest.hpp>
#include <Test/RNG/LCRNGTest.hpp>
#include <Test/RNG/MTTest.hpp>
#include <Test/RNG/SFMTTest.hpp>
#include <Test/RNG/SHA1Test.hpp>
#include <Test/RNG/TinyMTTest.hpp>
#include <Test/RNG/XoroshiroTest.hpp>
#include <Test/RNG/XorshiftTest.hpp>
#include <Test/Util/DateTimeTest.hpp>
#include <Test/Util/EncounterSlotTest.hpp>
#include <Test/Util/IVCheckerTest.hpp>
#include <Test/Util/IVToPIDCalculatorTest.hpp>

template <class Testname>
int runTest(QStringList &fails)
{
    Testname test;

    int ret = QTest::qExec(&test);
    if (ret != 0)
    {
        fails.append(QString("%1: %2 test(s) failed").arg(test.metaObject()->className()).arg(ret));
    }

    return ret;
}

int main()
{
    int status = 0;
    QStringList fails;

    // Gen 3
    status += runTest<EggGenerator3Test>(fails);
    status += runTest<GameCubeGeneratorTest>(fails);
    status += runTest<GameCubeSearcherTest>(fails);
    status += runTest<IDGenerator3Test>(fails);
    status += runTest<JirachiPatternTest>(fails);
    status += runTest<PIDToIVCalculatorTest>(fails);
    status += runTest<PokeSpotGeneratorTest>(fails);
    status += runTest<SeedToTimeCalculator3Test>(fails);
    status += runTest<StaticGenerator3Test>(fails);
    status += runTest<StaticSearcher3Test>(fails);
    status += runTest<WildGenerator3Test>(fails);
    status += runTest<WildSearcher3Test>(fails);

    // Gen 4
    status += runTest<ChainedSIDCalcTest>(fails);
    status += runTest<EggGenerator4Test>(fails);
    status += runTest<IDGenerator4Test>(fails);
    status += runTest<IDSearcher4Test>(fails);
    status += runTest<SeedToTimeCalculator4Test>(fails);
    status += runTest<StaticGenerator4Test>(fails);
    status += runTest<StaticSearcher4Test>(fails);
    status += runTest<WildGenerator4Test>(fails);
    status += runTest<WildSearcher4Test>(fails);

    // Gen 5
    status += runTest<DreamRadarGeneratorTest>(fails);
    status += runTest<EggGenerator5Test>(fails);
    status += runTest<EventGenerator5Test>(fails);
    status += runTest<HiddenGrottoGeneratorTest>(fails);
    status += runTest<IDGenerator5Test>(fails);
    status += runTest<ProfileSearcher5Test>(fails);

    // Gen 8
    status += runTest<IDGenerator8Test>(fails);
    status += runTest<EggGenerator8Test>(fails);
    status += runTest<EventGenerator8Test>(fails);
    status += runTest<RaidGeneratorTest>(fails);
    status += runTest<StaticGenerator8Test>(fails);
    status += runTest<UndergroundGeneratorTest>(fails);
    status += runTest<WildGenerator8Test>(fails);

    // RNG Tests
    status += runTest<LCRNGTest>(fails);
    status += runTest<LCRNGReverseTest>(fails);
    status += runTest<LCRNG64Test>(fails);
    status += runTest<MTTest>(fails);
    status += runTest<MultiSHA1Test>(fails);
    status += runTest<SFMTTest>(fails);
    status += runTest<SHA1Test>(fails);
    status += runTest<TinyMTTest>(fails);
    status += runTest<XoroshiroTest>(fails);
    status += runTest<XoroshiroBDSPTest>(fails);
    status += runTest<XorshiftTest>(fails);

    // Util Tests
    status += runTest<DateTest>(fails);
    status += runTest<DateTimeTest>(fails);
    status += runTest<TimeTest>(fails);
    status += runTest<EncounterSlotTest>(fails);
    status += runTest<IVCheckerTest>(fails);
    status += runTest<IVToPIDCalculatorTest>(fails);

    qDebug() << "";
    // Summary of failures at end for easy viewing
    for (const QString &fail : fails)
    {
        qDebug() << fail;
    }

    if (fails.empty())
    {
        qDebug() << "No failures";
    }

    return status;
}
