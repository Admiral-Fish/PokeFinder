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

#include "EncounterArea4.hpp"
#include <Core/Parents/Slot.hpp>

EncounterArea4::EncounterArea4(u8 location, Encounter type, const QVector<Slot> &pokemon) : EncounterArea(location, type, pokemon)
{
}

u8 EncounterArea4::calcLevel(u8 index, u16 prng) const
{
    return (prng % (pokemon.at(index).getMaxLevel() - pokemon.at(index).getMinLevel() + 1)) + pokemon.at(index).getMinLevel();
}

u8 EncounterArea4::calcLevel(u8 index) const
{
    return pokemon.at(index).getMinLevel();
}

u16 EncounterArea4::getEncounterRate() const
{
    switch (location)
    {
    case 117: // Cerulean Cave 1F
    case 119: // Cerulean Cave B1F
        return 5;
    case 8: // Ruins of Alpha (Outside)
    case 88: // Rock Tunnel 2F
        return 20;
    case 35: // Cianwood City
    case 66: // Cliff Cave
    case 73: // Route 19
        return 30;
    case 78: // Vermillion City
    case 114: // Victory Road 3F
        return 40;
    case 52: // Dark Cave (Route 31)
        return 50;
    default:
        return 0;
    }
}
