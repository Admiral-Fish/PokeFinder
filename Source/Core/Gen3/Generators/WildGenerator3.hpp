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

#ifndef WILDGENERATOR3_HPP
#define WILDGENERATOR3_HPP

#include <Core/Gen3/EncounterArea3.hpp>
#include <Core/Gen3/Profile3.hpp>
#include <Core/Parents/Filters/StateFilter.hpp>
#include <Core/Parents/Generators/WildGenerator.hpp>

/**
 * @brief Wild encounter generator for Gen3
 */
class WildGenerator3 : public WildGenerator<EncounterArea3, Profile3, WildStateFilter>
{
public:
    /**
     * @brief Construct a new WildGenerator3 object
     *
     * @param initialAdvances Initial number of advances
     * @param maxAdvances Maximum number of advances
     * @param offset Number of advances to offset
     * @param method Encounter method
     * @param lead Encounter lead
     * @param feebasTile Whether Feebas tiles are active
     * @param area Wild pokemon info
     * @param profile Profile Information
     * @param filter State filter
     */
    WildGenerator3(u32 initialAdvances, u32 maxAdvances, u32 offset, Method method, Lead lead, bool feebasTile, const EncounterArea3 &area,
                   const Profile3 &profile, const WildStateFilter &filter);

    /**
     * @brief Generates states
     *
     * @param seed Starting PRNG state
     *
     * @return Vector of computed states
     */
    std::vector<WildGeneratorState3> generate(u32 seed) const;

private:
    bool feebasTile;
};

#endif // WILDGENERATOR3_HPP
