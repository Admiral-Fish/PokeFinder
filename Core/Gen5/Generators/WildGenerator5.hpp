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

#ifndef WILDGENERATOR5_HPP
#define WILDGENERATOR5_HPP

#include <Core/Gen5/EncounterArea5.hpp>
#include <Core/Gen5/Profile5.hpp>
#include <Core/Parents/Filters/StateFilter.hpp>
#include <Core/Parents/Generators/WildGenerator.hpp>

class WildState5;

class WildGenerator5 : public WildGenerator<EncounterArea5, Profile5, WildStateFilter>
{
public:
    /**
     * @brief Construct a new WildGenerator5 object
     *
     * @param initialAdvances Initial number of advances
     * @param maxAdvances Maximum number of advances
     * @param offset Number of advances to offset
     * @param method Encounter method
     * @param lead Encounter lead
     * @param luckyPower Lucky power level
     * @param area Wild pokemon info
     * @param profile Profile Information
     * @param filter State filter
     */
    WildGenerator5(u32 initialAdvances, u32 maxAdvances, u32 offset, Method method, Lead lead, u8 luckyPower, const EncounterArea5 &area,
                   const Profile5 &profile, const WildStateFilter &filter);

    /**
     * @brief Generates states for the \p encounterArea
     *
     * @param seed Starting PRNG state
     * @param initialAdvances Initial number of IV advances
     * @param maxAdvances Maximum number of IV advances
     *
     * @return Vector of computed states
     */
    std::vector<WildState5> generate(u64 seed, u32 initialAdvances, u32 maxAdvances) const;

    /**
     * @brief Generates states for the \p encounterArea
     *
     * @param seed Starting PRNG state
     * @param iv Vector of IV advances and IVs
     *
     * @return Vector of computed states
     */
    std::vector<WildState5> generate(u64 seed, const std::vector<std::pair<u32, std::array<u8, 6>>> &ivs) const;

private:
    u8 luckyPower;
};

#endif // WILDGENERATOR5_HPP
