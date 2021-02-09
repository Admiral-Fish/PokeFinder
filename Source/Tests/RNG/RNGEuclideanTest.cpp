#include "RNGEuclideanTest.hpp"
#include <Core/RNG/RNGEuclidean.hpp>
#include <QTest>
#include <array>

void RNGEuclideanTest::ivs_data()
{
    using Results = std::vector<std::pair<u32, u32>>;

    QTest::addColumn<std::array<u8, 6>>("ivs");
    QTest::addColumn<Results>("results");

    QTest::newRow("6IV") << std::array<u8, 6> { 31, 31, 31, 31, 31, 31 }
                         << Results { std::make_pair(0x7FFF14C6, 0x7FFFF871), std::make_pair(0x7FFF8A5D, 0xFFFFB3AC),
                                      std::make_pair(0x7FFFFFF4, 0x7FFF6EE7) };
    QTest::newRow("Atk") << std::array<u8, 6> { 31, 31, 31, 0, 31, 31 }
                         << Results { std::make_pair(0x7FFF1396, 0x7C1F3C01), std::make_pair(0x7FFFD792, 0xFC1FE00D) };
    QTest::newRow("SpA") << std::array<u8, 6> { 31, 0, 31, 31, 31, 31 }
                         << Results { std::make_pair(0x7C1F3869, 0x7FFFD988), std::make_pair(0x7C1FAE00, 0xFFFF94C3) };
}

void RNGEuclideanTest::ivs()
{
    using IVs = std::array<u8, 6>;
    using Results = std::vector<std::pair<u32, u32>>;

    QFETCH(IVs, ivs);
    QFETCH(Results, results);

    QCOMPARE(RNGEuclidean::recoverLower16BitsIV(ivs[0], ivs[1], ivs[2], ivs[3], ivs[4], ivs[5]), results);
}

void RNGEuclideanTest::pid_data()
{
    using Results = std::vector<std::pair<u32, u32>>;

    QTest::addColumn<u32>("pid");
    QTest::addColumn<Results>("results");

    QTest::newRow("Test 1") << 0x00000000U << Results { std::make_pair(0x00004E59, 0x000057B8) };
    QTest::newRow("Test 2") << 0x80000000U << Results { std::make_pair(0x8000C3F0, 0x000012F3) };
}

void RNGEuclideanTest::pid()
{
    using Results = std::vector<std::pair<u32, u32>>;

    QFETCH(u32, pid);
    QFETCH(Results, results);

    QCOMPARE(RNGEuclidean::recoverLower16BitsPID(pid), results);
}

void RNGEuclideanTest::ivsChannel_data()
{
    using IVs = std::array<u8, 6>;
    using Results = std::vector<u32>;

    QTest::addColumn<IVs>("ivs");
    QTest::addColumn<Results>("results");

    QTest::newRow("6IV") << IVs { 31, 31, 31, 31, 31, 31 } << Results { 0xFBA5085C, 0xFBCB9DE0, 0xFD156C92, 0xFD3C0216 };
    QTest::newRow("Atk") << IVs { 31, 31, 31, 0, 31, 31 } << Results { 0xFCC8418A, 0xFCEED70E, 0xFD560753, 0xFED2AC77 };
    QTest::newRow("SpA") << IVs { 31, 0, 31, 31, 31, 31 } << Results { 0xFC87A6C9, 0xFCAE3C4D, 0xFE9211B6 };
}

void RNGEuclideanTest::ivsChannel()
{
    using IVs = std::array<u8, 6>;
    using Results = std::vector<u32>;

    QFETCH(IVs, ivs);
    QFETCH(Results, results);

    QCOMPARE(RNGEuclidean::recoverLower27BitsChannel(ivs[0], ivs[1], ivs[2], ivs[3], ivs[4], ivs[5]), results);
}
