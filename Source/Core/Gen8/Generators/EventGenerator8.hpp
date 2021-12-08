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

#ifndef EVENTGENERATOR8_HPP
#define EVENTGENERATOR8_HPP

#include <Core/Gen8/WB8.hpp>
#include <Core/Parents/Generators/Generator.hpp>
#include <Core/Parents/States/State.hpp>

class EventGenerator8 : public Generator
{
public:
    EventGenerator8() = default;
    EventGenerator8(u32 initialAdvances, u32 maxAdvances, u16 tid, u16 sid, const StateFilter &filter, const WB8 &parameters);
    std::vector<State> generate(u64 seed0, u64 seed1) const;

private:
    WB8 parameters;
    u8 ivCount;
};

#endif // EVENTGENERATOR8_HPP
