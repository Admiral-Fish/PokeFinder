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
#include <Core/Util/DateTime.hpp>
#include <array>

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

SHA1::SHA1(const Profile5 &profile) :
    SHA1(profile.getVersion(), profile.getLanguage(), profile.getDSType(), profile.getMac(), profile.getSoftReset(), profile.getVFrame(),
         profile.getGxStat())
{
}

SHA1::SHA1(Game version, Language language, DSType type, u64 mac, bool softReset, u8 vFrame, u8 gxStat)
{
    auto nazos = Nazos::getNazo(version, language, type);
    std::copy(nazos.begin(), nazos.end(), data);

    data[6] = mac & 0xFFFF;
    if (softReset)
    {
        data[6] ^= 0x01000000;
    }
    data[7] = static_cast<u32>((mac >> 16) ^ static_cast<u32>(vFrame << 24) ^ gxStat);

    // Set values
    data[10] = 0;
    data[11] = 0;
    data[13] = 0x80000000;
    data[14] = 0;
    data[15] = 0x000001A0;

    // Precompute data[18]
    data[18] = rotateLeft(data[15] ^ data[10] ^ data[4] ^ data[2], 1);
}

u64 SHA1::hashSeed()
{
    u32 a = alpha[0];
    u32 b = alpha[1];
    u32 c = alpha[2];
    u32 d = alpha[3];
    u32 e = alpha[4];
    u32 t;

    auto section1Calc = [](const u32 &a, u32 &b, const u32 &c, const u32 &d, const u32 &e, u32 &t, const u32 &input) {
        t = rotateLeft(a, 5) + ((b & c) | (~b & d)) + e + 0x5A827999 + input;
        b = rotateRight(b, 2);
    };
    auto section2Calc = [](const u32 &a, u32 &b, const u32 &c, const u32 &d, const u32 &e, u32 &t, const u32 &input) {
        t = rotateLeft(a, 5) + (b ^ c ^ d) + e + 0x6ED9EBA1 + input;
        b = rotateRight(b, 2);
    };
    auto section3Calc = [](const u32 &a, u32 &b, const u32 &c, const u32 &d, const u32 &e, u32 &t, const u32 &input) {
        t = rotateLeft(a, 5) + ((b & c) | ((b | c) & d)) + e + 0x8F1BBCDC + input;
        b = rotateRight(b, 2);
    };
    auto section4Calc = [](const u32 &a, u32 &b, const u32 &c, const u32 &d, const u32 &e, u32 &t, const u32 &input) {
        t = rotateLeft(a, 5) + (b ^ c ^ d) + e + 0xCA62C1D6 + input;
        b = rotateRight(b, 2);
    };

    auto calcW = [this](int i) {
        u32 val = rotateLeft(data[i - 3] ^ data[i - 8] ^ data[i - 14] ^ data[i - 16], 1);
        data[i] = val;
        return val;
    };
    auto calcWSIMD = [this](int i) {
        u32 val = rotateLeft(data[i - 6] ^ data[i - 16] ^ data[i - 28] ^ data[i - 32], 2);
        data[i] = val;
        return val;
    };

    // Section 1: 0-19
    // 0-8 already computed
    section1Calc(a, b, c, d, e, t, data[9]);
    section1Calc(t, a, b, c, d, e, data[10]);
    section1Calc(e, t, a, b, c, d, data[11]);
    section1Calc(d, e, t, a, b, c, data[12]);
    section1Calc(c, d, e, t, a, b, data[13]);
    section1Calc(b, c, d, e, t, a, data[14]);
    section1Calc(a, b, c, d, e, t, data[15]);
    section1Calc(t, a, b, c, d, e, data[16]);
    section1Calc(e, t, a, b, c, d, calcW(17));
    section1Calc(d, e, t, a, b, c, data[18]);
    section1Calc(c, d, e, t, a, b, data[19]);

    // Section 2: 20 - 39
    section2Calc(b, c, d, e, t, a, calcW(20));
    section2Calc(a, b, c, d, e, t, data[21]);
    section2Calc(t, a, b, c, d, e, data[22]);
    section2Calc(e, t, a, b, c, d, calcW(23));
    section2Calc(d, e, t, a, b, c, data[24]);
    section2Calc(c, d, e, t, a, b, calcW(25));
    section2Calc(b, c, d, e, t, a, calcW(26));
    section2Calc(a, b, c, d, e, t, data[27]);
    section2Calc(t, a, b, c, d, e, calcW(28));
    section2Calc(e, t, a, b, c, d, calcW(29));
    section2Calc(d, e, t, a, b, c, data[30]);
    section2Calc(c, d, e, t, a, b, calcW(31));
    section2Calc(b, c, d, e, t, a, calcWSIMD(32));
    section2Calc(a, b, c, d, e, t, calcWSIMD(33));
    section2Calc(t, a, b, c, d, e, calcWSIMD(34));
    section2Calc(e, t, a, b, c, d, calcWSIMD(35));
    section2Calc(d, e, t, a, b, c, calcWSIMD(36));
    section2Calc(c, d, e, t, a, b, calcWSIMD(37));
    section2Calc(b, c, d, e, t, a, calcWSIMD(38));
    section2Calc(a, b, c, d, e, t, calcWSIMD(39));

    // Section 3: 40 - 59
    section3Calc(t, a, b, c, d, e, calcWSIMD(40));
    section3Calc(e, t, a, b, c, d, calcWSIMD(41));
    section3Calc(d, e, t, a, b, c, calcWSIMD(42));
    section3Calc(c, d, e, t, a, b, calcWSIMD(43));
    section3Calc(b, c, d, e, t, a, calcWSIMD(44));
    section3Calc(a, b, c, d, e, t, calcWSIMD(45));
    section3Calc(t, a, b, c, d, e, calcWSIMD(46));
    section3Calc(e, t, a, b, c, d, calcWSIMD(47));
    section3Calc(d, e, t, a, b, c, calcWSIMD(48));
    section3Calc(c, d, e, t, a, b, calcWSIMD(49));
    section3Calc(b, c, d, e, t, a, calcWSIMD(50));
    section3Calc(a, b, c, d, e, t, calcWSIMD(51));
    section3Calc(t, a, b, c, d, e, calcWSIMD(52));
    section3Calc(e, t, a, b, c, d, calcWSIMD(53));
    section3Calc(d, e, t, a, b, c, calcWSIMD(54));
    section3Calc(c, d, e, t, a, b, calcWSIMD(55));
    section3Calc(b, c, d, e, t, a, calcWSIMD(56));
    section3Calc(a, b, c, d, e, t, calcWSIMD(57));
    section3Calc(t, a, b, c, d, e, calcWSIMD(58));
    section3Calc(e, t, a, b, c, d, calcWSIMD(59));

    // Section 3: 60 - 79
    section4Calc(d, e, t, a, b, c, calcWSIMD(60));
    section4Calc(c, d, e, t, a, b, calcWSIMD(61));
    section4Calc(b, c, d, e, t, a, calcWSIMD(62));
    section4Calc(a, b, c, d, e, t, calcWSIMD(63));
    section4Calc(t, a, b, c, d, e, calcWSIMD(64));
    section4Calc(e, t, a, b, c, d, calcWSIMD(65));
    section4Calc(d, e, t, a, b, c, calcWSIMD(66));
    section4Calc(c, d, e, t, a, b, calcWSIMD(67));
    section4Calc(b, c, d, e, t, a, calcWSIMD(68));
    section4Calc(a, b, c, d, e, t, calcWSIMD(69));
    section4Calc(t, a, b, c, d, e, calcWSIMD(70));
    section4Calc(e, t, a, b, c, d, calcWSIMD(71));
    section4Calc(d, e, t, a, b, c, calcWSIMD(72));
    section4Calc(c, d, e, t, a, b, calcWSIMD(73));
    section4Calc(b, c, d, e, t, a, calcWSIMD(74));
    section4Calc(a, b, c, d, e, t, calcWSIMD(75));
    section4Calc(t, a, b, c, d, e, calcWSIMD(76));
    section4Calc(e, t, a, b, c, d, calcWSIMD(77));
    section4Calc(d, e, t, a, b, c, calcWSIMD(78));
    section4Calc(c, d, e, t, a, b, calcWSIMD(79));

    u64 part1 = changeEndian(b + 0x67452301);
    u64 part2 = changeEndian(c + 0xEFCDAB89);

    u64 seed = (part2 << 32) | part1;
    return BWRNG(seed).next();
}

void SHA1::precompute()
{
    // For hashes computed on the same date, the first 8 rounds will be the same
    u32 a = 0x67452301;
    u32 b = 0xEFCDAB89;
    u32 c = 0x98BADCFE;
    u32 d = 0x10325476;
    u32 e = 0xC3D2E1F0;
    u32 t;

    auto section1Calc = [](const u32 &a, u32 &b, const u32 &c, const u32 &d, const u32 &e, u32 &t, const u32 &input) {
        t = rotateLeft(a, 5) + ((b & c) | (~b & d)) + e + 0x5A827999 + input;
        b = rotateRight(b, 2);
    };
    auto calcW = [this](int i) { data[i] = rotateLeft(data[i - 3] ^ data[i - 8] ^ data[i - 14] ^ data[i - 16], 1); };

    section1Calc(a, b, c, d, e, t, data[0]);
    section1Calc(t, a, b, c, d, e, data[1]);
    section1Calc(e, t, a, b, c, d, data[2]);
    section1Calc(d, e, t, a, b, c, data[3]);
    section1Calc(c, d, e, t, a, b, data[4]);
    section1Calc(b, c, d, e, t, a, data[5]);
    section1Calc(a, b, c, d, e, t, data[6]);
    section1Calc(t, a, b, c, d, e, data[7]);
    section1Calc(e, t, a, b, c, d, data[8]);

    alpha[0] = d;
    alpha[1] = e;
    alpha[2] = t;
    alpha[3] = a;
    alpha[4] = b;

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

void SHA1::setDate(const Date &date)
{
    auto parts = date.getParts();
    u32 val = static_cast<u32>((bcd(parts[0] - 2000) << 24) | (bcd(parts[1]) << 16) | (bcd(parts[2]) << 8) | date.dayOfWeek());
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
