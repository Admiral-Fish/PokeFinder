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

#include "WildGenerator.hpp"
#include <Core/Enum/Lead.hpp>

WildGenerator::WildGenerator(u32 initialFrame, u32 maxResults, u16 tid, u16 sid, u8 genderRatio, Method method, const FrameFilter &filter) :
    Generator(initialFrame, maxResults, tid, sid, genderRatio, method, filter),
    lead(Lead::None)
{
}

void WildGenerator::setEncounter(Encounter encounter)
{
    this->encounter = encounter;
}

void WildGenerator::setLead(Lead lead)
{
    this->lead = lead;
}

void WildGenerator::setSynchNature(u8 synchNature)
{
    this->synchNature = synchNature;
}
