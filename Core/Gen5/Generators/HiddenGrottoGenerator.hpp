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

#ifndef HIDDENGROTTOGENERATOR_HPP
#define HIDDENGROTTOGENERATOR_HPP

#include <Core/Gen5/Filters/HiddenGrottoFilter.hpp>
#include <Core/Gen5/HiddenGrottoArea.hpp>
#include <Core/Gen5/Profile5.hpp>
#include <Core/Parents/Filters/StateFilter.hpp>
#include <Core/Parents/Generators/Generator.hpp>

class HiddenGrottoState;
class State5;
enum class Lead : u8;

/**
 * @brief Hidden grotto slot generator for Gen 5
 */
class HiddenGrottoSlotGenerator : public Generator<Profile5, HiddenGrottoFilter>
{
public:
    /**
     * @brief Construct a new HiddenGrottoSlotGenerator object
     *
     * @param initialAdvances Initial number of advances
     * @param maxAdvances Maximum number of advances
     * @param offset Number of advances to offset
     * @param powerLevel Hidden grotto encounter rate
     * @param encounterArea Hidden grotto information
     * @param profile Profile Information
     * @param filter State filter
     */
    HiddenGrottoSlotGenerator(u32 initialAdvances, u32 maxAdvances, u32 offset, u8 powerLevel, const HiddenGrottoArea &encounterArea,
                              const Profile5 &profile, const HiddenGrottoFilter &filter);

    /**
     * @brief Generates states
     *
     * @param seed Starting PRNG state
     *
     * @return Vector of computed states
     */
    std::vector<HiddenGrottoState> generate(u64 seed) const;

private:
    HiddenGrottoArea encounterArea;
    u8 powerLevel;
};

/**
 * @brief Hidden grotto pokemon generator for Gen 5
 */
class HiddenGrottoGenerator : public Generator<Profile5, StateFilter>
{
public:
    /**
     * @brief Construct a new HiddenGrottoGenerator object
     *
     * @param initialAdvances Initial number of advances
     * @param maxAdvances Maximum number of advances
     * @param offset Number of advances to offset
     * @param powerLevel Hidden grotto encounter rate
     * @param encounterArea Hidden grotto information
     * @param profile Profile Information
     * @param filter State filter
     */
    HiddenGrottoGenerator(u32 initialAdvances, u32 maxAdvances, u32 offset, Lead lead, u8 gender, const HiddenGrottoSlot &slot,
                          const Profile5 &profile, const StateFilter &filter);

    /**
     * @brief Generates states for the \p encounterArea
     *
     * @param seed Starting PRNG state
     * @param initialAdvances Initial number of IV advances
     * @param maxAdvances Maximum number of IV advances
     *
     * @return Vector of computed states
     */
    std::vector<State5> generate(u64 seed, u32 initialAdvances, u32 maxAdvances) const;

    /**
     * @brief Generates states for the \p encounterArea
     *
     * @param seed Starting PRNG state
     * @param iv Vector of IV advances and IVs
     *
     * @return Vector of computed states
     */
    std::vector<State5> generate(u64 seed, const std::vector<std::pair<u32, std::array<u8, 6>>> &ivs) const;

private:
    HiddenGrottoSlot slot;
    Lead lead;
    u8 gender;
};

#endif // HIDDENGROTTOGENERATOR_HPP
