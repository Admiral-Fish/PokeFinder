/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2024 by Admiral_Fish, bumba, and EzPzStreamz
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

#ifndef ADJACENTSEEDCALCULATOR_HPP
#define ADJACENTSEEDCALCULATOR_HPP

#include <Core/Gen5/Profile5.hpp>
#include <Core/Gen5/States/AdjacentSeedState.hpp>
#include <Core/Global.hpp>
#include <Core/Util/DateTime.hpp>
#include <vector>

enum class Buttons : u16;

namespace AdjacentSeedCalculator
{
    std::vector<AdjacentSeedState> generate(const Profile5 &profile, const DateTime &dateTime, Buttons buttons, int seconds,
                                            u32 minIVAdvance, u32 maxIVAdvance, bool roamer);
    std::vector<u32> previewPRNG(u64 seed, const Profile5 &profile, u32 pidAdvance, u32 count);
}

#endif // ADJACENTSEEDCALCULATOR_HPP
