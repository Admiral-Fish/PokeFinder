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

#ifndef PIDTOIVSTATE_HPP
#define PIDTOIVSTATE_HPP

#include <Core/Global.hpp>

enum class Method : u8;

/**
 * @brief State class for PID to IV information
 */
class PIDToIVState
{
public:
    /**
     * @brief Construct a new PIDIVState object
     *
     * @param seed Origin seed
     * @param iv1 First IV call
     * @param iv2 Second IV call
     * @param method Generation method
     */
    PIDToIVState(u32 seed, u16 iv1, u16 iv2, Method method) : seed(seed), method(method)
    {
        ivs[0] = iv1 & 0x1f;
        ivs[1] = (iv1 >> 5) & 0x1f;
        ivs[2] = (iv1 >> 10) & 0x1f;
        ivs[3] = (iv2 >> 5) & 0x1f;
        ivs[4] = (iv2 >> 10) & 0x1f;
        ivs[5] = iv2 & 0x1f;
    }

    /**
     * @brief Construct a new PIDIVState object
     *
     * @param seed Origin seed
     * @param hp HP IV
     * @param atk Atk IV
     * @param def Def IV
     * @param spa SpA IV
     * @param spd SpD IV
     * @param spe Spe IV
     * @param method Generation method
     */
    PIDToIVState(u32 seed, u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe, Method method) :
        seed(seed), method(method), ivs { hp, atk, def, spa, spd, spe }
    {
    }

    /**
     * @brief Returns the specified IV
     *
     * @param index IV index to get
     *
     * @return IV value
     */
    u8 getIV(u8 index) const
    {
        return ivs[index];
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
     * @brief Returns the seed of the state
     *
     * @return State seed
     */
    u32 getSeed() const
    {
        return seed;
    }

private:
    u32 seed;
    Method method;
    u8 ivs[6];
};

#endif // PIDTOIVSTATE_HPP
