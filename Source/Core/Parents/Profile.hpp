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

#ifndef PROFILE_HPP
#define PROFILE_HPP

#include <Core/Global.hpp>
#include <string>

enum class Game : u32;

/**
 * @brief Provides per generation storage to keep track of relevant information
 */
class Profile
{
public:
    /**
     * @brief Construct a new Profile object
     *
     * @param name Profile name
     * @param version Game version
     * @param tid Trainer ID
     * @param sid Secret ID
     */
    Profile(const std::string &name, Game version, u16 tid, u16 sid);

    /**
     * @brief Returns the profile name
     *
     * @return Profile name
     */
    std::string getName() const;

    /**
     * @brief Returns the profile secret ID
     *
     * @return Profile SID
     */
    u16 getSID() const;

    /**
     * @brief Returns the profile trainer ID
     *
     * @return Profile TID
     */
    u16 getTID() const;

    /**
     * @brief Returns the profile game version
     *
     * @return Profile game version
     */
    Game getVersion() const;

    /**
     * @brief Checks if two profiles are equal
     *
     * @param other Profile to compare
     *
     * @return True if equal, false otherwise
     */
    bool operator==(const Profile &other) const;

    /**
     * @brief Checks if two profiles are not equal
     *
     * @param other Profile to compare
     *
     * @return True if not equal, false otherwise
     */
    bool operator!=(const Profile &other) const;

protected:
    u16 sid;
    u16 tid;
    Game version;
    std::string name;
};

#endif // PROFILE_HPP
