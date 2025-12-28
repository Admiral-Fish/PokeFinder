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

#ifndef LOCKINFO_HPP
#define LOCKINFO_HPP

#include <Core/Global.hpp>

class LockInfo
{
public:
    /**
     * @brief Construct a new LockInfo object
     */
    constexpr LockInfo() : ignore(true), gender(0), genderRatio(0), nature(0)
    {
    }

    /**
     * @brief Construct a new LockInfo object
     *
     * @param nature Forced nature of the lock
     * @param gender Forced gender of the lock
     * @param genderRatio Gender ratio of the lock
     */
    constexpr LockInfo(u8 nature, u8 gender, u8 genderRatio) :
        ignore(nature == 0 && gender == 0 && genderRatio == 0), gender(gender), genderRatio(genderRatio), nature(nature)
    {
    }

    /**
     * @brief Determines whether the \p pid matches the lock criteria
     *
     * @param pid PID value
     *
     * @return true PID matches lock criteria
     * @return false PID does not match lock criteria
     */
    bool compare(u32 pid) const
    {
        if (gender != 2 && gender != ((pid & 255) < genderRatio))
        {
            return false;
        }

        return nature == (pid % 25);
    }

    /**
     * @brief Lock should be ignored. This means the lock is a shadow pokemon itself
     *
     * @return true Lock should be ignored
     * @return false Lock should not be ignored
     */
    bool getIgnore() const
    {
        return ignore;
    }

private:
    bool ignore;
    u8 gender;
    u8 genderRatio;
    u8 nature;
};

#endif // LOCKINFO_HPP
