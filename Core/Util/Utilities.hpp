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

#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include <Core/Util/DateTime.hpp>
#include <Core/Util/Global.hpp>
#include <string>

class HGSSRoamer;

namespace Utilities
{
    u16 calcGen3Seed(const DateTime &dateTime);
    u32 calcGen4Seed(const DateTime &dateTime, u32 delay);
    std::string coinFlips(u32 seed);
    std::string getCalls(u32 seed, const HGSSRoamer &info);
    std::string getChatot(u32 seed);
    std::string getChatot64(u32 seed);
    u32 initialAdvancesBW(u64 seed, u8 rounds = 5);
    u32 initialAdvancesBW2(u64 seed, bool memory);
    u32 initialAdvancesBW2ID(u64 seed, u8 rounds = 3);
    u32 forceGender(u32 pid, u64 rand, u8 gender, u8 genderRatio);
}

#endif // UTILITIES_HPP
