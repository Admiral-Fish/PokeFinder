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

#include "Slot.hpp"

Slot::Slot(u16 specie, u8 minLevel, u8 maxLevel, const PersonalInfo &info) :
    minLevel(minLevel), maxLevel(maxLevel), specie(specie), info(info)
{
}

Slot::Slot(u16 specie, u8 level, const PersonalInfo &info) : minLevel(level), maxLevel(level), specie(specie), info(info)
{
}

u8 Slot::getMinLevel() const
{
    return minLevel;
}

u8 Slot::getMaxLevel() const
{
    return maxLevel;
}

u16 Slot::getSpecie() const
{
    return specie;
}

PersonalInfo Slot::getInfo() const
{
    return info;
}

void Slot::setSpecie(u16 specie, const PersonalInfo &info)
{
    this->specie = specie;
    this->info = info;
}
