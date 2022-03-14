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

#ifndef IDGENERATOR3_HPP
#define IDGENERATOR3_HPP

#include <Core/Parents/Filters/IDFilter.hpp>
#include <Core/Parents/Generators/IDGenerator.hpp>

class IDState3;

class IDGenerator3 : public IDGenerator<IDFilter>
{
public:
    IDGenerator3(u32 initialAdvances, u32 maxAdvances, const IDFilter &filter);
    std::vector<IDState3> generateXDColo(u32 seed);
    std::vector<IDState3> generateFRLGE(u16 tid);
    std::vector<IDState3> generateRS(u32 seed);
};

#endif // IDGENERATOR3_HPP
