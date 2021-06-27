/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2021 by Admiral_Fish, bumba, and EzPzStreamz
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

#ifndef ENCOUNTERAREA_HPP
#define ENCOUNTERAREA_HPP

#include <Core/Parents/Slot.hpp>
#include <Core/Util/Global.hpp>
#include <string>
#include <vector>

enum Encounter : u8;

class EncounterArea
{
public:
    EncounterArea() = default;
    EncounterArea(u8 location, Encounter encounter, const std::vector<Slot> &pokemon);
    Encounter getEncounter() const;
    u8 getLocation() const;
    std::vector<Slot> getPokemon() const;
    std::vector<u16> getUniqueSpecies() const;
    std::vector<bool> getSlots(u16 num) const;
    std::pair<u8, u8> getLevelRange(u16 specie) const;
    std::vector<std::string> getSpecieNames() const;

protected:
    std::vector<Slot> pokemon;
    u8 location;
    Encounter encounter;
};

#endif // ENCOUNTERAREA_HPP
