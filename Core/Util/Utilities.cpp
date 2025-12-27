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

#include "Utilities.hpp"
#include <Core/Enum/Game.hpp>
#include <Core/Enum/Shiny.hpp>
#include <Core/Gen5/Profile5.hpp>
#include <Core/RNG/LCRNG.hpp>
#include <Core/RNG/MT.hpp>
#include <Core/Util/DateTime.hpp>
#include <zstd.h>

static u32 advanceProbabilityTable(BWRNG &rng)
{
    u32 count = 0;

    // Round 1: 50, 100
    rng.advance(1, &count);

    // Round 2: 50, 50, 100
    if (rng.nextUInt(101, &count) > 50)
    {
        rng.advance(1, &count);
    }

    // Round 3: 30, 50, 100
    if (rng.nextUInt(101, &count) > 30)
    {
        rng.advance(1, &count);
    }

    // Round 4: 25, 30, 50, 100
    if (rng.nextUInt(101, &count) > 25)
    {
        if (rng.nextUInt(101, &count) > 30)
        {
            rng.advance(1, &count);
        }
    }

    // Round 5: 20, 25, 33, 50, 100
    if (rng.nextUInt(101, &count) > 20)
    {
        if (rng.nextUInt(101, &count) > 25)
        {
            if (rng.nextUInt(101, &count) > 33)
            {
                rng.advance(1, &count);
            }
        }
    }

    return count;
}

static std::string getPitch(u8 result)
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

namespace Utilities
{
    void *decompress(const u8 *compressedData, u32 compressedSize, u32 &size)
    {
        size = ZSTD_getFrameContentSize(compressedData, compressedSize);
        u8 *data = new u8[size];

        ZSTD_decompress(data, size, compressedData, compressedSize);

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
        return (seed >> 16) ^ (seed & 0xffff);
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

namespace Utilities5
{
    u32 createPID(u16 tsv, u8 ability, u8 gender, Shiny shiny, bool wild, u8 ratio, BWRNG &rng)
    {
        u32 pid = rng.nextUInt();

        if (gender < 2)
        {
            u8 low = 8;
            if (ratio > 0 && ratio < 254)
            {
                if (gender == 0)
                {
                    low = ratio;
                }
                else if (ratio)
                {
                    low = ratio - 1;
                }
            }

            if (gender == 0)
            {
                low = rng.nextUInt(0xfe - low) + low;
            }
            else if (gender == 1)
            {
                low = rng.nextUInt(low) + 1;
            }

            pid = (pid & 0xffffff00) | low;
        }

        if (shiny == Shiny::Always)
        {
            u32 low = pid & 0xff;
            pid = ((low ^ tsv) << 16) | low;
        }
        else if (shiny == Shiny::Never && Utilities::isShiny<true>(pid, tsv))
        {
            pid ^= 0x10000000;
        }

        // 0: Force ability bit low
        // 1: Force ability bit high
        // 2: Force abliity bit low
        // 255: Flip ability bit
        if (ability == 2)
        {
            ability = 0;
        }

        if (((pid >> 16) & 1) != ability)
        {
            pid ^= 0x10000;
        }

        // This is the 0x80000000 XOR decision for wild encounters
        // This was supposed to boost the shiny rate but operator precedence led to this
        if (wild && shiny == Shiny::Random)
        {
            if ((tsv ^ pid) & 1)
            {
                pid |= 0x80000000;
            }
            else
            {
                pid &= 0x7fffffff;
            }
        }

        return pid;
    }

    std::string getChatot(u8 prng)
    {
        return getPitch(prng);
    }

    u32 initialAdvances(u64 seed, const Profile5 &profile)
    {
        if ((profile.getVersion() & Game::BW) != Game::None)
        {
            return initialAdvancesBW(seed);
        }
        else
        {
            return initialAdvancesBW2(seed, profile.getMemoryLink());
        }
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

    u32 initialAdvancesID(u64 seed, Game version)
    {
        if ((version & Game::BW) != Game::None)
        {
            return initialAdvancesBWID(seed);
        }
        else
        {
            return initialAdvancesBW2ID(seed);
        }
    }
}
