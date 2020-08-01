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

#include "Utilities.hpp"
#include <Core/Gen4/HGSSRoamer.hpp>
#include <Core/RNG/LCRNG.hpp>
#include <Core/RNG/LCRNG64.hpp>
#include <Core/RNG/MT.hpp>

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
}

namespace Utilities
{
    u16 calcGen3Seed(const QDateTime &dateTime)
    {
        const QDate &date = dateTime.date();
        const QTime &time = dateTime.time();

        u32 d = static_cast<u32>(QDate(date.year() == 2000 ? 1999 : 2000, 12, 31).daysTo(date));
        u32 h = static_cast<u32>(time.hour());
        u32 m = static_cast<u32>(time.minute());

        u32 seed = 1440 * d + 960 * (h / 10) + 60 * (h % 10) + 16 * (m / 10) + m % 10;
        return (seed >> 16) ^ (seed & 0xFFFF);
    }

    u32 calcGen4Seed(const QDateTime &dateTime, u32 delay)
    {
        const QDate &date = dateTime.date();
        const QTime &time = dateTime.time();

        u8 ab = static_cast<u8>(date.month() * date.day() + time.minute() + time.second());
        u8 cd = static_cast<u8>(time.hour());

        return static_cast<u32>(((ab << 24) | (cd << 16))) + delay;
    }

    QString coinFlips(u32 seed)
    {
        QStringList coins;

        MT<20> mt(seed);

        for (u8 i = 0; i < 20; i++)
        {
            coins.append((mt.next() & 1) == 0 ? "T" : "H");
        }

        return coins.join(", ");
    }

    QString getCalls(u32 seed, const HGSSRoamer &info)
    {
        QString calls = "";

        u8 skips = info.getSkips();
        if (skips > 0)
        {
            calls += "(";
        }

        PokeRNG rng(seed);

        for (u8 i = 0; i < 20 + skips; i++)
        {
            u8 call = rng.nextUShort() % 3;

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

    u32 initialAdvancesBW(u64 seed, u8 rounds)
    {
        BWRNG rng(seed);
        u32 count = 0;

        for (u8 i = 0; i < rounds; i++)
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

    u32 initialAdvancesBW2ID(u64 seed, u8 rounds)
    {
        BWRNG rng(seed);
        u32 count = 0;

        for (u8 i = 0; i < rounds; i++)
        {
            count += advanceProbabilityTable(rng);
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
}
