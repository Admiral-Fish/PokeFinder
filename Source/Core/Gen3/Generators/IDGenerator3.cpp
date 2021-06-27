/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2021 by Admiral_Fish, bumba, and EzPzStreamz
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

#include "IDGenerator3.hpp"
#include <Core/Parents/Filters/IDFilter.hpp>
#include <Core/RNG/LCRNG.hpp>
#include <Core/Enum/Method.hpp>
#include <Core/RNG/RNGCache.hpp>
#include <Core/RNG/RNGEuclidean.hpp>
#include <Core/Util/DateTime.hpp>

IDGenerator3::IDGenerator3(u32 initialAdvances, u32 maxAdvances, const IDFilter &filter) : IDGenerator(initialAdvances, maxAdvances, filter)
{
}

std::vector<IDState3> IDGenerator3::generateXDColo(u32 seed)
{
    std::vector<IDState3> states;

    XDRNG rng(seed);
    rng.advance(initialAdvances);

    for (u32 cnt = 0; cnt <= maxAdvances; cnt++, rng.next())
    {
        XDRNG go(rng.getSeed());

        u16 tid = go.nextUShort();
        u16 sid = go.nextUShort();

        IDState3 state(initialAdvances + cnt, tid, sid);

        if (filter.compare(state))
        {
            states.emplace_back(state);
        }
    }

    return states;
}

std::vector<IDState3> IDGenerator3::generateFRLGE(u16 tid)
{
    std::vector<IDState3> states;

    PokeRNG rng(tid);
    rng.advance(initialAdvances);

    for (u32 cnt = 0; cnt <= maxAdvances; cnt++)
    {
        u16 sid = rng.nextUShort();

        IDState3 state(initialAdvances + cnt, tid, sid);
        if (filter.compare(state))
        {
            states.emplace_back(state);
        }
    }

    return states;
}

std::vector<IDState3> IDGenerator3::generateRS(u32 seed)
{
    std::vector<IDState3> states;

    PokeRNG rng(seed);
    rng.advance(initialAdvances);

    for (u32 cnt = 0; cnt <= maxAdvances; cnt++, rng.next())
    {
        PokeRNG go(rng.getSeed());

        u16 sid = go.nextUShort();
        u16 tid = go.nextUShort();

        IDState3 state(initialAdvances + cnt, tid, sid);

        if (filter.compare(state))
        {
            states.emplace_back(state);
        }
    }

    return states;
}

std::vector<LiveIDState3> IDGenerator3::generateRSLive(u32 pid, u16 tid)
{
    std::vector<LiveIDState3> states;
    u16 low = pid & 0xFFFF;
    u16 high = pid >> 16;
    u16 pxor = low ^ high;
    u16 psv = pxor / 8;
    u16 square = pxor ^ tid;
    u16 rangeLow = square / 8;
    rangeLow *= 8;
    RNGCache cache(Method::Method1);

    for (u16 sid = rangeLow; sid <= rangeLow+7; sid++)
    {
        u32 tsid = (tid << 16) | sid;
        auto seeds = cache.recoverLower16BitsPID(tsid);
        for (const u32 seed : seeds)
        {
            PokeRNGR backward(seed);
            u32 originSeed = backward.next();
            PokeRNGR rng(originSeed);
            u32 advances = 0;
            while (rng.getSeed() > 0xFFFF)
            {
                rng.next();
                advances++;
            }
            u16 initial = rng.getSeed();
            DateTime d;
            constexpr Date start;
            Date calc(2000, 1, 1);
            Date end(2000, 12, 31);

            for (; calc <= end; calc = calc.addDays(1))
            {
                // The if statement considering the year is to handle a bug the game has
                // If the year is greater then 2000 it does not count the days in that year
                u16 days = start.daysTo(calc) + 1;

                for (u8 hour = 0; hour < 24; hour++)
                {
                    for (u8 minute = 0; minute < 60; minute++)
                    {
                        u32 v = 1440 * days + 960 * (hour / 10) + 60 * (hour % 10) + 16 * (minute / 10) + (minute % 10);
                        v = (v >> 16) ^ (v & 0xffff);
                        if (v == initial)
                        {
                            d = DateTime(calc, Time(hour, minute, 0));
                            minute = 60;
                            hour = 24;
                            calc = end.addDays(1);
                            break;
                        }
                    }
                }
            }
            std::string date = d.toString();

            std::string shiny = sid == square ? "Square" : "Star";

            LiveIDState3 state(advances, tid, sid, initial, shiny, date);
            states.emplace_back(state);
        }
    }

    return states;
}

std::vector<LiveXDColoIDState3> IDGenerator3::generateXDColoLive(u32 pid, u16 tid)
{
    std::vector<LiveXDColoIDState3> states;
    u16 low = pid & 0xFFFF;
    u16 high = pid >> 16;
    u16 pxor = low ^ high;
    u16 psv = pxor / 8;
    u16 square = pxor ^ tid;
    u16 rangeLow = square / 8;
    rangeLow *= 8;

    for (u16 sid = rangeLow; sid <= rangeLow+7; sid++)
    {
        u32 tsid = (tid << 16) | sid;
        auto seeds = RNGEuclidean::recoverLower16BitsPID(tsid);
        for (const auto &pair : seeds)
        {
            XDRNGR backward(pair.first);
            u32 originSeed = backward.next();
            std::string shiny = sid == square ? "Square" : "Star";

            LiveXDColoIDState3 state(tid, sid, originSeed, shiny);
            states.emplace_back(state);
        }
    }

    return states;
}
