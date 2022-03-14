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

#include "SHA1Test.hpp"
#include <Core/Enum/DSType.hpp>
#include <Core/Enum/Game.hpp>
#include <Core/Gen5/Keypresses.hpp>
#include <Core/Gen5/Profile5.hpp>
#include <Core/RNG/SHA1.hpp>
#include <Core/Util/DateTime.hpp>
#include <QTest>

Q_DECLARE_METATYPE(DateTime)
Q_DECLARE_METATYPE(Profile5)

void SHA1Test::hash_data()
{
    QTest::addColumn<DateTime>("dateTime");
    QTest::addColumn<Profile5>("profile");
    QTest::addColumn<u64>("seed");

    QTest::newRow("Black 1") << DateTime(2000, 1, 1)
                             << Profile5("", Game::Black, 0, 0, 0x9BF123456, { true, false, false, false }, 0x2e, 0x6, 0x5, false, 0x608,
                                         0x608, false, false, false, DSType::DS)
                             << 0x5E89803C95FE8240ULL;
    QTest::newRow("White 1") << DateTime(2000, 1, 1)
                             << Profile5("", Game::White, 0, 0, 0x9BF123456, { true, false, false, false }, 0x2f, 0x6, 0x5, false, 0x621,
                                         0x621, false, false, false, DSType::DS)
                             << 0xb082b4a755192171ULL;
    QTest::newRow("Black 2") << DateTime(2000, 1, 1)
                             << Profile5("", Game::Black2, 0, 0, 0x9BF123456, { true, false, false, false }, 0x48, 0x6, 0x5, false, 0x972,
                                         0x972, false, false, false, DSType::DS)
                             << 0x490EABDA126D5432ULL;
    QTest::newRow("White 2") << DateTime(2000, 1, 1)
                             << Profile5("", Game::White2, 0, 0, 0x9BF123456, { true, false, false, false }, 0x48, 0x6, 0x5, false, 0x96f,
                                         0x96f, false, false, false, DSType::DS)
                             << 0xE299A7D5D949A38EULL;
}

void SHA1Test::hash()
{
    QFETCH(DateTime, dateTime);
    QFETCH(Profile5, profile);
    QFETCH(u64, seed);

    auto buttons = Keypresses::getKeyPresses(profile.getKeypresses(), profile.getSkipLR());
    auto values = Keypresses::getValues(buttons);

    const Date &date = dateTime.getDate();
    const Time &time = dateTime.getTime();

    SHA1 sha(profile);
    sha.setButton(values.front());
    sha.setDate(date);
    sha.setTime(time.hour(), time.minute(), time.second(), profile.getDSType());
    sha.setTimer0(profile.getTimer0Min(), profile.getVCount());

    sha.precompute();
    QCOMPARE(sha.hashSeed(), seed);
}
