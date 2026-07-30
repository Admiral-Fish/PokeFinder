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

#ifndef PHENOMENONSTATE_HPP
#define PHENOMENONSTATE_HPP

#include <Core/Global.hpp>

/**
 * @brief State class for Gen5 phenomenon
 */
class PhenomenonState
{
public:
    /**
     * @brief Construct a new PhenomenonState object
     *
     * @param prng PRNG call to determine Chatot pitch
     * @param advances Advances of the state
     */
    PhenomenonState(u16 prng, u32 advances) :
        advances(advances), item(false), chatot(prng / 82)
    {
    }

    /**
     * @brief Construct a new PhenomenonState object
     *
     * @param prng PRNG call to determine Chatot pitch
     * @param advances Advances of the state
     * @param item Item number
     */
    PhenomenonState(u16 prng, u32 advances, u16 item) :
        advances(advances), data(item), item(true), chatot(prng / 82)
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

    /**
     * @brief Returns the chatot pitch
     *
     * @return Chatot pitch
     */
    u8 getChatot() const
    {
        return chatot;
    }

    /**
     * @brief Returns the data of the state
     * If the item flag it true it is the item number.
     *
     * @return State data
     */
    u16 getData() const
    {
        return data;
    }

    /**
     * @brief getItem
     *
     * @return true State is a item
     * @return false State is a pokemon
     */
    bool getItem() const
    {
        return item;
    }

private:
    u32 advances;
    u16 data;
    bool item;
    u8 chatot;
};

#endif // PHENOMENONSTATE_HPP
