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

#include "DreamRadarGenerator.hpp"
#include <Core/RNG/LCRNG64.hpp>
#include <Core/RNG/MTRNG.hpp>
#include <Core/RNG/RNGList.hpp>
#include <Core/Util/Utilities.hpp>

DreamRadarGenerator::DreamRadarGenerator(u32 initialFrame, u32 maxResults, u16 tid, u16 sid, u8 genderRatio, Method method,
                                         const FrameFilter &filter, const QVector<DreamRadarSlot> &radarSlots) :
    Generator(initialFrame, maxResults, tid, sid, genderRatio, method, filter),
    pidAdvances(0),
    ivAdvances(0),
    radarSlot(radarSlots.last())
{
    tsv = (tid ^ sid) >> 3;

    for (u8 i = 0; i < radarSlots.size(); i++)
    {
        auto slot = radarSlots.at(i);
        if (slot.type == 0)
        {
            pidAdvances += 5;
            ivAdvances += 13;
        }

        if (i != (radarSlots.size() - 1))
        {
            pidAdvances += (slot.genderRatio == 255) ? 4 : 5;
            ivAdvances += 13;
        }
    }
}

QVector<Frame> DreamRadarGenerator::generate(u64 seed, bool memory)
{
    QVector<Frame> frames;

    BWRNG rng(seed);
    rng.advanceFrames(Utilities::initialFrameBW2(seed, memory) - 1);
    rng.advanceFrames(1); // Opening menu advances 1 frame
    rng.advanceFrames((initialFrame - 1) * 2);
    if (!memory)
    {
        rng.nextULong();
    }

    MersenneTwister mt(seed >> 32);
    mt.advanceFrames(9); // Initial advances
    mt.advanceFrames((initialFrame - 1) * 2); // Starting frame
    mt.advanceFrames(ivAdvances); // Slot advances

    RNGList<u8, 100> rngList([&mt = mt] { return mt.nextUInt() >> 27; });

    for (u32 cnt = 0; cnt < maxResults; cnt++, rngList.advanceStates(2), rng.advanceFrames(2))
    {
        Frame frame(cnt + initialFrame);
        frame.setSeed(rng.getSeed(8));

        BWRNG go(rng.getSeed());
        go.advanceFrames(pidAdvances);

        for (u8 i = 0; i < 6; i++)
        {
            frame.setIVs(i, rngList.getValue());
        }
        frame.calculateHiddenPower();

        go.nextULong(); // Frame skip ???
        u32 pid = go.nextUInt();

        // Gender modification
        if (radarSlot.type == 0 || radarSlot.type == 1) // Genies already male, gen 4 legends also get assigned male pids
        {
            pid = Utilities::forceGender(pid, go.nextULong() >> 32, 0, 0);
            frame.setGender(radarSlot.gender);
        }
        else if (radarSlot.gender == 0 || radarSlot.gender == 1)
        {
            pid = Utilities::forceGender(pid, go.nextULong() >> 32, radarSlot.gender, radarSlot.genderRatio);
            frame.setGender(pid & 0xff, radarSlot.genderRatio);
        }
        else
        {
            frame.setGender(2);
        }

        // Flip ability
        pid ^= 0x10000;

        // Force non-shiny
        if ((((pid >> 16) ^ (pid & 0xffff)) >> 3) == tsv)
        {
            pid ^= 0x10000000;
        }

        frame.setPID(pid);
        frame.setAbility(2);
        frame.setShiny(0);

        go.advanceFrames(2);

        frame.setNature(go.nextUInt(25));

        if (filter.compareFrame(frame))
        {
            frames.append(frame);
        }
    }

    return frames;
}
