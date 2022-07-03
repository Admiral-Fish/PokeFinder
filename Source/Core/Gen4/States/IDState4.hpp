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

#ifndef IDSTATE4_HPP
#define IDSTATE4_HPP

#include <Core/Parents/States/IDState.hpp>

/**
 * @brief State class for Gen4 TID/SID
 */
class IDState4 : public IDState
{
public:
    /**
     * @brief Construct a new IDState4 object
     *
     * @param seed State seed
     * @param delay State delay
     * @param tid Trainer ID
     * @param sid Secret ID
     * @param seconds State seconds
     */
    IDState4(u32 seed, u32 delay, u16 tid, u16 sid, u8 seconds) :
        IDState(tid, sid, (tid ^ sid) >> 3), delay(delay), seed(seed), seconds(seconds)
    {
    }

    /**
     * @brief Construct a new IDState4 object
     *
     * @param seed State seed
     * @param delay State delay
     * @param tid Trainer ID
     * @param sid Secret ID
     */
    IDState4(u32 seed, u32 delay, u16 tid, u16 sid) : IDState(tid, sid, (tid ^ sid) >> 3), delay(delay), seed(seed)
    {
    }

    /**
     * @brief Returns the delay of the state
     *
     * @return State delay
     */
    u32 getDelay() const
    {
        return delay;
    }

    /**
     * @brief Returns the seconds of the state
     *
     * @return State seconds
     */
    u8 getSeconds() const
    {
        return seconds;
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

private:
    u32 delay;
    u32 seed;
    u8 seconds;
};

#endif // IDSTATE4_HPP
