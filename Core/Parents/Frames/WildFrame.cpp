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

#include "WildFrame.hpp"
#include <Core/Enum/Encounter.hpp>
#include <Core/Enum/Lead.hpp>

WildFrame::WildFrame(u32 frame) : Frame(frame), lead(Lead::None)
{
}

Lead WildFrame::getLead() const
{
    return lead;
}

void WildFrame::setLead(Lead lead)
{
    this->lead = lead;
}

u8 WildFrame::getEncounterSlot() const
{
    return encounterSlot;
}

void WildFrame::setEncounterSlot(u8 encounterSlot)
{
    this->encounterSlot = encounterSlot;
}

Encounter WildFrame::getEncounter() const
{
    return encounter;
}

void WildFrame::setEncounter(Encounter encounter)
{
    this->encounter = encounter;
}
