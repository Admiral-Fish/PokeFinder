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

#ifndef EGGGENERATOR5_HPP
#define EGGGENERATOR5_HPP

#include <Core/Gen5/Profile5.hpp>
#include <Core/Parents/Filters/StateFilter.hpp>
#include <Core/Parents/Generators/EggGenerator.hpp>

class EggState5;
class PersonalInfo;

/**
 * @brief Egg generator for Gen 5
 */
class EggGenerator5 : public EggGenerator<Profile5, StateFilter>
{
public:
    /**
     * @brief Construct a new EggGenerator5 object
     *
     * @param initialAdvances Initial number of advances
     * @param maxAdvances Maximum number of advances
     * @param delay Number of advances to offset
     * @param daycare Daycare parent information
     * @param profile Profile Information
     * @param filter State filter
     */
    EggGenerator5(u32 initialAdvances, u32 maxAdvances, u32 delay, const Daycare &daycare, const Profile5 &profile,
                  const StateFilter &filter);

    /**
     * @brief Generates states
     *
     * @param seed Starting PRNG state
     *
     * @return Vector of computed states
     */
    std::vector<EggState5> generate(u64 seed) const;

private:
    bool ditto;
    u8 everstone;
    u8 parentAbility;
    u8 poweritem;
    u8 rolls;

    std::vector<EggState5> generateBW(u64 seed) const;
    std::vector<EggState5> generateBW2(u64 seed) const;
    EggState5 generateBW2Egg(u64 seed, const PersonalInfo **info) const;
};

#endif // EGGGENERATOR5_HPP
