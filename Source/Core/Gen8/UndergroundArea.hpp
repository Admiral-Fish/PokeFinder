/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2023 by Admiral_Fish, bumba, and EzPzStreamz
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
    u16 rate;
    u16 specie;
    u8 size;
    u8 type[2];
};

struct SpecialPokemon
{
    u16 rate;
    u16 specie;
};

/**
 * @brief Contains information about the encounters for an underground area.
 * Underground area does not work on the model of set encounter slot numbers like most other games. This class also provides the
 * functionality to dynamically determine the encountered pokemon based on the rates of types and pokemon.
 */
class UndergroundArea
{
public:
    /**
     * @brief Construct a new UndergroundArea object
     *
     * @param location
     * @param min Minimum number of pokemon spawned
     * @param max Maximum number of pokemon spawned
     * @param pokemon List of normal pokemon
     * @param specialPokemon List of rare pokemon
     * @param typeRates List of rates per pokemon type
     * @param typeSizes List of types with their associated sizes
     */
    UndergroundArea(u8 location, u8 min, u8 max, const std::vector<Pokemon> &pokemon, const std::vector<SpecialPokemon> &specialPokemon,
                    const std::array<u8, 18> &typeRates, const std::vector<TypeSize> &typeSizes);

    /**
     * @brief Returns the location of the area
     *
     * @return Location number
     */
    u8 getLocation() const
    {
        return location;
    }

    /**
     * @brief Returns the maximum number of pokemon spawned
     *
     * @return Maximum spawn
     */
    u8 getMax() const
    {
        return max;
    }

    /**
     * @brief Returns the minimum number of pokemon spawned
     *
     * @return Minimum spawn
     */
    u8 getMin() const
    {
        return min;
    }

    /**
     * @brief Returns the pokemon to create based on the \p type
     * Filters from the available pokemon ones that match the necessary type and size. This filtered list is then randomly selected from
     * based upon the pokemon encounter rates.
     *
     * @param rngList RNG object
     * @param type Pokemon type and associated size
     *
     * @return Pokemon to create
     */
    u16 getPokemon(RNGList<u32, Xorshift, 256> &rngList, const TypeSize &type) const;

    /**
     * @brief Returns the list of types and associated sizes to help determine which pokemon to create
     * A type is randomly selected from the available pokemon. Of these matches a size is randomly selected and paired with the type.
     *
     * @param rngList RNG object
     * @param count Number of pokemon to spawn
     *
     * @return List of type/sizes
     */
    std::array<TypeSize, 10> getSlots(RNGList<u32, Xorshift, 256> &rngList, u8 count) const;

    /**
     * @brief Returns the rare pokemon to create based
     *
     * @param rngList RNG object
     *
     * @return Pokemon to create
     */
    u16 getSpecialPokemon(RNGList<u32, Xorshift, 256> &rngList) const;

    /**
     * @brief Return the species numbers of the area
     *
     * @return Vector of pokemon species
     */
    std::vector<u16> getSpecies() const;

    /**
     * @brief Return vector of names of all pokemon slots
     *
     * @return Vector of pokemon name
     */
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
