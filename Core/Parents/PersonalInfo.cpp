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
#include <QFile>

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

QVector<PersonalInfo> PersonalInfo::loadPersonal(u8 gen)
{
    QString path;
    int size = 0;
    if (gen == 3)
    {
        path = ":/personal/personal3.bin";
        size = 9;
    }
    else if (gen == 4)
    {
        path = ":/personal/personal4.bin";
        size = 12;
    }
    else if (gen == 5)
    {
        path = ":/personal/personal5.bin";
        size = 13;
    }

    QVector<PersonalInfo> pokemon;
    QFile file(path);
    if (file.open(QIODevice::ReadOnly))
    {
        QByteArray data = file.readAll();
        file.close();

        for (auto i = 0; i < data.size(); i += size)
        {
            u8 hp = static_cast<u8>(data[i]);
            u8 atk = static_cast<u8>(data[i + 1]);
            u8 def = static_cast<u8>(data[i + 2]);
            u8 spe = static_cast<u8>(data[i + 3]);
            u8 spa = static_cast<u8>(data[i + 4]);
            u8 spd = static_cast<u8>(data[i + 5]);
            u8 gender = static_cast<u8>(data[i + 6]);
            u16 ability1 = data.mid(i + 7, 1).toHex().toUShort(nullptr, 16);
            u16 ability2 = data.mid(i + 8, 1).toHex().toUShort(nullptr, 16);
            u8 formCount = 1;
            u16 abilityH = 0;
            u16 formStatIndex = 0;

            if (gen == 4)
            {
                formCount = static_cast<u8>(data[i + 9]);
                formStatIndex = data.mid(i + 10, 2).toHex().toUShort(nullptr, 16);
            }
            else if (gen == 5)
            {
                abilityH = data.mid(i + 9, 1).toHex().toUShort(nullptr, 16);
                formCount = static_cast<u8>(data[i + 10]);
                formStatIndex = data.mid(i + 11, 2).toHex().toUShort(nullptr, 16);
            }

            pokemon.append(PersonalInfo(hp, atk, def, spa, spd, spe, gender, ability1, ability2, abilityH, formCount, formStatIndex));
        }
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

QVector<u8> PersonalInfo::getBaseStats() const
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
