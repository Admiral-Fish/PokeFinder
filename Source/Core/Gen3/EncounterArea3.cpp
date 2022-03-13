/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2022 by Admiral_Fish, bumba, and EzPzStreamz
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

#include "EncounterArea3.hpp"
#include <Core/Parents/Slot.hpp>

EncounterArea3::EncounterArea3(u8 location, u8 rate, Encounter encounter, const std::vector<Slot> &pokemon) :
    EncounterArea(location, rate, encounter, pokemon)
{
}

u8 EncounterArea3::calcLevel(u8 index, u16 prng) const
{
    return (prng % (pokemon[index].getMaxLevel() - pokemon[index].getMinLevel() + 1)) + pokemon[index].getMinLevel();
}

u8 EncounterArea3::calcLevel(u8 index) const
{
    return pokemon[index].getMinLevel();
}

bool EncounterArea3::rseSafariZone() const
{
    switch (location)
    {
    // Ruby/Sapphire
    case 90: // Safari Zone Northeast
    case 187: // Safari Zone Northeast
    case 89: // Safari Zone Northwest
    case 186: // Safari Zone Northwest
    case 92: // Safari Zone Southeast
    case 189: // Safari Zone Southeast
    case 91: // Safari Zone Southwest
    case 188: // Safari Zone Southwest
    // Emerald
    case 73: // Safari Zone North
    case 98: // Safari Zone Northeast
    case 74: // Safari Zone Northwest
    case 20: // Safari Zone South
    case 97: // Safari Zone Southeast
    case 72: // Safari Zone Southwest
        return true;
    default:
        return false;
    }
}
