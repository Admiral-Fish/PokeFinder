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

#ifndef EGGSTATE8_HPP
#define EGGSTATE8_HPP

#include <Core/Parents/States/EggState.hpp>

/**
 * @brief State class for Gen8 eggs
 */
class EggState8 : public EggGeneratorState
{
public:
    /**
     * @brief Construct a new EggState8 object
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
     * @param inheritance Pokemon IV inheritance
     * @param seed Egg generation seed
     * @param info Pokemon information
     */
    EggState8(u32 advances, u32 ec, u32 pid, const std::array<u8, 6> &ivs, u8 ability, u8 gender, u8 level, u8 nature, u8 shiny, u8 height, u8 weight,
              const std::array<u8, 6> &inheritance, u32 seed, const PersonalInfo *info) :
        EggGeneratorState(advances, ec, pid, ivs, ability, gender, level, nature, shiny, inheritance, info), height(height), weight(weight), seed(seed)
    {
    }

    /**
     * @brief Returns the egg seed of the pokemon
     *
     * @return Pokemon egg seed
     */
    u32 getSeed() const
    {
        return seed;
    }

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
    u32 seed;
    u8 height;
    u8 weight;
};

#endif // EGGSTATE8_HPP
