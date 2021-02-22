#include "SFMTTest.hpp"
#include <Core/RNG/SFMT.hpp>
#include <QTest>

void SFMTTest::advance_data()
{
    QTest::addColumn<u32>("seed");
    QTest::addColumn<u32>("advances");
    QTest::addColumn<u64>("result");

    QTest::newRow("Advance 1") << 0x00000000U << 312U << 0x850F9C256959E905ULL;
    QTest::newRow("Advance 2") << 0x00000000U << 624U << 0xCF28F86E759721F9ULL;
    QTest::newRow("Advance 3") << 0x80000000U << 312U << 0x8B1D05116EFC4EC1ULL;
    QTest::newRow("Advance 4") << 0x80000000U << 624U << 0x39A9C69523428380ULL;
}

void SFMTTest::advance()
{
    QFETCH(u32, seed);
    QFETCH(u32, advances);
    QFETCH(u64, result);

    SFMT rng(seed);
    rng.advance(advances - 1);
    QCOMPARE(rng.next(), result);
}

void SFMTTest::next_data()
{
    QTest::addColumn<u32>("seed");
    QTest::addColumn<u64>("result");

    QTest::newRow("Next 1") << 0x00000000U << 0x0FCF240A2E0CAA58ULL;
    QTest::newRow("Next 2") << 0x40000000U << 0xEA99371E78ADDC1CULL;
    QTest::newRow("Next 3") << 0x80000000U << 0x8BFA1535E2A81345ULL;
    QTest::newRow("Next 4") << 0xC0000000U << 0xF42D08B9889C465FULL;
}

void SFMTTest::next()
{
    QFETCH(u32, seed);
    QFETCH(u64, result);

    SFMT rng(seed);
    QCOMPARE(rng.next(), result);
}

void SFMTTest::nextUInt_data()
{
    QTest::addColumn<u32>("seed");
    QTest::addColumn<u32>("result");

    QTest::newRow("Next 1") << 0x00000000U << 0x2E0CAA58U;
    QTest::newRow("Next 2") << 0x40000000U << 0x78ADDC1CU;
    QTest::newRow("Next 3") << 0x80000000U << 0xE2A81345U;
    QTest::newRow("Next 4") << 0xC0000000U << 0x889C465FU;
}

void SFMTTest::nextUInt()
{
    QFETCH(u32, seed);
    QFETCH(u32, result);

    SFMT rng(seed);
    QCOMPARE(rng.nextUInt(), result);
}
