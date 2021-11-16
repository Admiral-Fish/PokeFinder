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

#include "EggGenerator8.hpp"
#include <Core/Enum/Method.hpp>
#include <Core/RNG/Xoroshiro.hpp>
#include <Core/RNG/Xorshift.hpp>

EggGenerator8::EggGenerator8(u32 initialAdvances, u32 maxAdvances, u16 tid, u16 sid, u8 genderRatio, const StateFilter &filter,
                             const Daycare &daycare, bool shinyCharm) :
    EggGenerator(initialAdvances, maxAdvances, tid, sid, genderRatio, Method::Null, filter, daycare), shinyCharm(shinyCharm)
{
}

std::vector<EggState> EggGenerator8::generate(u64 seed0, u64 seed1) const
{
    return std::vector<EggState>();
}