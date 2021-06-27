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

#ifndef UNOWNGENERATOR_HPP
#define UNOWNGENERATOR_HPP

#include <Core/Parents/Generators/Generator.hpp>

class UnownGenerator : public Generator
{
public:
    UnownGenerator() = default;
    UnownGenerator(u32 initialAdvances, u32 maxAdvances, u16 tid, u16 sid, u8 genderRatio, Method method, const StateFilter &filter);
    void setEncounter(Encounter encounter);
    void setLead(Lead lead);
    void setSynchNature(u8 synchNature);

protected:
    Encounter encounter;
    Lead lead;
    u8 synchNature;
};

#endif // UNOWNGENERATOR_HPP
