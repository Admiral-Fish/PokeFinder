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

#include "UnownState.hpp"
#include <Core/Enum/Encounter.hpp>
#include <Core/Enum/Lead.hpp>

UnownState::UnownState(u32 advance) : State(advance)
{
}

std::string UnownState::getLetter() const
{
    return letter;
}

void UnownState::setLetter(std::string letter)
{
    this->letter = letter;
}
// Lead UnownState::getLead() const
// {
//     return lead;
// }

// void UnownState::setLead(Lead lead)
// {
//     this->lead = lead;
// }

// u8 UnownState::getEncounterSlot() const
// {
//     return encounterSlot;
// }

// void UnownState::setEncounterSlot(u8 encounterSlot)
// {
//     this->encounterSlot = encounterSlot;
// }

// Encounter UnownState::getEncounter() const
// {
//     return encounter;
// }

// void UnownState::setEncounter(Encounter encounter)
// {
//     this->encounter = encounter;
// }
