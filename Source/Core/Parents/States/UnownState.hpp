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
#include <functional>

// enum Encounter : u8;
enum Lead : u8;

class UnownState : public State
{
public:
    UnownState() = default;

    explicit UnownState(u32 advance) : State(advance)
    {
    }

    Lead getLead() const
    {
        return lead;
    }

    void setLead(Lead lead)
    {
        this->lead = lead;
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
        setLetter(letters[letterIndex]);
    }
    
    void setLetterIndex4(u8 letterIndex)
    {
        this->letterIndex = letters4ToLetters[letterIndex];
        setLetter(letters4[letterIndex]);
    }
    
    void setLetterIndex4b(u8 letterIndex)
    {
        this->letterIndex = letterIndex+26;
        setLetter(letters[letterIndex+26]);
    }

    u8 getLetterIndex() const
    {
        return letterIndex;
    }
    std::array<std::string,28> letters = { "A", "B", "C", "D", "E",
                                           "F", "G", "H", "I", "J",
                                           "K", "L", "M", "N", "O",
                                           "P", "Q", "R", "S", "T",
                                           "U", "V", "W", "X", "Y",
                                                "Z", "!", "?" };
    std::array<std::string,20> letters4 = { "A","B","C","G","H","J","K","L","M","O","P","Q","S","T","U","V","W","X","Y","Z" };
    std::array<u8,20> letters4ToLetters = { 0,  1,  2,  6,  7,  9,  10, 11, 12, 14, 15, 16, 18, 19, 20, 21, 22, 23, 24, 25  };

    // Lead getLead() const;
    // void setLead(Lead lead);
    // u8 getEncounterSlot() const;
    // void setEncounterSlot(u8 encounterSlot);
    // Encounter getEncounter() const;
    // void setEncounter(Encounter encounter);

protected:
    Lead lead;
    std::string letter;
    // Lead lead;
    u8 letterIndex;
    // Encounter encounter;
};

#endif // UNOWNSTATE_HPP
