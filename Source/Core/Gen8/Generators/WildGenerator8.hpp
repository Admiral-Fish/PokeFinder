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

#ifndef WILDGENERATOR8_HPP
#define WILDGENERATOR8_HPP

#include <Core/Gen8/EncounterArea8.hpp>
#include <Core/Parents/Generators/WildGenerator.hpp>
#include <Core/Gen8/States/WildState8.hpp>

class WildGenerator8 : public WildGenerator
{
public:
    WildGenerator8() = default;
    WildGenerator8(u32 initialAdvances, u32 maxAdvances, u16 tid, u16 sid, const StateFilter &filter);
    std::vector<WildState8> generate(u64 seed0, u64 seed1) const;
    void setEncounterArea(const EncounterArea8 &encounterArea);

private:
    EncounterArea8 encounterArea;
};

#endif // WILDGENERATOR8_HPP
