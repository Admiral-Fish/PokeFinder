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

#include "IVCheckerTest.hpp"
#include <Core/Enum/Game.hpp>
#include <Core/Parents/PersonalInfo.hpp>
#include <Core/Parents/PersonalLoader.hpp>
#include <Core/Util/IVChecker.hpp>
#include <QTest>

using Stats = std::vector<std::array<u16, 6>>;
using Levels = std::vector<u8>;
using Results = std::array<std::vector<u8>, 6>;

void IVCheckerTest::calculateIVRange_data()
{
    QTest::addColumn<u16>("species");
    QTest::addColumn<u8>("form");
    QTest::addColumn<Game>("game");
    QTest::addColumn<Stats>("stats");
    QTest::addColumn<Levels>("levels");
    QTest::addColumn<u8>("nature");
    QTest::addColumn<u8>("characteristic");
    QTest::addColumn<u8>("hiddenPower");
    QTest::addColumn<Results>("results");

    QTest::newRow("Shaymin Land Form") << static_cast<u16>(492) << static_cast<u8>(0) << Game::HGSS
                                       << Stats { { 25, 14, 15, 16, 15, 17 }, { 41, 24, 26, 27, 25, 30 }, { 57, 34, 37, 38, 35, 42 } }
                                       << Levels { 5, 10, 15 } << static_cast<u8>(10) << static_cast<u8>(16) << static_cast<u8>(10)
                                       << Results { std::vector<u8> { 14, 15, 16, 17, 18, 19 },
                                                    std::vector<u8> { 20, 21, 22, 23, 24, 25, 26 },
                                                    std::vector<u8> { 14, 15, 16, 17, 18, 19 },
                                                    std::vector<u8> { 20, 22, 24, 26 },
                                                    std::vector<u8> { 1, 3, 5 },
                                                    std::vector<u8> { 31 } };
    QTest::newRow("Shaymin Sky Form") << static_cast<u16>(492) << static_cast<u8>(1) << Game::HGSS
                                      << Stats { { 25, 14, 13, 18, 12, 20 }, { 41, 25, 21, 31, 20, 36 }, { 57, 35, 29, 44, 28, 51 } }
                                      << Levels { 5, 10, 15 } << static_cast<u8>(10) << static_cast<u8>(16) << static_cast<u8>(10)
                                      << Results { std::vector<u8> { 14, 15, 16, 17, 18, 19 },
                                                   std::vector<u8> { 24, 25, 26, 27 },
                                                   std::vector<u8> { 10, 11, 12, 13, 14, 15, 16 },
                                                   std::vector<u8> { 20, 22, 24, 26 },
                                                   std::vector<u8> { 5, 7, 9 },
                                                   std::vector<u8> { 31 } };
    QTest::newRow("Alakazam") << static_cast<u16>(65) << static_cast<u8>(0) << Game::HGSS
                              << Stats { { 22, 9, 11, 22, 15, 18 }, { 34, 13, 17, 38, 25, 32 }, { 46, 18, 23, 55, 35, 45 } }
                              << Levels { 5, 10, 15 } << static_cast<u8>(15) << static_cast<u8>(1) << static_cast<u8>(14)
                              << Results { std::vector<u8> { 31 },     std::vector<u8> { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 },
                                           std::vector<u8> { 30, 31 }, std::vector<u8> { 31 },
                                           std::vector<u8> { 31 },     std::vector<u8> { 31 } };
    QTest::newRow("Alakazam Gen 6+") << static_cast<u16>(65) << static_cast<u8>(0) << Game::BDSP
                                     << Stats { { 22, 9, 11, 22, 16, 18 }, { 34, 13, 17, 38, 27, 32 }, { 46, 18, 23, 55, 38, 45 } }
                                     << Levels { 5, 10, 15 } << static_cast<u8>(15) << static_cast<u8>(1) << static_cast<u8>(14)
                                     << Results { std::vector<u8> { 31 },     std::vector<u8> { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 },
                                                  std::vector<u8> { 30, 31 }, std::vector<u8> { 31 },
                                                  std::vector<u8> { 31 },     std::vector<u8> { 31 } };
}

void IVCheckerTest::calculateIVRange()
{
    QFETCH(u16, species);
    QFETCH(u8, form);
    QFETCH(Game, game);
    QFETCH(Stats, stats);
    QFETCH(Levels, levels);
    QFETCH(u8, nature);
    QFETCH(u8, characteristic);
    QFETCH(u8, hiddenPower);
    QFETCH(Results, results);

    PersonalInfo info = PersonalLoader::getPersonal(game, species, form);
    auto range = IVChecker::calculateIVRange(info.getStats(), stats, levels, nature, characteristic, hiddenPower);

    QCOMPARE(range, results);
}
