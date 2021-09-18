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

#include "PersonalInfo.hpp"

std::array<u8, 6> PersonalInfo::getBaseStats() const
{
    return { baseHP, baseAtk, baseDef, baseSpA, baseSpD, baseSpe };
}

u16 PersonalInfo::getGender() const
{
    return gender;
}

u16 PersonalInfo::getAbility1() const
{
    return ability1;
}

u16 PersonalInfo::getAbility2() const
{
    return ability2;
}

u16 PersonalInfo::getAbilityH() const
{
    return abilityH;
}

u8 PersonalInfo::getFormCount() const
{
    return formCount;
}

u16 PersonalInfo::getFormStatIndex() const
{
    return formStatIndex;
}
