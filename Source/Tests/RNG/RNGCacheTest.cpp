#include "RNGCacheTest.hpp"
#include <Core/Enum/Method.hpp>
#include <Core/RNG/RNGCache.hpp>
#include <QTest>
#include <array>

using IVs = std::array<u8, 6>;
using Results = std::vector<u32>;
Q_DECLARE_METATYPE(Method)
Q_DECLARE_METATYPE(IVs)
Q_DECLARE_METATYPE(Results)

void RNGCacheTest::ivs_data()
{
    QTest::addColumn<Method>("method");
    QTest::addColumn<IVs>("ivs");
    QTest::addColumn<Results>("results");

    QTest::newRow("Method 1/2 6IV") << Method::Method1 << IVs { 31, 31, 31, 31, 31, 31 } << Results { 0x7FFF305A, 0x7FFF982D, 0x7FFFF961 };
    QTest::newRow("Method 1/2 Atk") << Method::Method1 << IVs { 31, 31, 31, 0, 31, 31 } << Results { 0x7FFF753B, 0x7FFFDD0E };
    QTest::newRow("Method 1/2 SpA") << Method::Method1 << IVs { 31, 0, 31, 31, 31, 31 } << Results { 0x7C1F0DEC, 0x7C1F75BF };
    QTest::newRow("Method 4 6IV") << Method::Method4 << IVs { 31, 31, 31, 31, 31, 31 } << Results { 0x7FFF52E5, 0x7FFF8D6E };
    QTest::newRow("Method 4 Atk") << Method::Method4 << IVs { 31, 31, 31, 0, 31, 31 } << Results { 0x7FFFA85E, 0x7FFFE2E7 };
    QTest::newRow("Method 4 SpA") << Method::Method4 << IVs { 31, 0, 31, 31, 31, 31 } << Results { 0x7C1FF9B6 };
}

void RNGCacheTest::ivs()
{
    QFETCH(Method, method);
    QFETCH(IVs, ivs);
    QFETCH(Results, results);

    RNGCache cache(method);
    QCOMPARE(cache.recoverLower16BitsIV(ivs[0], ivs[1], ivs[2], ivs[3], ivs[4], ivs[5]), results);
}

void RNGCacheTest::pid_data()
{
    QTest::addColumn<Method>("method");
    QTest::addColumn<u32>("pid");
    QTest::addColumn<Results>("results");

    QTest::newRow("Method 1/2") << Method::Method1 << 0x00000000U << Results { 0x00000000, 0x000067D3 };
    QTest::newRow("Method 1/2") << Method::Method1 << 0x80000000U << Results { 0x0000C907 };
    QTest::newRow("Method 4") << Method::Method4 << 0x00000000U << Results {};
    QTest::newRow("Method 4") << Method::Method4 << 0x80000000U << Results { 0x00000280, 0x00003D09 };
}

void RNGCacheTest::pid()
{
    QFETCH(Method, method);
    QFETCH(u32, pid);
    QFETCH(Results, results);

    RNGCache cache(method);
    QCOMPARE(cache.recoverLower16BitsPID(pid), results);
}
