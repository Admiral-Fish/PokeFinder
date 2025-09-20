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

#include "ChainedSIDCalc.hpp"
#include <Core/Enum/Method.hpp>
#include <Core/Parents/PersonalInfo.hpp>
#include <Core/RNG/LCRNG.hpp>
#include <Core/RNG/LCRNGReverse.hpp>
#include <algorithm>

ChainedSIDCalc::ChainedSIDCalc(u16 tid) : tid(tid)
{
    for (u32 i = 0; i <= 0xffff; i += 8)
    {
        sids.emplace_back(i);
    }
}

void ChainedSIDCalc::addEntry(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe, u16 ability, u8 gender, u8 nature, const PersonalInfo *info)
{
    std::vector<std::pair<u32, u32>> pids;

    auto seeds = LCRNGReverse::recoverPokeRNGIV(hp, atk, def, spa, spd, spe, Method::Method1);
    for (int i = 0; i < seeds.count; i++)
    {
        PokeRNGR rng(seeds[i]);

        u32 adjust = 0;
        for (u8 j = 0; j < 13; j++)
        {
            adjust |= static_cast<u32>((rng.nextUShort() & 1) << (15 - j));
        }

        u16 pid2 = rng.nextUShort();
        u16 pid1 = rng.nextUShort();

        u32 adjustLow = adjust | (pid1 & 7);

        u16 abilityNum = info->getAbility(adjustLow & 1);

        u8 genderNum;
        switch (info->getGender())
        {
        case 255: // Genderless
            genderNum = 2;
            break;
        case 254: // Female
            genderNum = 1;
            break;
        case 0: // Male
            genderNum = 0;
            break;
        default: // Random gender
            genderNum = (adjustLow & 255) < info->getGender();
            break;
        }

        if (ability == abilityNum && gender == genderNum)
        {
            pids.emplace_back(adjustLow, pid2);
        }
    }

    std::vector<u16> newSids;
    for (u16 sid : sids)
    {
        for (const auto &pair : pids)
        {
            u32 adjustHigh = pair.first ^ tid ^ sid;
            adjustHigh &= 0xFFF8;
            adjustHigh += (pair.second & 7);

            u32 pid = (adjustHigh << 16) | pair.first;
            if ((pid % 25) == nature)
            {
                newSids.emplace_back(sid);
            }
        }
    }

    sids = newSids;
    sids.erase(std::unique(sids.begin(), sids.end()), sids.end());
}

std::vector<u16> ChainedSIDCalc::getSIDs() const
{
    return sids;
}
