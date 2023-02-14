/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2023 by Admiral_Fish, bumba, and EzPzStreamz
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

#include "PIDToIVCalculator.hpp"
#include <Core/Enum/Method.hpp>
#include <Core/Gen3/States/PIDToIVState.hpp>
#include <Core/RNG/LCRNG.hpp>
#include <Core/RNG/LCRNGReverse.hpp>

/**
 * @brief Computes IVs from the \p pid for Method 1/2/4
 *
 * @param pid PID value
 *
 * @return Vector of computed IVs
 */
static std::vector<PIDToIVState> calcMethod124(u32 pid)
{
    std::vector<PIDToIVState> states;

    u32 seeds[3];
    int size = LCRNGReverse::recoverPokeRNGPID(pid, seeds);
    for (int i = 0; i < size; i++)
    {
        u32 seed = PokeRNGR(seeds[i]).next();

        PokeRNG forward(seeds[i]);
        forward.advance(1);

        u16 iv1 = forward.nextUShort(); // Method 1/4
        u16 iv2 = forward.nextUShort(); // Method 1/2
        u16 iv3 = forward.nextUShort(); // Method 2/4

        states.emplace_back(seed, iv1, iv2, Method::Method1);
        states.emplace_back(seed, iv2, iv3, Method::Method2);
        states.emplace_back(seed, iv1, iv3, Method::Method4);
    }

    return states;
}

/**
 * @brief Computes IVs from the \p pid for Channel
 *
 * @param pid PID value
 *
 * @return Vector of computed IVs
 */
static std::vector<PIDToIVState> calcMethodChannel(u32 pid)
{
    std::vector<PIDToIVState> states;

    // Whether PID is xored or unxored is determined by SID which we don't know by only providing a PID
    // So we have to check both xored and unxored and recalculate the PID to see if we have a match

    u32 seeds[2];
    int size = LCRNGReverse::recoverXDRNGPID(pid, seeds);
    for (int i = 0; i < size; i++)
    {
        XDRNGR backward(seeds[i]);
        u16 sid = backward.nextUShort();
        u32 seed = backward.next();

        XDRNG forward(seed);
        forward.advance(1);

        u16 high = forward.nextUShort();
        u16 low = forward.nextUShort();

        if ((low > 7 ? 0 : 1) != (high ^ 40122 ^ sid))
        {
            high ^= 0x8000;
        }

        u32 val = static_cast<u32>((high << 16) | low);
        if (val == pid) // PID matches based on SID
        {
            forward.advance(3);

            u8 hp = forward.next() >> 27;
            u8 atk = forward.next() >> 27;
            u8 def = forward.next() >> 27;
            u8 spe = forward.next() >> 27;
            u8 spa = forward.next() >> 27;
            u8 spd = forward.next() >> 27;

            states.emplace_back(seed, hp, atk, def, spa, spd, spe, Method::Channel);
        }
    }

    size = LCRNGReverse::recoverXDRNGPID(pid ^ 0x80000000, seeds);
    for (int i = 0; i < size; i++)
    {
        XDRNGR backward(seeds[i]);
        u16 sid = backward.nextUShort();
        u32 seed = backward.next();

        XDRNG forward(seed);
        forward.advance(1);

        u16 high = forward.nextUShort();
        u16 low = forward.nextUShort();

        if ((low > 7 ? 0 : 1) != (high ^ 40122 ^ sid))
        {
            high ^= 0x8000;
        }

        u32 val = static_cast<u32>((high << 16) | low);
        if (val == pid) // PID matches based on SID
        {
            forward.advance(3);

            u8 hp = forward.next() >> 27;
            u8 atk = forward.next() >> 27;
            u8 def = forward.next() >> 27;
            u8 spe = forward.next() >> 27;
            u8 spa = forward.next() >> 27;
            u8 spd = forward.next() >> 27;

            states.emplace_back(seed, hp, atk, def, spa, spd, spe, Method::Channel);
        }
    }

    return states;
}

/**
 * @brief Computes IVs from the \p pid for XD/Colo
 *
 * @param pid PID value
 *
 * @return Vector of computed IVs
 */
static std::vector<PIDToIVState> calcMethodXDColo(u32 pid)
{
    std::vector<PIDToIVState> states;

    u32 seeds[2];
    int size = LCRNGReverse::recoverXDRNGPID(pid, seeds);
    for (int i = 0; i < size; i++)
    {
        XDRNGR backward(seeds[i]);
        backward.advance(1);

        u16 iv2 = backward.nextUShort();
        u16 iv1 = backward.nextUShort();
        u32 seed = backward.next();

        states.emplace_back(seed, iv1, iv2, Method::XDColo);
    }

    return states;
}

namespace PIDToIVCalculator
{
    std::vector<PIDToIVState> calculateIVs(u32 pid)
    {
        std::vector<PIDToIVState> states;

        auto results1 = calcMethod124(pid);
        auto results2 = calcMethodXDColo(pid);
        auto results3 = calcMethodChannel(pid);

        states.insert(states.end(), results1.begin(), results1.end());
        states.insert(states.end(), results2.begin(), results2.end());
        states.insert(states.end(), results3.begin(), results3.end());

        return states;
    }
}
