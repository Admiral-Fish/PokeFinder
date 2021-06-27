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

#ifndef UNOWNSTATE_HPP
#define UNOWNSTATE_HPP

#include <Core/Parents/States/State.hpp>
#include <string>

// enum Encounter : u8;
// enum Lead : u8;

class UnownState : public State
{
public:
    UnownState() = default;

    explicit UnownState(u32 advance) : State(advance)
    {
    }

    std::string getLetter() const
    {
        return letter;
    }

    void setLetter(std::string letter)
    {
        this->letter = letter;
    }
    
    void setLetterIndex(u8 letterIndex)
    {
        this->letterIndex = letterIndex;
    }

    u8 getLetterIndex() const
    {
        return letterIndex;
    }

    // Lead getLead() const;
    // void setLead(Lead lead);
    // u8 getEncounterSlot() const;
    // void setEncounterSlot(u8 encounterSlot);
    // Encounter getEncounter() const;
    // void setEncounter(Encounter encounter);

protected:
    std::string letter;
    // Lead lead;
    u8 letterIndex;
    // Encounter encounter;
};

#endif // UNOWNSTATE_HPP
