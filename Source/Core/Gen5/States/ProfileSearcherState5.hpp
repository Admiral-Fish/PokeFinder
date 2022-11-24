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

#ifndef PROFILESEARCHERSTATE5_HPP
#define PROFILESEARCHERSTATE5_HPP

#include <Core/Global.hpp>

/**
 * @brief Provides information to calibrate profiles
 */
class ProfileSearcherState5
{
public:
    /**
     * @brief Construct a new ProfileModel5 object
     *
     * @param seed Calibrated seed
     * @param timer0 Calibrated Timer0
     * @param vcount Calibrated VCount
     * @param vframe Calibrated VFrame
     * @param gxstat Calibrated GxStat
     * @param second Calibrated second
     */
    ProfileSearcherState5(u64 seed, u16 timer0, u8 vcount, u8 vframe, u8 gxstat, u8 second) :
        seed(seed), timer0(timer0), gxstat(gxstat), second(second), vcount(vcount), vframe(vframe)
    {
    }

    /**
     * @brief Returns the calibrated GxStat
     *
     * @return GxStat
     */
    u8 getGxStat() const
    {
        return gxstat;
    }

    /**
     * @brief Returns the calibrated second
     *
     * @return Seconds
     */
    u8 getSecond() const
    {
        return second;
    }

    /**
     * @brief Returns the calibrated seed
     *
     * @return Seed
     */
    u64 getSeed() const
    {
        return seed;
    }

    /**
     * @brief Returns the calibrated Timer0
     *
     * @return Timer0
     */
    u16 getTimer0() const
    {
        return timer0;
    }

    /**
     * @brief Returns the calibrated VCount
     *
     * @return VCount
     */
    u8 getVCount() const
    {
        return vcount;
    }

    /**
     * @brief Returns the calibrated VFrame
     *
     * @return VFrame
     */
    u8 getVFrame() const
    {
        return vframe;
    }

private:
    u64 seed;
    u16 timer0;
    u8 gxstat;
    u8 second;
    u8 vcount;
    u8 vframe;
};

#endif // PROFILESEARCHERSTATE5_HPP
