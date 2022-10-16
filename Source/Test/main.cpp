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

#include <QDebug>
#include <QTest>
#include <Test/Gen3/IDGenerator3Test.hpp>
#include <Test/Gen3/StaticGenerator3Test.hpp>
#include <Test/Gen3/StaticSearcher3Test.hpp>
#include <Test/Gen3/WildGenerator3Test.hpp>
#include <Test/Gen3/WildSearcher3Test.hpp>
#include <Test/Gen4/IDGenerator4Test.hpp>
#include <Test/Gen4/IDSearcher4Test.hpp>
#include <Test/Gen4/StaticGenerator4Test.hpp>
#include <Test/Gen4/StaticSearcher4Test.hpp>
#include <Test/Gen4/WildGenerator4Test.hpp>
#include <Test/Gen4/WildSearcher4Test.hpp>
#include <Test/RNG/LCRNG64Test.hpp>
#include <Test/RNG/LCRNGReverseTest.hpp>
#include <Test/RNG/LCRNGTest.hpp>
#include <Test/RNG/MTTest.hpp>
#include <Test/RNG/SFMTTest.hpp>
#include <Test/RNG/SHA1Test.hpp>
#include <Test/RNG/TinyMTTest.hpp>
#include <Test/Util/DateTimeTest.hpp>
#include <Test/Util/EncounterSlotTest.hpp>
#include <Test/Util/IVCheckerTest.hpp>

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
    status += runTest<IDGenerator3Test>(fails);
    status += runTest<StaticGenerator3Test>(fails);
    status += runTest<StaticSearcher3Test>(fails);
    status += runTest<WildGenerator3Test>(fails);
    status += runTest<WildSearcher3Test>(fails);

    // Gen 4
    status += runTest<IDGenerator4Test>(fails);
    status += runTest<IDSearcher4Test>(fails);
    status += runTest<StaticGenerator4Test>(fails);
    status += runTest<StaticSearcher4Test>(fails);
    status += runTest<WildGenerator4Test>(fails);
    status += runTest<WildSearcher4Test>(fails);

    // RNG Tests
    status += runTest<LCRNGTest>(fails);
    status += runTest<LCRNGReverseTest>(fails);
    status += runTest<LCRNG64Test>(fails);
    status += runTest<MTTest>(fails);
    status += runTest<SFMTTest>(fails);
    status += runTest<SHA1Test>(fails);
    status += runTest<TinyMTTest>(fails);

    // Util Tests
    status += runTest<DateTest>(fails);
    status += runTest<DateTimeTest>(fails);
    status += runTest<TimeTest>(fails);
    status += runTest<EncounterSlotTest>(fails);
    status += runTest<IVCheckerTest>(fails);

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
