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

#ifndef WILDGENERATOR8_HPP
#define WILDGENERATOR8_HPP

#include <Core/Gen8/EncounterArea8.hpp>
#include <Core/Gen8/Profile8.hpp>
#include <Core/Parents/Filters/StateFilter.hpp>
#include <Core/Parents/Generators/WildGenerator.hpp>

class WildGeneratorState;

/**
 * @brief Wild encounter generator for Gen8
 */
class WildGenerator8 : public WildGenerator<EncounterArea8, Profile8, WildStateFilter>
{
public:
    /**
     * @brief Construct a new WildGenerator8 object
     *
     * @param initialAdvances Initial number of advances
     * @param maxAdvances Maximum number of advances
     * @param delay Number of advances to offset
     * @param lead Encounter lead
     * @param area Wild pokemon info
     * @param profile Profile Information
     * @param filter State filter
     */
    WildGenerator8(u32 initialAdvances, u32 maxAdvances, u32 delay, Lead lead, const EncounterArea8 &area, const Profile8 &profile,
                   const WildStateFilter &filter);

    /**
     * @brief Generates states
     *
     * @param seed0 Upper half of PRNG state
     * @param seed1 Lower half of PRNG state
     *
     * @return Vector of computed states
     */
    std::vector<WildGeneratorState> generate(u64 seed0, u64 seed1) const;
};

#endif // WILDGENERATOR8_HPP
