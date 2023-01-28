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

#ifndef WILDGENERATOR4_HPP
#define WILDGENERATOR4_HPP

#include <Core/Gen4/Filters/StateFilter4.hpp>
#include <Core/Gen4/Profile4.hpp>
#include <Core/Parents/Generators/WildGenerator.hpp>

class EncounterArea4;

/**
 * @brief Wild encounter generator for Gen4
 */
class WildGenerator4 : public WildGenerator<Profile4, WildStateFilter4>
{
public:
    /**
     * @brief Construct a new WildGenerator4 object
     *
     * @param initialAdvances Initial number of advances
     * @param maxAdvances Maximum number of advances
     * @param delay Number of advances to offset
     * @param encounter Encounter type
     * @param method Encounter method
     * @param lead Encounter lead
     * @param shiny Whether Poke Radar is forced shiny
     * @param profile Profile Information
     * @param filter State filter
     */
    WildGenerator4(u32 initialAdvances, u32 maxAdvances, u32 delay, Method method, Encounter encounter, Lead lead, bool shiny,
                   const Profile4 &profile, const WildStateFilter4 &filter);

    /**
     * @brief Generates states for the \p encounterArea
     *
     * @param seed Starting PRNG state
     * @param encounterArea Wild pokemon info
     * @param index Pokeradar slot index
     *
     * @return Vector of computed states
     */
    std::vector<WildGeneratorState4> generate(u32 seed, const EncounterArea4 &encounterArea, u8 index) const;

private:
    bool shiny;

    /**
     * @brief Generates states for the \p encounterArea via Method J
     *
     * @param seed Starting PRNG state
     * @param encounterArea Wild pokemon info
     *
     * @return Vector of computed states
     */
    std::vector<WildGeneratorState4> generateMethodJ(u32 seed, const EncounterArea4 &encounterArea) const;

    /**
     * @brief Generates states for the \p encounterArea via Method K
     *
     * @param seed Starting PRNG state
     * @param encounterArea Wild pokemon info
     *
     * @return Vector of computed states
     */
    std::vector<WildGeneratorState4> generateMethodK(u32 seed, const EncounterArea4 &encounterArea) const;

    /**
     * @brief Generates states for the \p encounterArea via Poke Radar
     *
     * @param seed Starting PRNG state
     * @param encounterArea Wild pokemon info
     * @param index Pokeradar slot index
     *
     * @return Vector of computed states
     */
    std::vector<WildGeneratorState4> generatePokeRadar(u32 seed, const EncounterArea4 &encounterArea, u8 index) const;

    /**
     * @brief Generates states for the \p encounterArea via Poke Radar chained shiny
     *
     * @param seed Starting PRNG state
     * @param encounterArea Wild pokemon info
     * @param index Pokeradar slot index
     *
     * @return Vector of computed states
     */
    std::vector<WildGeneratorState4> generatePokeRadarShiny(u32 seed, const EncounterArea4 &encounterArea, u8 index) const;
};

#endif // WILDGENERATOR4_HPP
