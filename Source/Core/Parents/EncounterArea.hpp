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

#ifndef ENCOUNTERAREA_HPP
#define ENCOUNTERAREA_HPP

#include <Core/Global.hpp>
#include <string>
#include <vector>

class Slot;
enum class Encounter : u8;

/**
 * @brief Contains information about the encounters for an area. This includes location, rate, and the slots.
 */
class EncounterArea
{
public:
    /**
     * @brief Construct a new Encounter Area object
     *
     * @param location Location number
     * @param rate Encounter rate of the area
     * @param encounter Encounter type of the area
     * @param pokemon Available pokemon of the area
     */
    EncounterArea(u8 location, u8 rate, Encounter encounter, const std::vector<Slot> &pokemon);

    /**
     * @brief Calculates the level of a pokemon that has a range
     *
     * @param index Pokemon index to use
     * @param prng Random value
     *
     * @return Level of the encounter
     */
    u8 calcLevel(u8 index, u16 prng) const;

    /**
     * @brief Calculates the level of a pokemon that has no range
     *
     * @param index Pokemon index to use
     *
     * @return Level of the encounter
     */
    u8 calcLevel(u8 index) const;

    /**
     * @brief Returns the type of encounter of the area
     *
     * @return Encounter type
     */
    Encounter getEncounter() const;

    /**
     * @brief Returns the location of the area
     *
     * @return Location number
     */
    u8 getLocation() const;

    /**
     * @brief Return the encounter rate of the area
     *
     * @return Encounter rate
     */
    u8 getRate() const;

    /**
     * @brief Return the pokemon of the area
     *
     * @return Vector of pokemon
     */
    std::vector<Slot> getPokemon() const;

    /**
     * @brief Return the species numbers of unique pokemon of the area
     *
     * @return Vector of pokemon species
     */
    std::vector<u16> getUniqueSpecies() const;

    /**
     * @brief Return vector of true/false which indicate slots that match the \p specie
     *
     * @param specie Species number
     *
     * @return Vector of true/false for matching slots
     */
    std::vector<bool> getSlots(u16 specie) const;

    /**
     * @brief Calculates the level range of a \p specie across all possible slots it can be encountered
     *
     * @param specie Species number
     *
     * @return Pair of level range
     */
    std::pair<u8, u8> getLevelRange(u16 specie) const;

    /**
     * @brief Return vector of names of all pokemon slots
     *
     * @return Vector of pokemon name
     */
    std::vector<std::string> getSpecieNames() const;

protected:
    std::vector<Slot> pokemon;
    Encounter encounter;
    u8 location;
    u8 rate;
};

#endif // ENCOUNTERAREA_HPP
