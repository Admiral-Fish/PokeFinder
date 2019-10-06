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

#ifndef POKEMON_HPP
#define POKEMON_HPP

#include <QVector>
#include <Core/Util/Global.hpp>

namespace PokeFinderCore
{
    class Pokemon
    {
    public:
        Pokemon() = default;
        Pokemon(u16 hp, u16 atk, u16 def, u16 spa, u16 spd, u16 spe, u16 gender, u16 ability1, u16 ability2, u16 abilityH = 0, u16 formCount = 0, u16 formStatIndex = 0);
        static QVector<Pokemon> loadPersonal(u8 gen);
        u16 getBaseHP() const;
        u16 getBaseAtk() const;
        u16 getBaseDef() const;
        u16 getBaseSpA() const;
        u16 getBaseSpD() const;
        u16 getBaseSpe() const;
        QVector<u16> getBaseStats() const;
        u16 getGender() const;
        u16 getAbility1() const;
        u16 getAbility2() const;
        u16 getAbilityH() const;
        u16 getFormCount() const;
        u16 getFormStatIndex() const;

    private:
        u16 baseHP;
        u16 baseAtk;
        u16 baseDef;
        u16 baseSpA;
        u16 baseSpD;
        u16 baseSpe;
        u16 gender;
        u16 ability1;
        u16 ability2;
        u16 abilityH;
        u16 formCount;
        u16 formStatIndex;
    };
}

#endif // POKEMON_HPP
