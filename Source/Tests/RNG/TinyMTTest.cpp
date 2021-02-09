#include "TinyMTTest.hpp"
#include <Core/RNG/TinyMT.hpp>
#include <QTest>

void TinyMTTest::advance_data()
{
    QTest::addColumn<u32>("seed");
    QTest::addColumn<u32>("advances");
    QTest::addColumn<u32>("result");

    QTest::newRow("Advance 1") << 0x00000000U << 5U << 0x8D5F56B0U;
    QTest::newRow("Advance 2") << 0x00000000U << 10U << 0x4315A2A3U;
    QTest::newRow("Advance 3") << 0x80000000U << 5U << 0x4C4DDD7CU;
    QTest::newRow("Advance 4") << 0x80000000U << 10U << 0x527971DAU;
}

void TinyMTTest::advance()
{
    QFETCH(u32, seed);
    QFETCH(u32, advances);
    QFETCH(u32, result);

    TinyMT rng(seed);
    rng.advance(advances - 1);
    QCOMPARE(rng.next(), result);
}

void TinyMTTest::next_data()
{
    QTest::addColumn<u32>("seed");
    QTest::addColumn<u32>("result");

    QTest::newRow("Next 1") << 0x00000000U << 0x7C159927U;
    QTest::newRow("Next 2") << 0x40000000U << 0x28EF00ACU;
    QTest::newRow("Next 3") << 0x80000000U << 0xCBD0CB62U;
    QTest::newRow("Next 4") << 0xC0000000U << 0xBD61B252U;
}

void TinyMTTest::next()
{
    QFETCH(u32, seed);
    QFETCH(u32, result);

    TinyMT rng(seed);
    QCOMPARE(rng.next(), result);
}
