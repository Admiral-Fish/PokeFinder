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

#include "PersonalInfo.hpp"
#include <Core/Resources/Resources.hpp>

PersonalInfo::PersonalInfo(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe, u8 gender, u16 ability1, u16 ability2, u16 abilityH, u8 formCount,
                           u16 formStatIndex) :
    baseHP(hp),
    baseAtk(atk),
    baseDef(def),
    baseSpA(spa),
    baseSpD(spd),
    baseSpe(spe),
    gender(gender),
    ability1(ability1),
    ability2(ability2),
    abilityH(abilityH),
    formCount(formCount),
    formStatIndex(formStatIndex)
{
}

std::vector<PersonalInfo> PersonalInfo::loadPersonal(u8 gen)
{
    const u8 *data;
    int size;
    int offset;

    if (gen == 3)
    {
        data = personal3;
        size = 3483;
        offset = 9;
    }
    else if (gen == 4)
    {
        data = personal4;
        size = 6096;
        offset = 12;
    }
    else
    {
        data = personal5;
        size = 9217;
        offset = 13;
    }

    std::vector<PersonalInfo> pokemon;
    for (int i = 0; i < size; i += offset)
    {
        u8 hp = data[i];
        u8 atk = data[i + 1];
        u8 def = data[i + 2];
        u8 spe = data[i + 3];
        u8 spa = data[i + 4];
        u8 spd = data[i + 5];
        u8 gender = data[i + 6];
        u16 ability1 = data[i + 7];
        u16 ability2 = data[i + 8];
        u8 formCount = 1;
        u16 abilityH = 0;
        u16 formStatIndex = 0;

        if (gen == 4)
        {
            formCount = data[i + 9];
            formStatIndex = static_cast<u16>(data[i + 10] << 8) | data[i + 11];
        }
        else if (gen == 5)
        {
            abilityH = data[i + 9];
            formCount = data[i + 10];
            formStatIndex = static_cast<u16>(data[i + 11] << 8) | data[i + 12];
        }

        pokemon.emplace_back(hp, atk, def, spa, spd, spe, gender, ability1, ability2, abilityH, formCount, formStatIndex);
    }

    return pokemon;
}

u16 PersonalInfo::getBaseHP() const
{
    return baseHP;
}

u16 PersonalInfo::getBaseAtk() const
{
    return baseAtk;
}

u16 PersonalInfo::getBaseDef() const
{
    return baseDef;
}

u16 PersonalInfo::getBaseSpA() const
{
    return baseSpA;
}

u16 PersonalInfo::getBaseSpD() const
{
    return baseSpD;
}

u16 PersonalInfo::getBaseSpe() const
{
    return baseSpe;
}

std::vector<u8> PersonalInfo::getBaseStats() const
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
