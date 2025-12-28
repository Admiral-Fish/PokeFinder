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

#ifndef WILDSTATE8_H
#define WILDSTATE8_H

#include <Core/Parents/States/WildState.hpp>

/**
 * @brief State class for Gen8 wild encounters
 */
class WildState8 : public WildGeneratorState
{
public:
    /**
     * @brief Construct a new WildGeneratorState object
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
     * @param encounterSlot Pokemon encounter slot
     * @param item Pokemon item
     * @param specie Pokemon specie
     * @param form Pokemon form
     * @param height Pokemon height
     * @param weight Pokemon weight
     * @param info Pokemon information
     */
    WildState8(u32 advances, u32 ec, u32 pid, const std::array<u8, 6> &ivs, u8 ability, u8 gender, u8 level, u8 nature, u8 shiny,
               u8 encounterSlot, u16 item, u16 specie, u8 form, u8 height, u8 weight, const PersonalInfo *info) :
        WildGeneratorState(advances, ec, pid, ivs, ability, gender, level, nature, shiny, encounterSlot, item, specie, form, info),
        height(height),
        weight(weight)
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

#endif // WILDSTATE8_H
