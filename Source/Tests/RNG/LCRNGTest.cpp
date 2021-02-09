#include "LCRNGTest.hpp"
#include <Core/RNG/LCRNG.hpp>
#include <QTest>
#include <array>

void LCRNGTest::advance_data()
{
    using Results = std::array<u32, 6>;

    QTest::addColumn<u32>("seed");
    QTest::addColumn<u32>("advances");
    QTest::addColumn<Results>("results");

    QTest::newRow("Advance 1") << 0x00000000U << 5U << Results { 0xDE85D0CD, 0x2D834527, 0x8E425287, 0x0A84D1ED, 0xA2974C77, 0x16AEB36D };
    QTest::newRow("Advance 2") << 0x00000000U << 10U << Results { 0x014D329E, 0x6C662D72, 0xEF2CF4B2, 0x64E86D5E, 0x77948382, 0x3E86BD4E };
    QTest::newRow("Advance 3") << 0x80000000U << 5U << Results { 0x5E85D0CD, 0xAD834527, 0x0E425287, 0x8A84D1ED, 0x22974C77, 0x96AEB36D };
    QTest::newRow("Advance 4") << 0x80000000U << 10U << Results { 0x814D329E, 0xEC662D72, 0x6F2CF4B2, 0xE4E86D5E, 0xF7948382, 0xBE86BD4E };
}

void LCRNGTest::advance()
{
    using Results = std::array<u32, 6>;

    QFETCH(u32, seed);
    QFETCH(u32, advances);
    QFETCH(Results, results);

    ARNG arng(seed);
    arng.advance(advances);
    QCOMPARE(arng.getSeed(), results[0]);

    ARNGR arngr(seed);
    arngr.advance(advances);
    QCOMPARE(arngr.getSeed(), results[1]);

    PokeRNG pokerng(seed);
    pokerng.advance(advances);
    QCOMPARE(pokerng.getSeed(), results[2]);

    PokeRNGR pokerngr(seed);
    pokerngr.advance(advances);
    QCOMPARE(pokerngr.getSeed(), results[3]);

    XDRNG xdrng(seed);
    xdrng.advance(advances);
    QCOMPARE(xdrng.getSeed(), results[4]);

    XDRNGR xdrngr(seed);
    xdrngr.advance(advances);
    QCOMPARE(xdrngr.getSeed(), results[5]);
}

void LCRNGTest::next_data()
{
    using Results = std::array<u32, 6>;

    QTest::addColumn<u32>("seed");
    QTest::addColumn<Results>("results");

    QTest::newRow("Next 1") << 0x00000000U << Results { 0x00000001, 0x69C77F93, 0x00006073, 0x0A3561A1, 0x00269EC3, 0xA170F641 };
    QTest::newRow("Next 2") << 0x40000000U << Results { 0x40000001, 0xA9C77F93, 0x40006073, 0x4A3561A1, 0x40269EC3, 0xE170F641 };
    QTest::newRow("Next 3") << 0x80000000U << Results { 0x80000001, 0xE9C77F93, 0x80006073, 0x8A3561A1, 0x80269EC3, 0x2170F641 };
    QTest::newRow("Next 4") << 0xC0000000U << Results { 0xC0000001, 0x29C77F93, 0xC0006073, 0xCA3561A1, 0xC0269EC3, 0x6170F641 };
}

void LCRNGTest::next()
{
    using Results = std::array<u32, 6>;

    QFETCH(u32, seed);
    QFETCH(Results, results);

    ARNG arng(seed);
    QCOMPARE(arng.next(), results[0]);

    ARNGR arngr(seed);
    QCOMPARE(arngr.next(), results[1]);

    PokeRNG pokerng(seed);
    QCOMPARE(pokerng.next(), results[2]);

    PokeRNGR pokerngr(seed);
    QCOMPARE(pokerngr.next(), results[3]);

    XDRNG xdrng(seed);
    QCOMPARE(xdrng.next(), results[4]);

    XDRNGR xdrngr(seed);
    QCOMPARE(xdrngr.next(), results[5]);
}
