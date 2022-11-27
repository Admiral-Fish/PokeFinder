/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2022 by Admiral_Fish, bumba, and EzPzStreamz
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

#ifndef UNDERGROUNDSTATE_HPP
#define UNDERGROUNDSTATE_HPP

#include <Core/Parents/States/State.hpp>

class PersonalInfo;

/**
 * @brief State class for Gen8 wild generator encounters
 */
class UndergroundState : public GeneratorState
{
public:
    /**
     * @brief Construct a new UndergroundState object
     *
     * @param advances Advances of the state
     * @param pid Pokemon PID
     * @param ivs Pokemon IVs
     * @param ability Pokemon ability
     * @param gender Pokemon gender
     * @param level Pokemon level
     * @param nature Pokemon nature
     * @param shiny Pokemon shininess
     * @param eggMove Pokemon egg move
     * @param item Pokemon item
     * @param specie Pokemon specie
     * @param info Pokemon personal information
     */
    UndergroundState(u32 advances, u32 pid, const std::array<u8, 6> &ivs, u8 ability, u8 gender, u8 level, u8 nature, u8 shiny, u16 eggMove,
                     u16 item, u16 specie, const PersonalInfo *info) :
        GeneratorState(advances, pid, ivs, ability, gender, level, nature, shiny, info), eggMove(eggMove), item(item), specie(specie)
    {
    }

    /**
     * @brief Returns the egg move of the pokemon
     *
     * @return Pokemon egg move
     */
    u16 getEggMove() const
    {
        return eggMove;
    }

    /**
     * @brief Returns the item of the pokemon
     *
     * @return Pokemon item
     */
    u16 getItem() const
    {
        return item;
    }

    /**
     * @brief Returns the specie
     *
     * @return Pokemon specie
     */
    u16 getSpecie() const
    {
        return specie;
    }

private:
    u16 eggMove;
    u16 item;
    u16 specie;
};

#endif // UNDERGROUNDSTATE_HPP
