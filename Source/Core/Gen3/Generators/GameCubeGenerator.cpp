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

#include "GameCubeGenerator.hpp"
#include <Core/Enum/Game.hpp>
#include <Core/Enum/Method.hpp>
#include <Core/Enum/ShadowType.hpp>
#include <Core/Gen3/ShadowTemplate.hpp>
#include <Core/Gen3/StaticTemplate3.hpp>
#include <Core/Parents/PersonalInfo.hpp>
#include <Core/Parents/States/State.hpp>
#include <Core/RNG/LCRNG.hpp>
#include <Core/Util/Utilities.hpp>
#include <algorithm>

static bool isShiny(u16 high, u16 low, u16 tsv)
{
    return (high ^ low ^ tsv) < 8;
}

GameCubeGenerator::GameCubeGenerator(u32 initialAdvances, u32 maxAdvances, u32 offset, Method method, bool unset, const Profile3 &profile,
                                     const StateFilter &filter) :
    Generator(initialAdvances, maxAdvances, offset, method, profile, filter), unset(unset)
{
}

std::vector<GeneratorState> GameCubeGenerator::generate(u32 seed, const ShadowTemplate *shadowTemplate) const
{
    if ((profile.getVersion() & Game::Colosseum) != Game::None)
    {
        return generateColoShadow(seed, shadowTemplate);
    }
    return generateGalesShadow(seed, shadowTemplate);
}

std::vector<GeneratorState> GameCubeGenerator::generate(u32 seed, const StaticTemplate3 *staticTemplate) const
{
    if (method == Method::Channel)
    {
        return generateChannel(seed, staticTemplate);
    }
    return generateNonLock(seed, staticTemplate);
}

std::vector<GeneratorState> GameCubeGenerator::generateChannel(u32 seed, const StaticTemplate3 *staticTemplate) const
{
    std::vector<GeneratorState> states;
    const PersonalInfo *info = staticTemplate->getInfo();

    constexpr u16 threshHolds[2] = { 0x4000, 0x547a };

    XDRNG rng(seed, initialAdvances + offset);
    for (u32 cnt = 0; cnt <= maxAdvances; cnt++, rng.next())
    {
        XDRNG go(rng);

        // Advance through menu pattern
        for (u8 mask = 0; mask < 14;)
        {
            mask |= 1 << (go.nextUShort() >> 14);
        }

        // Advance jirachi pattern
        go.advance(4);
        if (std::any_of(std::begin(threshHolds), std::end(threshHolds), [&go](u16 thresh) { return go.nextUShort() <= thresh; }))
        {
            go.advance(1);
        }
        else
        {
            go.advance(2);
        }

        constexpr u16 tid = 40122;
        u16 sid = go.nextUShort();
        u16 high = go.nextUShort();
        u16 low = go.nextUShort();

        // u16 berry = go.nextUShort() >> 13; If >= 4 salac, else ganlon
        // u16 game = go.nextUShort() >> 12; If >= 8 ruby, else sapphire
        // u16 gender = go.nextUShort() >> 11; If >= 16 female, else male
        go.advance(3);

        // Failed non-shiny check due to operator precedence
        if (tid ^ sid ^ high ^ low < 8)
        {
            high ^= 0x8000;
        }
        u32 pid = (high << 16) | low;

        std::array<u8, 6> ivs;
        ivs[0] = go.nextUShort() >> 11;
        ivs[1] = go.nextUShort() >> 11;
        ivs[2] = go.nextUShort() >> 11;
        ivs[5] = go.nextUShort() >> 11;
        ivs[3] = go.nextUShort() >> 11;
        ivs[4] = go.nextUShort() >> 11;

        GeneratorState state(initialAdvances + cnt, pid, ivs, pid & 1, 2, staticTemplate->getLevel(), pid % 25,
                             Utilities::getShiny<true>(pid, tid ^ sid), info);
        if (filter.compareState(static_cast<const State &>(state)))
        {
            states.emplace_back(state);
        }
    }

    return states;
}

std::vector<GeneratorState> GameCubeGenerator::generateColoShadow(u32 seed, const ShadowTemplate *shadowTemplate) const
{
    std::vector<GeneratorState> states;
    const PersonalInfo *info = shadowTemplate->getInfo();

    XDRNG rng(seed, initialAdvances + offset);
    for (u32 cnt = 0; cnt <= maxAdvances; cnt++, rng.next())
    {
        XDRNG go(rng);

        // Trainer TID/SID
        u16 trainerTSV = go.nextUShort() ^ go.nextUShort();

        u8 ability;
        u16 high;
        u16 low;
        for (s8 i = 0; i < shadowTemplate->getCount(); i++)
        {
            const LockInfo &lock = shadowTemplate->getLock(i);

            // Temporary PID: 2 advances
            // IVs: 2 advances
            // Ability: 1 state
            go.advance(4);
            ability = go.nextUShort(2);

            u32 pid;
            do
            {
                high = go.nextUShort();
                low = go.nextUShort();
                pid = (high << 16) | low;

                // Shiny lock is from enemy TSV
                if (isShiny(high, low, trainerTSV))
                {
                    continue;
                }
            } while (!lock.compare(pid));
        }

        // E-Reader is included as part of the above loop, just need to set IVs to 0
        u16 iv1;
        u16 iv2;
        if (shadowTemplate->getType() == ShadowType::EReader)
        {
            iv1 = 0;
            iv2 = 0;
        }
        else
        {
            go.advance(2); // Fake PID

            iv1 = go.nextUShort();
            iv2 = go.nextUShort();
            ability = go.nextUShort(2);
            high = go.nextUShort();
            low = go.nextUShort();

            // Shiny lock is from enemy TSV
            while (isShiny(high, low, trainerTSV))
            {
                high = go.nextUShort();
                low = go.nextUShort();
            }
        }

        ability &= info->getAbility(0) != info->getAbility(1);

        u32 pid = (high << 16) | low;
        std::array<u8, 6> ivs;
        ivs[0] = iv1 & 31;
        ivs[1] = (iv1 >> 5) & 31;
        ivs[2] = (iv1 >> 10) & 31;
        ivs[3] = (iv2 >> 5) & 31;
        ivs[4] = (iv2 >> 10) & 31;
        ivs[5] = iv2 & 31;

        GeneratorState state(initialAdvances + cnt, pid, ivs, ability, Utilities::getGender(pid, info), shadowTemplate->getLevel(),
                             pid % 25, Utilities::getShiny<true>(pid, tsv), info);
        if (filter.compareState(static_cast<const State &>(state)))
        {
            states.emplace_back(state);
        }
    }

    return states;
}

std::vector<GeneratorState> GameCubeGenerator::generateGalesShadow(u32 seed, const ShadowTemplate *shadowTemplate) const
{
    std::vector<GeneratorState> states;
    const PersonalInfo *info = shadowTemplate->getInfo();

    XDRNG rng(seed, initialAdvances + offset);
    for (u32 cnt = 0; cnt <= maxAdvances; cnt++, rng.next())
    {
        XDRNG go(rng);

        // Enemy TID/SID
        go.advance(2);

        for (s8 i = 0; i < shadowTemplate->getCount(); i++)
        {
            const LockInfo &lock = shadowTemplate->getLock(i);

            // Temporary PID: 2 advances
            // IVs: 2 advances
            // Ability: 1 advance
            go.advance(5);

            // If we are looking at a shadow pokemon
            // We will assume it is already set and skip the PID process
            if (!lock.getIgnore())
            {
                u32 pid;
                do
                {
                    u16 high = go.nextUShort();
                    u16 low = go.nextUShort();
                    pid = (high << 16) | low;

                    // Shiny lock is from TSV of savefile
                    if (isShiny(high, low, tsv))
                    {
                        continue;
                    }
                } while (!lock.compare(pid));
            }
        }

        // Check for shiny lock with unset
        if ((shadowTemplate->getType() == ShadowType::SecondShadow || shadowTemplate->getType() == ShadowType::Salamence) && unset)
        {
            while (isShiny(go.nextUShort(), go.nextUShort(), tsv))
            {
            }
        }

        go.advance(2); // Fake PID

        u16 iv1 = go.nextUShort();
        u16 iv2 = go.nextUShort();
        u8 ability = go.nextUShort(2) & (info->getAbility(0) != info->getAbility(1));
        u16 high = go.nextUShort();
        u16 low = go.nextUShort();

        // Shiny lock is from TSV of savefile
        while (isShiny(high, low, tsv))
        {
            high = go.nextUShort();
            low = go.nextUShort();
        }

        u32 pid = (high << 16) | low;
        std::array<u8, 6> ivs;
        ivs[0] = iv1 & 31;
        ivs[1] = (iv1 >> 5) & 31;
        ivs[2] = (iv1 >> 10) & 31;
        ivs[3] = (iv2 >> 5) & 31;
        ivs[4] = (iv2 >> 10) & 31;
        ivs[5] = iv2 & 31;

        GeneratorState state(initialAdvances + cnt, pid, ivs, ability, Utilities::getGender(pid, info), shadowTemplate->getLevel(),
                             pid % 25, Utilities::getShiny<true>(pid, tsv), info);
        if (filter.compareState(static_cast<const State &>(state)))
        {
            states.emplace_back(state);
        }
    }

    return states;
}

std::vector<GeneratorState> GameCubeGenerator::generateNonLock(u32 seed, const StaticTemplate3 *staticTemplate) const
{
    std::vector<GeneratorState> states;
    const PersonalInfo *info = staticTemplate->getInfo();

    u16 actualTSV = tsv;

    // Ageto Pikachu/Celebi
    if (staticTemplate->getSpecie() == 25 || staticTemplate->getSpecie() == 251)
    {
        actualTSV = 31121; // TID: 31121 SID: 0
    }
    // Mattle Ho-Oh
    else if (staticTemplate->getSpecie() == 250)
    {
        actualTSV = 10048; // TID: 10048 SID: 0
    }

    XDRNG rng(seed, initialAdvances + offset);
    for (u32 cnt = 0; cnt <= maxAdvances; cnt++, rng.next())
    {
        XDRNG go(rng);

        u16 iv1;
        u16 iv2;
        u8 ability;
        u16 high;
        u16 low;

        // Gales eevee
        if (staticTemplate->getSpecie() == 133)
        {
            actualTSV = go.nextUShort() ^ go.nextUShort();
            go.advance(2);
        }

        // Colo Espeon/Umbreon
        if (staticTemplate->getSpecie() == 196 || staticTemplate->getSpecie() == 197)
        {
            actualTSV = go.nextUShort() ^ go.nextUShort();

            int rounds = staticTemplate->getSpecie() == 196 ? 2 : 1;
            for (int i = 0; i < rounds; i++)
            {
                go.advance(2);
                iv1 = go.nextUShort();
                iv2 = go.nextUShort();
                ability = go.nextUShort(2);
                high = go.nextUShort();
                low = go.nextUShort();

                // Colo starters are shiny locked and male locked
                while (isShiny(high, low, actualTSV) || (low & 255) < info->getGender())
                {
                    high = go.nextUShort();
                    low = go.nextUShort();
                }
            }
        }
        else
        {
            iv1 = go.nextUShort();
            iv2 = go.nextUShort();
            ability = go.nextUShort(2);
            high = go.nextUShort();
            low = go.nextUShort();

            if (staticTemplate->getShiny() == Shiny::Never)
            {
                while (isShiny(high, low, actualTSV))
                {
                    high = go.nextUShort();
                    low = go.nextUShort();
                }
            }
        }

        u32 pid = (high << 16) | low;
        ability &= info->getAbility(0) != info->getAbility(1);
        std::array<u8, 6> ivs;
        ivs[0] = iv1 & 31;
        ivs[1] = (iv1 >> 5) & 31;
        ivs[2] = (iv1 >> 10) & 31;
        ivs[3] = (iv2 >> 5) & 31;
        ivs[4] = (iv2 >> 10) & 31;
        ivs[5] = iv2 & 31;

        GeneratorState state(initialAdvances + cnt, pid, ivs, ability, Utilities::getGender(pid, info), staticTemplate->getLevel(),
                             pid % 25, Utilities::getShiny<true>(pid, actualTSV), info);
        if (filter.compareState(static_cast<const State &>(state)))
        {
            states.emplace_back(state);
        }
    }

    return states;
}
