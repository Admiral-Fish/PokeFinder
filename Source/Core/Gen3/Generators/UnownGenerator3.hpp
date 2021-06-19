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

#ifndef UNOWNGENERATOR3_HPP
#define UNOWNGENERATOR3_HPP

#include <Core/Parents/Generators/UnownGenerator.hpp>
#include <Core/Parents/States/UnownState.hpp>
#include <array>

class EncounterArea3;

class UnownGenerator3 : public UnownGenerator
{
public:
    std::array<std::string,28> letters = { "A", "B", "C", "D", "E",
                                           "F", "G", "H", "I", "J",
                                           "K", "L", "M", "N", "O",
                                           "P", "Q", "R", "S", "T",
                                           "U", "V", "W", "X", "Y",
                                                "Z", "!", "?" };
    UnownGenerator3() = default;
    UnownGenerator3(u32 initialAdvances, u32 maxAdvances, u16 tid, u16 sid, u8 genderRatio, Method method, const StateFilter &filter);
    std::string getLetter(u32 pid) const;
    std::string getTargetLetter(u8 location, u8 slot) const;
    std::vector<UnownState> generate(u32 seed, u8 location) const;
};

#endif // UNOWNGENERATOR3_HPP
