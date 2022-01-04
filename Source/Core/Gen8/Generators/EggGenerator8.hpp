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

#ifndef EGGGENERATOR8_HPP
#define EGGGENERATOR8_HPP

#include <Core/Parents/Generators/EggGenerator.hpp>
#include <Core/Parents/States/EggState.hpp>

class EggGenerator8 : public EggGenerator
{
public:
    EggGenerator8() = default;
    EggGenerator8(u32 initialAdvances, u32 maxAdvances, u16 tid, u16 sid, u8 genderRatio, const StateFilter &filter, const Daycare &daycare,
                  bool shinyCharm, u8 compatability);
    std::vector<EggState> generate(u64 seed0, u64 seed1) const;

private:
    bool shinyCharm;
    u8 compatability;
};

#endif // EGGGENERATOR8_HPP
