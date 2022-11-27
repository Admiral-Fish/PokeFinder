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

#ifndef WILDGENERATOR3_HPP
#define WILDGENERATOR3_HPP

#include <Core/Gen3/Filters/StateFilter3.hpp>
#include <Core/Parents/Generators/WildGenerator.hpp>

class EncounterArea3;

/**
 * @brief Wild encounter generator for Gen3
 */
class WildGenerator3 : public WildGenerator<WildStateFilter3>
{
public:
    /**
     * @brief Construct a new WildGenerator3 object
     *
     * @param initialAdvances Initial number of advances
     * @param maxAdvances Maximum number of advances
     * @param offset Number of advances to offset
     * @param tid Trainer ID
     * @param sid Secret ID
     * @param version Game version
     * @param encounter Encounter type
     * @param method Encounter method
     * @param lead Encounter lead
     * @param filter State filter
     */
    WildGenerator3(u32 initialAdvances, u32 maxAdvances, u32 offset, u16 tid, u16 sid, Game version, Method method, Encounter encounter,
                   Lead lead, const WildStateFilter3 &filter);

    /**
     * @brief Generates states for the \p encounterArea
     *
     * @param seed Starting PRNG state
     * @param encounterArea Wild pokemon info
     *
     * @return Vector of computed states
     */
    std::vector<WildGeneratorState> generate(u32 seed, const EncounterArea3 &encounterArea) const;
};

#endif // WILDGENERATOR3_HPP
