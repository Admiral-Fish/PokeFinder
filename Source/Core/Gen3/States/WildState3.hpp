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

#ifndef WILDSTATE3_HPP
#define WILDSTATE3_HPP

#include <Core/Parents/States/WildState.hpp>

/**
 * @brief State class for Gen3 wild generator encounters
 */
class WildGeneratorState3 : public WildGeneratorState
{
public:
    /**
     * @brief Construct a new WildGeneratorState3 object
     *
     * @param pidRollCount PID roll count
     * @param advances Advances of the state
     * @param pid Pokemon PID
     * @param ivs Pokemon IVs
     * @param ability Pokemon ability
     * @param gender Pokemon gender
     * @param level Pokemon level
     * @param nature Pokemon nature
     * @param shiny Pokemon shininess
     * @param encounterSlot Pokemon encounter slot
     * @param item Pokemon item
     * @param specie Pokemon specie
     * @param form Pokemon form
     * @param info Pokemon information
     */
    WildGeneratorState3(u32 pidRollCount, u32 advances, u32 pid, const std::array<u8, 6> &ivs, u8 ability, u8 gender, u8 level, u8 nature,
                        u8 shiny, u8 encounterSlot, u16 item, u16 specie, u8 form, const PersonalInfo *info) :
        WildGeneratorState(advances, pid, ivs, ability, gender, level, nature, shiny, encounterSlot, item, specie, form, info),
        pidRolls(pidRolls)
    {
    }

    /**
     * @brief Returns the pid roll count
     *
     * @return PID roll count
     */
    u32 getPidRollCount() const
    {
        return pidRollCount;
    }

private:
    u32 pidRollCount;
};

/**
 * @brief State class for Gen3 wild searcher encounters
 */
class WildSearcherState3 : public WildSearcherState
{
public:
    /**
     * @brief Construct a new WildSearcherState3 object
     *
     * @param pidRollCount PID roll count
     * @param seed Seed of the state
     * @param pid Pokemon PID
     * @param ivs Pokemon IVs
     * @param ability Pokemon ability
     * @param gender Pokemon gender
     * @param level Pokemon level
     * @param nature Pokemon nature
     * @param shiny Pokemon shininess
     * @param encounterSlot Pokemon encounter slot
     * @param item Pokemon item
     * @param specie Pokemon specie
     * @param form Pokemon form
     * @param info Pokemon information
     */
    WildSearcherState3(u32 pidRollCount, u32 seed, u32 pid, const std::array<u8, 6> &ivs, u8 ability, u8 gender, u8 level, u8 nature,
                       u8 shiny, u8 encounterSlot, u16 item, u16 specie, u8 form, const PersonalInfo *info) :
        WildSearcherState(seed, pid, ivs, ability, gender, level, nature, shiny, encounterSlot, item, specie, form, info),
        pidRollCount(pidRollCount)
    {
    }

    /**
     * @brief Returns the pid roll count
     *
     * @return PID roll count
     */
    u32 getPidRollCount() const
    {
        return pidRollCount;
    }

private:
    u32 pidRollCount;
};

#endif // WILDSTATE3_HPP
