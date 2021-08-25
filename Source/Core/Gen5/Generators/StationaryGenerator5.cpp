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

#include "StationaryGenerator5.hpp"
#include <Core/Enum/Encounter.hpp>
#include <Core/Enum/Lead.hpp>
#include <Core/Enum/Method.hpp>
#include <Core/Parents/Filters/StateFilter.hpp>
#include <Core/RNG/LCRNG64.hpp>
#include <Core/RNG/MT.hpp>
#include <Core/RNG/RNGList.hpp>
#include <Core/Util/Utilities.hpp>

StationaryGenerator5::StationaryGenerator5(u32 initialAdvances, u32 maxAdvances, u16 tid, u16 sid, u8 gender, u8 genderRatio, Method method,
                                           Encounter encounter, const StateFilter &filter) :
    StationaryGenerator(initialAdvances, maxAdvances, tid, sid, genderRatio, method, filter),
    idBit((tid & 1) ^ (sid & 1)),
    encounter(encounter),
    gender(gender)
{
}

std::vector<StationaryState> StationaryGenerator5::generate(u64 seed) const
{
    switch (method)
    {
    case Method::Method5IVs:
        switch (encounter)
        {
        case Encounter::Roamer:
            return generateRoamerIVs(seed);
        case Encounter::Stationary:
            return generateIVs(seed);
        default:
            break;
        }
    case Method::Method5CGear:
        switch (encounter)
        {
        case Encounter::Roamer:
            return generateRoamerCGear(seed);
        case Encounter::Stationary:
            return generateCGear(seed);
        default:
            break;
        }
    case Method::Method5:
        switch (encounter)
        {
        case Encounter::Stationary:
            return generateStationary(seed);
        case Encounter::Roamer:
            return generateRoamer(seed);
        case Encounter::Gift:
            return generateGift(seed);
        case Encounter::EntraLink:
            return generateEntraLink(seed);
        case Encounter::GiftEgg:
            return generateLarvestaEgg(seed);
        case Encounter::HiddenGrotto:
            return generateHiddenGrotto(seed);
        default:
            break;
        }
    default:
        break;
    }
    return std::vector<StationaryState>();
}

std::vector<StationaryState> StationaryGenerator5::generateRoamerIVs(u64 seed) const
{
    std::vector<StationaryState> states;

    MT mt(seed >> 32);
    mt.advance(initialAdvances + offset);

    RNGList<u8, MT, 8, 27> rngList(mt);

    for (u32 cnt = 0; cnt <= maxAdvances; cnt++, rngList.advanceState())
    {
        StationaryState state(initialAdvances + cnt);

        rngList.advance(1); // Blank ???
        u8 hp = rngList.getValue();
        u8 atk = rngList.getValue();
        u8 def = rngList.getValue();
        u8 spd = rngList.getValue();
        u8 spe = rngList.getValue();
        u8 spa = rngList.getValue();

        state.setIVs(hp, atk, def, spa, spd, spe);
        state.calculateHiddenPower();

        if (filter.compareIVs(state))
        {
            states.emplace_back(state);
        }
    }

    return states;
}

std::vector<StationaryState> StationaryGenerator5::generateIVs(u64 seed) const
{
    std::vector<StationaryState> states;

    MT mt(seed >> 32);
    mt.advance(initialAdvances + offset);

    RNGList<u8, MT, 8, 27> rngList(mt);

    for (u32 cnt = 0; cnt <= maxAdvances; cnt++, rngList.advanceState())
    {
        StationaryState state(initialAdvances + cnt);

        u8 hp = rngList.getValue();
        u8 atk = rngList.getValue();
        u8 def = rngList.getValue();
        u8 spa = rngList.getValue();
        u8 spd = rngList.getValue();
        u8 spe = rngList.getValue();

        state.setIVs(hp, atk, def, spa, spd, spe);
        state.calculateHiddenPower();

        if (filter.compareIVs(state))
        {
            states.emplace_back(state);
        }
    }

    return states;
}

std::vector<StationaryState> StationaryGenerator5::generateRoamerCGear(u64 seed) const
{
    std::vector<StationaryState> states;

    MT mt(seed >> 32);
    mt.advance(initialAdvances + offset);
    mt.advance(2); // Skip first two advances

    RNGList<u8, MT, 8, 27> rngList(mt);

    for (u32 cnt = 0; cnt <= maxAdvances; cnt++, rngList.advanceState())
    {
        StationaryState state(initialAdvances + cnt);

        rngList.advance(1); // Blank ???
        u8 hp = rngList.getValue();
        u8 atk = rngList.getValue();
        u8 def = rngList.getValue();
        u8 spd = rngList.getValue();
        u8 spe = rngList.getValue();
        u8 spa = rngList.getValue();

        state.setIVs(hp, atk, def, spa, spd, spe);
        state.calculateHiddenPower();

        if (filter.compareIVs(state))
        {
            states.emplace_back(state);
        }
    }

    return states;
}

std::vector<StationaryState> StationaryGenerator5::generateCGear(u64 seed) const
{
    std::vector<StationaryState> states;

    MT mt(seed >> 32);
    mt.advance(initialAdvances + offset);
    mt.advance(2); // Skip first two advances

    RNGList<u8, MT, 8, 27> rngList(mt);

    for (u32 cnt = 0; cnt <= maxAdvances; cnt++, rngList.advanceState())
    {
        StationaryState state(initialAdvances + cnt);

        u8 hp = rngList.getValue();
        u8 atk = rngList.getValue();
        u8 def = rngList.getValue();
        u8 spa = rngList.getValue();
        u8 spd = rngList.getValue();
        u8 spe = rngList.getValue();

        state.setIVs(hp, atk, def, spa, spd, spe);
        state.calculateHiddenPower();

        if (filter.compareIVs(state))
        {
            states.emplace_back(state);
        }
    }

    return states;
}

std::vector<StationaryState> StationaryGenerator5::generateStationary(u64 seed) const
{
    std::vector<StationaryState> states;

    BWRNG rng(seed);
    rng.advance(initialAdvances + offset);

    for (u32 cnt = 0; cnt <= maxAdvances; cnt++, rng.next())
    {
        StationaryState state(initialAdvances + cnt);
        BWRNG go(rng.getSeed());

        u32 pid;
        if (lead == Lead::Synchronize)
        {
            bool synch = (go.nextUInt() >> 31) == 1;
            pid = go.nextUInt() ^ 0x10000;

            if (synch)
            {
                state.setNature(synchNature);
            }
            else
            {
                state.setNature(go.nextUInt(25));
            }
        }
        else if (lead >= Lead::CuteCharm && lead <= Lead::CuteCharmFemale)
        {
            bool charm = (go.nextUInt(0xffff) / 656) < 67;
            pid = go.nextUInt() ^ 0x10000;

            if (!charm)
            {
                state.setNature(go.nextUInt(25));
            }
            else
            {
                // TODO: fix hard coded gender
                pid = Utilities::forceGender(pid, go.next(), 0, genderRatio);
                state.setNature(go.nextUInt(25));
            }
        }
        // else if (lead == Lead::Search)
        //{
        // TODO: do we live to suffer?
        //}
        else // No lead
        {
            go.advance(1);
            pid = go.nextUInt() ^ 0x10000;
            state.setNature(go.nextUInt(25));
        }

        u8 val = idBit ^ (pid & 1) ^ (pid >> 31);
        if (val == 1)
        {
            pid ^= 0x80000000;
        }

        state.setPID(pid);
        state.setAbility((pid >> 16) & 1);
        state.setGender(pid & 255, genderRatio);
        state.setShiny<8>(tsv, (pid >> 16) ^ (pid & 0xffff));

        if (filter.comparePID(state))
        {
            states.emplace_back(state);
        }
    }

    return states;
}

std::vector<StationaryState> StationaryGenerator5::generateRoamer(u64 seed) const
{
    std::vector<StationaryState> states;

    BWRNG rng(seed);
    rng.advance(initialAdvances + offset);

    for (u32 cnt = 0; cnt <= maxAdvances; cnt++, rng.next())
    {
        StationaryState state(initialAdvances + cnt);
        BWRNG go(rng.getSeed());

        u32 pid = go.nextUInt();
        state.setNature(go.nextUInt(25));

        state.setPID(pid);
        state.setAbility((pid >> 16) & 1);
        state.setGender(pid & 255, genderRatio);
        state.setShiny<8>(tsv, (pid >> 16) ^ (pid & 0xffff));

        if (filter.comparePID(state))
        {
            states.emplace_back(state);
        }
    }

    return states;
}

std::vector<StationaryState> StationaryGenerator5::generateGift(u64 seed) const
{
    std::vector<StationaryState> states;

    BWRNG rng(seed);
    rng.advance(initialAdvances + offset);

    for (u32 cnt = 0; cnt <= maxAdvances; cnt++, rng.next())
    {
        StationaryState state(initialAdvances + cnt);
        BWRNG go(rng.getSeed());

        u32 pid = go.nextUInt() ^ 0x10000;
        state.setNature(go.nextUInt(25));

        state.setPID(pid);
        state.setAbility((pid >> 16) & 1);
        state.setGender(pid & 255, genderRatio);
        state.setShiny<8>(tsv, (pid >> 16) ^ (pid & 0xffff));

        if (filter.comparePID(state))
        {
            states.emplace_back(state);
        }
    }

    return states;
}

std::vector<StationaryState> StationaryGenerator5::generateEntraLink(u64 seed) const
{
    std::vector<StationaryState> states;

    BWRNG rng(seed);
    rng.advance(initialAdvances + offset);

    for (u32 cnt = 0; cnt <= maxAdvances; cnt++, rng.next())
    {
        StationaryState state(initialAdvances + cnt);
        BWRNG go(rng.getSeed());

        // TODO
    }

    return states;
}

std::vector<StationaryState> StationaryGenerator5::generateLarvestaEgg(u64 seed) const
{
    std::vector<StationaryState> states;

    BWRNG rng(seed);
    rng.advance(initialAdvances + offset);

    for (u32 cnt = 0; cnt <= maxAdvances; cnt++, rng.next())
    {
        StationaryState state(initialAdvances + cnt);
        BWRNG go(rng.getSeed());

        u32 pid = go.nextUInt();
        go.advance(1);
        state.setNature(go.nextUInt(25));

        state.setPID(pid);
        state.setAbility((pid >> 16) & 1);
        state.setGender(pid & 255, genderRatio);
        state.setShiny<8>(tsv, (pid >> 16) ^ (pid & 0xffff));

        if (filter.comparePID(state))
        {
            states.emplace_back(state);
        }
    }

    return states;
}

std::vector<StationaryState> StationaryGenerator5::generateHiddenGrotto(u64 seed) const
{
    std::vector<StationaryState> states;

    BWRNG rng(seed);
    rng.advance(initialAdvances + offset);

    for (u32 cnt = 0; cnt <= maxAdvances; cnt++, rng.next())
    {
        StationaryState state(initialAdvances + cnt);
        BWRNG go(rng.getSeed());
        state.setSeed(go.nextUInt(0x1FFF));

        bool leadAffects = false;
        if (lead == Lead::Synchronize)
        {
            leadAffects = (go.nextUInt() >> 31) == 1;
        }
        else if (lead >= Lead::CuteCharm && lead <= Lead::CuteCharmFemale)
        {
            // PIDRNG frame is 'skipped' if cute charm would not have affected the frame
            leadAffects = (go.nextUInt(0xffff) / 656) < 67;
            if (!leadAffects)
            {
                go.advance(1);
            }
        }
        else
        {
            go.advance(1);
        }
        u32 pid = go.nextUInt();

        if (genderRatio == 255)
        {
            state.setNature(go.nextUInt(25));
        }
        else
        {
            pid = Utilities::forceGender(pid, go.nextUInt(), gender, genderRatio);
            state.setNature(go.nextUInt(25));
        }

        if (lead == Lead::Synchronize && leadAffects)
        {
            state.setNature(synchNature);
        }

        pid = pid ^ 0x10000;
        state.setPID(pid);
        state.setAbility((pid >> 16) & 1);
        state.setGender(pid & 255, genderRatio);
        if (filter.comparePID(state))
        {
            states.emplace_back(state);
        }
    }

    return states;
}
