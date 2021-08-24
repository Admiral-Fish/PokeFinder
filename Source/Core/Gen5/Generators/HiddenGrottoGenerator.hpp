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

#ifndef HIDDENGROTTOGENERATOR_HPP
#define HIDDENGROTTOGENERATOR_HPP

#include <Core/Gen5/Filters/HiddenGrottoFilter.hpp>
#include <Core/Gen5/States/HiddenGrottoState.hpp>
#include <Core/Parents/Generators/Generator.hpp>
#include <vector>

class HiddenGrottoGenerator
{
public:
    HiddenGrottoGenerator() = default;
    HiddenGrottoGenerator(u32 initialAdvances, u32 maxAdvances, u8 genderRatio, u8 powerLevel, const HiddenGrottoFilter &filter);
    std::vector<HiddenGrottoState> generate(u64 seed) const;
    void setInitialAdvances(u32 initialAdvances);

private:
    u32 initialAdvances;
    u32 maxAdvances;
    u8 genderRatio;
    u8 powerLevel;
    HiddenGrottoFilter filter;
};

#endif // HIDDENGROTTOGENERATOR_HPP
