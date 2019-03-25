/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2019 by Admiral_Fish, bumba, and EzPzStreamz
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

#include "Pokemon.hpp"

Pokemon::Pokemon(u16 hp, u16 atk, u16 def, u16 spa, u16 spd, u16 spe, u16 gender, u16 ability1, u16 ability2, u16 abilityH, u16 formCount, u16 formStatIndex)
{
    baseHP = hp;
    baseAtk = atk;
    baseDef = def;
    baseSpA = spa;
    baseSpD = spd;
    baseSpe = spe;
    this->gender = gender;
    this->ability1 = ability1;
    this->ability2 = ability2;
    this->abilityH = abilityH;
    this->formCount = formCount;
    this->formStatIndex = formStatIndex;
}

QVector<Pokemon> Pokemon::loadPersonal(u8 gen)
{
    QString path;
    int size;
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

    QVector<Pokemon> pokemon;
    QFile file(path);
    if (file.open(QIODevice::ReadOnly))
    {
        QByteArray data = file.readAll();
        file.close();

        while (data.size() > 0)
        {
            u16 hp = data.at(0);
            u16 atk = data.at(1);
            u16 def = data.at(2);
            u16 spe = data.at(3);
            u16 spa = data.at(4);
            u16 spd = data.at(5);
            u16 gender = data.at(6);
            u16 ability1 = data.at(7);
            u16 ability2 = data.at(8);
            u16 abilityH = 0, formCount = 0, formStatIndex = 0;

            if (gen == 4)
            {
                formCount = data.at(9);
                formStatIndex = (static_cast<u16>(data.at(10)) << 8) | data.at(11);
            }
            else if (gen == 5)
            {
                abilityH = data.at(9);
                formCount = data.at(10);
                formStatIndex = (static_cast<u16>(data.at(11)) << 8) | data.at(12);
            }

            pokemon.append(Pokemon(hp, atk, def, spa, spd, spe, gender, ability1, ability2, abilityH, formCount, formStatIndex));
            data = data.mid(size);
        }
    }

    return pokemon;
}

u16 Pokemon::getBaseHP() const
{
    return baseHP;
}

u16 Pokemon::getBaseAtk() const
{
    return baseAtk;
}

u16 Pokemon::getBaseDef() const
{
    return baseDef;
}

u16 Pokemon::getBaseSpA() const
{
    return baseSpA;
}

u16 Pokemon::getBaseSpD() const
{
    return baseSpD;
}

u16 Pokemon::getBaseSpe() const
{
    return baseSpe;
}

QVector<u16> Pokemon::getBaseStats() const
{
    return { baseHP, baseAtk, baseDef, baseSpA, baseSpD, baseSpe };
}

u16 Pokemon::getGender() const
{
    return gender;
}

u16 Pokemon::getAbility1() const
{
    return ability1;
}

u16 Pokemon::getAbility2() const
{
    return ability2;
}

u16 Pokemon::getAbilityH() const
{
    return abilityH;
}

u16 Pokemon::getFormCount() const
{
    return formCount;
}

u16 Pokemon::getFormStatIndex() const
{
    return formStatIndex;
}
