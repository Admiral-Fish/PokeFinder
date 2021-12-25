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

#ifndef GENERATOR5_HPP
#define GENERATOR5_HPP

#include <Core/Parents/Generators/StaticGenerator.hpp>
#include <Core/Parents/States/StaticState.hpp>

enum Encounter : u8;

class StaticGenerator5 : public StaticGenerator
{
public:
    StaticGenerator5() = default;
    StaticGenerator5(u32 initialAdvances, u32 maxAdvances, u16 tid, u16 sid, u8 gender, u8 genderRatio, Method method,
                         Encounter encounter, const StateFilter &filter);
    std::vector<StaticState> generate(u64 seed) const;

private:
    u8 idBit;
    Encounter encounter;
    u8 gender;

    std::vector<StaticState> generateRoamerIVs(u64 seed) const;
    std::vector<StaticState> generateIVs(u64 seed) const;
    std::vector<StaticState> generateRoamerCGear(u64 seed) const;
    std::vector<StaticState> generateCGear(u64 seed) const;
    std::vector<StaticState> generateStatic(u64 seed) const;
    std::vector<StaticState> generateRoamer(u64 seed) const;
    std::vector<StaticState> generateGift(u64 seed) const;
    std::vector<StaticState> generateEntraLink(u64 seed) const;
    std::vector<StaticState> generateLarvestaEgg(u64 seed) const;
    std::vector<StaticState> generateHiddenGrotto(u64 seed) const;
};

#endif // GENERATOR5_HPP
