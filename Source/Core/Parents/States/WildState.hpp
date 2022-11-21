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

#ifndef WILDSTATE_HPP
#define WILDSTATE_HPP

#include <Core/Parents/States/State.hpp>

/**
 * @brief Parent state class that provides additional wild information
 */
class WildState : public State
{
public:
    /**
     * @brief Returns the encounter slot of the state
     *
     * @return State encounter slot
     */
    u8 getEncounterSlot() const
    {
        return encounterSlot;
    }

    /**
     * @brief Returns the item of the pokemon
     *
     * @return State item
     */
    u16 getItem() const
    {
        return item;
    }

    /**
     * @brief Returns the specie
     *
     * @return State specie
     */
    u16 getSpecie() const
    {
        return specie;
    }

protected:
    u16 item;
    u16 specie;
    u8 encounterSlot;
};

/**
 * @brief Parent state class that provides additional information from the generator
 */
class WildGeneratorState : public WildState
{
public:
    /**
     * @brief Construct a new WildGeneratorState object
     *
     * @param advances Advances of the state
     */
    WildGeneratorState(u32 advances) : advances(advances)
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
class WildSearcherState : public WildState
{
public:
    /**
     * @brief Construct a new WildSearcherState object
     * @param seed Seed of the state
     */
    WildSearcherState(Integer seed) : seed(seed)
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

#endif // WILDSTATE_HPP
