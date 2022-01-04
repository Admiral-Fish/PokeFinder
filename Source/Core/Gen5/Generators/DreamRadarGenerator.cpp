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

#include "DreamRadarGenerator.hpp"
#include <Core/RNG/LCRNG64.hpp>
#include <Core/RNG/MT.hpp>
#include <Core/RNG/RNGList.hpp>
#include <Core/Util/Utilities.hpp>

DreamRadarGenerator::DreamRadarGenerator(u32 initialAdvances, u32 maxAdvances, u16 tid, u16 sid, u8 genderRatio, Method method,
                                         const StateFilter &filter, const std::vector<DreamRadarSlot> &radarSlots) :
    Generator(initialAdvances, maxAdvances, tid, sid, genderRatio, method, filter),
    pidAdvances(0),
    ivAdvances(0),
    radarSlot(radarSlots.back())
{
    for (size_t i = 0; i < radarSlots.size(); i++)
    {
        auto slot = radarSlots[i];
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

std::vector<DreamRadarState> DreamRadarGenerator::generate(u64 seed, bool memory) const
{
    std::vector<DreamRadarState> states;

    BWRNG rng(seed);
    u32 initialAdvancesBW2 = Utilities::initialAdvancesBW2(seed, memory);
    rng.advance(initialAdvancesBW2 + (initialAdvances * 2));
    if (!memory)
    {
        rng.next();
    }

    MT mt(seed >> 32);
    mt.advance(9 + (initialAdvances * 2) + ivAdvances); // Initial advances, starting advance, slot advances

    RNGList<u8, MT, 8, 27> rngList(mt);

    for (u32 cnt = 0; cnt <= maxAdvances; cnt++, rngList.advanceStates(2), rng.next())
    {
        DreamRadarState state((cnt + initialAdvances) * 2 + initialAdvancesBW2 + 1);
        state.setKeyAdvances(cnt + initialAdvances);

        BWRNG go(rng.getSeed());
        go.advance(pidAdvances);

        for (u8 i = 0; i < 6; i++)
        {
            state.setIVs(i, rngList.getValue());
        }
        state.calculateHiddenPower();

        go.next(); // Advance skip ???
        u32 pid = go.nextUInt();

        // Gender modification
        if (radarSlot.type == 0 || radarSlot.type == 1) // Genies already male, gen 4 legends also get assigned male pids
        {
            pid = Utilities::forceGender(pid, go.next() >> 32, 0, 0);
            state.setGender(radarSlot.gender);
        }
        else if (radarSlot.gender == 0 || radarSlot.gender == 1)
        {
            pid = Utilities::forceGender(pid, go.next() >> 32, radarSlot.gender, radarSlot.genderRatio);
            state.setGender(pid & 0xff, radarSlot.genderRatio);
        }
        else
        {
            state.setGender(2);
        }

        // Flip ability
        pid ^= 0x10000;

        // Force non-shiny
        if (((pid >> 16) ^ (pid & 0xffff) ^ tsv) < 8)
        {
            pid ^= 0x10000000;
        }

        state.setPID(pid);
        state.setAbility(2);
        state.setShiny(0);

        go.advance(2);

        state.setNature(go.nextUInt(25));

        state.setSeed(rng.nextUInt(8)); // Needle calculation

        if (filter.compareState(state))
        {
            states.emplace_back(state);
        }
    }

    return states;
}
