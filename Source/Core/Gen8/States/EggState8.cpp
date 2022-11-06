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

#include "EggState8.hpp"
#include <Core/Parents/PersonalInfo.hpp>
#include <Core/Util/Nature.hpp>

constexpr int order[6] = { 0, 1, 2, 5, 3, 4 };

EggState8::EggState8(u32 advances, u8 gender, u8 nature, u8 ability, const std::array<u8, 6> &inheritance, const std::array<u8, 6> &ivs,
                     u32 pid, u8 shiny, const PersonalInfo *info) :
    EggGeneratorState(advances)
{
    this->level = 1;
    this->gender = gender;
    this->nature = nature;
    this->ability = ability;
    abilityIndex = info->getAbility(ability);
    this->inheritance = inheritance;

    this->ivs = ivs;
    for (int i = 0; i < 6; i++)
    {
        u16 stat = ((2 * info->getStat(i) + ivs[i]) * level) / 100;
        if (i == 0)
        {
            stats[i] = stat + level + 10;
        }
        else
        {
            stats[i] = Nature::computeStat(stat + 5, nature, i);
        }
    }

    this->pid = pid;
    this->shiny = shiny;
}
