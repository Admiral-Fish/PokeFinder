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

#ifndef EGGGENERATOR3_HPP
#define EGGGENERATOR3_HPP

#include <Core/Gen3/Profile3.hpp>
#include <Core/Parents/Filters/StateFilter.hpp>
#include <Core/Parents/Generators/EggGenerator.hpp>

class EggState3;

/**
 * @brief Egg encounter generator for Gen3
 */
class EggGenerator3 : public EggGenerator<Profile3, StateFilter>
{
public:
    /**
     * @brief Construct a new EggGenerator3 object
     *
     * @param initialAdvances Initial number of advances
     * @param maxAdvances Maximum number of advances
     * @param delay Number of advances to offset
     * @param initialAdvancesPickup Initial number of pickup advances
     * @param maxAdvancesPickup Maximum number of pickup advances
     * @param delayPickup Number of pickup advances to offset
     * @param calibration Emerald PID calibration value
     * @param minRedraw Minimum number of redraws
     * @param maxRedraw Maximum number of redraws
     * @param method Encounter method
     * @param compatability Parent compatability
     * @param daycare Daycare parent information
     * @param profile Profile Information
     * @param filter State filter
     */
    EggGenerator3(u32 initialAdvances, u32 maxAdvances, u32 delay, u32 initialAdvancesPickup, u32 maxAdvancesPickup, u32 delayPickup,
                  u8 calibration, u8 minRedraw, u8 maxRedraw, Method method, u8 compatability, const Daycare &daycare,
                  const Profile3 &profile, const StateFilter &filter);

    /**
     * @brief Generates states
     *
     * @param seedHeld Starting PRNG held state
     * @param seedPickup Starting PRNG pickup state
     *
     * @return Vector of computed states
     */
    std::vector<EggState3> generate(u32 seedHeld = 0, u32 seedPickup = 0) const;

private:
    u32 delayPickup;
    u32 initialAdvancesPickup;
    u32 maxAdvancesPickup;
    u8 calibration;
    u8 inh;
    u8 iv1;
    u8 iv2;
    u8 maxRedraw;
    u8 minRedraw;

    /**
     * @brief Generates states for when the daycare man has the egg
     *
     * @return Vector of computed held states
     */
    std::vector<EggState3> generateEmeraldHeld() const;

    /**
     * @brief Generates states for picking up the egg from the daycare man
     *
     * @param held Vector of held states
     *
     * @return Vector of computed pickup states
     */
    std::vector<EggState3> generateEmeraldPickup(const std::vector<EggState3> &held) const;

    /**
     * @brief Generates states for when the daycare man has the egg
     *
     * @param seed Starting PRNG state
     *
     * @return Vector of computed held states
     */
    std::vector<EggState3> generateRSFRLGHeld(u32 seed) const;

    /**
     * @brief Generates states for picking up the egg from the daycare man
     *
     * @param seed Starting PRNG state
     * @param held Vector of held states
     *
     * @return Vector of computed pickup states
     */
    std::vector<EggState3> generateRSFRLGPickup(u32 seed, const std::vector<EggState3> &held) const;
};

#endif // EGGGENERATOR3_HPP
