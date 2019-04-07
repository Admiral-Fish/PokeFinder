/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2019 by Admiral_Fish, bumba, and EzPzStreamz
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

#define K0 0x5A827999
#define K1 0x6ED9EBA1
#define K2 0x8F1BBCDC
#define K3 0xCA62C1D6

#define H0 0x67452301
#define H1 0xEFCDAB89
#define H2 0x98BADCFE
#define H3 0x10325476
#define H4 0xC3D2E1F0

#define section1Calc() \
    temp = ((a << 5) | (a >> 27)) + ((b & c) | (~b & d)) + e + K0 + temp

#define section2Calc() \
    temp = ((a << 5) | (a >> 27)) + (b ^ c ^ d) + e + K1 + temp

#define section3Calc() \
    temp = ((a << 5) | (a >> 27)) + ((b & c) | ((b | c) & d)) + e + K2 + temp

#define section4Calc() \
    temp = ((a << 5) | (a >> 27)) + (b ^ c ^ d) + e + K3 + temp

#define updateVars() \
    e = d; \
    d = c; \
    c = (b << 30) | (b >> 2); \
    b = a; \
    a = temp

#define calcW(i) \
    temp = data[i - 3] ^ data[i - 8] ^ data[i - 14] ^ data[i - 16]; \
    data[i] = temp = (temp << 1) | (temp >> 31)


SHA1::SHA1(const Profile5 &profile)
{
    this->profile = profile;

    QVector<u32> nazos = Nazos::getNazo(profile);
    std::copy(nazos.begin(), nazos.end(), data);

    data[6] = profile.getMac() & 0xFFFF;
    if (profile.getSoftReset())
    {
        data[6] ^= 0x01000000;
    }
    data[7] = (profile.getMac() >> 16) ^ (profile.getVFrame() * 0x1000000) ^ profile.getGxStat();
    data[13] = 0x80000000;
    data[15] = 0x000001A0;
}

u64 SHA1::hashSeed()
{
    u32 temp = 0;
    u32 a = alpha[0];
    u32 b = alpha[1];
    u32 c = alpha[2];
    u32 d = alpha[3];
    u32 e = alpha[4];

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
    calcW(30); section2Calc(); updateVars();
    calcW(31); section2Calc(); updateVars();
    calcW(32); section2Calc(); updateVars();
    calcW(33); section2Calc(); updateVars();
    calcW(34); section2Calc(); updateVars();
    calcW(35); section2Calc(); updateVars();
    calcW(36); section2Calc(); updateVars();
    calcW(37); section2Calc(); updateVars();
    calcW(38); section2Calc(); updateVars();
    calcW(39); section2Calc(); updateVars();

    // Section 3: 40 - 59
    calcW(40); section3Calc(); updateVars();
    calcW(41); section3Calc(); updateVars();
    calcW(42); section3Calc(); updateVars();
    calcW(43); section3Calc(); updateVars();
    calcW(44); section3Calc(); updateVars();
    calcW(45); section3Calc(); updateVars();
    calcW(46); section3Calc(); updateVars();
    calcW(47); section3Calc(); updateVars();
    calcW(48); section3Calc(); updateVars();
    calcW(49); section3Calc(); updateVars();
    calcW(50); section3Calc(); updateVars();
    calcW(51); section3Calc(); updateVars();
    calcW(52); section3Calc(); updateVars();
    calcW(53); section3Calc(); updateVars();
    calcW(54); section3Calc(); updateVars();
    calcW(55); section3Calc(); updateVars();
    calcW(56); section3Calc(); updateVars();
    calcW(57); section3Calc(); updateVars();
    calcW(58); section3Calc(); updateVars();
    calcW(59); section3Calc(); updateVars();

    // Section 3: 60 - 79
    calcW(60); section4Calc(); updateVars();
    calcW(61); section4Calc(); updateVars();
    calcW(62); section4Calc(); updateVars();
    calcW(63); section4Calc(); updateVars();
    calcW(64); section4Calc(); updateVars();
    calcW(65); section4Calc(); updateVars();
    calcW(66); section4Calc(); updateVars();
    calcW(67); section4Calc(); updateVars();
    calcW(68); section4Calc(); updateVars();
    calcW(69); section4Calc(); updateVars();
    calcW(70); section4Calc(); updateVars();
    calcW(71); section4Calc(); updateVars();
    calcW(72); section4Calc(); updateVars();
    calcW(73); section4Calc(); updateVars();
    calcW(74); section4Calc(); updateVars();
    calcW(75); section4Calc(); updateVars();
    calcW(76); section4Calc(); updateVars();
    calcW(77); section4Calc(); updateVars();
    calcW(78); section4Calc(); updateVars();
    calcW(79); section4Calc();

    u64 part1 = reorder(temp + H0);
    u64 part2 = reorder(a + H1);

    u64 seed = (part2 << 32) | part1;
    seed = seed * 0x5d588b656c078965 + 0x269ec3;

    return seed;
}

void SHA1::preCompute()
{
    // For hashes computed on the same date, the first 8 steps will be the same

    u32 temp = 0;
    u32 a = H0;
    u32 b = H1;
    u32 c = H2;
    u32 d = H3;
    u32 e = H4;

    temp = data[0]; section1Calc(); updateVars();
    temp = data[1]; section1Calc(); updateVars();
    temp = data[2]; section1Calc(); updateVars();
    temp = data[3]; section1Calc(); updateVars();
    temp = data[4]; section1Calc(); updateVars();
    temp = data[5]; section1Calc(); updateVars();
    temp = data[6]; section1Calc(); updateVars();
    temp = data[7]; section1Calc(); updateVars();
    temp = data[8]; section1Calc(); updateVars();

    alpha[0] = a;
    alpha[1] = b;
    alpha[2] = c;
    alpha[3] = d;
    alpha[4] = e;

    // Also select values will be the same for same date
    data[16] = rotateLeft(data[13] ^ data[8] ^ data[2] ^ data[0], 1);
    data[18] = rotateLeft(data[15] ^ data[10] ^ data[4] ^ data[2], 1);
    data[19] = rotateLeft(data[16] ^ data[11] ^ data[5] ^ data[3], 1);
    data[21] = rotateLeft(data[18] ^ data[13] ^ data[7] ^ data[5], 1);
    data[22] = rotateLeft(data[19] ^ data[14] ^ data[8] ^ data[6], 1);
    data[24] = rotateLeft(data[21] ^ data[16] ^ data[10] ^ data[8], 1);
    data[27] = rotateLeft(data[24] ^ data[19] ^ data[13] ^ data[11], 1);
}

void SHA1::setTime(u8 hour, u8 minute, u8 second)
{
    u32 h = (toBCD(hour) + (hour >= 12 && profile.getDSType() != DSType::DS3 ? 0x40 : 0)) << 24;
    u32 m = toBCD(minute) << 16;
    u32 s = toBCD(second) << 8;
    u32 val = h | m | s;
    data[9] = val;
}

void SHA1::setTimer0(u32 timer0)
{
    data[5] = reorder((profile.getVCount() << 16) | timer0);
}

void SHA1::setDate(QDate date)
{
    u32 val = (toBCD((date.year() - 2000)) << 24) | (toBCD(date.day()) << 16) |
              (toBCD(date.day()) << 8) | toBCD(date.dayOfWeek());
    data[8] = val;
}

void SHA1::setButton(u32 button)
{
    data[12] = button;
}

u32 SHA1::reorder(u32 val)
{
    val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF);
    return (val << 16) | (val >> 16);
}

u32 SHA1::rotateLeft(u32 val, u8 count)
{
    return (val << count) | (val >> (32 - count));
}

u32 SHA1::rotateRight(u32 val, u8 count)
{
    return (val >> count) | (val << (32 - count));
}

u32 SHA1::toBCD(u32 value)
{
    u32 thousands = value / 1000;
    u32 allHundreds = value / 100;
    u32 allTens = value / 10;

    u32 hundreds = allHundreds - (thousands * 10);
    u32 tens = allTens - (allHundreds * 10);

    return (thousands << 12) | (hundreds << 8) | (tens << 4) | (value - (allTens * 10));
}
