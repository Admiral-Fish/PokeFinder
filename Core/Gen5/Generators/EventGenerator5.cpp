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

#include "EventGenerator5.hpp"
#include <Core/Enum/Game.hpp>
#include <Core/RNG/LCRNG64.hpp>
#include <Core/Util/Utilities.hpp>

EventGenerator5::EventGenerator5(u32 initialFrame, u32 maxResults, u16 tid, u16 sid, u8 genderRatio, Method method,
                                 const FrameFilter &filter) :
    Generator(initialFrame, maxResults, tid, sid, genderRatio, method, filter)
{
    tsv = (tid ^ sid) >> 3;
}

QVector<Frame> EventGenerator5::generate(u64 seed, const PGF &parameters) const
{
    QVector<Frame> frames;

    BWRNG rng(seed);
    rng.advanceFrames(initialFrame + offset - 1);

    u16 cardXOR = parameters.getTID() ^ parameters.getSID();
    u16 actualXOR = parameters.isEgg() ? (tid ^ sid) : cardXOR;

    u8 advances = parameters.getAdvances();

    for (u32 cnt = 0; cnt < maxResults; cnt++, rng.nextULong())
    {
        // TODO: set seed for chatot pitch
        Frame frame(initialFrame + cnt);

        BWRNG go(rng.getSeed());
        go.advanceFrames(advances); // Advances from loading wondercard

        // IVs
        for (u8 i = 0; i < 6; i++)
        {
            u8 parameterIV = parameters.getIV(i);
            if (parameterIV == 255)
            {
                frame.setIVs(i, go.nextUInt() >> 27);
            }
            else
            {
                frame.setIVs(i, parameterIV);
            }
        }
        frame.calculateHiddenPower();

        // 2 blanks
        go.advanceFrames(2);

        u32 pid = go.nextUInt();

        // Gender locked handling
        if (parameters.getGender() == 0 || parameters.getGender() == 1)
        {
            u64 rand = go.nextUInt();
            pid = Utilities::forceGender(pid, rand, parameters.getGender(), genderRatio);
            frame.setGender(parameters.getGender());
        }
        else
        {
            frame.setGender(pid & 0xff, genderRatio);
        }

        if (parameters.getPIDType() == 0) // No shiny
        {
            if (((pid >> 16) ^ (pid & 0xffff) ^ actualXOR) < 8)
            {
                pid ^= 0x10000000;
            }
        }
        else if (parameters.getPIDType() == 2) // Force shiny
        {
            u32 low = pid & 0xff;
            pid = ((low ^ actualXOR) << 16) | low;
        }

        // Handle ability
        if (parameters.getAbilityType() < 3)
        {
            if (parameters.getAbilityType() == 1)
            {
                pid |= 0x10000U;
            }
            else
            {
                pid &= ~0x10000U;
            }

            frame.setAbility(parameters.getAbilityType());
        }
        else if (parameters.getAbilityType() == 3) // Ability flip
        {
            pid ^= 0x10000;
            frame.setAbility((pid >> 16) & 1);
        }

        frame.setPID(pid);
        frame.setShiny(tsv, ((pid >> 16) ^ (pid & 0xffff)) >> 3, 8);

        if (parameters.getNature() != 0xff)
        {
            frame.setNature(parameters.getNature());
        }
        else
        {
            // Unused frame
            go.advanceFrames(1);

            frame.setNature(go.nextUInt(25));
        }

        if (filter.compareFrame(frame))
        {
            frames.append(frame);
        }
    }

    return frames;
}
