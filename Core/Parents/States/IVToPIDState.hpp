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

#ifndef IVTOPIDSTATE_HPP
#define IVTOPIDSTATE_HPP

#include <Core/Global.hpp>

enum class Method : u8;

/**
 * @brief State class for PID to IV information
 */
class IVToPIDState
{
public:
    /**
     * @brief Construct a new IVToPIDState object
     *
     * @param seed Origin seed
     * @param pid PID value
     * @param sid Secret Trainer ID
     * @param method Generation method
     */
    IVToPIDState(u32 seed, u32 pid, u16 sid, Method method) : seed(seed), pid(pid), sid(sid), method(method)
    {
    }

    /**
     * @brief Returns the generation method of the state
     *
     * @return Generation method
     */
    Method getMethod() const
    {
        return method;
    }

    /**
     * @brief Returns the PID of the state
     *
     * @return State PID
     */
    u32 getPID() const
    {
        return pid;
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

    /**
     * @brief Returns the SID of the state
     *
     * @return State SID
     */
    u16 getSID() const
    {
        return sid;
    }

private:
    u32 pid;
    u32 seed;
    u16 sid;
    Method method;
};

#endif // IVTOPIDSTATE_HPP
