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

#ifndef UNOWNGENERATOR4_HPP
#define UNOWNGENERATOR4_HPP

#include <Core/Gen4/EncounterArea4.hpp>
#include <Core/Gen4/States/UnownState4.hpp>
#include <Core/Parents/Generators/WildGenerator.hpp>

class UnownGenerator4 : public WildGenerator
{
public:
    UnownGenerator4() = default;
    UnownGenerator4(u32 initialAdvances, u32 maxAdvances, u16 tid, u16 sid, u8 genderRatio, Method method, const StateFilter &filter);
    std::vector<UnownState4> generate(u32 seed) const;
    void setEncounterArea(const u8 encounterArea);

private:
    std::vector<UnownState4> generateMethodJ(u32 seed) const;
    std::vector<UnownState4> generateMethodK(u32 seed) const;
    std::vector<UnownState4> generateChainedShiny(u32 seed) const;

    u8 encounterArea;
};

#endif // UNOWNGENERATOR4_HPP
