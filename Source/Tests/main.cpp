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
#include <Tests/RNG/LCRNG64Test.hpp>
#include <Tests/RNG/LCRNGTest.hpp>
#include <Tests/RNG/MTTest.hpp>
#include <Tests/RNG/RNGCacheTest.hpp>
#include <Tests/RNG/RNGEuclideanTest.hpp>
#include <Tests/RNG/SFMTTest.hpp>
#include <Tests/RNG/SHA1Test.hpp>
#include <Tests/RNG/TinyMTTest.hpp>
#include <Tests/Util/DateTimeTest.hpp>
#include <Tests/Util/EncounterSlotTest.hpp>

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

    // RNG Tests
    status += runTest<LCRNGTest>(fails);
    status += runTest<LCRNG64Test>(fails);
    status += runTest<MTTest>(fails);
    status += runTest<RNGCacheTest>(fails);
    status += runTest<RNGEuclideanTest>(fails);
    status += runTest<SFMTTest>(fails);
    status += runTest<SHA1Test>(fails);
    status += runTest<TinyMTTest>(fails);

    // Util Tests
    status += runTest<DateTest>(fails);
    status += runTest<DateTimeTest>(fails);
    status += runTest<TimeTest>(fails);
    status += runTest<EncounterSlotTest>(fails);

    qDebug() << "";
    // Summary of failures at end for easy viewing
    for (const QString &fail : fails)
    {
        qDebug() << fail;
    }

    return status;
}
