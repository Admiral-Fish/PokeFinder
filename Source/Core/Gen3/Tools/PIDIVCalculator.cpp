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

#include "PIDIVCalculator.hpp"
#include <Core/Enum/Method.hpp>
#include <Core/RNG/LCRNG.hpp>
#include <Core/RNG/RNGCache.hpp>
#include <Core/RNG/RNGEuclidean.hpp>

namespace PIDIVCalculator
{
    namespace
    {
        void setIVs(PIDIVState &state, u16 iv1, u16 iv2)
        {
            state.setIV(0, iv1 & 0x1f);
            state.setIV(1, (iv1 >> 5) & 0x1f);
            state.setIV(2, (iv1 >> 10) & 0x1f);
            state.setIV(3, (iv2 >> 5) & 0x1f);
            state.setIV(4, (iv2 >> 10) & 0x1f);
            state.setIV(5, iv2 & 0x1f);
        }

        std::vector<PIDIVState> calcMethod124(u32 pid)
        {
            std::vector<PIDIVState> states;

            RNGCache cache(Method::Method1);

            auto seeds = cache.recoverLower16BitsPID(pid);
            for (const u32 seed : seeds)
            {
                PokeRNGR backward(seed);
                u32 originSeed = backward.next();

                PokeRNG forward(seed);
                forward.advance(1);

                u16 iv1 = forward.nextUShort(); // Method 1/4
                u16 iv2 = forward.nextUShort(); // Method 1/2
                u16 iv3 = forward.nextUShort(); // Method 2/4

                PIDIVState state1(originSeed, Method::Method1);
                PIDIVState state2(originSeed, Method::Method2);
                PIDIVState state3(originSeed, Method::Method4);

                setIVs(state1, iv1, iv2);
                setIVs(state2, iv2, iv3);
                setIVs(state3, iv1, iv3);

                states.emplace_back(state1);
                states.emplace_back(state2);
                states.emplace_back(state3);
            }

            return states;
        }

        std::vector<PIDIVState> calcMethodXDColo(u32 pid)
        {
            std::vector<PIDIVState> states;

            auto seeds = RNGEuclidean::recoverLower16BitsPID(pid);
            for (const auto &pair : seeds)
            {
                XDRNGR backward(pair.first);
                backward.advance(1);

                u16 iv2 = backward.nextUShort();
                u16 iv1 = backward.nextUShort();
                u32 seed = backward.next();

                PIDIVState state(seed, Method::XDColo);

                setIVs(state, iv1, iv2);

                states.emplace_back(state);
            }

            return states;
        }

        std::vector<PIDIVState> calcMethodChannel(u32 pid)
        {
            std::vector<PIDIVState> states;

            // Whether PID is xored or unxored is determined by SID which we don't know by only providing a PID
            // So we have to check both xored and unxored and recalculate the PID to see if we have a match

            auto seeds = RNGEuclidean::recoverLower16BitsPID(pid);
            for (const auto &pair : seeds)
            {
                XDRNGR backward(pair.first);
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

                    PIDIVState state(seed, Method::Channel);

                    for (u8 i : { 0, 1, 2, 5, 3, 4 })
                    {
                        state.setIV(i, forward.next() >> 27);
                    }

                    states.emplace_back(state);
                }
            }

            auto seedsXOR = RNGEuclidean::recoverLower16BitsPID(pid ^ 0x80000000);
            for (const auto &pair : seedsXOR)
            {
                XDRNGR backward(pair.first);
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

                    PIDIVState state(seed, Method::Channel);

                    for (u8 i : { 0, 1, 2, 5, 3, 4 })
                    {
                        state.setIV(i, forward.next() >> 27);
                    }

                    states.emplace_back(state);
                }
            }

            return states;
        }
    }

    std::vector<PIDIVState> calculateIVs(u32 pid)
    {
        std::vector<PIDIVState> states;

        auto results1 = calcMethod124(pid);
        auto results2 = calcMethodXDColo(pid);
        auto results3 = calcMethodChannel(pid);

        states.insert(states.end(), results1.begin(), results1.end());
        states.insert(states.end(), results2.begin(), results2.end());
        states.insert(states.end(), results3.begin(), results3.end());

        return states;
    }
}
