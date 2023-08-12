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

#ifndef POKESPOTGENERATOR_HPP
#define POKESPOTGENERATOR_HPP

#include <Core/Gen3/Profile3.hpp>
#include <Core/Parents/Filters/StateFilter.hpp>
#include <Core/Parents/Generators/Generator.hpp>

class EncounterArea;
class PokeSpotState;

/**
 * @brief Poke Spot encounter generator for Gen3
 */
class PokeSpotGenerator : public Generator<Profile3, WildStateFilter>
{
public:
    /**
     * @brief Construct a new PokeSpotGenerator object
     *
     * @param initialAdvances Initial number of advances
     * @param maxAdvances Maximum number of advances
     * @param delay Number of advances to offset
     * @param initialAdvancesEncounter Initial number of pickup advances
     * @param maxAdvancesEncounter Maximum number of pickup advances
     * @param delayEncounter Number of pickup advances to offset
     * @param profile Profile Information
     * @param filter State filter
     */
    PokeSpotGenerator(u32 initialAdvances, u32 maxAdvances, u32 delay, u32 initialAdvancesEncounter, u32 maxAdvancesEncounter,
                      u32 delayEncounter, const Profile3 &profile, const WildStateFilter &filter);

    /**
     * @brief Generates states
     *
     * @param seedFood Starting PRNG food state
     * @param seedEncounter Starting PRNG encounter state
     * @param encounterArea Wild pokemon info
     *
     * @return Vector of computed states
     */
    std::vector<PokeSpotState> generate(u32 seedFood, u32 seedEncounter, const EncounterArea &encounterArea) const;

private:
    u32 delayEncounter;
    u32 initialAdvancesEncounter;
    u32 maxAdvancesEncounter;

    /**
     * @brief Generates states for encountering the pokemon
     *
     * @param seed Starting PRNG state
     * @param food Vector of food states
     * @param encounterArea Wild pokemon info
     *
     * @return Vector of computed states
     */
    std::vector<PokeSpotState> generateEncounter(u32 seed, const std::vector<PokeSpotState> &food,
                                                 const EncounterArea &encounterArea) const;

    /**
     * @brief Generates states for the pokemon eating the food at the Poke Spot
     *
     * @param seed Starting PRNG state
     * @param encounterArea Wild pokemon info
     *
     * @return Vector of computed food states
     */
    std::vector<PokeSpotState> generateFood(u32 seed, const EncounterArea &encounterArea) const;
};

#endif // POKESPOTGENERATOR_HPP
