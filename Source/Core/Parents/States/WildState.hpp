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

#ifndef WILDSTATE_HPP
#define WILDSTATE_HPP

#include <Core/Enum/Encounter.hpp>
#include <Core/Enum/Lead.hpp>
#include <Core/Parents/States/State.hpp>

class WildState : public State
{
public:
    WildState() = default;

    explicit WildState(u32 advance) : State(advance), lead(Lead::None)
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

    u8 getEncounterSlot() const
    {
        return encounterSlot;
    }

    void setEncounterSlot(u8 encounterSlot)
    {
        this->encounterSlot = encounterSlot;
    }

    Encounter getEncounter() const
    {
        return encounter;
    }

    void setEncounter(Encounter encounter)
    {
        this->encounter = encounter;
    }

    u8 getItem() const
    {
        return item;
    }

    void setItem(u8 item)
    {
        this->item = item;
    }

protected:
    Lead lead;
    u8 encounterSlot;
    Encounter encounter;
    u8 item;
};

#endif // WILDSTATE_HPP
