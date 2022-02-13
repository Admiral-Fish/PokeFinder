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

#ifndef EGGGENERATOR5_HPP
#define EGGGENERATOR5_HPP

#include <Core/Parents/Generators/EggGenerator.hpp>

class Daycare;
class EggState;

class EggGenerator5 : public EggGenerator
{
public:
    EggGenerator5(u32 initialAdvances, u32 maxAdvances, u16 tid, u16 sid, u8 genderRatio, Method method, const StateFilter &filter,
                  const Daycare &daycare, bool shinyCharm);
    std::vector<EggState> generate(u64 seed) const;

private:
    u8 rolls;
    u8 everstone;
    u8 poweritem;
    bool ditto;
    u8 parentAbility;

    std::vector<EggState> generateBW(u64 seed) const;
    std::vector<EggState> generateBW2(u64 seed) const;
    EggState generateBW2Egg(u64 seed) const;
};

#endif // EGGGENERATOR5_HPP
