#include "MTTest.hpp"
#include <Core/RNG/MT.hpp>
#include <QTest>

void MTTest::advance_data()
{
    QTest::addColumn<u32>("seed");
    QTest::addColumn<u32>("advances");
    QTest::addColumn<u32>("result");

    QTest::newRow("Advance 1") << 0x00000000U << 624U << 0xE2031CE4U;
    QTest::newRow("Advance 2") << 0x00000000U << 1248U << 0x44463F17U;
    QTest::newRow("Advance 3") << 0x80000000U << 624U << 0x881A2E77U;
    QTest::newRow("Advance 4") << 0x80000000U << 1248U << 0x64C4EF44U;
}

void MTTest::advance()
{
    QFETCH(u32, seed);
    QFETCH(u32, advances);
    QFETCH(u32, result);

    MT rng(seed);
    rng.advance(advances - 1);
    QCOMPARE(rng.next(), result);
}

void MTTest::next_data()
{
    QTest::addColumn<u32>("seed");
    QTest::addColumn<u32>("result");

    QTest::newRow("Next 1") << 0x00000000U << 0x8C7F0AACU;
    QTest::newRow("Next 2") << 0x40000000U << 0x355A22A8U;
    QTest::newRow("Next 3") << 0x80000000U << 0x26E9A91AU;
    QTest::newRow("Next 4") << 0xC0000000U << 0x4CD3ADB9U;
}

void MTTest::next()
{
    QFETCH(u32, seed);
    QFETCH(u32, result);

    MT rng(seed);
    QCOMPARE(rng.next(), result);
}
