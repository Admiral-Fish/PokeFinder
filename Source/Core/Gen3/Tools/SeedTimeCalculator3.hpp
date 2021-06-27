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

#ifndef SEEDTIMECALCULATOR3_HPP
#define SEEDTIMECALCULATOR3_HPP

#include <Core/Gen3/States/SeedTimeState3.hpp>
#include <vector>

namespace SeedTimeCalculator3
{
    std::vector<SeedTimeState3> calculateTimes(u32 seed, u32 advance, u16 year);
    u16 calculateOriginSeed(u32 seed);
}

#endif // SEEDTIMECALCULATOR3_HPP
