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

#ifndef STATICGENERATOR8_HPP
#define STATICGENERATOR8_HPP

#include <Core/Parents/Generators/StaticGenerator.hpp>

class StaticState;
class StaticTemplate;

class StaticGenerator8 : public StaticGenerator
{
public:
    StaticGenerator8(u32 initialAdvances, u32 maxAdvances, u16 tid, u16 sid, const StateFilter &filter);
    std::vector<StaticState> generate(u64 seed0, u64 seed1, const StaticTemplate &parameters) const;
    std::vector<StaticState> generateRoamer(u64 seed0, u64 seed1, const StaticTemplate &parameters) const;
};

#endif // STATICGENERATOR8_HPP
