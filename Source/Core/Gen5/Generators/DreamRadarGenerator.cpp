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

#include "DreamRadarGenerator.hpp"
#include <Core/Enum/Method.hpp>
#include <Core/Gen5/States/DreamRadarState.hpp>
#include <Core/Parents/PersonalInfo.hpp>
#include <Core/Parents/PersonalLoader.hpp>
#include <Core/RNG/LCRNG64.hpp>
#include <Core/RNG/MT.hpp>
#include <Core/RNG/RNGList.hpp>
#include <Core/Util/Utilities.hpp>
#include <algorithm>

constexpr u8 levelTable[9] = { 5, 10, 10, 20, 20, 30, 30, 40, 40 };

static u8 gen(MT &rng)
{
    return rng.next() >> 27;
}

DreamRadarGenerator::DreamRadarGenerator(u32 initialAdvances, u32 maxAdvances, u8 badgeCount,
                                         const std::vector<DreamRadarTemplate> &radarTemplates, const Profile5 &profile,
                                         const StateFilter &filter) :
    Generator(initialAdvances, maxAdvances, 0, Method::None, profile, filter),
    radarTemplate(radarTemplates.back()),
    ivAdvances(0),
    level(levelTable[badgeCount]),
    pidAdvances(0)
{
    for (size_t i = 0; i < radarTemplates.size(); i++)
    {
        auto slot = radarTemplates[i];
        if (slot.getGenie())
        {
            pidAdvances += 5;
            ivAdvances += 13;
        }

        const PersonalInfo *info = slot.getInfo();
        if (i != (radarTemplates.size() - 1))
        {
            pidAdvances += (slot.getLegend() || info->getGender() != 255) ? 5 : 4;
            ivAdvances += 13;
        }
    }
}

std::vector<DreamRadarState> DreamRadarGenerator::generate(u64 seed) const
{
    const PersonalInfo *info = radarTemplate.getInfo();

    BWRNG rng(seed, (initialAdvances * 2) + Utilities5::initialAdvancesBW2(seed, profile.getMemoryLink()));
    auto jump = rng.getJump(pidAdvances);

    if (!profile.getMemoryLink())
    {
        rng.next();
    }

    RNGList<u8, MT, 8, gen> rngList(seed >> 32, (initialAdvances * 2) + ivAdvances + 9);

    std::vector<DreamRadarState> states;
    for (u32 cnt = 0; cnt <= maxAdvances; cnt++, rngList.advanceStates(2), rng.next())
    {
        BWRNG go(rng, jump);

        std::array<u8, 6> ivs;
        std::generate(ivs.begin(), ivs.end(), [&rngList] { return rngList.next(); });

        go.next();
        u32 pid = go.nextUInt();

        // Gender modification
        if (radarTemplate.getLegend()) // All dream radar legends are treated as 100% male
        {
            pid = Utilities5::forceGender(pid, go, 0, 0);
        }
        else if (radarTemplate.getGender() < 2)
        {
            pid = Utilities5::forceGender(pid, go, radarTemplate.getGender(), info->getGender());
        }

        // Flip ability
        pid ^= 0x10000;

        // Force non-shiny
        if (Utilities::isShiny<true>(pid, tsv))
        {
            pid ^= 0x10000000;
        }

        u8 ability = 2;
        if (radarTemplate.getAbility() == 255)
        {
            ability = (pid >> 16) & 1;
        }

        go.advance(2);

        u8 nature = go.nextUInt(25);

        DreamRadarState state(rng.nextUInt(8), initialAdvances + cnt, pid, ivs, ability, radarTemplate.getGender(), level, nature, 0, info);
        if (filter.compareState(static_cast<const State &>(state)))
        {
            states.emplace_back(state);
        }
    }

    return states;
}
