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

#ifndef PROFILE3_HPP
#define PROFILE3_HPP

#include <Core/Parents/Profile.hpp>

/**
 * @brief Provides additional storage specific to Gen3
 */
class Profile3 : public Profile
{
public:
    /**
     * @brief Construct a new Profile3 object
     *
     * @param name Profile name
     * @param version Game version
     * @param tid Trainer ID
     * @param sid Secret ID
     * @param deadBattery Whether profile has a dead battery or not
     */
    Profile3(const std::string &name, Game version, u16 tid, u16 sid, bool deadBattery);

    /**
     * @brief Returns the profiles dead battery status
     *
     * @return true Dead battery is enabled
     * @return false Dead battery is not enabled
     */
    bool getDeadBattery() const;

    /**
     * @brief Checks if two profiles are equal
     *
     * @param other Profile to compare
     *
     * @return true Profiles are equal
     * @return false Profiles are not equal
     */
    bool operator==(const Profile3 &other) const;

    /**
     * @brief Checks if two profiles are not equal
     *
     * @param other Profile to compare
     *
     * @return true Profiles are not equal
     * @return false Profiles are equal
     */
    bool operator!=(const Profile3 &other) const;

private:
    bool deadBattery;
};

#endif // PROFILE3_HPP
