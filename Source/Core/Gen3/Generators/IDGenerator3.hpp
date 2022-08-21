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

#include <Core/Parents/Generators/IDGenerator.hpp>

class IDState;

class IDGenerator3 : public IDGenerator
{
public:
    IDGenerator3(u32 initialAdvances, u32 maxAdvances, const IDFilter &filter);
    std::vector<IDState> generateXDColo(u32 seed);
    std::vector<IDState> generateFRLGE(u16 tid);
    std::vector<IDState> generateRS(u16 seed);
};

#endif // IDGENERATOR3_HPP
