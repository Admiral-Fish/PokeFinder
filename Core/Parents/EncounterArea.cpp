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

u8 EncounterArea::getLocation() const
{
    return location;
}

QVector<Slot> EncounterArea::getPokemon() const
{
    return pokemon;
}

QVector<u16> EncounterArea::getUniqueSpecies() const
{
    QVector<u16> nums;

    for (const auto &mon : pokemon)
    {
        if (!nums.contains(mon.getSpecie()))
        {
            nums.append(mon.getSpecie());
        }
    }

    return nums;
}

QVector<bool> EncounterArea::getSlots(u16 num) const
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

void EncounterArea::setSlot(u8 index, u8 specie)
{
    pokemon[index].setSpecie(specie);
}


Slot::Slot(u16 specie, u8 minLevel, u8 maxLevel)
{
    this->specie = specie;
    this->minLevel = minLevel;
    this->maxLevel = maxLevel;
}

Slot::Slot(u16 specie, u8 level)
{
    this->specie = specie;
    minLevel = level;
    maxLevel = level;
}
