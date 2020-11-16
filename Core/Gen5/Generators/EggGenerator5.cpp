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

#include "EggGenerator5.hpp"
#include <Core/Enum/Game.hpp>
#include <Core/Enum/Method.hpp>
#include <Core/RNG/LCRNG64.hpp>
#include <Core/RNG/MT.hpp>
#include <algorithm>

inline bool isShiny(u32 pid, u16 tsv)
{
    return ((pid >> 16) ^ (pid & 0xffff) ^ tsv) < 8;
}

EggGenerator5::EggGenerator5(u32 initialAdvances, u32 maxAdvances, u16 tid, u16 sid, u8 genderRatio, Method method,
                             const StateFilter &filter, const Daycare &daycare, bool shinyCharm) :
    EggGenerator(initialAdvances, maxAdvances, tid, sid, genderRatio, method, filter, daycare),
    rolls((shinyCharm ? 2 : 0) + (daycare.getMasuada() ? 5 : 0)),
    everstone(daycare.getEverstoneCount(Game::BW)),
    poweritem(daycare.getPowerItemCount()),
    ditto(daycare.getDitto()),
    parentAbility(daycare.getParentAbility(1))
{
}

std::vector<EggState> EggGenerator5::generate(u64 seed) const
{
    switch (method)
    {
    case Method::BWBred:
        return generateBW(seed);
    case Method::BW2Bred:
        return generateBW2(seed);
    default:
        return std::vector<EggState>();
    }
}

std::vector<EggState> EggGenerator5::generateBW(u64 seed) const
{
    std::vector<EggState> states;

    MTFast<13, true> mt(seed >> 32, 7);

    u8 ivs[6];
    std::generate(std::begin(ivs), std::end(ivs), [&mt] { return mt.nextUShort() >> 11; });

    BWRNG rng(seed);
    rng.advance(initialAdvances + offset);

    for (u32 cnt = 0; cnt <= maxAdvances; cnt++, rng.next())
    {
        EggState state(cnt + initialAdvances);

        BWRNG go(rng.getSeed());
        state.setSeed(go.nextUInt(0x1fff)); // Chatot pitch

        go.advance(1);

        // False: Nidoran-F / Volbeat
        // True: Nidoran-M / Illumise
        if (daycare.getNidoranVolbeat())
        {
            // TODO
            go.nextUInt(2);
        }

        u8 nature = go.nextUInt(25);
        // Everstone
        if (everstone != 0)
        {
            if ((go.nextUInt(2)) == 1)
            {
                // 0->parent1 / 1->parent2
                if (everstone == 2)
                {
                    nature = daycare.getParentNature(go.nextUInt(2));
                }
                else
                {
                    u8 parent = daycare.getParentItem(0) == 1 ? 0 : 1;
                    nature = daycare.getParentNature(parent);
                }
            }
        }
        state.setNature(nature);

        // Add check for mother having HA
        bool hiddenAbility = go.nextUInt(100) >= 40 && parentAbility == 2;

        // Ability inheritance doesn't get used outside of hidden ability
        /*u8 ability;
        u8 abilityRand = go.nextUInt(100);
        if (parentAbility == 0)
        {
            ability = abilityRand < 80 ? 0 : 1;
        }
        else if (parentAbility == 1)
        {
            ability = abilityRand < 20 ? 0 : 1;
        }
        else
        {
            ability = abilityRand < 20 ? 0 : abilityRand < 40 ? 1 : 2;
        }*/

        // Reroll ability to remove HA
        if (ditto)
        {
            // ability = go.nextUInt(2);
            go.advance(1);
            hiddenAbility = false;
        }

        // Power Items
        u8 inheritance = 0;
        if (poweritem != 0)
        {
            inheritance = 1;

            // If both parents holding power item
            // go.nextUInt(2) to determine which parent
            if (poweritem == 2)
            {
                u8 parent = go.nextUInt(2);
                u8 item = daycare.getParentItem(parent);

                state.setIVs(item - 2, daycare.getParentIV(parent, item - 2));
                state.setInheritance(item - 2, parent + 1);
            }
            else
            {
                u8 parent = (daycare.getParentItem(0) >= 2 && daycare.getParentItem(1) <= 7) ? 0 : 1;
                u8 item = daycare.getParentItem(parent);

                state.setIVs(item - 2, daycare.getParentIV(parent, item - 2));
                state.setInheritance(item - 2, parent + 1);
            }
        }

        // IV Inheritance
        for (; inheritance < 3;)
        {
            u8 index = go.nextUInt(6);
            u8 parent = go.nextUInt(2);

            // Assign stat inheritance
            if (state.getInheritance(index) == 0)
            {
                inheritance++;
                state.setIVs(index, daycare.getParentIV(parent, index));
                state.setInheritance(index, parent + 1);
            }
        }

        for (u8 i = 0; i < 6; i++)
        {
            if (state.getInheritance(i) == 0)
            {
                state.setIVs(i, ivs[i]);
            }
        }
        state.calculateHiddenPower();

        u32 pid = go.nextUInt(0xffffffff);
        for (u8 i = 0; i < rolls && !isShiny(pid, tsv); i++)
        {
            pid = go.nextUInt(0xffffffff);
        }

        state.setPID(pid);
        state.setAbility(hiddenAbility ? 2 : ((pid >> 16) & 1));
        state.setGender(pid & 255, genderRatio);
        state.setShiny(tsv, (pid >> 16) ^ (pid & 0xffff), 8);

        if (filter.compareState(state))
        {
            states.emplace_back(state);
        }
    }

    return states;
}

std::vector<EggState> EggGenerator5::generateBW2(u64 seed) const
{
    std::vector<EggState> states;

    MTFast<4> mt(seed >> 32, 2);

    u64 eggSeed = static_cast<u64>(mt.next()) << 32;
    eggSeed |= mt.next();

    EggState state = generateBW2Egg(eggSeed);
    if (filter.compareIVs(state) && filter.compareAbility(state) && filter.compareNature(state))
    {
        BWRNG rng(seed);
        rng.advance(initialAdvances + offset);
        for (u32 cnt = 0; cnt <= maxAdvances; cnt++, rng.next())
        {
            BWRNG go(rng.getSeed());
            state.setSeed(go.nextUInt(0x1fff));

            go.advance(1);

            u32 pid = go.nextUInt();
            if (((pid >> 16) & 1) != state.getAbility())
            {
                pid ^= 0x10000;
            }

            for (u8 i = 0; i < rolls && !isShiny(pid, tsv); i++)
            {
                pid = go.nextUInt();
                if (((pid >> 16) & 1) != state.getAbility())
                {
                    pid ^= 0x10000;
                }
            }

            state.setPID(pid);
            state.setGender(pid & 255, genderRatio);
            state.setShiny(tsv, (pid >> 16) ^ (pid & 0xffff), 8);

            if (filter.compareShiny(state) && filter.compareGender(state))
            {
                state.setAdvances(initialAdvances + cnt);
                states.emplace_back(state);
            }
        }
    }

    return states;
}

EggState EggGenerator5::generateBW2Egg(u64 seed) const
{
    EggState state(0);
    BWRNG rng(seed);

    // False: Nidoran-F / Volbeat
    // True: Nidoran-M / Illumise
    if (daycare.getNidoranVolbeat())
    {
        // TODO
        rng.nextUInt(2);
    }

    u8 nature = rng.nextUInt(25);
    // Everstone
    if (everstone != 0)
    {
        // 0->parent1 / 1->parent2
        if (everstone == 2)
        {
            nature = daycare.getParentNature(rng.nextUInt(2));
        }
        else
        {
            u8 parent = daycare.getParentItem(0) == 1 ? 0 : 1;
            nature = daycare.getParentNature(parent);
        }
    }
    state.setNature(nature);

    u8 ability;
    if (!ditto)
    {
        u8 abilityRand = rng.nextUInt(100);
        if (parentAbility == 0)
        {
            ability = abilityRand < 80 ? 0 : 1;
        }
        else if (parentAbility == 1)
        {
            ability = abilityRand < 20 ? 0 : 1;
        }
        else
        {
            ability = abilityRand < 20 ? 0 : abilityRand < 40 ? 1 : 2;
        }
    }
    else
    {
        rng.advance(1);
        ability = rng.nextUInt(2);
    }
    state.setAbility(ability);

    // Power Items
    u8 inheritance = 0;
    if (poweritem != 0)
    {
        inheritance = 1;

        // If both parents holding power item
        // go.nextUInt(2) to determine which parent
        if (poweritem == 2)
        {
            u8 parent = rng.nextUInt(2);
            u8 item = daycare.getParentItem(parent);

            state.setIVs(item - 2, daycare.getParentIV(parent, item - 2));
            state.setInheritance(item - 2, parent + 1);
        }
        else
        {
            u8 parent = (daycare.getParentItem(0) >= 2 && daycare.getParentItem(1) <= 7) ? 0 : 1;
            u8 item = daycare.getParentItem(parent);

            state.setIVs(item - 2, daycare.getParentIV(parent, item - 2));
            state.setInheritance(item - 2, parent + 1);
        }
    }

    // IV Inheritance
    for (; inheritance < 3;)
    {
        u8 index = rng.nextUInt(6);
        u8 parent = rng.nextUInt(2);

        if (state.getInheritance(index) == 0)
        {
            state.setIVs(index, daycare.getParentIV(parent, index));
            state.setInheritance(index, parent + 1);
            inheritance++;
        }
    }

    for (u8 i = 0; i < 6; i++)
    {
        if (state.getInheritance(i) == 0)
        {
            state.setIVs(i, rng.nextUInt(32));
        }
    }
    state.calculateHiddenPower();

    return state;
}
