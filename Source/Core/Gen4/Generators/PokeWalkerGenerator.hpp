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

#ifndef POKEWALKERGENERATOR4_HPP
#define POKEWALKERGENERATOR4_HPP

#include <Core/Parents/States/PokeWalkerState.hpp>
#include <Core/Parents/Generators/Generator.hpp>

class PokeWalkerGenerator : public Generator
{
public:
    PokeWalkerGenerator() = default;
    PokeWalkerGenerator(u32 initialAdvances, u32 maxAdvances, u16 tid, u16 sid, u8 genderRatio, Method method, const StateFilter &filter, u8 gender);
    std::vector<PokeWalkerState> generate(u32 seed) const;

private:
    std::vector<PokeWalkerState> generatePokeWalkerPID(u32 seed) const;
    std::vector<PokeWalkerState> generatePokeWalkerIVs(u32 seed) const;
    u8 gender;
};

#endif // POKEWALKERGENERATOR4_HPP
