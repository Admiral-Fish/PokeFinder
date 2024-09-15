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
static std::vector<IVToPIDState> calcMethod12(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe, u8 nature, u16 tid)
{
    u32 seeds[6];
    int size = LCRNGReverse::recoverPokeRNGIV(hp, atk, def, spa, spd, spe, seeds, Method::Method1);

    std::vector<IVToPIDState> states;
    for (int i = 0; i < size; i++)
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

    for (int i = 0; i < size; i++)
    {
        PokeRNGR rng(seeds[i]);

        rng.advance(1);
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

    return states;
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
static std::vector<IVToPIDState> calcMethod4(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe, u8 nature, u16 tid)
{
    u32 seeds[6];
    int size = LCRNGReverse::recoverPokeRNGIV(hp, atk, def, spa, spd, spe, seeds, Method::Method4);

    std::vector<IVToPIDState> states;
    for (int i = 0; i < size; i++)
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
    return states;
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
static std::vector<IVToPIDState> calcMethodChannel(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe, u8 nature)
{
    u32 seeds[12];
    int size = LCRNGReverse::recoverChannelIV(hp, atk, def, spa, spd, spe, seeds);

    std::vector<IVToPIDState> states;
    for (int i = 0; i < size; i++)
    {
        XDRNGR rng(seeds[i]);
        rng.advance(3);

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
    return states;
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
static std::vector<IVToPIDState> calcMethodXDColo(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe, u8 nature, u16 tid)
{
    u32 seeds[6];
    int size = LCRNGReverse::recoverXDRNGIV(hp, atk, def, spa, spd, spe, seeds);

    std::vector<IVToPIDState> states;
    for (int i = 0; i < size; i++)
    {
        u32 seed = XDRNGR(seeds[i]).next();

        XDRNG rng(seeds[i]);
        rng.advance(2);

        u16 high = rng.nextUShort();
        u16 low = rng.nextUShort();
        u16 sid = (high ^ low ^ tid) & 0xfff8;

        u32 pid = (high << 16) | low;
        if ((pid % 25) == nature)
        {
            states.emplace_back(seed, pid, sid, Method::XDColo);
        }
    }
    return states;
}

namespace IVToPIDCalculator
{
    std::vector<IVToPIDState> calculatePIDs(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe, u8 nature, u16 tid)
    {
        std::vector<IVToPIDState> states;

        auto results1 = calcMethod12(hp, atk, def, spa, spd, spe, nature, tid);
        auto results2 = calcMethod4(hp, atk, def, spa, spd, spe, nature, tid);
        auto results3 = calcMethodXDColo(hp, atk, def, spa, spd, spe, nature, tid);
        auto results4 = calcMethodChannel(hp, atk, def, spa, spd, spe, nature);

        states.insert(states.end(), results1.begin(), results1.end());
        states.insert(states.end(), results2.begin(), results2.end());
        states.insert(states.end(), results3.begin(), results3.end());
        states.insert(states.end(), results4.begin(), results4.end());

        return states;
    }
}
