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
#include <Core/RNG/MTRNG.hpp>

inline bool isShiny(u32 pid, u16 tsv)
{
    return ((pid >> 16) ^ (pid & 0xffff) ^ tsv) < 8;
}

EggGenerator5::EggGenerator5(u32 initialFrame, u32 maxResults, u16 tid, u16 sid, u8 genderRatio, Method method, const FrameFilter &filter,
                             const Daycare &daycare, bool shinyCharm) :
    EggGenerator(initialFrame, maxResults, tid, sid, genderRatio, method, filter),
    daycare(daycare),
    rolls((shinyCharm ? 2 : 0) + (daycare.getMasuada() ? 5 : 0)),
    everstone(daycare.getEverstone(Game::BW)),
    poweritem(daycare.getPowerItem()),
    ditto(daycare.getDitto()),
    parentAbility(daycare.getParentAbility(1))
{
    tsv = (tid ^ sid) >> 3;
}

QVector<EggFrame> EggGenerator5::generate(u64 seed) const
{
    switch (method)
    {
    case Method::BWBred:
        return generateBW(seed);
    case Method::BW2Bred:
        return generateBW2(seed);
    default:
        return QVector<EggFrame>();
    }
}

QVector<EggFrame> EggGenerator5::generateBW(u64 seed) const
{
    QVector<EggFrame> frames;

    MersenneTwisterFast mt(seed >> 32, 13);
    mt.advanceFrames(7);

    u8 ivs[6];
    for (u8 i = 0; i < 6; i++)
    {
        ivs[i] = mt.nextUShort() >> 11;
    }

    BWRNG rng(seed);
    rng.advanceFrames(initialFrame - 1 + offset);

    for (u32 cnt = 0; cnt < maxResults; cnt++, rng.nextULong())
    {
        EggFrame frame(cnt + initialFrame);
        // TODO: chatot

        BWRNG go(rng.getSeed());
        go.advanceFrames(2);

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
                    nature = daycare.getEverstoneNature(go.nextUInt(2));
                }
                else
                {
                    u8 parent = daycare.getItem(0) == 1 ? 0 : 1;
                    nature = daycare.getEverstoneNature(parent);
                }
            }
        }
        frame.setNature(nature);

        // Add check for mother having HA
        bool hiddenAbility = go.nextUInt(100) >= 40 && daycare.getParentAbility(1) == 2;

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
            go.advanceFrames(1);
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
                u8 item = daycare.getItem(parent);

                frame.setIVs(item - 2, daycare.getParentIV(parent, item - 2));
                frame.setInheritance(item - 2, parent + 1);
            }
            else
            {
                u8 parent = (daycare.getItem(0) >= 2 && daycare.getItem(1) <= 7) ? 0 : 1;
                u8 item = daycare.getItem(parent);

                frame.setIVs(item - 2, daycare.getParentIV(parent, item - 2));
                frame.setInheritance(item - 2, parent + 1);
            }
        }

        // IV Inheritance
        for (; inheritance < 3;)
        {
            u8 index = go.nextUInt(6);
            u8 parent = go.nextUInt(2);

            // Assign stat inheritance
            if (frame.getInheritance(index) == 0)
            {
                inheritance++;
                frame.setIVs(index, daycare.getParentIV(parent, index));
                frame.setInheritance(index, parent + 1);
            }
        }

        for (u8 i = 0; i < 6; i++)
        {
            if (frame.getInheritance(i) == 0)
            {
                frame.setIVs(i, ivs[i]);
            }
        }
        frame.calculateHiddenPower();

        u32 pid = go.nextUInt(0xffffffff);
        for (u8 i = 0; i < rolls && !isShiny(pid, tsv); i++)
        {
            pid = go.nextUInt(0xffffffff);
        }

        frame.setPID(pid);
        frame.setAbility(hiddenAbility ? 2 : ((pid >> 16) & 1));
        frame.setGender(pid & 255, genderRatio);
        frame.setShiny(tsv, (pid >> 16) ^ (pid ^ 0xffff), 8);

        if (filter.compareFrame(frame))
        {
            frames.append(frame);
        }
    }

    return frames;
}

QVector<EggFrame> EggGenerator5::generateBW2(u64 seed) const
{
    QVector<EggFrame> frames;

    MersenneTwister mt(seed >> 32);
    mt.advanceFrames(2);

    u64 eggSeed = static_cast<u64>(mt.nextUInt()) << 32;
    eggSeed |= mt.nextUInt();

    EggFrame frame = generateBW2Egg(eggSeed);
    if (filter.compareIVs(frame) && filter.compareAbility(frame) && filter.compareNature(frame))
    {
        BWRNG rng(seed);
        rng.advanceFrames(initialFrame - 1 + offset);
        for (u32 cnt = 0; cnt < maxResults; cnt++, rng.nextULong())
        {
            BWRNG go(rng.getSeed());

            go.advanceFrames(2); // 2 blanks

            u32 pid = go.nextUInt();
            if (((pid >> 16) & 1) != frame.getAbility())
            {
                pid ^= 0x10000;
            }

            for (u8 i = 0; i < rolls && !isShiny(pid, tsv); i++)
            {
                pid = go.nextUInt();
                if (((pid >> 16) & 1) != frame.getAbility())
                {
                    pid ^= 0x10000;
                }
            }

            frame.setPID(pid);
            frame.setGender(pid & 255, genderRatio);
            frame.setShiny(tsv, (pid >> 16) ^ (pid & 0xffff), 8);

            if (filter.compareShiny(frame) && filter.compareGender(frame))
            {
                frame.setFrame(initialFrame + cnt);
                frames.append(frame);
            }
        }
    }

    return frames;
}

EggFrame EggGenerator5::generateBW2Egg(u64 seed) const
{
    EggFrame frame(0);
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
            nature = daycare.getEverstoneNature(rng.nextUInt(2));
        }
        else
        {
            u8 parent = daycare.getItem(0) == 1 ? 0 : 1;
            nature = daycare.getEverstoneNature(parent);
        }
    }
    frame.setNature(nature);

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
        rng.advanceFrames(1);
        ability = rng.nextUInt(2);
    }
    frame.setAbility(ability);

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
            u8 item = daycare.getItem(parent);

            frame.setIVs(item - 2, daycare.getParentIV(parent, item - 2));
            frame.setInheritance(item - 2, parent + 1);
        }
        else
        {
            u8 parent = (daycare.getItem(0) >= 2 && daycare.getItem(1) <= 7) ? 0 : 1;
            u8 item = daycare.getItem(parent);

            frame.setIVs(item - 2, daycare.getParentIV(parent, item - 2));
            frame.setInheritance(item - 2, parent + 1);
        }
    }

    // IV Inheritance
    for (; inheritance < 3;)
    {
        u8 index = rng.nextUInt(6);
        u8 parent = rng.nextUInt(2);

        if (frame.getInheritance(index) == 0)
        {
            frame.setIVs(index, daycare.getParentIV(parent, index));
            frame.setInheritance(index, parent + 1);
            inheritance++;
        }
    }

    for (u8 i = 0; i < 6; i++)
    {
        if (frame.getInheritance(i) == 0)
        {
            frame.setIVs(i, rng.nextUInt(32));
        }
    }
    frame.calculateHiddenPower();

    return frame;
}
