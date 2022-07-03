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

#ifndef WILDSTATE3_HPP
#define WILDSTATE3_HPP

#include <Core/Parents/States/WildState.hpp>

/**
 * @brief State class for Gen3 wild searcher encounters
 */
class WildSearcherState3 : public WildSearcherState
{
public:
    /**
     * @brief Construct a new WildSearcherState3 object
     *
     * @param seed Seed of the state
     * @param pid Pokemon PID
     * @param ivs Pokemon IVs
     * @param ability Pokemon ability
     * @param gender Pokemon gender
     * @param level Pokemon level
     * @param nature Pokemon nature
     * @param shiny Pokemon shininess
     * @param encounterSlot Pokemon encounter slot
     * @param specie Pokemon specie
     * @param info Pokemon information
     */
    WildSearcherState3(u32 seed, u32 pid, const std::array<u8, 6> &ivs, u8 ability, u8 gender, u8 level, u8 nature, u8 shiny,
                       u8 encounterSlot, u16 specie, const PersonalInfo *info) :
        WildSearcherState(seed, pid, ivs, ability, gender, level, nature, shiny, encounterSlot, 0, specie, info)
    {
    }

    /**
     * @brief Sets the state seed
     *
     * @param seed State seed
     */
    void setSeed(u32 seed)
    {
        this->seed = seed;
    }
};

#endif // WILDSTATE3_HPP
