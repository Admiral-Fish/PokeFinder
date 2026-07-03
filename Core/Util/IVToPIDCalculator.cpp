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

#include "IVToPIDCalculator.hpp"
#include <Core/Enum/Method.hpp>
#include <Core/Parents/States/IVToPIDState.hpp>
#include <Core/RNG/LCRNG.hpp>
#include <Core/RNG/LCRNGReverse.hpp>

constexpr u8 genderThreshHolds[5] = { 0, 0x32, 0x4b, 0x96, 0xc8 };

/**
 * @brief Computes PID from the IVs for Method 1/2
 *
 * @param hp HP IV
 * @param atk Atk IV
 * @param def Def IV
 * @param spa SpA IV
 * @param spd SpD IV
 * @param spe Spe IV
 * @param nature Nature value
 * @param tid Trainer ID
 *
 * @return Vector of computed PIDs
 */
static void calcMethod12(std::vector<IVToPIDState> &states, u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe, u8 nature, u16 tid)
{
    auto seeds = LCRNGReverse::recoverPokeRNGIV(hp, atk, def, spa, spd, spe, Method::Method1);

    for (int i = 0; i < seeds.count; i++)
    {
        PokeRNGR rng(seeds[i]);

        u16 high = rng.nextUShort();
        u16 low = rng.nextUShort();
        u16 sid = (high ^ low ^ tid) & 0xfff8;
        u32 seed = rng.next();

        u32 pid = (high << 16) | low;
        if ((pid % 25) == nature)
        {
            states.emplace_back(seed, pid, sid, Method::Method1);
        }

        pid = (low << 16) | high;
        if ((pid % 25) == nature)
        {
            states.emplace_back(seed, pid, sid, Method::Method1Reverse);
        }

        if ((low / 0x5556) != 0 && (high / 0xa3e) == nature)
        {
            for (u8 thresh : genderThreshHolds)
            {
                pid = nature + thresh;
                sid = (pid ^ tid) & 0xfff8;
                states.emplace_back(seed, pid, sid, Method::CuteCharmDPPt);
            }
        }

        if ((low % 3) != 0 && (high % 25) == nature)
        {
            for (u8 thresh : genderThreshHolds)
            {
                pid = nature + thresh;
                sid = (pid ^ tid) & 0xfff8;
                states.emplace_back(seed, pid, sid, Method::CuteCharmHGSS);
            }
        }
    }

    for (int i = 0; i < seeds.count; i++)
    {
        PokeRNGR rng(seeds[i], 1);

        u16 high = rng.nextUShort();
        u16 low = rng.nextUShort();
        u16 sid = (high ^ low ^ tid) & 0xfff8;
        u32 seed = rng.next();

        u32 pid = (high << 16) | low;
        if ((pid % 25) == nature)
        {
            states.emplace_back(seed, pid, sid, Method::Method2);
        }
    }
}

/**
 * @brief Computes PID from the IVs for Method 4
 *
 * @param hp HP IV
 * @param atk Atk IV
 * @param def Def IV
 * @param spa SpA IV
 * @param spd SpD IV
 * @param spe Spe IV
 * @param nature Nature value
 * @param tid Trainer ID
 *
 * @return Vector of computed PIDs
 */
static void calcMethod4(std::vector<IVToPIDState> &states, u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe, u8 nature, u16 tid)
{
    auto seeds = LCRNGReverse::recoverPokeRNGIV(hp, atk, def, spa, spd, spe, Method::Method4);

    for (int i = 0; i < seeds.count; i++)
    {
        PokeRNGR rng(seeds[i]);

        u16 high = rng.nextUShort();
        u16 low = rng.nextUShort();
        u16 sid = (high ^ low ^ tid) & 0xfff8;
        u32 seed = rng.next();

        u32 pid = (high << 16) | low;
        if ((pid % 25) == nature)
        {
            states.emplace_back(seed, pid, sid, Method::Method4);
        }
    }
}

/**
 * @brief Computes PID from the IVs for Channel
 *
 * @param hp HP IV
 * @param atk Atk IV
 * @param def Def IV
 * @param spa SpA IV
 * @param spd SpD IV
 * @param spe Spe IV
 * @param nature Nature value
 *
 * @return Vector of computed PIDs
 */
static void calcMethodChannel(std::vector<IVToPIDState> &states, u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe, u8 nature)
{
    auto seeds = LCRNGReverse::recoverChannelIV(hp, atk, def, spa, spd, spe);

    for (int i = 0; i < seeds.count; i++)
    {
        XDRNGR rng(seeds[i], 3);

        u16 low = rng.nextUShort();
        u16 high = rng.nextUShort();
        u16 sid = rng.nextUShort();
        u32 seed = rng.next();

        // Determine if PID needs to be XORed
        if ((low > 7 ? 0 : 1) != (high ^ sid ^ 40122))
        {
            high ^= 0x8000;
        }

        u32 pid = (high << 16) | low;
        if ((pid % 25) == nature)
        {
            states.emplace_back(seed, pid, sid, Method::Channel);
        }
    }
}

/**
 * @brief Computes PID from the IVs for XD/Colo
 *
 * @param hp HP IV
 * @param atk Atk IV
 * @param def Def IV
 * @param spa SpA IV
 * @param spd SpD IV
 * @param spe Spe IV
 * @param nature Nature value
 * @param tid Trainer ID
 *
 * @return Vector of computed PIDs
 */
static void calcMethodXDColo(std::vector<IVToPIDState> &states, u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe, u8 nature, u16 tid)
{
    auto seeds = LCRNGReverse::recoverXDRNGIV(hp, atk, def, spa, spd, spe);

    for (int i = 0; i < seeds.count; i++)
    {
        u32 seed = XDRNGR(seeds[i]).next();

        XDRNG rng(seeds[i], 2);

        u16 high = rng.nextUShort();
        u16 low = rng.nextUShort();
        u16 sid = (high ^ low ^ tid) & 0xfff8;

        u32 pid = (high << 16) | low;
        if ((pid % 25) == nature)
        {
            states.emplace_back(seed, pid, sid, Method::XDColo);
        }
    }
}

namespace IVToPIDCalculator
{
    std::vector<IVToPIDState> calculatePIDs(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe, u8 nature, u16 tid)
    {
        std::vector<IVToPIDState> states;

        calcMethod12(states, hp, atk, def, spa, spd, spe, nature, tid);
        calcMethod4(states, hp, atk, def, spa, spd, spe, nature, tid);
        calcMethodXDColo(states, hp, atk, def, spa, spd, spe, nature, tid);
        calcMethodChannel(states, hp, atk, def, spa, spd, spe, nature);

        return states;
    }
}
