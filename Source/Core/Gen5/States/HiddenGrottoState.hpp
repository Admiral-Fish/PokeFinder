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

#ifndef HIDDENGROTTOSTATE_HPP
#define HIDDENGROTTOSTATE_HPP

#include <Core/Global.hpp>

/**
 * @brief State class for Gen5 hidden grottos
 */
class HiddenGrottoState
{
public:
    /**
     * @brief Construct a new HiddenGrottoState object
     *
     * @param prng PRNG call to determine Chatot pitch
     * @param advances Advances of the state
     * @param group Pokemon group
     * @param slot Pokemon slot
     * @param specie Pokemon specie
     * @param gender Pokemon gender
     */
    HiddenGrottoState(u16 prng, u32 advances, u8 group, u8 slot, u16 specie, u8 gender) :
        advances(advances), data(specie), item(false), chatot(prng / 82), gender(gender), group(group), slot(slot)
    {
    }

    /**
     * @brief Construct a new HiddenGrottoState object
     *
     * @param prng PRNG call to determine Chatot pitch
     * @param advances Advances of the state
     * @param group Item group
     * @param slot Item slot
     * @param item Item number
     */
    HiddenGrottoState(u16 prng, u32 advances, u8 group, u8 slot, u16 item) :
        advances(advances), data(item), item(true), chatot(prng / 82), gender(0), group(group), slot(slot)
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
     * If the item flag it true it is the item number otherwise the pokemon specie.
     *
     * @return State data
     */
    u16 getData() const
    {
        return data;
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

    /**
     * @brief Returns the group of the state
     *
     * @return State group
     */
    u8 getGroup() const
    {
        return group;
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

    /**
     * @brief Returns the slot of the state
     *
     * @return State slot
     */
    u8 getSlot() const
    {
        return slot;
    }

private:
    u32 advances;
    u16 data;
    bool item;
    u8 chatot;
    u8 gender;
    u8 group;
    u8 slot;
};

#endif // HIDDENGROTTOSTATE_HPP
