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

<<<<<<< HEAD
=======
<<<<<<< HEAD:Source/Core/Parents/Generators/UnownGenerator.cpp
#include "UnownGenerator.hpp"
#include <Core/Enum/Lead.hpp>

UnownGenerator::UnownGenerator(u32 initialAdvances, u32 maxAdvances, u16 tid, u16 sid, u8 genderRatio, Method method,
                             const StateFilter &filter) :
    Generator(initialAdvances, maxAdvances, tid, sid, genderRatio, method, filter), lead(Lead::None)
{
}

void UnownGenerator::setEncounter(Encounter encounter)
{
    this->encounter = encounter;
}

void UnownGenerator::setLead(Lead lead)
{
    this->lead = lead;
}

void UnownGenerator::setSynchNature(u8 synchNature)
{
    this->synchNature = synchNature;
}
=======
>>>>>>> 2774aec6ede85294050891aeae671819c22373ae
#ifndef PERSONALLOADER_HPP
#define PERSONALLOADER_HPP

class PersonalInfo;

namespace PersonalLoader3
{
    const PersonalInfo *getPersonal();
}

namespace PersonalLoader4
{
    const PersonalInfo *getPersonal();
}

namespace PersonalLoader5
{
    const PersonalInfo *getPersonal();
}

#endif // PERSONALLOADER_HPP
<<<<<<< HEAD
=======
>>>>>>> 2774aec6ede85294050891aeae671819c22373ae:Source/Core/Parents/PersonalLoader.hpp
>>>>>>> 2774aec6ede85294050891aeae671819c22373ae
