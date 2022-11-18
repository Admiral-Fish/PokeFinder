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

#ifndef UNDERGROUNDAREA_HPP
#define UNDERGROUNDAREA_HPP

#include <Core/Global.hpp>
#include <Core/RNG/RNGList.hpp>
#include <array>
#include <string>
#include <vector>

class PersonalInfo;
class Xorshift;

struct TypeRate
{
    u16 rate;
    u8 type;
};

struct TypeSize
{
    u16 value;
    u8 size;
    u8 type;
};

struct Pokemon
{
    const PersonalInfo *info;
    u16 rate;
    u16 specie;
    u8 size;
};

struct SpecialPokemon
{
    const PersonalInfo *info;
    u16 specie;
    u16 rate;
};

class UndergroundArea
{
public:
    UndergroundArea(u8 location, u8 min, u8 max, const std::vector<Pokemon> &pokemon, const std::vector<SpecialPokemon> &specialPokemon,
                    const std::array<u8, 18> &typeRates, const std::vector<TypeSize> &typeSizes);

    u8 getLocation() const;

    u8 getMax() const;

    u8 getMin() const;

    std::pair<const PersonalInfo *, u16> getPokemon(RNGList<u32, Xorshift, 256> &rngList, const TypeSize &type, u8 story,
                                                    bool diglett) const;

    std::array<TypeSize, 10> getSlots(RNGList<u32, Xorshift, 256> &rngList, u8 count) const;

    std::pair<const PersonalInfo *, u16> getSpecialPokemon(RNGList<u32, Xorshift, 256> &rngList) const;

    std::vector<u16> getSpecies() const;

    std::vector<std::string> getSpecieNames() const;

private:
    std::vector<Pokemon> pokemon;
    std::vector<SpecialPokemon> specialPokemon;
    std::vector<TypeRate> typeRates;
    std::vector<TypeSize> typeSizes;
    u16 specialSum;
    u16 typeSum;
    u8 location;
    u8 max;
    u8 min;
};

#endif // UNDERGROUNDAREA_HPP
