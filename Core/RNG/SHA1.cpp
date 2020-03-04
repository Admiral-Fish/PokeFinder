/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2020 by Admiral_Fish, bumba, and EzPzStreamz
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

#include "SHA1.hpp"
#include <Core/Gen5/Nazos.hpp>
#include <Core/Gen5/Profile5.hpp>
#include <Core/RNG/LCRNG64.hpp>
#include <array>
#include <cstring>

inline u32 changeEndian(u32 val)
{
    val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF);
    return (val << 16) | (val >> 16);
}

inline u32 rotateLeft(u32 val, u8 count)
{
    return (val << count) | (val >> (32 - count));
}

inline u32 rotateRight(u32 val, u8 count)
{
    return (val << (32 - count)) | (val >> count);
}

inline u8 bcd(u8 value)
{
    u8 tens = value / 10;
    u8 ones = value % 10;

    return static_cast<u8>(tens << 4) | ones;
}

SHA1::SHA1(const Profile5 &profile)
{
    std::memset(data, 0, sizeof(data));

    auto nazos = Nazos::getNazo(profile);
    std::copy(nazos.begin(), nazos.end(), data);

    data[6] = profile.getMac() & 0xFFFF;
    if (profile.getSoftReset())
    {
        data[6] ^= 0x01000000;
    }
    data[7] = static_cast<u32>((profile.getMac() >> 16) ^ static_cast<u32>(profile.getVFrame() << 24) ^ profile.getGxStat());
    data[13] = 0x80000000;
    data[15] = 0x000001A0;

    // Precompute data[18]
    data[18] = rotateLeft(data[15] ^ data[10] ^ data[4] ^ data[2], 1);
}

u64 SHA1::hashSeed()
{
    u32 temp;
    u32 a = alpha[0];
    u32 b = alpha[1];
    u32 c = alpha[2];
    u32 d = alpha[3];
    u32 e = alpha[4];

    auto section1Calc = [&temp, &a, &b, &c, &d, &e] { temp = rotateLeft(a, 5) + ((b & c) | (~b & d)) + e + 0x5A827999 + temp; };
    auto section2Calc = [&temp, &a, &b, &c, &d, &e] { temp = rotateLeft(a, 5) + (b ^ c ^ d) + e + 0x6ED9EBA1 + temp; };
    auto section3Calc = [&temp, &a, &b, &c, &d, &e] { temp = rotateLeft(a, 5) + ((b & c) | ((b | c) & d)) + e + 0x8F1BBCDC + temp; };
    auto section4Calc = [&temp, &a, &b, &c, &d, &e] { temp = rotateLeft(a, 5) + (b ^ c ^ d) + e + 0xCA62C1D6 + temp; };
    auto updateVars = [&temp, &a, &b, &c, &d, &e] {
        e = d;
        d = c;
        c = rotateRight(b, 2);
        b = a;
        a = temp;
    };
    auto calcW = [&temp, this](int i) {
        temp = data[i - 3] ^ data[i - 8] ^ data[i - 14] ^ data[i - 16];
        data[i] = temp = rotateLeft(temp, 1);
    };
    auto calcWSIMD = [&temp, this](int i) {
        temp = data[i - 6] ^ data[i - 16] ^ data[i - 28] ^ data[i - 32];
        data[i] = temp = rotateLeft(temp, 2);
    };

    // clang-format off
    
    // Section 1: 0-19
    // 0-8 already computed
    temp = data[9]; section1Calc(); updateVars();
    temp = data[10]; section1Calc(); updateVars();
    temp = data[11]; section1Calc(); updateVars();
    temp = data[12]; section1Calc(); updateVars();
    temp = data[13]; section1Calc(); updateVars();
    temp = data[14]; section1Calc(); updateVars();
    temp = data[15]; section1Calc(); updateVars();
    temp = data[16]; section1Calc(); updateVars();
    calcW(17); section1Calc(); updateVars();
    temp = data[18]; section1Calc(); updateVars();
    temp = data[19]; section1Calc(); updateVars();

    // Section 2: 20 - 39
    calcW(20); section2Calc(); updateVars();
    temp = data[21]; section2Calc(); updateVars();
    temp = data[22]; section2Calc(); updateVars();
    calcW(23); section2Calc(); updateVars();
    temp = data[24]; section2Calc(); updateVars();
    calcW(25); section2Calc(); updateVars();
    calcW(26); section2Calc(); updateVars();
    temp = data[27]; section2Calc(); updateVars();
    calcW(28); section2Calc(); updateVars();
    calcW(29); section2Calc(); updateVars();
    temp = data[30]; section2Calc(); updateVars();
    calcW(31); section2Calc(); updateVars();
    calcWSIMD(32); section2Calc(); updateVars();
    calcWSIMD(33); section2Calc(); updateVars();
    calcWSIMD(34); section2Calc(); updateVars();
    calcWSIMD(35); section2Calc(); updateVars();
    calcWSIMD(36); section2Calc(); updateVars();
    calcWSIMD(37); section2Calc(); updateVars();
    calcWSIMD(38); section2Calc(); updateVars();
    calcWSIMD(39); section2Calc(); updateVars();

    // Section 3: 40 - 59
    calcWSIMD(40); section3Calc(); updateVars();
    calcWSIMD(41); section3Calc(); updateVars();
    calcWSIMD(42); section3Calc(); updateVars();
    calcWSIMD(43); section3Calc(); updateVars();
    calcWSIMD(44); section3Calc(); updateVars();
    calcWSIMD(45); section3Calc(); updateVars();
    calcWSIMD(46); section3Calc(); updateVars();
    calcWSIMD(47); section3Calc(); updateVars();
    calcWSIMD(48); section3Calc(); updateVars();
    calcWSIMD(49); section3Calc(); updateVars();
    calcWSIMD(50); section3Calc(); updateVars();
    calcWSIMD(51); section3Calc(); updateVars();
    calcWSIMD(52); section3Calc(); updateVars();
    calcWSIMD(53); section3Calc(); updateVars();
    calcWSIMD(54); section3Calc(); updateVars();
    calcWSIMD(55); section3Calc(); updateVars();
    calcWSIMD(56); section3Calc(); updateVars();
    calcWSIMD(57); section3Calc(); updateVars();
    calcWSIMD(58); section3Calc(); updateVars();
    calcWSIMD(59); section3Calc(); updateVars();

    // Section 3: 60 - 79
    calcWSIMD(60); section4Calc(); updateVars();
    calcWSIMD(61); section4Calc(); updateVars();
    calcWSIMD(62); section4Calc(); updateVars();
    calcWSIMD(63); section4Calc(); updateVars();
    calcWSIMD(64); section4Calc(); updateVars();
    calcWSIMD(65); section4Calc(); updateVars();
    calcWSIMD(66); section4Calc(); updateVars();
    calcWSIMD(67); section4Calc(); updateVars();
    calcWSIMD(68); section4Calc(); updateVars();
    calcWSIMD(69); section4Calc(); updateVars();
    calcWSIMD(70); section4Calc(); updateVars();
    calcWSIMD(71); section4Calc(); updateVars();
    calcWSIMD(72); section4Calc(); updateVars();
    calcWSIMD(73); section4Calc(); updateVars();
    calcWSIMD(74); section4Calc(); updateVars();
    calcWSIMD(75); section4Calc(); updateVars();
    calcWSIMD(76); section4Calc(); updateVars();
    calcWSIMD(77); section4Calc(); updateVars();
    calcWSIMD(78); section4Calc(); updateVars();
    calcWSIMD(79); section4Calc();

    // clang-format on

    u64 part1 = changeEndian(temp + 0x67452301);
    u64 part2 = changeEndian(a + 0xEFCDAB89);

    u64 seed = (part2 << 32) | part1;
    return BWRNG(seed).nextULong();
}

void SHA1::precompute()
{
    // For hashes computed on the same date, the first 8 steps will be the same
    u32 temp;
    u32 a = 0x67452301;
    u32 b = 0xEFCDAB89;
    u32 c = 0x98BADCFE;
    u32 d = 0x10325476;
    u32 e = 0xC3D2E1F0;

    auto section1Calc = [&temp, &a, &b, &c, &d, &e] { temp = rotateLeft(a, 5) + ((b & c) | (~b & d)) + e + 0x5A827999 + temp; };
    auto updateVars = [&temp, &a, &b, &c, &d, &e] {
        e = d;
        d = c;
        c = rotateRight(b, 2);
        b = a;
        a = temp;
    };
    auto calcW = [this](int i) { data[i] = rotateLeft(data[i - 3] ^ data[i - 8] ^ data[i - 14] ^ data[i - 16], 1); };

    // clang-format off

    temp = data[0]; section1Calc(); updateVars();
    temp = data[1]; section1Calc(); updateVars();
    temp = data[2]; section1Calc(); updateVars();
    temp = data[3]; section1Calc(); updateVars();
    temp = data[4]; section1Calc(); updateVars();
    temp = data[5]; section1Calc(); updateVars();
    temp = data[6]; section1Calc(); updateVars();
    temp = data[7]; section1Calc(); updateVars();
    temp = data[8]; section1Calc(); updateVars();

    // clang-format on

    alpha[0] = a;
    alpha[1] = b;
    alpha[2] = c;
    alpha[3] = d;
    alpha[4] = e;

    // Select values will be the same for same date
    calcW(16);
    // calcW(18); Enough information is known to calculate this in the constructor
    calcW(19);
    calcW(21);
    calcW(22);
    calcW(24);
    calcW(27);
    calcW(30);
}

void SHA1::setTimer0(u32 timer0, u8 vcount)
{
    data[5] = changeEndian(static_cast<u32>(vcount << 16) | timer0);
}

void SHA1::setDate(u8 year, u8 month, u8 day, u8 week)
{
    u32 val = static_cast<u32>((bcd(year) << 24) | (bcd(month) << 16) | (bcd(day) << 8) | week);
    data[8] = val;
}

void SHA1::setTime(u8 hour, u8 minute, u8 second, DSType dsType)
{
    u32 h = static_cast<u32>((bcd(hour) + (hour >= 12 && dsType != DSType::DS3 ? 0x40 : 0)) << 24);
    u32 m = static_cast<u32>(bcd(minute) << 16);
    u32 s = static_cast<u32>(bcd(second) << 8);
    u32 val = h | m | s;
    data[9] = val;
}

void SHA1::setButton(u32 button)
{
    data[12] = button;
}
