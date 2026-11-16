/*
 * This file is part of PokéFinder
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "ShinyPatchCalculatorTest.hpp"
#include <Core/Gen4/Tools/ShinyPatchCalculator.hpp>
#include <Core/RNG/LCRNG.hpp>
#include <QTest>
#include <algorithm>
#include <bit>
#include <set>

using namespace ShinyPatchCalculator;

static Grass allGrass()
{
    Grass grass;
    grass.fill(true);
    grass[40] = false;
    return grass;
}

void ShinyPatchCalculatorTest::rejected312()
{
    // Reported Platinum false positive: call 318's 0006 selects a shake type.
    PokeRNG rng(0x1d0b3634, 312);
    QCOMPARE(rng.getSeed(), 0xd726afec);
    const auto positions = calculate(rng.getSeed(), 1, false, allGrass()).patches;
    for (u32 mask = 0; mask < 16; mask++)
    {
        Grass grass {};
        for (u8 i = 0; i < 4; i++)
        {
            grass[(positions[i].y + 4) * 9 + positions[i].x + 4] = (mask & (1 << i)) != 0;
        }
        for (bool captured : { false, true })
        {
            const auto state = calculate(rng.getSeed(), 1, captured, grass);
            for (const auto &patch : state.patches)
            {
                QVERIFY(!patch.shiny);
            }
            QCOMPARE(state.calls, 4u + 2u * std::popcount(mask));
        }
    }
    const auto state = calculate(rng.getSeed(), 1, false, allGrass());
    const std::array<u8, 4> rolls { 98, 74, 80, 52 };
    for (u8 i = 0; i < 4; i++)
    {
        QCOMPARE(state.patches[i].chainRoll, rolls[i]);
        QVERIFY(!state.patches[i].continues);
        QCOMPARE(state.patches[i].shinyCall, 0);
    }
}

void ShinyPatchCalculatorTest::candidate466()
{
    PokeRNG rng(0xa01513d6, 466);
    QCOMPARE(rng.getSeed(), 0x11bae100);
    auto grass = allGrass();
    const auto state = calculate(rng.getSeed(), 1, false, grass);
    const auto &patch = state.patches[0];
    QCOMPARE(patch.ring, 4);
    QCOMPARE(patch.x, 3);
    QCOMPARE(patch.y, 4);
    QCOMPARE(patch.chainRoll, 84);
    QCOMPARE(patch.shinyRoll, 8);
    QCOMPARE(patch.shinyCall, 6); // Absolute check advance 472, activation advance 466
    QVERIFY(patch.continues);
    QVERIFY(patch.shiny);
    QCOMPARE(state.calls, 12u);
    QCOMPARE(state.endSeed, 0x0cbafd6cu);

    grass[8 * 9 + 7] = false;
    const auto blocked = calculate(rng.getSeed(), 1, false, grass);
    QVERIFY(!blocked.patches[0].active);
    for (const auto &p : blocked.patches)
    {
        QVERIFY(!p.shiny);
    }
    QCOMPARE(blocked.patches[1].chainCall, 5);
    QCOMPARE(blocked.calls, 10u);
}

void ShinyPatchCalculatorTest::innerRing()
{
    const auto state = calculate(0xc4, 40, false, allGrass());
    QVERIFY(!state.patches[0].continues);
    const auto &patch = state.patches[2];
    QCOMPARE(patch.ring, 2);
    QCOMPARE(patch.x, 2);
    QCOMPARE(patch.y, -1);
    QCOMPARE(patch.chainRoll, 21);
    QCOMPARE(patch.shinyRoll, 229);
    QCOMPARE(patch.shinyCall, 10);
    QVERIFY(patch.shiny);
    QCOMPARE(state.endSeed, 0x11f6d1f0u);
}

void ShinyPatchCalculatorTest::captureBonus()
{
    const auto normal = calculate(8, 1, false, allGrass());
    const auto captured = calculate(8, 1, true, allGrass());
    QCOMPARE(normal.patches[0].chainRoll, 91);
    QVERIFY(!normal.patches[0].continues);
    QVERIFY(captured.patches[0].continues);
    QCOMPARE(normal.patches[2].chainRoll, 48); // Equality fails the normal ring-2 rate.
    QVERIFY(!normal.patches[2].continues);
    QVERIFY(captured.patches[2].continues);
    QCOMPARE(captured.patches[2].shinyCall, 10);
    QCOMPARE(normal.endSeed, captured.endSeed);
}

void ShinyPatchCalculatorTest::zeroChain()
{
    const auto state = calculate(0, 0, false, allGrass());
    const std::array<u8, 4> calls { 5, 6, 8, 9 };
    const std::array<u8, 4> rolls { 55, 88, 40, 98 };
    for (u8 i = 0; i < 4; i++)
    {
        QCOMPARE(state.patches[i].chainCall, calls[i]);
        QCOMPARE(state.patches[i].chainRoll, rolls[i]);
        QCOMPARE(state.patches[i].shinyCall, 0);
        QVERIFY(!state.patches[i].shiny);
    }
    QCOMPARE(state.calls, 10u);
    QCOMPARE(state.endSeed, 4012700850u);
}

void ShinyPatchCalculatorTest::inactivePatches()
{
    Grass grass {};
    auto state = calculate(0, 1, false, grass);
    QCOMPARE(state.calls, 4u);
    QCOMPARE(state.endSeed, 833674724u);
    const std::array<s8, 4> x { -4, 3, -2, 0 };
    const std::array<s8, 4> y { -4, 1, 2, -1 };
    for (u8 i = 0; i < 4; i++)
    {
        QCOMPARE(state.patches[i].x, x[i]);
        QCOMPARE(state.patches[i].y, y[i]);
        QCOMPARE(state.patches[i].chainCall, 0);
        QVERIFY(!state.patches[i].active);
    }
    // Only the third and first rings are valid: no RNG calls for the other rings.
    grass[(1 + 4) * 9 + 3 + 4] = true;
    grass[(-1 + 4) * 9 + 0 + 4] = true;
    state = calculate(0, 1, false, grass);
    QCOMPARE(state.patches[1].chainCall, 5);
    QCOMPARE(state.patches[1].chainRoll, 55);
    QCOMPARE(state.patches[1].shinyRoll, 58060);
    QCOMPARE(state.patches[3].chainCall, 7);
    QCOMPARE(state.patches[3].chainRoll, 68);
    QCOMPARE(state.calls, 8u);
    QCOMPARE(state.endSeed, 1742452232u);
}

void ShinyPatchCalculatorTest::shinyThreshold_data()
{
    QTest::addColumn<u32>("seed");
    QTest::addColumn<u16>("chain");
    QTest::addColumn<u16>("roll");
    QTest::addColumn<bool>("shiny");
    QTest::newRow("chain-1-last-shiny") << 0xf9ddc322u << u16(1) << u16(8) << true;
    QTest::newRow("chain-1-first-normal") << 0xb426c322u << u16(1) << u16(9) << false;
    QTest::newRow("chain-39-last-shiny") << 0x44517d6bu << u16(39) << u16(163) << true;
    QTest::newRow("chain-39-first-normal") << 0x7e59c322u << u16(39) << u16(164) << false;
    QTest::newRow("chain-40-last-shiny") << 0x1ad4c322u << u16(40) << u16(327) << true;
    QTest::newRow("chain-40-first-normal") << 0xd51dc322u << u16(40) << u16(328) << false;
    QTest::newRow("chain-41-capped") << 0xd51dc322u << u16(41) << u16(328) << false;
    QTest::newRow("chain-999-capped") << 0x1ad4c322u << u16(999) << u16(327) << true;
    QTest::newRow("chain-u16-max-capped") << 0xd51dc322u << u16(65535) << u16(328) << false;
}

void ShinyPatchCalculatorTest::shinyThreshold()
{
    QFETCH(u32, seed);
    QFETCH(u16, chain);
    QFETCH(u16, roll);
    QFETCH(bool, shiny);
    const auto patch = calculate(seed, chain, false, allGrass()).patches[0];
    QVERIFY(patch.continues);
    QCOMPARE(patch.shinyCall, 6);
    QCOMPARE(patch.shinyRoll, roll);
    QCOMPARE(patch.shiny, shiny);
}

void ShinyPatchCalculatorTest::ringPositions()
{
    std::array<std::set<std::pair<s8, s8>>, 4> positions;
    for (u32 seed = 0; seed < 10000; seed++)
    {
        const auto state = calculate(seed, 1, false, allGrass());
        for (u8 i = 0; i < 4; i++)
        {
            const auto &patch = state.patches[i];
            QCOMPARE(patch.ring, 4 - i);
            QCOMPARE(std::max(std::abs(patch.x), std::abs(patch.y)), patch.ring);
            QVERIFY(patch.active);
            positions[i].insert({ patch.x, patch.y });
        }
    }
    QCOMPARE(positions[0].size(), size_t(32));
    QCOMPARE(positions[1].size(), size_t(24));
    QCOMPARE(positions[2].size(), size_t(16));
    QCOMPARE(positions[3].size(), size_t(8));
}
