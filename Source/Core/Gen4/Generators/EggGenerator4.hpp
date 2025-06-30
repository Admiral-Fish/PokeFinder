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

#ifndef EGGGENERATOR4_HPP
#define EGGGENERATOR4_HPP

#include <Core/Gen4/Profile4.hpp>
#include <Core/Parents/Filters/StateFilter.hpp>
#include <Core/Parents/Generators/EggGenerator.hpp>

class EggGeneratorState4;

/**
 * @brief Egg encounter generator for Gen4
 */
class EggGenerator4 : public EggGenerator<Profile4, StateFilter>
{
public:
    /**
     * @brief Construct a new EggGenerator4 object
     *
     * @param initialAdvances Initial number of advances
     * @param maxAdvances Maximum number of advances
     * @param offset Number of advances to offset
     * @param initialAdvancesPickup Initial number of pickup advances
     * @param maxAdvancesPickup Maximum number of pickup advances
     * @param offsetPickup Number of pickup advances to offset
     * @param daycare Daycare parent information
     * @param profile Profile Information
     * @param filter State filter
     */
    EggGenerator4(u32 initialAdvances, u32 maxAdvances, u32 offset, u32 initialAdvancesPickup, u32 maxAdvancesPickup, u32 offsetPickup,
                  const Daycare &daycare, const Profile4 &profile, const StateFilter &filter);

    /**
     * @brief Generates states
     *
     * @param seedHeld Starting PRNG held state
     * @param seedPickup Starting PRNG pickup state
     *
     * @return Vector of computed states
     */
    std::vector<EggGeneratorState4> generate(u32 seedHeld, u32 seedPickup) const;

private:
    u32 initialAdvancesPickup;
    u32 maxAdvancesPickup;
    u32 offsetPickup;

    /**
     * @brief Generates states for when the daycare man has the egg
     *
     * @param seed Starting PRNG state
     *
     * @return Vector of computed held states
     */
    std::vector<EggGeneratorState4> generateHeld(u32 seed) const;

    /**
     * @brief Generates states for picking up the egg from the daycare man
     *
     * @param seed Starting PRNG state
     * @param held Vector of held states
     *
     * @return Vector of computed pickup states
     */
    std::vector<EggGeneratorState4> generatePickup(u32 seed, const std::vector<EggGeneratorState4> &held) const;
};

#endif // EGGGENERATOR4_HPP
