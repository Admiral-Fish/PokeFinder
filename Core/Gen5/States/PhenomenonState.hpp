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
    PhenomenonState(u32 prng, u32 advances, u16 item, bool phenomenon, bool valid) :
        advances(advances),
        item(item),
        phenomenon(phenomenon),
        valid(valid),
        chatot(((static_cast<u64>(prng) * 0x1fff) >> 32) / 82),
        needle((static_cast<u64>(prng) * 8) >> 32)
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
     * @brief Returns the item of the state
     *
     * @return State item
     */
    u16 getItem() const
    {
        return item;
    }

    /**
     * @brief Returns the needle value
     *
     * @return Needle value
     */
    u8 getNeedle() const
    {
        return needle;
    }

    /**
     * @brief Returns whether this state generates a phenomenon tile
     *
     * @return true State generates a phenomenon tile
     * @return false State does not generate a phenomenon tile
     */
    bool getPhenomenon() const
    {
        return phenomenon;
    }

    /**
     * @brief Determines if the state can be hit
     *
     * @return true State can be hit
     * @return false State cannot be hit
     */
    bool isValid() const
    {
        return valid;
    }

private:
    u32 advances;
    u16 item;
    bool phenomenon;
    bool valid;
    u8 chatot;
    u8 needle;
};

#endif // PHENOMENONSTATE_HPP
