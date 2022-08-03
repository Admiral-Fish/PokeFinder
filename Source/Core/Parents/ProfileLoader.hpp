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

#ifndef PROFILELOADER_HPP
#define PROFILELOADER_HPP

#include <string>
#include <vector>

class Profile3;
class Profile4;
class Profile5;
class Profile8;

namespace ProfileLoader
{
    /**
     * @brief Initializes the ProfileLoader with the location to read/write profiles from
     * @param location Location of profiles json file
     * @return True if profiles json file exists, false otherwise
     */
    bool init(const std::string &location);
}

namespace ProfileLoader3
{
    /**
     * @brief Reads profiles from stored json file
     * @return Vector of profiles
     */
    std::vector<Profile3> getProfiles();

    /**
     * @brief Adds a profile to the stored json file
     * @param profile Profile being added
     */
    void addProfile(const Profile3 &profile);

    /**
     * @brief Deletes a profile from the stored json file
     * @param remove Profile being removed
     */
    void removeProfile(const Profile3 &remove);

    /**
     * @brief Updates a profile from the stored json file
     * @param update Profile with new modifications
     * @param original Profile being edited
     */
    void updateProfile(const Profile3 &update, const Profile3 &original);
}

namespace ProfileLoader4
{
    /**
     * @brief Reads profiles from stored json file
     * @return Vector of profiles
     */
    std::vector<Profile4> getProfiles();

    /**
     * @brief Adds a profile to the stored json file
     * @param profile Profile being added
     */
    void addProfile(const Profile4 &profile);

    /**
     * @brief Deletes a profile from the stored json file
     * @param remove Profile being removed
     */
    void removeProfile(const Profile4 &remove);

    /**
     * @brief Updates a profile from the stored json file
     * @param update Profile with new modifications
     * @param original Profile being edited
     */
    void updateProfile(const Profile4 &update, const Profile4 &original);
}

namespace ProfileLoader5
{
    /**
     * @brief Reads profiles from stored json file
     * @return Vector of profiles
     */
    std::vector<Profile5> getProfiles();

    /**
     * @brief Adds a profile to the stored json file
     * @param profile Profile being added
     */
    void addProfile(const Profile5 &profile);

    /**
     * @brief Deletes a profile from the stored json file
     * @param remove Profile being removed
     */
    void removeProfile(const Profile5 &remove);

    /**
     * @brief Updates a profile from the stored json file
     * @param update Profile with new modifications
     * @param original Profile being edited
     */
    void updateProfile(const Profile5 &update, const Profile5 &original);
}

namespace ProfileLoader8
{
    /**
     * @brief Reads profiles from stored json file
     * @return Vector of profiles
     */
    std::vector<Profile8> getProfiles();

    /**
     * @brief Adds a profile to the stored json file
     * @param profile Profile being added
     */
    void addProfile(const Profile8 &profile);

    /**
     * @brief Deletes a profile from the stored json file
     * @param remove Profile being removed
     */
    void removeProfile(const Profile8 &remove);

    /**
     * @brief Updates a profile from the stored json file
     * @param update Profile with new modifications
     * @param original Profile being edited
     */
    void updateProfile(const Profile8 &update, const Profile8 &original);
}

#endif // PROFILELOADER_HPP
