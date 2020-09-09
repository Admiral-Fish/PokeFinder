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

#include "Daycare.hpp"
#include <Core/Enum/Game.hpp>
#include <algorithm>

Daycare::Daycare(const QVector<u8> &parent1IVs, const QVector<u8> &parent2IVs, u8 parent1Ability, u8 parent2Ability, u8 parent1Gender,
                 u8 parent2Gender, u8 parent1Item, u8 parent2Item, u8 parent1Nature, u8 parent2Nature, bool masuada, bool nidoranVolbeat) :
    parent1IVs(parent1IVs),
    parent2IVs(parent2IVs),
    parentAbility { parent1Ability, parent2Ability },
    parentGender { parent1Gender, parent2Gender },
    parentItem { parent1Item, parent2Item },
    parentNature { parent1Nature, parent2Nature },
    masuada(masuada),
    nidoranVolbeat(nidoranVolbeat)
{
}

u8 Daycare::getParentIV(u8 parent, u8 index) const
{
    if (parent == 0)
    {
        return parent1IVs.at(index);
    }

    return parent2IVs.at(index);
}

u8 Daycare::getParentAbility(u8 parent) const
{
    return parentAbility[parent];
}

u8 Daycare::getParentGender(u8 parent) const
{
    return parentGender[parent];
}

u8 Daycare::getParentItem(u8 parent) const
{
    return parentItem[parent];
}

u8 Daycare::getParentNature(u8 parent) const
{
    return parentNature[parent];
}

u8 Daycare::getEverstoneCount(Game version) const
{
    int count = 0;

    if ((version & Game::Emerald) | (version & Game::DPPt))
    {
        u8 parent;

        for (u8 i = 0; i < 2; i++)
        {
            if (parentGender[i] == 1)
            {
                parent = i;
            }
        }

        for (u8 i = 0; i < 2; i++)
        {
            if (parentGender[i] == 3)
            {
                parent = i;
            }
        }

        if (parentItem[parent] == 1)
        {
            count++;
        }
    }
    else
    {
        count = std::count_if(std::begin(parentItem), std::end(parentItem), [](u8 item) { return item == 1; });
    }

    return count;
}

u8 Daycare::getPowerItemCount() const
{
    return std::count_if(std::begin(parentItem), std::end(parentItem), [](u8 item) { return item >= 2 && item <= 7; });
}

bool Daycare::getDitto() const
{
    return std::any_of(std::begin(parentGender), std::end(parentGender), [](u8 gender) { return gender == 3; });
}

bool Daycare::getMasuada() const
{
    return masuada;
}

bool Daycare::getNidoranVolbeat() const
{
    return nidoranVolbeat;
}
