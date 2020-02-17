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

#ifndef PERSONALINFO_HPP
#define PERSONALINFO_HPP

#include <Core/Util/Global.hpp>
#include <QVector>

class PersonalInfo
{
public:
    PersonalInfo() = default;
    PersonalInfo(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe, u8 gender, u16 ability1, u16 ability2, u16 abilityH = 0, u8 formCount = 1,
                 u16 formStatIndex = 0);
    static QVector<PersonalInfo> loadPersonal(u8 gen);
    u16 getBaseHP() const;
    u16 getBaseAtk() const;
    u16 getBaseDef() const;
    u16 getBaseSpA() const;
    u16 getBaseSpD() const;
    u16 getBaseSpe() const;
    QVector<u8> getBaseStats() const;
    u16 getGender() const;
    u16 getAbility1() const;
    u16 getAbility2() const;
    u16 getAbilityH() const;
    u8 getFormCount() const;
    u16 getFormStatIndex() const;

private:
    u8 baseHP;
    u8 baseAtk;
    u8 baseDef;
    u8 baseSpA;
    u8 baseSpD;
    u8 baseSpe;
    u8 gender;
    u16 ability1;
    u16 ability2;
    u16 abilityH;
    u8 formCount;
    u16 formStatIndex;
};

#endif // PERSONALINFO_HPP
