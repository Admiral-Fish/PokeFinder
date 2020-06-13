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

#ifndef DAYCARE_HPP
#define DAYCARE_HPP

#include <Core/Util/Global.hpp>
#include <QVector>

enum Game : u16;

class Daycare
{
public:
    Daycare() = default;
    Daycare(const QVector<u8> &parent1IVs, const QVector<u8> &parent2IVs, u8 parent1Ability, u8 parent2Ability, u8 parent1Gender,
            u8 parent2Gender, u8 parent1Item, u8 parent2Item, u8 parent1Nature, u8 parent2Nature, bool masuada, bool nidoranVolbeat);
    u8 getEverstone(Game version) const;
    u8 getEverstoneNature(u8 parent) const;
    u8 getItem(u8 parent) const;
    u8 getPowerItem() const;
    u8 getParentIV(u8 parent, u8 index) const;
    u8 getParentAbility(u8 parent) const;
    bool getDitto() const;
    bool getMasuada() const;
    bool getNidoranVolbeat() const;

private:
    QVector<u8> parent1IVs;
    QVector<u8> parent2IVs;
    u8 parentAbility[2];
    u8 parentGender[2]; // 3 - Ditto
    u8 parentItem[2]; // 0 - No item, 1 - Everstone, 2-7 Power items
    u8 parentNature[2];
    bool masuada;
    bool nidoranVolbeat;
};

#endif // DAYCARE_HPP
