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

#ifndef STATE8_HPP
#define STATE8_HPP

#include <Core/Parents/States/State.hpp>

/**
 * @brief State class for Gen8 encounters
 */
class State8 : public GeneratorState
{
public:
    /**
     * @brief Construct a new State8 object
     *
     * @param advances Advances of the state
     * @param ec Pokemon EC
     * @param pid Pokemon PID
     * @param ivs Pokemon IVs
     * @param ability Pokemon ability
     * @param gender Pokemon gender
     * @param level Pokemon level
     * @param nature Pokemon nature
     * @param shiny Pokemon shininess
     * @param height Pokemon height
     * @param weight Pokemon weight
     * @param specie Pokemon specie
     * @param info Pokemon personal information
     */
    State8(u32 advances, u32 ec, u32 pid, const std::array<u8, 6> &ivs, u8 ability, u8 gender, u8 level, u8 nature, u8 shiny, u8 height,
           u8 weight, const PersonalInfo *info) :
        GeneratorState(advances, ec, pid, ivs, ability, gender, level, nature, shiny, info), height(height), weight(weight)
    {
    }

    /**
     * @brief Returns the height of the pokemon
     *
     * @return Pokemon height
     */
    u8 getHeight() const
    {
        return height;
    }

    /**
     * @brief Returns the weight of the pokemon
     *
     * @return Pokemon weight
     */
    u8 getWeight() const
    {
        return weight;
    }

private:
    u8 height;
    u8 weight;
};

#endif // STATE8_HPP
