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

#ifndef WILDGENERATOR4_HPP
#define WILDGENERATOR4_HPP

#include <Core/Gen4/EncounterArea4.hpp>
#include <Core/Gen4/Profile4.hpp>
#include <Core/Parents/Filters/StateFilter.hpp>
#include <Core/Parents/Generators/WildGenerator.hpp>

class WildGeneratorState4;

/**
 * @brief Wild encounter generator for Gen4
 */
class WildGenerator4 : public WildGenerator<EncounterArea4, Profile4, WildStateFilter>
{
public:
    /**
     * @brief Construct a new WildGenerator4 object
     *
     * @param initialAdvances Initial number of advances
     * @param maxAdvances Maximum number of advances
     * @param delay Number of advances to offset
     * @param method Encounter method
     * @param lead Encounter lead
     * @param shiny Whether Poke Radar is forced shiny
     * @param unownRadio Whether the radio station gives undiscovered Unowns more frequently
     * @param happiness Encounter rate modifier for fishing in HGSS
     * @param area Wild pokemon info
     * @param profile Profile Information
     * @param filter State filter
     */
    WildGenerator4(u32 initialAdvances, u32 maxAdvances, u32 delay, Method method, Lead lead, bool shiny, bool unownRadio, u8 happiness,
                   const EncounterArea4 &area, const Profile4 &profile, const WildStateFilter &filter);

    /**
     * @brief Generates states
     *
     * @param seed Starting PRNG state
     * @param index Pokeradar slot index
     *
     * @return Vector of computed states
     */
    std::vector<WildGeneratorState4> generate(u32 seed, u8 index) const;

private:
    bool shiny;
    bool unownRadio;
    u8 happiness;

    /**
     * @brief Generates states via Method J
     *
     * @param seed Starting PRNG state
     *
     * @return Vector of computed states
     */
    std::vector<WildGeneratorState4> generateMethodJ(u32 seed) const;

    /**
     * @brief Generates states via Method K
     *
     * @param seed Starting PRNG state
     *
     * @return Vector of computed states
     */
    std::vector<WildGeneratorState4> generateMethodK(u32 seed) const;

    /**
     * @brief Generates states via Poke Radar
     *
     * @param seed Starting PRNG state
     * @param index Pokeradar slot index
     *
     * @return Vector of computed states
     */
    std::vector<WildGeneratorState4> generatePokeRadar(u32 seed, u8 index) const;

    /**
     * @brief Generates states via Poke Radar chained shiny
     *
     * @param seed Starting PRNG state
     * @param index Pokeradar slot index
     *
     * @return Vector of computed states
     */
    std::vector<WildGeneratorState4> generatePokeRadarShiny(u32 seed, u8 index) const;
};

#endif // WILDGENERATOR4_HPP
