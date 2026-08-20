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

#ifndef POKERADARGENERATOR_HPP
#define POKERADARGENERATOR_HPP

#include <Core/Gen4/States/PokeRadarState.hpp>
#include <Core/Global.hpp>
#include <Core/RNG/LCRNG.hpp>
#include <array>
#include <utility>
#include <vector>

enum class PokeRadarChainType : u8
{
    Regular,
    Strong,
    RegularShiny,
    StrongShiny
};

class PokeRadarGenerator
{
public:
    PokeRadarGenerator(u32 initialAdvances, u32 maxAdvances, u16 chainCount, PokeRadarChainType chainType, PokeRadarResult result,
                       const std::array<bool, 81> &grass);

    static std::pair<u8, u8> getSkips(u32 seed, u32 advances);

    std::vector<PokeRadarState> generate(u32 seed) const;
    PokeRadarState generate(u32 seed, u32 advances, u32 patchAdvances) const;
    PokeRadarState generatePrevious(u32 seed, u32 advances) const;
    u32 getAdvanceConsumption(u32 seed, u32 advances, PokeRadarResult result) const;
    u32 getPostBattleAdvanceConsumption(const std::array<PokeRadarPatch, 4> &patches) const;

private:
    PokeRadarPatch buildPatch(u8 ring, u8 rand) const;
    std::array<PokeRadarPatch, 4> generatePatches(PokeRNG &rng) const;
    bool isGrass(u8 x, u8 y) const;

    u32 initialAdvances;
    u32 maxAdvances;
    u16 chainCount;
    PokeRadarChainType chainType;
    PokeRadarResult result;
    std::array<bool, 81> grass;
};

#endif // POKERADARGENERATOR_HPP
