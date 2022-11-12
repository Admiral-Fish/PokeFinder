/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2022 by Admiral_Fish, bumba, and EzPzStreamz
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

#include "Utilities.hpp"
#include <Core/RNG/LCRNG.hpp>
#include <Core/RNG/LCRNG64.hpp>
#include <Core/RNG/MT.hpp>
#include <Core/Util/DateTime.hpp>
#include <bzlib.h>

namespace
{
    u32 advanceProbabilityTable(BWRNG &rng)
    {
        u32 count = 0;

        // Round 1
        count++;
        rng.advance(1);

        // Round 2
        count++;
        if (rng.nextUInt(101) > 50)
        {
            count++;
            rng.advance(1);
        }

        // Round 3
        count++;
        if (rng.nextUInt(101) > 30)
        {
            count++;
            rng.advance(1);
        }

        // Round 4
        count++;
        if (rng.nextUInt(101) > 25)
        {
            count++;
            if (rng.nextUInt(101) > 30)
            {
                count++;
                rng.advance(1);
            }
        }

        // Round 5
        count++;
        if (rng.nextUInt(101) > 20)
        {
            count++;
            if (rng.nextUInt(101) > 25)
            {
                count++;
                if (rng.nextUInt(101) > 33)
                {
                    count++;
                    rng.advance(1);
                }
            }
        }

        return count;
    }

    std::string getPitch(u8 result)
    {
        std::string pitch;
        if (result < 20)
        {
            pitch = "L ";
        }
        else if (result < 40)
        {
            pitch = "ML ";
        }
        else if (result < 60)
        {
            pitch = "M ";
        }
        else if (result < 80)
        {
            pitch = "MH ";
        }
        else
        {
            pitch = "H ";
        }

        return pitch + std::to_string(result);
    }
}

namespace Utilities
{
    char *decompress(const char *compressedData, u32 compressedSize, u32 &size)
    {
        size = *reinterpret_cast<const u16 *>(compressedData);
        char *data = new char[size];

        BZ2_bzBuffToBuffDecompress(data, &size, const_cast<char *>(compressedData + sizeof(u16)), compressedSize, 0, 0);

        return data;
    }

    u8 *decompress(const u8 *compressedData, u32 compressedSize, u32 &size)
    {
        size = *reinterpret_cast<const u16 *>(compressedData);
        u8 *data = new u8[size];

        BZ2_bzBuffToBuffDecompress(reinterpret_cast<char *>(data), &size,
                                   reinterpret_cast<char *>(const_cast<u8 *>(compressedData + sizeof(u16))), compressedSize, 0, 0);

        return data;
    }
}

namespace Utilities3
{
    u16 calcSeed(const DateTime &dateTime)
    {
        const Date &date = dateTime.getDate();
        const Time &time = dateTime.getTime();

        u32 d = Date().daysTo(date) - (date.year() > 2000 ? 366 : 0) + 1;

        u32 h = time.hour();
        u32 m = time.minute();

        u32 seed = 1440 * d + 960 * (h / 10) + 60 * (h % 10) + 16 * (m / 10) + m % 10;
        return (seed >> 16) ^ (seed & 0xFFFF);
    }
}

namespace Utilities4
{
    u32 calcSeed(const DateTime &dateTime, u32 delay)
    {
        const Date &date = dateTime.getDate();
        const Time &time = dateTime.getTime();

        auto parts = date.getParts();

        u8 ab = static_cast<u8>(parts.month * parts.day + time.minute() + time.second());
        u8 cd = static_cast<u8>(time.hour());

        return static_cast<u32>(((ab << 24) | (cd << 16))) + delay + parts.year - 2000;
    }

    std::string coinFlips(u32 seed)
    {
        std::string coins;

        MT mt(seed);
        for (u8 i = 0; i < 20; i++)
        {
            coins += (mt.next() & 1) == 0 ? "T" : "H";
            if (i != 19)
            {
                coins += ", ";
            }
        }

        return coins;
    }

    std::string getCalls(u32 seed, u8 skips)
    {
        std::string calls;

        if (skips > 0)
        {
            calls += "(";
        }

        PokeRNG rng(seed);
        for (u8 i = 0; i < 20 + skips; i++)
        {
            u8 call = rng.nextUShort(3);

            calls += call == 0 ? "E" : call == 1 ? "K" : "P";

            if (i != (20 + skips - 1))
            {
                if (skips != 0 && skips == i + 1)
                {
                    calls += " skipped)  ";
                }
                else
                {
                    calls += ", ";
                }
            }
        }
        return calls;
    }

    std::string getChatot(u8 prng)
    {
        return getPitch(prng);
    }
}

/*namespace Utilities5
{
    std::string getChatot(u32 seed)
    {
        return getPitch(seed / 82);
    }

    u32 initialAdvancesBW(u64 seed)
    {
        BWRNG rng(seed);
        u32 count = 0;

        for (u8 i = 0; i < 5; i++)
        {
            count += advanceProbabilityTable(rng);
        }

        return count;
    }

    u32 initialAdvancesBW2(u64 seed, bool memory)
    {
        BWRNG rng(seed);
        u32 count = 0;

        for (u8 i = 0; i < 5; i++)
        {
            count += advanceProbabilityTable(rng);

            if (i == 0)
            {
                count += memory ? 2 : 3;
                rng.advance(memory ? 2 : 3);
            }
        }

        for (u8 limit = 0; limit < 100; limit++)
        {
            count += 3;
            u8 rand1 = rng.nextUInt(15);
            u8 rand2 = rng.nextUInt(15);
            u8 rand3 = rng.nextUInt(15);

            // This check is to see if any of the 3 rand calls are duplicates
            // If they aren't then break the loop early
            if (rand1 != rand2 && rand1 != rand3 && rand2 != rand3)
            {
                break;
            }
        }

        return count;
    }

    u32 initialAdvancesBWID(u64 seed)
    {
        BWRNG rng(seed);

        // 2 advances are done after the probability table
        u32 count = 2;

        for (u8 i = 0; i < 3; i++)
        {
            count += advanceProbabilityTable(rng);
        }

        return count;
    }

    u32 initialAdvancesBW2ID(u64 seed)
    {
        BWRNG rng(seed);

        // 10 advances are done by default
        // 2 are after the first probability table
        // 3 are after the second probability table
        // 1 is done when the main menu is loaded
        // 2 are done after the third probability table
        // 2 are done right before Professor Juniper appears
        u32 count = 10;

        for (u8 i = 0; i < 3; i++)
        {
            count += advanceProbabilityTable(rng);

            if (i == 0)
            {
                rng.advance(2);
            }
            else if (i == 1)
            {
                rng.advance(4);
            }
        }

        return count;
    }

    u32 forceGender(u32 pid, u64 rand, u8 gender, u8 genderRatio)
    {
        pid &= 0xffffff00;

        if (genderRatio == 0) // Male only
        {
            u8 val = ((rand * 0xF6) >> 32) + 8;
            pid |= val;
        }
        else if (genderRatio == 254) // Female only
        {
            u8 val = ((rand * 0x8) >> 32) + 1;
            pid |= val;
        }
        else // Gender ratio
        {
            if (gender == 0) // Male
            {
                u8 val = ((rand * (0xFE - genderRatio)) >> 32) + genderRatio;
                pid |= val;
            }
            else if (gender == 1) // Female
            {
                u8 val = ((rand * (genderRatio - 1)) >> 32) + 1;
                pid |= val;
            }
            else
            {
                u8 val = rand >> 32;
                pid |= val;
            }
        }

        return pid;
    }
}*/
