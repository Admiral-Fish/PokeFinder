/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2024 by Admiral_Fish, bumba, and EzPzStreamz
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
#include <Core/Enum/DSType.hpp>
#include <Core/Gen5/Nazos.hpp>
#include <Core/Gen5/Profile5.hpp>
#include <Core/RNG/LCRNG64.hpp>
#include <Core/Util/DateTime.hpp>
#include <bit>

static u32 calcW(u32 *data, int i)
{
    u32 val = std::rotl(data[i - 3] ^ data[i - 8] ^ data[i - 14] ^ data[i - 16], 1);
    data[i] = val;
    return val;
}

static void calcWSIMD(u32 *data, int i)
{
    for (int index = i; index < i + 4; index++)
    {
        u32 val = std::rotl(data[index - 6] ^ data[index - 16] ^ data[index - 28] ^ data[index - 32], 2);
        data[index] = val;
    }
};

static consteval u32 computeBCD(u8 val)
{
    return ((val / 10) << 4) + (val % 10);
}

static consteval u32 computeWeekday(u16 year, u8 month, u8 day)
{
    u32 a = month < 3 ? 1 : 0;
    u32 y = year + 4800 - a;
    u32 m = month + 12 * a - 3;
    u32 jd = day + ((153 * m + 2) / 5) - 32045 + 365 * y + (y / 4) - (y / 100) + (y / 400);
    return (jd + 1) % 7;
}

static consteval std::array<u32, 36525> computeDateValues()
{
    std::array<u32, 36525> dates;

    constexpr u8 days[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    int index = 0;
    for (u16 year = 0; year < 100; year++)
    {
        u32 y = computeBCD(year) << 24;
        for (u8 month = 1; month <= 12; month++)
        {
            u32 m = computeBCD(month) << 16;

            u8 maxDays = days[month - 1];
            if (month == 2 && (year % 4) == 0)
            {
                maxDays++;
            }

            for (u8 day = 1; day <= maxDays; day++)
            {
                u32 d = computeBCD(day) << 8;
                dates[index++] = y | m | d | computeWeekday(year + 2000, month, day);
            }
        }
    }

    return dates;
}

static consteval std::array<u32, 86400> computeTimeValues()
{
    std::array<u32, 86400> times;

    int index = 0;
    for (u8 hour = 0; hour < 24; hour++)
    {
        u32 h = computeBCD(hour) << 24;
        if (hour >= 12)
        {
            hour |= 0x40000000;
        }

        for (u8 minute = 0; minute < 60; minute++)
        {
            u32 m = computeBCD(minute) << 16;
            for (u8 second = 0; second < 60; second++)
            {
                u32 s = computeBCD(second) << 8;
                times[index++] = h | m | s;
            }
        }
    }

    return times;
}

static inline void section1Calc(u32 a, u32 &b, u32 c, u32 d, u32 e, u32 &t, u32 input)
{
    t = std::rotl(a, 5) + ((b & c) | (~b & d)) + e + 0x5a827999 + input;
    b = std::rotr(b, 2);
};

static inline void section2Calc(u32 a, u32 &b, u32 c, u32 d, u32 e, u32 &t, u32 input)
{
    t = std::rotl(a, 5) + (b ^ c ^ d) + e + 0x6ed9eba1 + input;
    b = std::rotr(b, 2);
};

static inline void section3Calc(u32 a, u32 &b, u32 c, u32 d, u32 e, u32 &t, u32 input)
{
    t = std::rotl(a, 5) + ((b & c) | ((b | c) & d)) + e + 0x8f1bbcdc + input;
    b = std::rotr(b, 2);
};

static inline void section4Calc(u32 a, u32 &b, u32 c, u32 d, u32 e, u32 &t, u32 input)
{
    t = std::rotl(a, 5) + (b ^ c ^ d) + e + 0xca62c1d6 + input;
    b = std::rotr(b, 2);
};

constexpr std::array<u32, 36525> dateValues = computeDateValues();
constexpr std::array<u32, 86400> timeValues = computeTimeValues();

SHA1::SHA1(const Profile5 &profile) :
    SHA1(profile.getVersion(), profile.getLanguage(), profile.getDSType(), profile.getMac(), profile.getSoftReset(), profile.getVFrame(),
         profile.getGxStat())
{
}

SHA1::SHA1(Game version, Language language, DSType type, u64 mac, bool softReset, u8 vFrame, u8 gxStat)
{
    auto nazos = Nazos::getNazo(version, language, type);
    std::copy(nazos.begin(), nazos.end(), data);

    data[6] = mac & 0xffff;
    if (softReset)
    {
        data[6] ^= 0x01000000;
    }
    data[7] = static_cast<u32>((mac >> 16) ^ static_cast<u32>(vFrame << 24) ^ gxStat);

    // Set values
    data[10] = 0x00000000;
    data[11] = 0x00000000;
    data[13] = 0x80000000;
    data[14] = 0x00000000;
    data[15] = 0x000001a0;

    // Precompute data[18]
    calcW(data, 18);
}

u64 SHA1::hashSeed(const std::array<u32, 5> &alpha)
{
    u32 a = alpha[0];
    u32 b = alpha[1];
    u32 c = alpha[2];
    u32 d = alpha[3];
    u32 e = alpha[4];
    u32 t;

    // Section 1: 0-19
    // 0-8 already computed
    section1Calc(a, b, c, d, e, t, data[9]);
    section1Calc(t, a, b, c, d, e, 0x00000000); // data[10] is constant 0
    section1Calc(e, t, a, b, c, d, 0x00000000); // data[11] is constant 0
    section1Calc(d, e, t, a, b, c, data[12]);
    section1Calc(c, d, e, t, a, b, 0x80000000); // data[13] is constant 0x80000000
    section1Calc(b, c, d, e, t, a, 0x00000000); // data[14] is constant 0
    section1Calc(a, b, c, d, e, t, 0x000001a0); // data[15] is constant 0x000001a0
    section1Calc(t, a, b, c, d, e, data[16]);
    section1Calc(e, t, a, b, c, d, calcW(data, 17));
    section1Calc(d, e, t, a, b, c, data[18]);
    section1Calc(c, d, e, t, a, b, data[19]);

    // Section 2: 20 - 39
    section2Calc(b, c, d, e, t, a, calcW(data, 20));
    section2Calc(a, b, c, d, e, t, data[21]);
    section2Calc(t, a, b, c, d, e, data[22]);
    section2Calc(e, t, a, b, c, d, calcW(data, 23));
    section2Calc(d, e, t, a, b, c, data[24]);
    section2Calc(c, d, e, t, a, b, calcW(data, 25));
    section2Calc(b, c, d, e, t, a, calcW(data, 26));
    section2Calc(a, b, c, d, e, t, data[27]);
    section2Calc(t, a, b, c, d, e, calcW(data, 28));
    section2Calc(e, t, a, b, c, d, calcW(data, 29));
    section2Calc(d, e, t, a, b, c, data[30]);
    section2Calc(c, d, e, t, a, b, calcW(data, 31));
    calcWSIMD(data, 32);
    section2Calc(b, c, d, e, t, a, data[32]);
    section2Calc(a, b, c, d, e, t, data[33]);
    section2Calc(t, a, b, c, d, e, data[34]);
    section2Calc(e, t, a, b, c, d, data[35]);
    calcWSIMD(data, 36);
    section2Calc(d, e, t, a, b, c, data[36]);
    section2Calc(c, d, e, t, a, b, data[37]);
    section2Calc(b, c, d, e, t, a, data[38]);
    section2Calc(a, b, c, d, e, t, data[39]);

    // Section 3: 40 - 59
    calcWSIMD(data, 40);
    section3Calc(t, a, b, c, d, e, data[40]);
    section3Calc(e, t, a, b, c, d, data[41]);
    section3Calc(d, e, t, a, b, c, data[42]);
    section3Calc(c, d, e, t, a, b, data[43]);
    calcWSIMD(data, 44);
    section3Calc(b, c, d, e, t, a, data[44]);
    section3Calc(a, b, c, d, e, t, data[45]);
    section3Calc(t, a, b, c, d, e, data[46]);
    section3Calc(e, t, a, b, c, d, data[47]);
    calcWSIMD(data, 48);
    section3Calc(d, e, t, a, b, c, data[48]);
    section3Calc(c, d, e, t, a, b, data[49]);
    section3Calc(b, c, d, e, t, a, data[50]);
    section3Calc(a, b, c, d, e, t, data[51]);
    calcWSIMD(data, 52);
    section3Calc(t, a, b, c, d, e, data[52]);
    section3Calc(e, t, a, b, c, d, data[53]);
    section3Calc(d, e, t, a, b, c, data[54]);
    section3Calc(c, d, e, t, a, b, data[55]);
    calcWSIMD(data, 56);
    section3Calc(b, c, d, e, t, a, data[56]);
    section3Calc(a, b, c, d, e, t, data[57]);
    section3Calc(t, a, b, c, d, e, data[58]);
    section3Calc(e, t, a, b, c, d, data[59]);

    // Section 3: 60 - 79
    calcWSIMD(data, 60);
    section4Calc(d, e, t, a, b, c, data[60]);
    section4Calc(c, d, e, t, a, b, data[61]);
    section4Calc(b, c, d, e, t, a, data[62]);
    section4Calc(a, b, c, d, e, t, data[63]);
    calcWSIMD(data, 64);
    section4Calc(t, a, b, c, d, e, data[64]);
    section4Calc(e, t, a, b, c, d, data[65]);
    section4Calc(d, e, t, a, b, c, data[66]);
    section4Calc(c, d, e, t, a, b, data[67]);
    calcWSIMD(data, 68);
    section4Calc(b, c, d, e, t, a, data[68]);
    section4Calc(a, b, c, d, e, t, data[69]);
    section4Calc(t, a, b, c, d, e, data[70]);
    section4Calc(e, t, a, b, c, d, data[71]);
    calcWSIMD(data, 72);
    section4Calc(d, e, t, a, b, c, data[72]);
    section4Calc(c, d, e, t, a, b, data[73]);
    section4Calc(b, c, d, e, t, a, data[74]);
    section4Calc(a, b, c, d, e, t, data[75]);
    calcWSIMD(data, 76);
    section4Calc(t, a, b, c, d, e, data[76]);
    section4Calc(e, t, a, b, c, d, data[77]);
    section4Calc(d, e, t, a, b, c, data[78]);
    section4Calc(c, d, e, t, a, b, data[79]);

    u64 part1 = std::byteswap(b + 0x67452301);
    u64 part2 = std::byteswap(c + 0xefcdab89);

    u64 seed = (part2 << 32) | part1;
    return BWRNG(seed).next();
}

std::array<u32, 5> SHA1::precompute()
{
    u32 a = 0x67452301;
    u32 b = 0xefcdab89;
    u32 c = 0x98badcfe;
    u32 d = 0x10325476;
    u32 e = 0xc3d2e1f0;
    u32 t;

    section1Calc(a, b, c, d, e, t, data[0]);
    section1Calc(t, a, b, c, d, e, data[1]);
    section1Calc(e, t, a, b, c, d, data[2]);
    section1Calc(d, e, t, a, b, c, data[3]);
    section1Calc(c, d, e, t, a, b, data[4]);
    section1Calc(b, c, d, e, t, a, data[5]);
    section1Calc(a, b, c, d, e, t, data[6]);
    section1Calc(t, a, b, c, d, e, data[7]);
    section1Calc(e, t, a, b, c, d, data[8]);

    // Select values will be the same for same date
    data[16] = std::rotl(0x80000000 ^ data[8] ^ data[2] ^ data[0], 1); // data[13] is constant 0x80000000
    data[19] = std::rotl(data[16] ^ 0 ^ data[5] ^ data[3], 1); // data[11] is constant 0
    data[21] = std::rotl(data[18] ^ 0x80000000 ^ data[7] ^ data[5], 1); // data[13] is constant 0x80000000
    data[22] = std::rotl(data[19] ^ 0 ^ data[8] ^ data[6], 1); // data[14] is constant 0
    data[24] = std::rotl(data[21] ^ data[16] ^ 0 ^ data[8], 1); // data[10] is constant 0
    data[27] = std::rotl(data[24] ^ data[19] ^ 0x80000000 ^ 0, 1); // data[13] is constant 0x80000000 and data[11] is constant 0
    data[30] = std::rotl(data[27] ^ data[22] ^ data[16] ^ 0, 1); // data[14] is constant 0

    return { d, e, t, a, b };
}

void SHA1::setButton(u32 button)
{
    data[12] = button;
}

void SHA1::setDate(const Date &date)
{
    data[8] = dateValues[date.getJD() - Date().getJD()];
}

void SHA1::setTimer0(u32 timer0, u8 vcount)
{
    data[5] = std::byteswap(static_cast<u32>(vcount << 16) | timer0);
}

void SHA1::setTime(u32 time, DSType dsType)
{
    u32 val = timeValues[time];
    if (time >= 43200 && dsType != DSType::DS3)
    {
        val |= 0x40000000;
    }
    data[9] = val;
}

void SHA1::setTime(u8 hour, u8 minute, u8 second, DSType dsType)
{
    setTime(hour * 3600 + minute * 60 + second, dsType);
}
