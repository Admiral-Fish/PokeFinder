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

#ifndef EGGSTATE_HPP
#define EGGSTATE_HPP

#include <Core/Parents/States/State.hpp>

class EggState : public State
{
public:
    /**
     * @brief Construct a new EggState object
     *
     * @param pid Pokemon PID
     * @param ivs Pokemon IVs
     * @param ability Pokemon ability
     * @param gender Pokemon gender
     * @param level Pokemon level
     * @param nature Pokemon nature
     * @param shiny Pokemon shininess
     * @param inheritance Pokemon IV inheritance
     * @param info Pokemon information
     */
    EggState(u32 pid, const std::array<u8, 6> &ivs, u8 ability, u8 gender, u8 level, u8 nature, u8 shiny,
             const std::array<u8, 6> &inheritance, const PersonalInfo *info) :
        State(pid, ivs, ability, gender, level, nature, shiny, info), inheritance(inheritance)
    {
    }

    /**
     * @brief Returns the inheritance of the pokemon
     *
     * @return Pokemon inheritance
     */
    std::array<u8, 6> getInheritance() const
    {
        return inheritance;
    }

    /**
     * @brief Returns the specified inheritance of the pokemon
     *
     * @param index Inheritance index to get
     *
     * @return Pokemon inheritance
     */
    u8 getInheritance(u8 index) const
    {
        return inheritance[index];
    }

protected:
    std::array<u8, 6> inheritance;
};

/**
 * @brief Parent state class that provides additional information from the generator
 */
class EggGeneratorState : public EggState
{
public:
    /**
     * @brief Construct a new EggGeneratorState object
     *
     * @param advances Advances of the state
     * @param pid Pokemon PID
     * @param ivs Pokemon IVs
     * @param ability Pokemon ability
     * @param gender Pokemon gender
     * @param level Pokemon level
     * @param nature Pokemon nature
     * @param shiny Pokemon shininess
     * @param inheritance Pokemon IV inheritance
     * @param info Pokemon information
     */
    EggGeneratorState(u32 advances, u32 pid, const std::array<u8, 6> &ivs, u8 ability, u8 gender, u8 level, u8 nature, u8 shiny,
                      const std::array<u8, 6> &inheritance, const PersonalInfo *info) :
        EggState(pid, ivs, ability, gender, level, nature, shiny, inheritance, info), advances(advances)
    {
    }

    /**
     * @brief Returns the advances of the state
     *
     * @return State advances
     */
    u32 getAdvances() const
    {
        return advances;
    }

protected:
    u32 advances;
};

/**
 * @brief Parent state class that provides additional information from the searcher
 */
class EggSearcherState : public EggState
{
public:
    /**
     * @brief Construct a new EggSearcherState object
     *
     * @param seed Seed of the state
     * @param pid Pokemon PID
     * @param ivs Pokemon IVs
     * @param ability Pokemon ability
     * @param gender Pokemon gender
     * @param level Pokemon level
     * @param nature Pokemon nature
     * @param shiny Pokemon shininess
     * @param inheritance Pokemon IV inheritance
     * @param info Pokemon information
     */
    EggSearcherState(u32 seed, u32 pid, const std::array<u8, 6> &ivs, u8 ability, u8 gender, u8 level, u8 nature, u8 shiny,
                     const std::array<u8, 6> &inheritance, const PersonalInfo *info) :
        EggState(pid, ivs, ability, gender, level, nature, shiny, inheritance, info), seed(seed)
    {
    }

    /**
     * @brief Returns the seed of the state
     *
     * @return State seed
     */
    u32 getSeed() const
    {
        return seed;
    }

protected:
    u32 seed;
};

#endif // EGGSTATE_HPP
