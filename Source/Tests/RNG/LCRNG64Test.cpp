#include "LCRNG64Test.hpp"
#include <Core/RNG/LCRNG64.hpp>
#include <QTest>
#include <array>

void LCRNG64Test::advance_data()
{
    using Results = std::array<u64, 2>;

    QTest::addColumn<u64>("seed");
    QTest::addColumn<u32>("advances");
    QTest::addColumn<Results>("results");

    QTest::newRow("Advance 1") << 0x0000000000000000ULL << 5U << Results { 0xC83FB970153A9227, 0xADC2D56AF375BEB5 };
    QTest::newRow("Advance 2") << 0x0000000000000000ULL << 10U << Results { 0x67795501267F125A, 0x44B99BE460CCF9D6 };
    QTest::newRow("Advance 3") << 0x8000000000000000ULL << 5U << Results { 0x483FB970153A9227, 0x2DC2D56AF375BEB5 };
    QTest::newRow("Advance 4") << 0x8000000000000000ULL << 10U << Results { 0xE7795501267F125A, 0xC4B99BE460CCF9D6 };
}

void LCRNG64Test::advance()
{
    using Results = std::array<u64, 2>;

    QFETCH(u64, seed);
    QFETCH(u32, advances);
    QFETCH(Results, results);

    BWRNG bwrng(seed);
    bwrng.advance(advances);
    QCOMPARE(bwrng.getSeed(), results[0]);

    BWRNGR bwrngr(seed);
    bwrngr.advance(advances);
    QCOMPARE(bwrngr.getSeed(), results[1]);
}

void LCRNG64Test::next_data()
{
    using Results = std::array<u64, 2>;

    QTest::addColumn<u64>("seed");
    QTest::addColumn<Results>("results");

    QTest::newRow("Next 1") << 0x0000000000000000ULL << Results { 0x0000000000269EC3, 0x9B1AE6E9A384E6F9 };
    QTest::newRow("Next 2") << 0x4000000000000000ULL << Results { 0x4000000000269EC3, 0xDB1AE6E9A384E6F9 };
    QTest::newRow("Next 3") << 0x8000000000000000ULL << Results { 0x8000000000269EC3, 0x1B1AE6E9A384E6F9 };
    QTest::newRow("Next 4") << 0xC000000000000000ULL << Results { 0xC000000000269EC3, 0x5B1AE6E9A384E6F9 };
}

void LCRNG64Test::next()
{
    using Results = std::array<u64, 2>;

    QFETCH(u64, seed);
    QFETCH(Results, results);

    BWRNG bwrng(seed);
    QCOMPARE(bwrng.next(), results[0]);

    BWRNGR bwrngr(seed);
    QCOMPARE(bwrngr.next(), results[1]);
}
