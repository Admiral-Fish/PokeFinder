/*
 * This file is part of PokéFinder
 * Copyright (C) 2017 by Admiral_Fish, bumba, and EzPzStreamz
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

#include "EncounterArea.hpp"

EncounterArea::EncounterArea(int location, Encounter type, const QVector<Slot> &pokemon)
{
    this->location = location;
    this->type = type;
    this->pokemon = pokemon;
}

Encounter EncounterArea::getType() const
{
    return type;
}

int EncounterArea::getLocation() const
{
    return location;
}

QVector<int> EncounterArea::getUniqueSpecies() const
{
    QVector<int> nums;

    for (const auto &mon : pokemon)
    {
        if (!nums.contains(mon.getSpecie()))
        {
            nums.append(mon.getSpecie());
        }
    }

    return nums;
}

QVector<bool> EncounterArea::getSlots(u32 num) const
{
    QVector<bool> flags;

    for (const auto &mon : pokemon)
    {
        flags.append(mon.getSpecie() == num);
    }

    return flags;
}

QStringList EncounterArea::getSpecieNames() const
{
    return Translator::getSpecies(getUniqueSpecies());
}

Slot::Slot(int specie, u16 minLevel, u16 maxLevel)
{
    this->specie = specie;
    this->minLevel = minLevel;
    this->maxLevel = maxLevel;
}

Slot::Slot(int specie, u16 level)
{
    this->specie = specie;
    minLevel = level;
    maxLevel = level;
}
