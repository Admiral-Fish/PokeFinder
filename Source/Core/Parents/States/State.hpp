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

#ifndef STATE_HPP
#define STATE_HPP

#include <Core/Global.hpp>
#include <array>

/**
 * @brief Parent state that contains all the common information for a Pokemon across each game
 */
class State
{
public:
    /**
     * @brief Returns the ability of the pokemon
     *
     * @return Pokemon ability
     */
    u8 getAbility() const
    {
        return ability;
    }

    /**
     * @brief Returns the ability index of the pokemon
     *
     * @return Pokemon ability index
     */
    u16 getAbilityIndex() const
    {
        return abilityIndex;
    }

    /**
     * @brief Returns the gender of the pokemon
     *
     * @return Pokemon gender
     */
    u8 getGender() const
    {
        return gender;
    }

    u8 getHiddenPower() const
    {
        return hiddenPower;
    }

    /**
     * @brief Returns the specified IV of the pokemon
     *
     * @param index IV index to get
     *
     * @return Pokemon IV
     */
    u8 getIV(int index) const
    {
        return ivs[index];
    }

    /**
     * @brief Returns the IVs of the pokemon
     *
     * @return Pokemon IVs
     */
    std::array<u8, 6> getIVs() const
    {
        return ivs;
    }

    /**
     * @brief Returns the level of the pokemon
     *
     * @return Pokemon level
     */
    u8 getLevel() const
    {
        return level;
    }

    /**
     * @brief Returns the nature of the pokemon
     *
     * @return Pokemon nature
     */
    u8 getNature() const
    {
        return nature;
    }

    /**
     * @brief Returns the PID of the pokemon
     *
     * @return Pokemon PID
     */
    u32 getPID() const
    {
        return pid;
    }

    /**
     * @brief Returns the shininess of the pokemon
     *
     * @return Pokemon shininess
     */
    u8 getShiny() const
    {
        return shiny;
    }

    /**
     * @brief Returns the specified stat of the pokemon
     *
     * @param index Stat index to get
     *
     * @return Pokemon stat
     */
    u16 getStat(int index) const
    {
        return stats[index];
    }

    /**
     * @brief Returns the stats of the pokemon
     *
     * @return Pokemon stats
     */
    std::array<u16, 6> getStats() const
    {
        return stats;
    }

protected:
    u32 pid;
    std::array<u16, 6> stats;
    u16 abilityIndex;
    std::array<u8, 6> ivs;
    u8 ability;
    u8 gender;
    u8 hiddenPower;
    u8 nature;
    u8 level;
    u8 shiny;
};

/**
 * @brief Parent state class that provides additional information from the generator
 */
class GeneratorState : public State
{
public:
#ifdef TEST
    GeneratorState() = default;
#endif

    /**
     * @brief Construct a new GeneratorState object
     *
     * @param advances Advances of the state
     */
    GeneratorState(u32 advances) : advances(advances)
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
 *
 * @tparam Integer Integer type of the seed
 */
template <typename Integer>
class SearcherState : public State
{
public:
#ifdef TEST
    SearcherState() = default;
#endif

    /**
     * @brief Construct a new SearcherState object
     * @param seed Seed of the state
     */
    SearcherState(Integer seed) : seed(seed)
    {
    }

    /**
     * @brief Returns the seed of the state
     *
     * @return State seed
     */
    Integer getSeed() const
    {
        return seed;
    }

protected:
    Integer seed;
};

#endif // STATE_HPP
